/* 
This class contains the main loop and will set up the whole application itself. It is a singleton class

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __GAMESESSION_H__
#define __GAMESESSION_H__

#include "GameUtil.h"

class GameStateManager;
class FrameTimer;

class GameSession
{
public:
	static GameSession* GetInstance()
	{
		if(!mInstance)
			mInstance = new GameSession();
		return mInstance;
	}
	~GameSession();

	// Initialise the session
	bool Init(HINSTANCE hInstance, bool fullscreenflag);
	// Update the session
	void Update();
	// Render the session
	void Render();
	// The Windows main loop
	WPARAM MainLoop();

private:
	GameSession(){};
	static GameSession* mInstance;

	// The Game State manager, that controls all the different screens
	SmartPtr<GameStateManager> mStateManager;
	// The Fram Rate Control
	SmartPtr<FrameTimer> mFps;

	int count;
};

#endif