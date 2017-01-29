/*
The menu State. It is a singleton class
This class will allow user to move onto the high score state, loading screen, help state or message box.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#ifndef __MAINMENUSTATE_H__
#define __MAINMENUSTATE_H__

#include "GameUtil.h"
#include "GameState.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class MenuEntry;
class InputManager;
class Texture;

class MainMenuState : public GameState, public IMenuButtonListener
{
public :
	static MainMenuState* GetInstance()
	{
		if(mMainMenuState == NULL)
			mMainMenuState = new MainMenuState();
		return mMainMenuState;
	} 

	virtual ~MainMenuState();

	void Init(GameStateManager* mGameState);
	void Exit(){}
	void Pause(){}
	void Resume();

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
	typedef vector<SmartPtr<MenuEntry> > MenuEntryList;
	MenuEntryList menuEntries;			// List of the menu entires

	static MainMenuState* mMainMenuState;
	MainMenuState();

	void Selected();					// Checks Which button has been pressed and allows the desired effect

	GL2DBox background;					// The background image for this screen
	GL2DBox title;						// The Title for the screen
	GL2DBox titleGlow;					// The Title Glow effect for the screen
	float mGlow;						// Value used to allo wglow effect for the title
	float cosAdd;						// Value used to allo wglow effect for the title
	
	int m_switchTo_state;				// A value that will determine which screen to switch to
	enum {SCORE = 1, HELP = 2, OPTIONS = 3, QUIT = 4};

	GL2DBox flash;						// THe White flash screen if switching from one screen to another
};

#endif