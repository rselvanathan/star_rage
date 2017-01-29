/*
This class deals will present a Remapping menu entry which will allow the user to select and remap the key

Author : Romesh Selvanathan

Date : 10/12
*/

#include "GameUtil.h"
#include "GL2DBox.h"
#include "InputConstants.h"
class GLText;

#ifndef __KEYBINDENTRY_H__
#define __KEYBINDENTRY_H__

class KeybindEntry
{
public:
	// Constructor
	KeybindEntry(std::string& action_name, std::string& button_name, unsigned int texture, unsigned int mask_texture, float posX, float posY, Action action);
	// Constructor
	KeybindEntry(std::string& state_name, std::string& button_name, unsigned int texture, unsigned int mask_texture, float posX, float posY, Button_State state);
	// Destructor
	~KeybindEntry();

	// Update the Entry
	void Update();
	// Render the Entry
	void Render();

	// The Mouse Poisition At any given time updated by the object that owns this class
	void UpdateMousePosition(float& x, float& y) {m_fMousePosX = x; m_fMousePosY = y;}

	// Accessor Methods
	const bool IsClicked();
	const bool IsAction() {return m_bIsAction;}
	const std::string GetActionName() {return m_sAction;}
	const std::string GetButtonName() {return m_sButton;}
	const Action GetAction() {return c_Action;}
	const Button_State GetState() {return c_State;}

	// Mutator Methods
	void SetActionName(std::string name) {m_sAction = name;}
	void SetButtonName(std::string name) {m_sButton = name;}
	void SetAction(Action& action) {c_Action = action;}
	void SetState(Button_State& state) {c_State = state;}

private:
	GL2DBox c_Box;						// Box which will have the entry image
	GL2DBox c_BoxGlow;					// Box to allow glowing effect (or lighting up effect) when hovering or clicking the entry
	GL2DBox c_BoxMask;					// A Mask to not allow the texture to be see through
	bool m_bGlow;						// boolean value to allow the glow

	std::string m_sAction;				// The Action/State string
	std::string m_sButton;				// The Raw Button String
	GLText* p_Text;						// A pointer to a text object

	Action c_Action;					// A reference to the action 
	Button_State c_State;				// A reference to the state
	bool m_bIsAction;					// A flag to see whether this entry is a action or state entry

	float m_fPosX, m_fPosY;				// Position X and Y of the item
	float m_fMousePosX, m_fMousePosY;	// Mouse Position x and y
};

#endif