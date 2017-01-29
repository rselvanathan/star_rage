/*
A Wrapper class that represents a box done in OpenGL. This class can be called by any object that will use a box 
as its geometry. 

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/ 

#include "GLBox.h"
#include "GameUtil.h"
#include "Matrix4.h"

/* Constructor */
GLBox::GLBox()
{
	// Initialise the Values
	x = y = z = width = height = length = pitch = yaw = roll = 0.0f;
	scale = r = g = b = a = 1.0f;
	texture = NULL;
	matrix = Matrix4::IDENTITY;
}

/* Destructor */
GLBox::~GLBox()
{}

/* Initialise the width height and length of the box */
void GLBox::Init(float Width, float Height, float Length)
{
	width = Width;
	height = Height;
	length = Length;
}
/* Set the Position of this box */
void GLBox::SetPosition(float X, float Y, float Z)
{x = X; y = Y; z = Z;}

/* Set the Color of this box, including the alpha channel */
void GLBox::SetColor(float _r, float _g, float _b, float _a)
{r = _r; g = _g; b = _b; a = _a;}

/* Set the yaw, pitch , roll (y, x, z) rotations of this box */
void GLBox::SetRotation(float Yaw, float Pitch, float Roll)
{yaw = Yaw; pitch = Pitch; roll = Roll;}

/* Set the scale of this box */
void GLBox::SetScale(float Scale)
{scale = Scale;}

/* Set The texture to be used by this box */ 
void GLBox::SetTexture(unsigned int Texture)
{texture = Texture;}

/* Render the box */
void GLBox::Render()
{
	glPushMatrix();

	// If a texture is specifed enable 2D texture 
	if(texture != NULL)
		glEnable(GL_TEXTURE_2D);

	if(matrix != Matrix4::IDENTITY)
	{
		matrix[3][0] = 0.0f;
		matrix[3][1] = 0.0f;
		matrix[3][2] = -1.0; 
		test = matrix[3][2]; //= 0.0f;

		glMultMatrixf(&matrix[0][0]);
	}

	// Translate, Rotate, Color and Scale the Box
	glTranslatef(x, y, z);
	glRotatef(yaw, 0, 1, 0);
	glRotatef(pitch, 1, 0, 0);
	glRotatef(roll, 0, 0, 1);
	glColor4f(r, g, b, a);
	glScalef(scale, scale, scale);

	// If a texture is specified Bind the texture to this box
	if(texture !=NULL)
		glBindTexture(GL_TEXTURE_2D, texture);

	// Begin Drawing th Box 
	glBegin(GL_QUADS);
			//Front Face 
			glNormal3f(0, 0, 1);
			glTexCoord2f(0 ,0);	glVertex3f(-width, -height,  length);	// Bottom Left Of The Tewidthture and Quad
			glTexCoord2f(1 ,0);	 glVertex3f( width, -height,  length);	// Bottom Right Of The Tewidthture and Quad
			glTexCoord2f(1 ,1);	 glVertex3f( width,  height,  length);	// Top Right Of The Tewidthture and Quad
			glTexCoord2f(0 ,1);	glVertex3f(-width,  height,  length);	// Top Left Of The Tewidthture and Quad
			// Back Face
			glNormal3f(0, 0, -1);
			glTexCoord2f(1 ,0);	 glVertex3f(-width, -height, -length);	// Bottom Right Of The Tewidthture and Quad
			glTexCoord2f(1 ,1);	glVertex3f(-width,  height, -length);	// Top Right Of The Tewidthture and Quad
			glTexCoord2f(0 ,1);	 glVertex3f( width,  height, -length);	// Top Left Of The Tewidthture and Quad
			glTexCoord2f(0 ,0);	 glVertex3f( width, -height, -length);	// Bottom Left Of The Tewidthture and Quad
			// Top Face
			 glNormal3f(0, 1, 0);
			glTexCoord2f(0 ,1);	 glVertex3f(-width,  height, -length);	// Top Left Of The Tewidthture and Quad
			glTexCoord2f(0 ,0);    glVertex3f(-width,  height,  length);	// Bottom Left Of The Tewidthture and Quad
			glTexCoord2f(1 ,0);	glVertex3f( width,  height,  length);	// Bottom Right Of The Tewidthture and Quad
			glTexCoord2f(1 ,1);	glVertex3f( width,  height, -length);	// Top Right Of The Tewidthture and Quad
		// Bottom Face
			 glNormal3f(0, -1, 0);
			glTexCoord2f(1 ,1);	 glVertex3f(-width, -height, -length);	// Top Right Of The Tewidthture and Quad
			glTexCoord2f(0 ,1);	glVertex3f( width, -height, -length);	// Top Left Of The Tewidthture and Quad
			glTexCoord2f(0 ,0);	glVertex3f( width, -height,  length);	// Bottom Left Of The Tewidthture and Quad
			glTexCoord2f(1 ,0);	 glVertex3f(-width, -height,  length);	// Bottom Right Of The Tewidthture and Quad
		// Right face
			 glNormal3f(1, 0, 0);
			glTexCoord2f(1 ,0);	glVertex3f( width, -height, -length);	// Bottom Right Of The Tewidthture and Quad
			glTexCoord2f(1 ,1);glVertex3f( width,  height, -length);	// Top Right Of The Tewidthture and Quad
			glTexCoord2f(0 ,1);	glVertex3f( width,  height,  length);	// Top Left Of The Tewidthture and Quad
			glTexCoord2f(0 ,0);	 glVertex3f( width, -height,  length);	// Bottom Left Of The Tewidthture and Quad
		// Left Face
			 glNormal3f(-1, 0, 0);
			glTexCoord2f(0 ,0);	 glVertex3f(-width, -height, -length);	// Bottom Left Of The Tewidthture and Quad
			glTexCoord2f(1 ,0);	glVertex3f(-width, -height,  length);	// Bottom Right Of The Tewidthture and Quad
			glTexCoord2f(1 ,1);	 glVertex3f(-width,  height,  length);	// Top Right Of The Tewidthture and Quad
			glTexCoord2f(0 ,1);	 glVertex3f(-width,  height, -length);	// Top Left Of The Tewidthture and Quad
	glEnd();

	// If a texture is specified disable the texture
	if(texture !=NULL)
		glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}