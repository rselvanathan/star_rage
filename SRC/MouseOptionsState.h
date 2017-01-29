/*
The Mouse Options State. It is a singleton class
This will allow the user to specify the mouse sensitivity as well as if the user wishes to play
with inverted Y rotations or not.

Author : Romesh Selvanathan
Date : 10/12
*/

#ifndef __MOUSEOPTIONSSTATE_H_
#define __MOUSEOPTIONSSTATE_H_

#include "GameState.h"
#include "GameUtil.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class GLText;
class MenuTickBox;
class MenuEntry;
class MenuScrollBar;

class MouseOptionsState : public GameState, public IMenuButtonListener
{
public:
	static MouseOptionsState* GetInstance()
	{
		if(mMouseOptionsState == NULL)
			mMouseOptionsState = new MouseOptionsState();
		return mMouseOptionsState;
	} 

	virtual ~MouseOptionsState();

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
	//-----------------------------------------------------------------------------------------------

private:
	// Shortcut type definitions
	typedef vector<SmartPtr<MenuEntry> > MenuEntryList;

private:
	// This value will determine which message box is active on screen
	int m_iMessageBoxState;
	// This enum Determines what message box this class is calling
	enum MessageBoxScreen { SAVE = 1, OPTIONS = 2 };

	// This value will determine whether the message box choice was Ok or Cancel
	int m_iMessageBoxChoice;
	// This enum determines whether Ok or Cancel was pressed with the message box
	enum MessageBoxChoice { OK = 1, CANCEL = 2};

private:
	GL2DBox background;					// The background image for this screen

	MenuEntryList menuEntries;			// List of the menu entires

	void Selected();					// Checks whether the a button has been selected and does an appropriate action

	// Description for the user
	SmartPtr<GLText> p_Text_Inverted;
	// Description for the user
	SmartPtr<GLText> p_Text_Sense;

	SmartPtr<MenuTickBox> p_TickBox;	// THe Tick box
	SmartPtr<MenuScrollBar> p_ScrollBar;// The Scroll bar

	GL2DBox flash;						// THe White flash screen if switching from one screen to another

	static MouseOptionsState* mMouseOptionsState;
	MouseOptionsState();

	// A temprorary value that will be displayed to show whther or not the mouse controls are inverted
	// This will only be used as display, due to the user being able to play around with the value before
	// saving it 
	bool temp_Inverted;

	// Call the message box by specifying which Message Box to call up
	void CallMessageBox();
	// Set whether or not the mouse control should be inverted
	void SetInverted(bool value);
	// Default the values
	void Default();
	// Save the remapped values
	void Save();
	// Check if any changes were made and if true call up the save message box
	bool CheckChanges();
};

#endif