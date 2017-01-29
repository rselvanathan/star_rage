/*
The Loading state which is the loading screen before the game begins. It is a singleton class
This class will automatically  move onto the game play screen when the timer is ready.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __SWITCHSTATE_H__
#define __SWITCHSTATE_H__

#include "GameState.h"
#include "GameUtil.h"
#include "GL2DBox.h"

class GLText;
class InputManager;

class SwitchState : public GameState
{
public:
	static SwitchState* GetInstance()
	{
		if(mSwitchState == NULL)
			mSwitchState = new SwitchState();
		return mSwitchState;
	} 

	virtual ~SwitchState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume(){}

	void Update(float dt);
	virtual void Render();
	bool HandleMessage(const Telegram& msg){return false;}

private:
	GLText* loadingText;				// The loading text

	GL2DBox background;					// The background image for this screen
	GL2DBox title;						// The Title for the screen
	GL2DBox titleGlow;					// The Title Glow effect for the screen
	float mGlow;						// Value used to allo wglow effect for the title
	float cosAdd;						// Value used to allo wglow effect for the title

	GL2DBox flash;						// THe White flash screen if switching from one screen to another
	GL2DBox m_progressBar;				// The progress bard
	float progress;						// The progress value

	static SwitchState* mSwitchState;
	SwitchState();
};

#endif