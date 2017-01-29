/*
The help State. It is a singleton class
This class will allow the user to view the help screen and allow them to go back to the menu.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __HELPSTATE_H_
#define __HELPSTATE_H_

#include "GameState.h"
#include "GameUtil.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class GL2DText;
class MenuEntry;

class HelpState : public GameState, public IMenuButtonListener
{
public:
	static HelpState* GetInstance()
	{
		if(mHelpState == NULL)
			mHelpState = new HelpState();
		return mHelpState;
	} 

	virtual ~HelpState();

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

private:
	GL2DBox background;					// The background image for this screen

	SmartPtr<MenuEntry> p_button;		// The single Button on the screen
	void Selected();					// Checks whether the a button has been selected and does an appropriate action

	GL2DBox flash;						// THe White flash screen if switching from one screen to another

	static HelpState* mHelpState;
	HelpState();
};

#endif