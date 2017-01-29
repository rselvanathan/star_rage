#ifndef __MENUENTRY_h__
#define __MENUENTRY_h__

#include "GL2DBox.h"

class MenuEntry
{
public:
	/*Constructor */
	MenuEntry(char* name ,unsigned int texture, float positionx, float positiony);
	/*Destructor */
	~MenuEntry();

	/* Update the Entry */
	void Update();
	/* Render the Entry */
	void Render();

	// The Mouse Poisition At any given time updated by the object that owns this class
	void UpdateMousePosition(float& x, float& y) {m_fMousePosX = x; m_fMousePosY = y;}

	/* Has the Entry been clicked on? */
	const bool IsClicked();
	/* The name of the entry */
	const char* GetName() const {return m_name;}

private:
	GL2DBox box;						// Box which will have the entry image
	GL2DBox boxGlow;					// Box to allow glowing effect (or lighting up effect) when hovering or clicking the entry
	bool m_glow;						// boolean value to allow the glow
	char* m_name;						// name of this specific entry
	float m_fMousePosX, m_fMousePosY;	// Mouse Position x and y
};

#endif