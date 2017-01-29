/*
Set up a basic Skybox with 6 Textures on each side, that will follow a spefcific target whenever it 
moves or rotates.

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "GameUtil.h"
#include "Skybox.h"
#include "Texture.h"
#include "Spaceship.h"

Skybox::Skybox()
{
	x = y = 0.0f;

	for(int i = 0; i < 6; i++)
	{
		mTextures[i] = new Texture();
	}

	mTextures[0]->LoadTGA("Textures//Space//Space_Front.tga", 3);
	mTextures[1]->LoadTGA("Textures//Space//Space_Back.tga", 3);
	mTextures[2]->LoadTGA("Textures//Space//Space_Right.tga", 3);
	mTextures[3]->LoadTGA("Textures//Space//Space_Left.tga", 3);
	mTextures[4]->LoadTGA("Textures//Space//Space_Top.tga", 3);
	mTextures[5]->LoadTGA("Textures//Space//Space_Bottom.tga", 3);
}

Skybox::~Skybox()
{
	for(int i = 0; i < 5; i++)
	{
		SAFE_DELETE(mTextures[i]);
	}
}


void Skybox::Render(float x, float y, float z, float width, float height, float length)
{
	glDisable(GL_LIGHTING);
	glPushMatrix();

	glColor3f(1, 1, 1);

	// Center the Skybox around the given x,y,z position
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;


	// Draw Front side
	glBindTexture(GL_TEXTURE_2D, mTextures[0]->GetTGATexture().texID);
	glBegin(GL_QUADS);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height, z+length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z+length);
	glEnd();

	// Draw Back side
	glBindTexture(GL_TEXTURE_2D, mTextures[1]->GetTGATexture().texID);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z);
	glEnd();

	// Draw Left side
	glBindTexture(GL_TEXTURE_2D, mTextures[2]->GetTGATexture().texID);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,		  y+height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,		  y,		z);		
	glEnd();

	// Draw Right side
	glBindTexture(GL_TEXTURE_2D, mTextures[3]->GetTGATexture().texID);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x+width, y,		z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height,	z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x+width, y+height,	z);
	glEnd();

	// Draw Up side
	glBindTexture(GL_TEXTURE_2D, mTextures[4]->GetTGATexture().texID);
	glBegin(GL_QUADS);		
		glTexCoord2f(1.0, 0.0f); glVertex3f(x+width, y+height, z);
 		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y+height, z+length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,		  y+height,	z+length);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,		  y+height,	z);
	glEnd();

	// Draw Down side
	glBindTexture(GL_TEXTURE_2D, mTextures[5]->GetTGATexture().texID);
	glBegin(GL_QUADS);		
		glTexCoord2f(0.0, 1.0f); glVertex3f(x,		  y,		z);
		glTexCoord2f(0.0, 0.0f); glVertex3f(x,		  y,		z+length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x+width, y,		z+length); 
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x+width, y,		z);
	glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);

}

