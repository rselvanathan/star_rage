/** Loads a texture file from a specified location into the buffer to be used in game

Author : Romesh Selvanathan

References :

craterz. (2001). OpenGL Texture Tutorial. [online]. Available at: <http://www.nullterminator.net/gltexture.html> [Accessed: 30 Match 2012].

Molofee,J. (2000), Tokens, Extensions, Scissor Testing and TGA Loading, [online]. Available at: <http://nehe.gamedev.net/tutorial/tokens_extensions_scissor_testing_and_tga_loading/19002/> [Accessed: 30 March 2012].
*/ 

#include "Texture.h"
#include "GameUtil.h"

/* Constructor */
Texture::Texture() : texture(NULL), width(0), height(0), format(0), data(NULL),file(NULL) 
{}

/* Destructor */
Texture::~Texture()
{
	glDeleteTextures(1, &texture);
	glDeleteTextures(1, &mTexture.texID);
}


/** Loads an RAW file to be used as an linear filtered texture */ 
void Texture::LoadTexture(const char* filename, int w, int h, int f,int FilterType)
{
	// Open texture data
	fopen_s(&file,filename, "rb");
	if(file == NULL)
		return;

	//allocate buffer
	width = w;
	height = h;
	format = f;
	data = malloc(width*height*format);

	// read texture dat
	fread(data, width * height * format, 1, file);
	fclose(file);

	// allocate texture name
	glGenTextures(1, &texture);

	// select the current texture
	glBindTexture(GL_TEXTURE_2D, texture);

	if(FilterType == Texture::NEAREST){
		// when texture is small bilinead the closes bitmap
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		// when texture is large bilinear filter the originla
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		// Build the Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	if(FilterType == Texture::LINEAR){
		// when texture is small bilinead the closes bitmap
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// when texture is large bilinear filter the originla
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Build the Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	if(FilterType == Texture::MIPMAPPED){
		// when texture is small bilinead the closes bitmap
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		// when texture is large bilinear filter the originla
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Build the Texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);
	}

	// free buffer
	if(data)
		free(data);
}

/* Load  a TGA file intro the buffer (Only uncompressed 24 bit or 32 bit) */
bool Texture::LoadTGA(char* filename, int FilterType)
{
	// Uncompressed TGA Header
	GLubyte TGAHeader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	// Used to Compare TGA header to determine if it is a TGA file
	GLubyte TGACompare[12];
	// First 6 Bytes from the Header
	GLubyte header[6];
	// Bytes Per Pixel Used ( 8 bits = 1 byte)
	GLuint bytesPerPixel;
	// Image Size 
	GLuint imageSize;
	// Temp variable used for swapping
	GLuint temp;
	// Default Mode RGBA (32 bit)
	GLuint type = GL_RGBA;

	// Open texture data
	fopen_s(&file,filename, "rb");
	   // Does the File Exits
	if(file == NULL || 
	   fread(TGACompare, 1, sizeof(TGACompare), file) != sizeof(TGACompare) || // Are there 12 bytes to Read?
	   memcmp(TGAHeader, TGACompare, sizeof(TGAHeader)) != 0 || // Does Header Match the TGA format given
	   fread(header, 1, sizeof(header), file) != sizeof(header))// Then Read the next 6 Header Bytes
	{
		// Did the File Exist?
		if(file == NULL)
			return false;
		else{
			// If anything failed closed the file
			fclose(file);
			return false;
		}
	}

	// Determine TGA Width ( highbyte * 256 + lowByte)
	mTexture.width = header[1] * 256 + header[0];
	// Determine TGA Height ( highbyte * 256 + lowByte)
	mTexture.height = header[3] * 256 + header[2];
	// Is The Width or Height less than 0 or is the TGA not a 24 or 32 bit?
	if(mTexture.width <= 0 || mTexture.height <= 0 || (header[4]!=24 && header[4]!=32))
	{
		// if they fail close file
		fclose(file);
		return false;
	}
	// Obtain bits per pixel
	mTexture.bitsPerPixel = header[4];
	// Calculate Bytes per Pixel
	bytesPerPixel = mTexture.bitsPerPixel / 8;
	// Calculate the Memory Required to store TGA (width * height * bytesPerPixel)
	imageSize = mTexture.width * mTexture.height * bytesPerPixel;
	// Reserve Memory to use the TGA
	mTexture.imageData = (GLubyte*) malloc(imageSize);
	// Does the Memory Exist if so Does the Image size match the memory reserved
	if(mTexture.imageData == NULL || fread(mTexture.imageData, 1, imageSize, file) != imageSize)
	{
		// Was Image Data loaded
		if(mTexture.imageData != NULL)
			// Release Image data
			free(mTexture.imageData);
		// Close file
		fclose(file);
		return false;
	}
	/* Data in TGA is stored as BGR(A). We want RBG(A) so we have to swap the 1st with the 3rd
	   or in computing 0th with 2nd to achieve this. We are also skipping every BytesPerPixel(3 or 4)
	   so that each group of BGR(A) is dealt with. */ 
	for(GLuint i = 0; i < (int)(imageSize); i += bytesPerPixel)
	{
		// Store 1st Value at temp
		temp = mTexture.imageData[i];
		// Set 1st Byte to Value of 3rd Byte
		mTexture.imageData[i] = mTexture.imageData[i + 2];
		// 3rd Byte = 1st Byte
		mTexture.imageData[i + 2] = temp;
	}

	// Close file
	fclose(file);

	// allocate texture name
	glGenTextures(1, &mTexture.texID);

	// select the current texture
	glBindTexture(GL_TEXTURE_2D, mTexture.texID);

	if(mTexture.bitsPerPixel == 24)
		type = GL_RGB;
	
	if(FilterType == Texture::NEAREST){
	// when texture is small bilinead the closes tga
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// when texture is large bilinear filter the originla
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	// Build the Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, mTexture.width, mTexture.height, 0, type, 
		GL_UNSIGNED_BYTE, mTexture.imageData);
	}
	if(FilterType == Texture::LINEAR){
		// when texture is small bilinead the closes tga
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// when texture is large bilinear filter the originla
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Build the Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, mTexture.width, mTexture.height, 0, type, 
		GL_UNSIGNED_BYTE, mTexture.imageData);
	}
	if(FilterType == Texture::MIPMAPPED){
		// when texture is small bilinead the closes tga
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
		// when texture is large bilinear filter the originla
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Build the Texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,  mTexture.width, mTexture.height, type, 
						  GL_UNSIGNED_BYTE, mTexture.imageData);
	}
	// used for skybox
	if(FilterType == 3){
		// when texture is small bilinead the closes tga
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		// when texture is large bilinear filter the originla
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		// Build the Texture
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3,  mTexture.width, mTexture.height, type, 
						  GL_UNSIGNED_BYTE, mTexture.imageData);
	}

	if(mTexture.imageData)
		free(mTexture.imageData);

	return true;
}

// Used by Model_3DS to build - Reference Escape GLTexture
void Texture::BuildColorTexture(unsigned char r, unsigned char g, unsigned char b)
{
	unsigned char data[12];	// a 2x2 texture at 24 bits

	// Store the data
	for(int i = 0; i < 12; i += 3)
	{
		data[i] = r;
		data[i+1] = g;
		data[i+2] = b;
	}

	// Generate the OpenGL texture id
	glGenTextures(1, &mTexture.texID);

	// Bind this texture to its id
	glBindTexture(GL_TEXTURE_2D, mTexture.texID);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Use mipmapping filter
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// Generate the texture
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, 2, 2, GL_RGB, GL_UNSIGNED_BYTE, data);
}