#ifndef __PAUSESTATE_H__
#define __PAUSESTATE_H__

#include "GameUtil.h"
#include "GameState.h"
#include "GL2DBox.h"
#include "IMenuButtonListener.h"

class MenuEntry;
class InputManager;

class PauseState : public GameState, public IMenuButtonListener
{
public :
	static PauseState* GetInstance()
	{
		if(mPauseState == NULL)
			mPauseState = new PauseState();
		return mPauseState;
	} 

	virtual ~PauseState();

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
	MenuEntryList menuEntries;

	GL2DBox background;					// The background image for this screen
	GL2DBox background_mask;			// The mask for the background

	static PauseState* mPauseState;
	PauseState();

	void Selected();

	int m_button_state;					// A value that will determine wheter to quit or resume back to the gameplay screen
	enum {RESUME = 1, QUIT = 2};
};

#endif