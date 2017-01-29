/*
The intro state is the first screen state the user will enter. It is a singleton class
This class will allow user to move onto the main menu state.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __INTROSTATE_H__
#define __INTROSTATE_H__

#include "GameState.h"
#include "GameUtil.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class MenuEntry;

class IntroState : public GameState, public IMenuButtonListener
{
public:
	static IntroState* GetInstance()
	{
		if(mIntroState == NULL)
			mIntroState = new IntroState();
		return mIntroState;
	} 
	virtual ~IntroState();

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
	static IntroState* mIntroState;
	IntroState();

	void Selected();					// Checks Which button has been pressed and allows the desired effect

	GL2DBox flash;						// THe White flash screen if switching from one screen to another

	GL2DBox background;					// The background image for this screen
	GL2DBox title;						// The Title for the screen
	GL2DBox titleGlow;					// The Title Glow effect for the screen
	float mGlow;						// Value used to allo wglow effect for the title

	SmartPtr<MenuEntry> p_button;		// The single Button on the screen

	float flashing;						// Determines the flashing behaviour of the title
};

#endif