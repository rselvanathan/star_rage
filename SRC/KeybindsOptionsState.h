/*
The Keybinds Options State. It is a singleton class
This will allow the user to specify the keybinds. 
In order to change keybinds the user has to click on a keybind entry on screen, which will prompt the user to press a key to rebind the action/state.
After this is done then the user can permantly save the changes or default back the changes. If the user does not save the changes then the screen
will ask whether to save the changes before exiting this screen.

Author : Romesh Selvanathan
Date : 10/12
*/

#ifndef __KEYBINDSOPTIONSSTATE_H_
#define __KEYBINDSOPTIONSSTATE_H_

#include "GameState.h"
#include "GameUtil.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"
#include "InputConstants.h"
#include "RawInputConstants.h"

class GLText;
class MenuEntry;
class KeybindEntry;

class KeybindsOptionsState : public GameState, public IMenuButtonListener
{
public:
	static KeybindsOptionsState* GetInstance()
	{
		if(mKeybindsOptionsState == NULL)
			mKeybindsOptionsState = new KeybindsOptionsState();
		return mKeybindsOptionsState;
	} 

	virtual ~KeybindsOptionsState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume();

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg);

	//---------------------------------------IMenuButtonListener-------------------------------------
	void OnMouseClick();
	void OnUpPressed();
	void OnDownPressed();
	void OnLeftPressed();
	void OnRightPressed();
	void OnSelectPressed();
	void OnMouseMove(float x, float y);
	//----------------------------------------------------------------------------------------------

private:
	// Shortcut type definitions
	typedef map<Action, RawInputButton> ActionMapType;
	typedef map<Button_State, RawInputButton> StateMapType;
	typedef map<RawInputButton, Action> ButtonToActionMapType;
	typedef map<RawInputButton, Button_State> ButtonToStateMapType;
	typedef map<std::string, KeybindEntry*> KeybindEntryMapType;

	typedef vector<SmartPtr<MenuEntry> > MenuEntryList;

private:
	// This value will determine which message box is active on screen
	int m_iMessageBoxState;
	// This enum Determines what message box this class is calling
	enum MessageBoxScreen { SAVE = 1, KEYBIND = 2, OPTIONS = 3 };

	// This value will determine whether the message box choice was Ok or Cancel
	int m_iMessageBoxChoice;
	// This enum determines whether Ok or Cancel was pressed with the message box
	enum MessageBoxChoice { OK = 1, CANCEL = 2};

private:
	GL2DBox background;					// The background image for this screen

	MenuEntryList menuEntries;			// List of the menu entires

	void Selected();					// Checks whether the a button has been selected and does an appropriate action

	GL2DBox flash;						// THe White flash screen if switching from one screen to another

	static KeybindsOptionsState* mKeybindsOptionsState;
	KeybindsOptionsState();

	// These maps will be used to remap the button presses for a specific action/state
	ActionMapType m_mActionMap;			// A map of actions to be remapped
	StateMapType m_mStateMap;			// A map of states to be remapped

	// Description for the user
	SmartPtr<GLText> p_Description;
	// The list of key bind entires displayed on screen
	KeybindEntryMapType m_vKeybindEntryMap;

	// This is the currently clicked keybind entry
	KeybindEntry* p_CurrentClicked;

	// Call the message box by specifying which Message Box to call up
	void CallMessageBox(MessageBoxScreen message);
	// Rebind the currently selected action/state to the new raw button
	void RebindKey(RawInputButton button);
	// Default the values
	void Default();
	// Save the remapped values
	void Save();
	// Check if any changes were made and if true call up the save message box
	bool CheckChanges();
};

#endif