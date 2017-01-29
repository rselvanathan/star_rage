/*
The help State. It is a singleton class
This class will allow the user to view the help screen and allow them to go back to the menu.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#include "GameUtil.h"
#include "GameStateManager.h"
#include "HelpState.h"
#include "GameState.h"
#include "MainMenuState.h"
#include "GL2DText.h"
#include "MenuEntry.h"
#include "PreLoader.h"
#include "Texture.h"
#include "GL2DBox.h"
#include "InputManager.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

HelpState* HelpState::mHelpState;

HelpState::HelpState() : GameState(ScreenIDList::GetInstance()->help)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

HelpState::~HelpState()
{
	SAFE_DELETE(p_button);
	GameState::~GameState();
}

void HelpState::Init(GameStateManager* mGameState)
{
	mInactive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	mGameStateManager = mGameState;

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->help_background->GetTGATexture().texID);

	// Set up the button
	p_button = new MenuEntry("Go",PreLoader::GetInstance()->menuButton->GetTGATexture().texID, WINDOW_WIDTH/2 - 100, WINDOW_HEIGHT - 900);
}

void HelpState::Selected()
{
	// If it has move to the next state
	if(p_button->IsClicked())
		mInactive = true;
}


void HelpState::Update(float dt)
{
	// If the screen was entered just now, then increase the switch Alpha
	if(mJustSwitched){
		if(switchAlpha < 0)
			mJustSwitched = false;
		switchAlpha -= 0.015f * 2;
	}

	if(mInactive){
		// if it is fully blended in change state
		if(alpha >= 1.0f){
			mGameStateManager->ChangeState(MainMenuState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MainMenuState::GetInstance());
		}
		else 
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f);
	}

	// Update every button
	p_button->Update();
}

void HelpState ::Render()
{
	// CLear the screen
	Graphics::GetInstance()->ClearScreen();

	// Draw background
	background.Render();

	// Render the button
	p_button->Render();

	// Allow blending and render the two fade in and out effects, when they are active
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	Graphics::GetInstance()->DisableDepthTest();
	if(mInactive) {
		flash.SetColor(1, 1, 1, alpha);
		flash.Render();
	}

	if(mJustSwitched) {
		flash.SetColor(1, 1, 1, switchAlpha);
		flash.Render();
	}
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->AllowDepthTest();
}

//---------------------------------------IMenuButtonListener-------------------------------------
void HelpState ::OnMouseClick()
{
	Selected();
}
void HelpState ::OnUpPressed() {return;}
void HelpState ::OnDownPressed() {return;}
void HelpState ::OnLeftPressed() {return;}
void HelpState ::OnRightPressed() {return;}
void HelpState ::OnSelectPressed() 
{
	mInactive = true;
}
void HelpState ::OnMouseMove(float x, float y)
{
	// Update The Mouse Position
	p_button->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------