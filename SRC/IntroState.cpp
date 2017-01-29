/*
The intro state is the first screen state the user will enter. It is a singleton class
This class will allow user to move onto the main menu state.

Author : Romesh Selvanathan
Date : 3/05/12
*/
#include "GameState.h"
#include "GameUtil.h"
#include "GameStateManager.h"
#include "MainMenuState.h"
#include "IntroState.h"
#include "Texture.h"
#include "Graphics.h"
#include "GL2DBox.h"
#include "MenuEntry.h"
#include "PreLoader.h"
#include "InputManager.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

IntroState* IntroState::mIntroState;

IntroState::IntroState() : GameState(ScreenIDList::GetInstance()->intro)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

// Destructor
IntroState::~IntroState()
{
	SAFE_DELETE(p_button);
	GameState::~GameState();
}

// Initialize the state
void IntroState::Init(GameStateManager* mGameState)
{
	flashing = 0;
	alpha = 0.0f;
	mInactive = false;
	mGameStateManager = mGameState;

	// Set up the different 2D quads
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->background->GetTGATexture().texID);

	// Create Title Texture and set the Two Title squares up
	title.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	title.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	titleGlow.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	titleGlow.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	mGlow = 0;

	// Set up the button
	p_button = new MenuEntry("Go", PreLoader::GetInstance()->introButton->GetTGATexture().texID, 100 , WINDOW_HEIGHT - 400);
}

void IntroState::Selected()
{
	// if btton has been pressed then move onto the next state
	if(p_button->IsClicked())
		mInactive = true;
}

void IntroState::Update(float dt)
{
	if(mInactive){
		// if it is fully blended in change state
		if(alpha >= 0.5f) {
			mGameStateManager->ChangeState(MainMenuState::GetInstance());	
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MainMenuState::GetInstance());
		}
		else 
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
	}

	p_button->Update();

	// the value 0.0xx controls how quickly the glow occurs - higher the number the more times it flashes brightly
	flashing += 0.081;
	// using the cosing graph to obtain value between 0 and 1 to create the pulsating glow effect
	mGlow = (float)cos(flashing);
}

void IntroState::Render()
{
	Graphics::GetInstance()->ClearScreen();

	// Draw background
	background.Render();

	p_button->Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Render the title and the title glow effect behind the title to create the flashing glow
	title.Render();
	titleGlow.SetColor(1, 1, 1, mGlow);
	titleGlow.Render();
	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	// Render the fade out effect
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	if(mInactive) {
		flash.SetColor(1, 1, 1, alpha);
		flash.Render();
	}
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();
}

//---------------------------------------IMenuButtonListener-------------------------------------
void IntroState ::OnMouseClick()
{
	Selected();
}
void IntroState ::OnUpPressed() {return;}
void IntroState ::OnDownPressed() {return;}
void IntroState ::OnLeftPressed() {return;}
void IntroState ::OnRightPressed() {return;}

void IntroState ::OnSelectPressed() 
{
	mInactive = true;
}

void IntroState ::OnMouseMove(float x, float y)
{
	p_button->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------