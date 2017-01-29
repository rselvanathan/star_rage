/** Loads and Creates a display list of fonts from a Bitmap to be used in game.

Author : Romesh Selvanathan

References :

Giuseppe, D and Molofee, J. (n.d). 2D Texture Font. [online]. Available at:
http://nehe.gamedev.net/tutorial/2d_texture_font/18002/ [Accessed at: 03 May 2012]
*/

#include "GL2DText.h"
#include "GameUtil.h"
#include "Texture.h"

#include "GL2DText.h"
#include "GameUtil.h"
#include "Texture.h"
#include "Graphics.h"

/* Constructor */
GL2DText::GL2DText()
{
	// Load the Font texture to be used
	mFontTexture = new Texture();				
	mFontTexture->LoadTGA("..//..//ASSETS//Textures//Font.tga", Texture::MIPMAPPED);

	// Set up the values
	r = g = b = a = scale = 1.f;
	// Build the font
	BuildFont();
}

/* Destructor */
GL2DText::~GL2DText()
{
	SAFE_DELETE(mFontTexture);
	// All 256 Display Lists deleted
	glDeleteLists(base, 256);
}

/* Build the Font Display List */ 
bool GL2DText::BuildFont()
{
	// X and Y coords
	float cx, cy = 0;
	// Create 256 Display Lists
	base = glGenLists(256);
	// Select the Desired Texture
	Graphics::GetInstance()->Bind2DTexture(mFontTexture->GetTGATexture().texID);

	for(int i = 0; i < 256; i++){
		// X and Y position of current Char
		// This makes sure that each time cy moves down a row, 16 characters in cx are gone through
		// 16 x 16 = 256
		cx = float(i % 16)/16.f;	
		cy = float(i / 16)/16.f;
		// Start Building a List
		glNewList(base + i, GL_COMPILE);
			// Use Quads for each Char
				/* 0.0625f = 1/16th of the texture = width/height of one character  //
				 Using glVertexi instead of f due to me using ortho screen for this //
				 This also means no need for negative number as the bottem left of 
				 the screen will be (0,0) */
				Graphics::GetInstance()->Draw2DQuad(0, 16, 0, 16, cx, cx + 0.0625f , 1.0f - cy - 0.0625f, 1.0f - cy - 0.001f);
			// Move to the right of the Character ( If it wasn't translated , Chars would appear
			// to be on top of each other)
			Graphics::GetInstance()->Translate(14, 0, 0);
		// Done Building the Lists
		glEndList();
	}
	return true;
}

/* This Method prints the text on screen
@ params : x - X positon// y - Y Position// scale - Scale of the Texture //
		   string - The actual text to be printed // CharType - Normal or Italic
*/
void GL2DText::glPrint(char* string, int CharType, ...)
{
	// Disable Lighting
	//glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	// Allow Blending and Enable 2D textures
	BLEND_TEX_ON;

	// If greater than one set it to 1 = italic
	if(CharType > 1) CharType = 1;
	// Select The Font Texture
	Graphics::GetInstance()->Bind2DTexture(mFontTexture->GetTGATexture().texID);
	// Set Orthograpghic View
	Graphics::GetInstance()->SetOrthoView(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
	// Store The Modelview Matrix
	Graphics::GetInstance()->StoreMatrix();
			// Reset The Modelview Matrix
			Graphics::GetInstance()->ResetMatrix();
			// Set the Color of the font
			Graphics::GetInstance()->Color4(r, g, b, a);
			// Translate it to desired Position ((0, 0) Bottom Left)
			Graphics::GetInstance()->Translate(mPosition.x, mPosition.y, 0);
			// Change the Scale of Font
			Graphics::GetInstance()->Scale(scale, scale, 0);
			// This Selects which Line we want (Same reason as GLText)
			// 128 lists were skipped
			glListBase(base - 32 + (128 * CharType));
			// Write the text to the screen 
			// ((length of the string), each char represented by a unsigned bye, string of text)
			glCallLists(strlen(string), GL_UNSIGNED_BYTE, string);
	// Restore back to perpetive view
	Graphics::GetInstance()->SetPerspective();
	// Restore Old Model View Matrix
	Graphics::GetInstance()->ReleaseMatrix();

	// Switch off the Blending and 2D Textures
	BLEND_TEX_OFF;
	glDisable(GL_TEXTURE_2D);
	// ReEnable the lighting
	//glEnable(GL_LIGHTING);
}