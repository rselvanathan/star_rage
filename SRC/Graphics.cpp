/*
This is wrapper class around OpenGL which wraps the most frequently and most common Opengl API calls.
This means that if I do choose to switch to DirectX for example all I have to do is change these methods 
without changing the whole game by a Lot.

This class will be global singleton class as I do not want to allow object to contain this. Neither do I want
to create pointers of this class as it will cause more errors. Moreover it will also increase in memory
where most classes will use this to access the OpenGL commands. So keeping it a global singleton where only
one of this object ever exists and any class can access it is the best way to come around that problem.

Author : Romesh Selavanthan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "Graphics.h"
#include "GameUtil.h"

Graphics* Graphics::mInstance;

/* Clears The Screen (Depth Buffer and Color) */
void Graphics::ClearScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
	glLoadIdentity();
}

void Graphics::SetOrthoView(int left, int right, int top, int bottom)
{
	// Switch to our projection matrix so that we can change it to ortho mode, not perspective.
	glMatrixMode(GL_PROJECTION);						
	// Push on a new matrix so that we can just pop it off to go back to perspective mode
	glPushMatrix();									
	// Reset the current matrix to our identify matrix
	glLoadIdentity();								
	//Pass in our 2D ortho screen coordinates like so (left, right, bottom, top).  The last
	// 2 parameters are the near and far planes.
	glOrtho( left, right, top, bottom, -1, 1 );	
	// Switch to model view so that we can render the scope image
	glMatrixMode(GL_MODELVIEW);								
	// Initialize the current model view matrix with the identity matrix
	glLoadIdentity();			
}

void Graphics::SetPerspective()
{
	// Enter into our projection matrix mode
	glMatrixMode( GL_PROJECTION );							
	// Pop off the last matrix pushed on when in projection mode (Get rid of ortho mode)
	glPopMatrix();											
	// Go back to our model view matrix like normal
	glMatrixMode( GL_MODELVIEW );			
}

bool Graphics::Project3DToScreen(Vec3& objectPosition, double* modelview, double* projection, int* viewport, double &winx, double &winy, double &winz)
{
	if(gluProject(objectPosition.x, objectPosition.y, objectPosition.z, modelview, 
		projection, viewport, &winx, &winy, &winz) == GL_TRUE)
		return true;

	return false;
}

/* Draw a simple Quad - only used by one class for now */
void Graphics::DrawQuad(float width, float height, float length)
{
	glBegin(GL_QUADS);
			glVertex3f(width,height, length);             
			glVertex3f(0,	height, length);              
			glVertex3f(0,	-height, length);            
			glVertex3f(width, -height, length);
	glEnd();
}

/* Draw a 2D Quad */
void Graphics::Draw2DQuad(float minX, float maxX, float minY, float maxY, 
		float texMinX, float texMaxX, float texMinY, float texMaxY)
{
	glBegin(GL_QUADS);
				// Bottom Left
				glTexCoord2f(texMinX, texMinY);	 glVertex2i(minX,minY);
				// Bottom Right
				glTexCoord2f(texMaxX, texMinY);  glVertex2i(maxX, minY);
				// Top Right
				glTexCoord2f(texMaxX, texMaxY);	 glVertex2i (maxX, maxY);
				// Top Left
				glTexCoord2f(texMinX, texMaxY);	 glVertex2i(minX, maxY);
	glEnd();
}

//-------------------------------Blending------------------------------------
void Graphics::Blend_Masking()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_COLOR,GL_ZERO);
}

void Graphics::AdditiveBlend_Src_Alpha()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE );
}

void Graphics::AdditiveBlend_Dest_Alpha()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_DST_ALPHA, GL_ONE );
}

void Graphics::Blend_SRCWithOne_MinusSRCColor()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_COLOR );
}

void Graphics::Blend_SRCWithOne_MinusSRCAlpha()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
}

void Graphics::DisableBlend()
{
	glDisable(GL_BLEND);
}
//---------------------------------------------------------------------------