/*
The message box State. It is a singleton class
This class will will popup telling the user to press a key. This will warn the user that next key press will
rebind the action/state to the new raw input button.

Author : Romesh Selvanathan
Date : 10/12
*/

#ifndef __MESSAGEBOX_KEYBIND_H__
#define __MESSAGEBOX_KEYBIND_H__

#include "GameUtil.h"
#include "GameState.h"
#include "GL2DBox.h"
#include "IRawButtonListener.h"

class GL2DText;
class GameStateManager;

class MessageBox_KeybindState : public GameState, public IRawButtonListener
{
public:
	static MessageBox_KeybindState* GetInstance()
	{
		if(mMessageBox == NULL)
			mMessageBox = new MessageBox_KeybindState();
		return mMessageBox;
	} 

	virtual ~MessageBox_KeybindState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume(){}

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg){return false;}

	const RawInputButton GetButton() {return m_currentButton;}

	//---------------------------------------IRawButtonListener-------------------------------------
	void OnRawButtonPress(RawInputButton button);
	//-----------------------------------------------------------------------------------------------

private:
	static MessageBox_KeybindState* mMessageBox;
	MessageBox_KeybindState();

	GL2DBox background;					// The background image for this screen
	GL2DBox background_mask;			// The mask for the background

	RawInputButton m_currentButton;
};

#endif