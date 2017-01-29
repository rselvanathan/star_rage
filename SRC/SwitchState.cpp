/*
The Loading state which is the loading screen before the game begins. It is a singleton class
This class will automatically  move onto the game play screen when the timer is ready.

Author : Romesh Selvanathan
Date : 3/05/12
*/
#include "GameUtil.h"
#include "GameStateManager.h"
#include "SwitchState.h"
#include "GameState.h"
#include "GameplayState.h"
#include "GLText.h"
#include "PauseState.h"
#include "PreLoader.h"
#include "GL2DBox.h"
#include "InputManager.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

SwitchState* SwitchState::mSwitchState;

SwitchState::SwitchState() : GameState(ScreenIDList::GetInstance()->intro)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

SwitchState::~SwitchState()
{
	SAFE_DELETE(loadingText);
	GameState::~GameState();
}

// Initialise the values
void SwitchState::Init(GameStateManager* mGameState)
{
	// set up the text
	loadingText = new GLText();
	loadingText->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	loadingText->SetColor3f(1, 1, 1);
	loadingText->SetPosition(WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT - 850);

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	// Set up the Progress bar
	m_progressBar.Init(0, 15, WINDOW_WIDTH/2 - 156, WINDOW_HEIGHT - 900);
	m_progressBar.SetColor(0, 1, 0, 1);
	progress = 0;

    mInactive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;
	mGameStateManager = mGameState;

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->help_background->GetTGATexture().texID);

	// Create Title Texture and set the Two Title squares up
	title.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	title.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	titleGlow.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	titleGlow.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	mGlow = 0;
	cosAdd = 0;

	// Pre Load the Pause State, if it has not been loaded. This is done so there is no delay, 
	// when the game play screen calls on the paus screen
	PauseState::GetInstance();
}

void SwitchState::Update(float dt)
{
	if(mInactive){
		// if it is fully blended in change state
		if(alpha >= 1.0f){
			mGameStateManager->ChangeState(GameplayState::GetInstance());
			//Pop the current Context and add the new context for the input
			InputManager::Instance()->PopContext();
			InputManager::Instance()->PushContext(L"maincontext");
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->AddButtonListener(GameplayState::GetInstance());
			InputManager::Instance()->AddMousePosListener(GameplayState::GetInstance());
			InputManager::Instance()->HideMouseCursor();
		}
		else 
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f);
	}

	
	if(mJustSwitched){
		if(switchAlpha < 0)
			mJustSwitched = false;
		switchAlpha -= 0.015f * 2;
	}

	// If progress is still going then increase the progress bar. Once the
	// progress bar is done then switch
	if(progress < 312) {
		progress += 50 * dt;
		m_progressBar.SetWidth(progress);
	} else mInactive = true;
	

	// the value 0.0xx controls how quickly the glow occurs - higher the number the more times it flashes brightly
	cosAdd += 0.081;
	// using the cosing graph to obtain value between 0 and 1 to create the pulsating glow effect
	mGlow = (float)cos(cosAdd);
}

void SwitchState ::Render()
{
	// Draw background
	background.Render();

	m_progressBar.Render();
	
	loadingText->print("Loading...");

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Render the title and the title glow effect behind the title to create the flashing glow
	title.Render();
	titleGlow.SetColor(1, 1, 1, mGlow);
	titleGlow.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();	
	// Render the Fade in and out effect
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