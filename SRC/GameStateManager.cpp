#include "GameStateManager.h"
#include "GameState.h"
#include "GameWindow.h"
#include "GameUtil.h"

/** Constructor */
GameStateManager::GameStateManager()
{
	messageBoxActive = false;
}

GameStateManager::~GameStateManager()
{
}

//////////////////////////////////////////IKeyCharListener/////////////////////////////////
// Listens for any character key presses directly from windows events - used for name entering
void GameStateManager::OnCharKeyDown(WPARAM param)
{
	states.back()->OnCharKeyDown(param);
}
///////////////////////////////////////////////////////////////////////////////////////////

/** Change the current State into a new state */ 
void GameStateManager::ChangeState(GameState* state)
{
	// If the vector is not empty remove the element in the back
	if(!states.empty()) {
		states.back()->Exit();
		states.pop_back();
	}

	// Add the new state into the vector and initialise it 
	states.push_back(state);
	states.back()->Init(this);
}

/** Add a new state into the vector and intitialise it */
void GameStateManager::AddState(GameState* state)
{
	states.push_back(state);
	states.back()->Init(this);
}

/** Remove the state in the back of the vector(current active state) */
void GameStateManager::RemoveState()
{
	if(!states.empty()) {
		states.back()->Exit();
		states.pop_back();
	}

	if(!states.empty())
		states.back()->Resume();
}

// Empties all the states out
void GameStateManager::ClearStates(GameState* state)
{
	if(!states.empty()) {
		for (vector< GameState* >::const_iterator it = states.begin(); it != states.end(); ++it)
			(*it)->Exit();
		states.clear();
	}

	states.push_back(state);
	states.back()->Init(this);
}

/** Update the last object in the vector(current state) */ 
void GameStateManager::Update(float dt)
{
	states.back()->Update(dt);
}

/** Render the last object in the vector (current state) */
void GameStateManager::Render()
{
	if(messageBoxActive){
		for(uint i = 0; i <states.size(); i++){
			states[i]->Render();
		}
	} else
		states.back()->Render();
}


void GameStateManager::FadeBackBuffer(bool value)
{
	if (value) {
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glLoadIdentity();

	glTranslatef(0.0, 0.0, -7.0);
	glColor4f(0.0f, 0.0f, 0.0f, 0.8f);
	glBegin(GL_QUADS);
		glVertex3f(-4.5f, 3.5f, 0.0f);              // Top Left
        glVertex3f( 4.5f, 3.5f, 0.0f);              // Top Right
        glVertex3f( 4.5f,-3.5f, 0.0f);              // Bottom Right
        glVertex3f(-4.5f,-3.5f, 0.0f);
	glEnd();

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_ONE,GL_ONE);
	}
}