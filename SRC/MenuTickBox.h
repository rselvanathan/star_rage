/*
This class will allow the user to Tick a box to enable a setting

Author : Romesh Selvanathan

Date : 10/12
*/

#ifndef __MENUTICKBOX_H__
#define __MENUTICKBOX_H__

#include "GL2DBox.h"

class MenuTickBox
{
public:
	// Constructor
	MenuTickBox(float posX, float posY);
	// Destructor
	~MenuTickBox(){}

	// Update the Entry
	void Update();
	// Render the Entry
	void Render();

	// Return whether or not this tick box has been ticked
	const bool IsTicked() {return m_bTicked;}
	// Set the ticked value
	void SetTicked(bool value) {m_bTicked = value;}

	// Check whether the TickBox has been clicked
	bool IsClicked();
	// The Mouse Poisition At any given time updated by the object that owns this class
	void UpdateMousePosition(float& x, float& y) {m_fMousePosX = x; m_fMousePosY = y;}
private:
	GL2DBox c_Box;						// Box which will have the tick box image
	GL2DBox c_BoxGlow;					// Box to allow glowing effect (or lighting up effect) when hovering or clicking the tick box
	GL2DBox c_BoxMask;					// A Mask to not allow the texture to be see through
	GL2DBox c_Box_ticked;				// Box which will have the tick box image with a tick
	GL2DBox c_Box_tickedGlow;			// Box to allow glowing effect (or lighting up effect) when hovering or clicking the tick box
	bool m_bGlow;						// boolean value to allow the glow
	bool m_bTicked;						// a value that determines whether this has been ticked or not

	float m_fPosX, m_fPosY;				// Position X and Y of the item
	float m_fMousePosX, m_fMousePosY;	// Mouse Position x and y

	// Method sets whether or not the box was ticked
	void TickBox();
};

#endif