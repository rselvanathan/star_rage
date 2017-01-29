/*
The message box State. It is a singleton class
This class will will popup telling the user to press a key. This will warn the user that next key press will
rebind the action/state to the new raw input button.

Author : Romesh Selvanathan
Date : 10/12
*/

#include "GameUtil.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "GL2DText.h"
#include "MessageBox_Keybind.h"
#include "KeybindsOptionsState.h"
#include "InputManager.h"
#include "PreLoader.h"
#include "Texture.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

MessageBox_KeybindState* MessageBox_KeybindState::mMessageBox;

MessageBox_KeybindState::MessageBox_KeybindState() : GameState(ScreenIDList::GetInstance()->message_box_rebind)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

MessageBox_KeybindState::~MessageBox_KeybindState()
{
	GameState::~GameState();
}

void MessageBox_KeybindState::Init(GameStateManager* mGameState)
{
	mGameStateManager = mGameState;

	background.Init(800, 400, 250, 500); 
	background.SetTexture(PreLoader::GetInstance()->messageBox_keybind->GetTGATexture().texID);

	background_mask.Init(800, 400, 250, 500); 
	background_mask.SetTexture(PreLoader::GetInstance()->messageBox_mask->GetTGATexture().texID);

	mInactive = false;
	alpha = 0.0f;
}

void MessageBox_KeybindState::Update(float dt)
{
	if(mInactive){
		mGameStateManager->SetMessageActive(false);
		// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
		// Without this the inactive screens will be updated which will cause issues
		InputManager::Instance()->RemoveRawButtonListener(this);
		InputManager::Instance()->AddMenuButtonListener(KeybindsOptionsState::GetInstance());
		mGameStateManager->RemoveState();
	}
}

void MessageBox_KeybindState::Render()
{
	// Render the different masks
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	background_mask.Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	background.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();
}

//---------------------------------------IMenuButtonListener-------------------------------------
void MessageBox_KeybindState::OnRawButtonPress(RawInputButton button)
{
	// Pass the button press to the keybinds options screen
	m_currentButton = button;
	mInactive = true;
}
//-----------------------------------------------------------------------------------------------