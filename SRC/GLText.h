
/** Loads and Creates a font from Windows to be used in game. 

Main Difference between this and GL2DText is that this allows variables to be printed on screen
so it will update that vairable as it changes.

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/
#ifndef __GLTEXT_H__
#define __GLTEXT_H__

#include "GameUtil.h"

class GLText
{
public :
	/* Constructor */
	GLText(){}
	/* Destructor */
	~GLText(){glDeleteLists(base, 96);}

	/* 
	Create a text with the following values : height of the texture, length of the texture,
	Angle of Escapement - This will rotate the font
	Orient - Best left to 0 (not sure what it does)
	fontWeight - the width of the font for ex FONT_BOLD ( value between 1 - 1000, higher the value more bold)
	italic - should font be italic?
	underline - should font be underlined?
	strikeout - should font be striked out?
	charSetIdentifier - the type of character to use - (There are many types check msdn)
	outputPrecision - tells Windows what type of character set to use if there is more than one type available (OUT_TT_PRECIS is best)
	clippingPrecision - type of clipping to do on the font if it goes outside the clipping region ( Leave to default)
	familt and pitch - check msdn for all types
	fontname - name of th efont to be used
	*/
	void CreateText(int height, int width, int angleOfEsc, int Orient, int fontWeight,
					bool italic, bool underline, bool strikeout, int charSetIdentifier,
					int outputPrecision, int clippingPrecision,int FamAndPitch, char* fontName);

	/* Get Methods */
	int GetXPos(){return x;}
	int GetYPos(){return y;}
	/* Set Methods */
	void SetPosition(int x, int y);
	void SetColor3f(float r, float g, float b);

	/* Print text onto screen , including a variable if required */
	void print(const char* text, ...);

private:
	HFONT font;					// Windows Font ID
	HFONT oldfont;

	HDC hDC;					// GDI device context

	GLuint base;				// base display list for font set

	int x, y;					// x and y position of the text
	Vec3 m_Color;			// the color of the text
};

#endif