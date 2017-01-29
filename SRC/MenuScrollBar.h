/*
This class will allow the user to scroll a box along a bar to enable a setting

Author : Romesh Selvanathan

Date : 10/12
*/

#ifndef __MENUSCROLLBAR_H__
#define __MENUSCROLLBAR_H__

#include "GL2DBox.h"
#include "GLText.h"

class MenuScrollBar
{
public:
	// Constructor
	MenuScrollBar(float posX, float posY, float width, float initialValue);
	// Destructor
	~MenuScrollBar(){}

	// Update the Entry
	void Update();
	// Render the Entry
	void Render();

	// Return whether or not this tick box has been ticked
	const int GetValue() {return m_iValue;}
	// Set the value
	void SetValue(int value);

	// Check whether the Orb has been clicked
	bool IsClicked();
	// The Mouse Poisition At any given time updated by the object that owns this class
	void UpdateMousePosition(float& x, float& y) {m_fMousePosX = x; m_fMousePosY = y;}
private:
	GL2DBox c_Box_background;			// Box which will have the Scroll bar image
	GL2DBox c_Box_background_left;		// Box which will have the left part of the Scroll bar image
	GL2DBox c_Box_background_right;		// Box which will have the right part of the Scroll bar image

	GL2DBox c_Box;						// Box that will contain the Scroll Box
	GL2DBox c_BoxGlow;					// Box to allow glowing effect (or lighting up effect) when hovering or clicking the scroll box
	GL2DBox c_BoxMask;					// A Mask to not allow the texture to be see through

	GL2DBox c_Box_Text;					// This box will show the text of the value represented by the scroll bar
	GL2DBox c_Box_Text_Mask;			// A Mask to not allow the texture to be see through

	bool m_bGlow;						// boolean value to allow the glow
	bool clicked;						// a boolean to check whether or not the orb is clicked;

	int m_iValue;						// Value that is represneted in the Scroll Bar
	SmartPtr<GLText> p_ValueText;		// The Text that will represent the value

	float m_fPosX, m_fPosY;				// Position X and Y of the item
	float m_fMousePosX, m_fMousePosY;	// Mouse Position x and y
};

#endif