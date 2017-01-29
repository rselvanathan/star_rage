/*
The message box State. It is a singleton class
This class will allow user to quit and get to the high score screen or quit the game completly. 
It is a way of making sure that the user can cancel his decision.
This screen will also be used to prompt the user before saving anything.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#include "GameUtil.h"
#include "GameState.h"
#include "MenuEntry.h"
#include "GameStateManager.h"
#include "HighScoreState.h"
#include "GameplayState.h"
#include "MainMenuState.h"
#include "MouseOptionsState.h"
#include "PauseState.h"
#include "KeybindsOptionsState.h"
#include "GL2DText.h"
#include "MessageBox.h"
#include "InputManager.h"
#include "PreLoader.h"
#include "Texture.h"
#include "ClassManager.h"
#include "ScreenIDList.h"
#include "MessageDispatcher.h"
#include "ScreenMessageTypes.h"

MessageBoxState* MessageBoxState::mMessageBox;

MessageBoxState::MessageBoxState() : GameState(ScreenIDList::GetInstance()->message_box)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

MessageBoxState::~MessageBoxState()
{
	// If the list is not empty destroy all objects in there
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}

	GameState::~GameState();
}

void MessageBoxState::Init(GameStateManager* mGameState)
{
	mGameStateManager = mGameState;

	background.Init(800, 400, 250, 500); 

	if(state == KEYBINDS)
		background.SetTexture(PreLoader::GetInstance()->messageBox_save->GetTGATexture().texID);
	else 
		background.SetTexture(PreLoader::GetInstance()->messageBox->GetTGATexture().texID);

	background_mask.Init(800, 400, 250, 500); 
	background_mask.SetTexture(PreLoader::GetInstance()->messageBox_mask->GetTGATexture().texID);

	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Ok", PreLoader::GetInstance()->okButton->GetTGATexture().texID,
			400, 500));
		menuEntries.push_back(new MenuEntry("Cancel", PreLoader::GetInstance()->cancelButton->GetTGATexture().texID,
			800, 500));
	}

	button_mask_ok.Init(100, 50, 400, 500);
	button_mask_ok.SetTexture( PreLoader::GetInstance()->buttonMask->GetTGATexture().texID);
	button_mask_cancel.Init(100, 50, 800, 500);
	button_mask_cancel.SetTexture( PreLoader::GetInstance()->buttonMask->GetTGATexture().texID);

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
	mInactive = m_cancel = false;
	alpha = 0.0f;
}

void MessageBoxState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Ok"){
			mInactive = true;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Cancel"){
			m_cancel = true;
			return;
		}
	}
}

void MessageBoxState::Update(float dt)
{
	if(mInactive){
		// If the state is Keybinds or Mouse remove the screen rather than switching to another screen with a flash screen
		if(state == KEYBINDS || state == MOUSE) {
			mGameStateManager->SetMessageActive(false);
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			// Depending on the calling screen
			if(state == KEYBINDS) {
				InputManager::Instance()->AddMenuButtonListener(KeybindsOptionsState::GetInstance());
				// Set what choice was made
				MessageDispatcher::GetInstance()->DispatchMessageA(0, ScreenIDList::GetInstance()->message_box,
					ScreenIDList::GetInstance()->keybind_options, ScreenMessageType::msg_messageBox_ok, NULL);
			}
			else if (state == MOUSE) {  
				InputManager::Instance()->AddMenuButtonListener(MouseOptionsState::GetInstance());
				// Set what choice was made
				MessageDispatcher::GetInstance()->DispatchMessageA(0, ScreenIDList::GetInstance()->message_box,
					ScreenIDList::GetInstance()->mouse_options, ScreenMessageType::msg_messageBox_ok, NULL);
			}
			mGameStateManager->RemoveState();
		}
		else {
			// if it is fully blended in change state
			if(alpha >= 1.0f){
				if(state == MessageBoxState::PAUSE){
					HighScoreState::GetInstance()->SetCurrentScore(GameplayState::GetInstance()->GetScore());
					mGameStateManager->ClearStates(HighScoreState::GetInstance());
					// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
					// Without this the inactive screens will be updated which will cause issues
					InputManager::Instance()->RemoveMenuButtonListener(this);
					InputManager::Instance()->AddMenuButtonListener(HighScoreState::GetInstance());
				}
				else
					PostQuitMessage(0);
			}
			else {
				// else do the alpha calculation to fade out
				alpha += (float)(0.015f) * 1.5;
			}
		}
	}

	if(m_cancel) {
		// If the state is Keybinds or Mouse remove the screen with a message 
		if(state == KEYBINDS || state == MOUSE) {
			mGameStateManager->SetMessageActive(false);
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			// Depending on the calling screen
			if(state == KEYBINDS) {
				InputManager::Instance()->AddMenuButtonListener(KeybindsOptionsState::GetInstance());
				// Set what choice was made
				MessageDispatcher::GetInstance()->DispatchMessageA(0, ScreenIDList::GetInstance()->message_box,
					ScreenIDList::GetInstance()->keybind_options, ScreenMessageType::msg_messageBox_cancel, NULL);
			}
			else if (state == MOUSE) {  
				InputManager::Instance()->AddMenuButtonListener(MouseOptionsState::GetInstance());
				// Set what choice was made
				MessageDispatcher::GetInstance()->DispatchMessageA(0, ScreenIDList::GetInstance()->message_box,
					ScreenIDList::GetInstance()->mouse_options, ScreenMessageType::msg_messageBox_cancel, NULL);
			}
		}
		else if(state == MessageBoxState::MAIN_MENU) {
				mGameStateManager->SetMessageActive(false);
				// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
				// Without this the inactive screens will be updated which will cause issues
				InputManager::Instance()->RemoveMenuButtonListener(this);
				InputManager::Instance()->AddMenuButtonListener(MainMenuState::GetInstance());
			}
		else {
				// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
				// Without this the inactive screens will be updated which will cause issues
				InputManager::Instance()->RemoveMenuButtonListener(this);
				InputManager::Instance()->AddMenuButtonListener(PauseState::GetInstance());
			}
		m_cancel = false;
		mGameStateManager->RemoveState();
	}

	// Update all the buttons - the glow effect when hovering over them
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();
}

void MessageBoxState::Render()
{
	// Render the different masks
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	button_mask_ok.Render();
	button_mask_cancel.Render();
	background_mask.Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	background.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	// Render Each button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();

	// Render the Flashing when screen state is changing
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	Graphics::GetInstance()->DisableDepthTest();
	if(mInactive) {
		flash.SetColor(1, 1, 1, alpha);
		flash.Render();
	}
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->AllowDepthTest();
}

//---------------------------------------IMenuButtonListener-------------------------------------
void MessageBoxState ::OnMouseClick()
{
	Selected();
}
void MessageBoxState ::OnUpPressed() {return;}
void MessageBoxState ::OnDownPressed() {return;}
void MessageBoxState ::OnLeftPressed() {return;}
void MessageBoxState ::OnRightPressed() {return;}
void MessageBoxState ::OnSelectPressed() {return;}
void MessageBoxState ::OnMouseMove(float x, float y) 
{
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------