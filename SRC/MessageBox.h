/*
The message box State. It is a singleton class
This class will allow user to quit and get to the high score screen or quit the game completly. 
It is a way of making sure that the user can cancel his decision

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __MESSAGEBOX_H__
#define __MESSAGEBOX_H__

#include "GameUtil.h"
#include "GameState.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class GL2DText;
class MenuEntry;
class GameStateManager;
class InputManager;

class MessageBoxState : public GameState, public IMenuButtonListener
{
public:
	static MessageBoxState* GetInstance(int state)
	{
		if(mMessageBox == NULL)
			mMessageBox = new MessageBoxState();
		mMessageBox->SetState(state);
		return mMessageBox;
	} 

	virtual ~MessageBoxState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume(){}

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg){return false;}

	//---------------------------------------IMenuButtonListener-------------------------------------
	void OnMouseClick();
	void OnUpPressed();
	void OnDownPressed();
	void OnLeftPressed();
	void OnRightPressed();
	void OnSelectPressed();
	void OnMouseMove(float x, float y);
	//-----------------------------------------------------------------------------------------------

	enum {MAIN_MENU, PAUSE, KEYBINDS, MOUSE};
	void SetState(int s) {state = s;}

private:
	typedef vector<SmartPtr<MenuEntry> > MenuEntryList;
	MenuEntryList menuEntries;			// List of menu entires

	static MessageBoxState* mMessageBox;
	MessageBoxState();

	GL2DBox background;					// The background image for this screen
	GL2DBox background_mask;			// The mask for the background

	GL2DBox button_mask_ok;				// the Ok Button mask
	GL2DBox button_mask_cancel;			// The Ok Button mask

	GL2DBox flash;						// THe White flash screen if switching from one screen to another

	void Selected();

	int state;							// A value that determines which screen state called the message box
	bool m_cancel;						// a boolean that will allow the transition from the message box to the previous screen
};

#endif