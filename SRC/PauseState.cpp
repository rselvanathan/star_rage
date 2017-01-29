#include "GameUtil.h"
#include "PauseState.h"
#include "MainMenuState.h"
#include "MessageBox.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "GameplayState.h"
#include "MenuEntry.h"
#include "InputManager.h"
#include "PreLoader.h"
#include "Texture.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

PauseState* PauseState::mPauseState;

PauseState::PauseState() : GameState(ScreenIDList::GetInstance()->pause)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

PauseState::~PauseState()
{
	GameState::~GameState();
}

void PauseState::Init(GameStateManager* mGameState)
{
	background.Init(800, 400, 250, 500); 
	background.SetTexture(PreLoader::GetInstance()->pauseBox->GetTGATexture().texID);

	background_mask.Init(800, 400, 250, 500); 
	background_mask.SetTexture(PreLoader::GetInstance()->messageBox_mask->GetTGATexture().texID);

	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Resume", PreLoader::GetInstance()->resumeButton->GetTGATexture().texID,
			400, 600));
		menuEntries.push_back(new MenuEntry("Quit", PreLoader::GetInstance()->quitButton->GetTGATexture().texID,
			800, 600));
	}

	mInactive = mMessageActive = false;
	alpha = 0.0f;
	m_button_state = 0;
		
	mGameStateManager = mGameState;
}

void PauseState::Resume()
{
	mMessageActive = false;
}

void PauseState::Update(float dt)
{
	// Check whether a button was pressed and which button hit was
	if(m_button_state == RESUME)
	{
		// Go back to the Gameplay screen
		mGameStateManager->SetMessageActive(false);
		mGameStateManager->RemoveState();
		//Pop the current Context and add the new context for the input
		InputManager::Instance()->PopContext();
		InputManager::Instance()->PushContext(L"maincontext");
		// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
		// Without this the inactive screens will be updated which will cause issues
		InputManager::Instance()->RemoveMenuButtonListener(this);
		// Add the the Gameplay screen as a listener to the buttons
		InputManager::Instance()->AddButtonListener(GameplayState::GetInstance());
		InputManager::Instance()->AddMousePosListener(GameplayState::GetInstance());
		InputManager::Instance()->HideMouseCursor();
		m_button_state = 0;
	} else if (m_button_state == QUIT) {
		mMessageActive = true;
		mGameStateManager->SetMessageActive(true);
		mGameStateManager->AddState(MessageBoxState::GetInstance(MessageBoxState::PAUSE));
		// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
		// Without this the inactive screens will be updated which will cause issues
		InputManager::Instance()->RemoveMenuButtonListener(this);
		// Add the the Messagebox screen as a listener to the buttons
		InputManager::Instance()->AddMenuButtonListener(MessageBoxState::GetInstance(MessageBoxState::PAUSE));
		m_button_state = 0;
	}

	// Update all the buttons - the glow effect when hovering over them
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();
}

void PauseState::Render()
{
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	background_mask.Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	background.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	// Render Each button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();
	
}


void PauseState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Resume"){
				m_button_state = RESUME; 
				return;
			}
		if((*it)->IsClicked() && (*it)->GetName() == "Quit"){
			m_button_state = QUIT;
			return;
		}
	}
}

//---------------------------------------IMenuButtonListener-------------------------------------
void PauseState ::OnMouseClick()
{
	Selected();
}
void PauseState ::OnUpPressed() {return;}
void PauseState ::OnDownPressed() {return;}
void PauseState ::OnLeftPressed() {return;}
void PauseState ::OnRightPressed() {return;}
void PauseState ::OnSelectPressed() {return;}
void PauseState ::OnMouseMove(float x, float y)
{
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------


