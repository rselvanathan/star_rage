/** Loads and Creates a display list of fonts from a Bitmap to be used in game.

Author : Romesh Selvanathan

References :

Giuseppe, D and Molofee, J. (n.d). 2D Texture Font. [online]. Available at:
http://nehe.gamedev.net/tutorial/2d_texture_font/18002/ [Accessed at: 03 May 2012]
*/
#ifndef __GL2DTEXT_h__
#define __GL2DTEXT_h__

#include "GameUtil.h"

class Texture;

class GL2DText
{
public:
	/* Constructor */
	GL2DText();
	/* Destructor */
	~GL2DText();

	/* Print a Text onto the screen */
	void glPrint(char* string, int CharType, ...); 
	/* Set the Color of the text */
	void glSetColor(GLfloat r, GLfloat g, GLfloat b, GLfloat a){this->r = r; this->g = g; 
																this->b = b; this->a = a;}
	/* Two types of text */
	enum {NORMAL, ITALIC};

	/* Get Methods */
	Vec3 GetPosition() {return mPosition;}

	/* Set Methods */
	void SetPosition(Vec3 v) {mPosition = v;}
	void SetScale(float v) {scale = v;}
private:
	bool BuildFont();				// Build the fonts

	GLuint base;					// Base Display List for the Font set
	SmartPtr<Texture> mFontTexture;	// The Font Texture
	GLfloat r, g, b, a;				// Red, Green, Blue, Alpha values to be used to set the font color
	Vec3 mPosition;				// Set the position of the text
	float scale;					// Set the scale of the text
};

#endif