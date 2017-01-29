/**
Manages The different GameStates and changing the states as required.

Author : Romesh Selvanathan
*/
#ifndef __GAMESTATEMANAGER_H__
#define __GAMESTATEMANAGER_H__

#include "GameUtil.h"
#include "IKeyCharListener.h"

class GameState;
class InputManager;

class GameStateManager : public IKeyCharListener
{
public :
	GameStateManager();
	~GameStateManager();

	// Update all current states
	void Update(float dt);
	// Render current State
	void Render();

	// Change to another state
	void ChangeState(GameState* state);
	// Add a state into the list
	void AddState(GameState* state);
	// Remove the last state added
	void RemoveState();
	// Clear all the states
	void ClearStates(GameState* state);
	// Fade the State that is behind the currrent one
	void FadeBackBuffer(bool value);


	//////////////////////////////////////////IKeyCharListener/////////////////////////////////
	// Listens for any character key presses directly from windows events - used for name entering
	void OnCharKeyDown(WPARAM param);
	///////////////////////////////////////////////////////////////////////////////////////////

	// Set that the message box is active
	void SetMessageActive(bool val) {messageBoxActive = val;}

private:
	vector< GameState* > states;				// A Vector of GameStates

	bool messageBoxActive;						// Is the message box active - if it is then more than one screen state will be drawn
};

#endif