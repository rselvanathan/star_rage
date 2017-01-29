/**
Base Class for Screens/States such as menu and and intro states

Author : Romesh Selvanathan
*/ 

#ifndef __GAMESTATE_H__
#define __GAMESTATE_H__

#include "GameWindow.h"
#include "GameStateManager.h"
#include "BaseClass.h"

class InputManager;

class GameState : public BaseClass
{
public:
	/*Constructor*/
	GameState(int& ID) : BaseClass(ID)
	{}
	/*Destructor*/
	virtual ~GameState() {SAFE_DELETE(mGameStateManager);}

	// Initialise the State
	virtual void Init(GameStateManager* mGameState) = 0;
	// Exit the Screen State 
	virtual void Exit() = 0;
	// Pause the state
	virtual void Pause() = 0;
	// Resume the State
	virtual void Resume() = 0;

	// Update the State
	virtual void Update(float dt) = 0;
	// Render the State
	virtual void Render();
	// LEGACY - Not Used
	virtual void Render2();

	// Listens for any character key presses directly from windows events - used for name entering
	virtual void OnCharKeyDown(WPARAM param) {}

	void FadeOut();

protected:
	GameState(){}									// Constructor
			
	bool mInactive, mMessageActive;					// Is Window Active/Is MessageBox Active
	float alpha;									// alpha value for fade effects

	bool mJustSwitched;								// I a screen just switched as the new screen on view
	float switchAlpha;								// THe alpha fade in value 

	virtual void OnSelected(int selectedEntry){}	
	int selectedEntry;								// The selected menu entry

	GameStateManager* mGameStateManager;			// Pointer to the Gamestate Manager class
};

#endif