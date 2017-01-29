/** Loads a texture file from a specified location into the buffer to be used in game

Author : Romesh Selvanathan

References :

craterz. (2001). OpenGL Texture Tutorial. [online]. Available at: <http://www.nullterminator.net/gltexture.html> [Accessed: 30 Match 2012].

Molofee,J. (2000), Tokens, Extensions, Scissor Testing and TGA Loading, [online]. Available at: <http://nehe.gamedev.net/tutorial/tokens_extensions_scissor_testing_and_tga_loading/19002/> [Accessed: 30 March 2012].
*/ 

#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "GameUtil.h"

typedef struct
{
	GLubyte *imageData;		// Image Data (Up to 32 Bits)
	GLuint	bitsPerPixel;	// Image Bits Per Pixel
	GLuint	width;			// Image Width
	GLuint	height;			// Image Height
	GLuint	texID;			// Texture ID (used to select the texture in OpenGL)
} TextureImage;				// Structure Name

class Texture
{
public:
	Texture();
	~Texture();

	const GLuint GetTexture() const {return texture;}
	const TextureImage GetTGATexture() const {return mTexture;}
	void LoadTexture(const char* filename, int w, int h, int f, int FilterType);
	bool LoadTGA(char* filename, int FilterType);
	void BuildColorTexture(unsigned char r, unsigned char g, unsigned char b);

	enum{NEAREST, LINEAR, MIPMAPPED}; // Filtering types

private:
	GLuint texture;					// The texture
	TextureImage mTexture;			// The Texture Structure
	int width, height, format;		// Height, Width and Format (BYtes per Pixel) of the texture
	void* data;						// The Buffer 
	FILE* file;						// File to be opened
};

#endif