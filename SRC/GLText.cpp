/** Loads and Creates a font from Windows to be used in game. 

Main Difference between this and GL2DText is that this allows variables to be printed on screen
so it will update that vairable as it changes.

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "GLText.h"
#include "GameUtil.h"
#include "Graphics.h"

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
void GLText::CreateText(int height, int width, int angleOfEsc, int Orient, int fontWeight,
					bool italic, bool underline, bool strikeout, int charSetIdentifier,
					int outputPrecision, int clippingPrecision,int FamAndPitch, char* fontName)
{
	hDC = wglGetCurrentDC();					// Get the current device context

	base = glGenLists(96);						// Storage for 96 characters

	// Create a font 
	font = CreateFont(height, width, angleOfEsc, Orient, fontWeight, italic, underline,
					  strikeout, charSetIdentifier, outputPrecision, clippingPrecision, 
					  ANTIALIASED_QUALITY, FamAndPitch, fontName);

	oldfont = (HFONT)SelectObject(hDC,font);	// Select the font we want	
	wglUseFontBitmaps(hDC, 32, 96, base);		// Builds 96 chars starting at 32
	SelectObject(hDC, oldfont);					// Select the font we want
	DeleteObject(font);							// Delete the font
}

/** Print a Text */ 
void GLText::print(const char* print, ...)
{
	// Disable Texture and Lighting */ 
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);

	// Select Ortho view
	Graphics::GetInstance()->SetOrthoView(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
	// Store The Modelview Matrix
	Graphics::GetInstance()->StoreMatrix();
		// Translate it to desired Position ((0, 0) Bottom Left)
		//glTranslated(x, y, 0);
		Graphics::GetInstance()->Translate(x, y, 0);
		// Set the Color of the font
		Graphics::GetInstance()->Color3(m_Color.x, m_Color.y, m_Color.z);
		// Position Bitmap font onto the screen (Needs to be here without it text dissapears)
		glRasterPos2f(0, 0);

		char text[256];			// Hold the string
		va_list ap;				// Pointer to a list of arguments

		if(print == NULL)		// If There is nothint to print do nothing
			return;

		va_start(ap, print);			// Parses the String variable
			vsprintf_s(text, print, ap);	// Converts Symbols to actual numbers
		va_end(ap);						// Results stored in text

		glPushAttrib(GL_LIST_BIT);		// Pushes the Diplay List bits
		/** Say we draw the letter 'A', it's represented by the number 65. Without glListBase(base-32) 
		OpenGL wouldn't know where to find this letter. It would look for it at display list 65, 
		but if base was equal to 1000, 'A' would actually be stored at display list 1065. 
		So by setting a base starting point, OpenGL knows where to get the proper display list from. 
		The reason I subtract 32 is because I never made the first 32 display lists. 
		I skipped them. So we have to let OpenGL know this by subtracting 32 from the base value.  */ 
		glListBase(base - 32);			// Sets the Base char to 32

		glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);		// Draws display List Text
		glPopAttrib();											// Pops the Display List
	// Seletect Perspective View
	Graphics::GetInstance()->SetPerspective();
	// Reset back to Perspective
	Graphics::GetInstance()->ReleaseMatrix();

	// Reenable Lighting and Texture 2D
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}

void GLText::SetColor3f(float r, float g, float b)
{m_Color.x = r; m_Color.y = g; m_Color.z = b;}

void GLText::SetPosition(int X, int Y)
{x = X; y = Y;}