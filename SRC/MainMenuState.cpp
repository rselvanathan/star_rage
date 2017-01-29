/*
The menu State. It is a singleton class
This class will allow user to move onto the high score state, loading screen, help state or message box.

Author : Romesh Selvanathan
Date : 3/05/12
*/

#include "GameUtil.h"
#include "MainMenuState.h"
#include "SwitchState.h"
#include "HighScoreState.h"
#include "HelpState.h"
#include "OptionsState.h"
#include "MessageBox.h"
#include "GameState.h"
#include "GameStateManager.h"
#include "MenuEntry.h"
#include "InputManager.h"
#include "Graphics.h"
#include "Texture.h"
#include "GL2DBox.h"
#include "PreLoader.h"
#include "ClassManager.h"
#include "ScreenIDList.h"

MainMenuState* MainMenuState::mMainMenuState;

MainMenuState::MainMenuState() : GameState(ScreenIDList::GetInstance()->main_menu)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

MainMenuState::~MainMenuState()
{
	// If the list is not empty destroy all objects in there
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}

	GameState::~GameState();
}

void MainMenuState::Init(GameStateManager* mGameState)
{
	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Start", PreLoader::GetInstance()->startButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 300));
		menuEntries.push_back(new MenuEntry("Help", PreLoader::GetInstance()->helpButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 410));
		menuEntries.push_back(new MenuEntry("Score", PreLoader::GetInstance()->scoreButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 520));
		menuEntries.push_back(new MenuEntry("Options", PreLoader::GetInstance()->optionsButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 630));
		menuEntries.push_back(new MenuEntry("Quit", PreLoader::GetInstance()->quitButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 740));
	}

	mInactive = mMessageActive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;
		
	mGameStateManager = mGameState;

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->background->GetTGATexture().texID);

	// Create Title Texture and set the Two Title squares up
	title.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	title.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	titleGlow.Init(500, 200, 30, WINDOW_HEIGHT - 100);
	titleGlow.SetTexture(PreLoader::GetInstance()->title->GetTGATexture().texID);
	mGlow = 0;
	cosAdd = 0;
	m_switchTo_state = 0;
}

void MainMenuState::Resume()
{
	mMessageActive = false;
}

void MainMenuState::Update(float dt)
{
	// Has this screen been switched to recently?
	if(mJustSwitched){
		// if the alpha for switching is less than 0
		if(switchAlpha < 0)
			// then switch of the fade In Effect
			mJustSwitched = false;
		// Decrease the alpha
		switchAlpha -= 0.015f * 2;
	}
	// If this screen is inactive then 
	if(mInactive){
		// if it is fully blended then change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(SwitchState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
		}
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	}

	if(m_switchTo_state == SCORE){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(HighScoreState::GetInstance());	
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(HighScoreState::GetInstance());
			m_switchTo_state = 0;
		}
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	} else if(m_switchTo_state == HELP){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(HelpState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(HelpState::GetInstance());
			m_switchTo_state = 0;
		}	
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	} else if (m_switchTo_state == OPTIONS) {
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(OptionsState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(OptionsState::GetInstance());
			m_switchTo_state = 0;
		}	
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	} else if (m_switchTo_state == QUIT) {
		mMessageActive = true;
		mGameStateManager->SetMessageActive(true);
		mGameStateManager->AddState(MessageBoxState::GetInstance(MessageBoxState::MAIN_MENU));
		// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
		// Without this the inactive screens will be updated which will cause issues
		InputManager::Instance()->RemoveMenuButtonListener(this);
		InputManager::Instance()->AddMenuButtonListener(MessageBoxState::GetInstance(MessageBoxState::MAIN_MENU));
		m_switchTo_state = 0;
	}

	// Update all the buttons - the glow effect when hovering over them
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();

	// the value 0.0xx controls how quickly the glow occurs - higher the number the more times it flashes brightly
	cosAdd += 0.081;
	// using the cosing graph to obtain value between 0 and 1 to create the pulsating glow effect
	mGlow = (float)cos(cosAdd);
}

void MainMenuState::Render()
{
	Graphics::GetInstance()->ClearScreen();

	/*if(mMessageActive)
		mGameStateManager->FadeBackBuffer(true);*/

	// Draw background
	background.Render();

	// Render Each button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();

	// Allow additive blending GL_ONE to dest and SRC blending using alpha channel
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Render the title and the title glow effect behind the title to create the flashing glow
	title.Render();
	titleGlow.SetColor(1, 1, 1, mGlow);
	titleGlow.Render();
	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCAlpha();
	Graphics::GetInstance()->DisableDepthTest();
	if(mInactive ||  m_switchTo_state == SCORE || m_switchTo_state == HELP|| m_switchTo_state == OPTIONS) {
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

void MainMenuState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Start"){
			mInactive = true;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Quit"){
			m_switchTo_state = QUIT;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Score"){
			m_switchTo_state = SCORE;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Help"){
			m_switchTo_state = HELP;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Options"){
			m_switchTo_state = OPTIONS;
			return;
		}
	}
}

//---------------------------------------IMenuButtonListener-------------------------------------
void MainMenuState ::OnMouseClick()
{
	Selected();
}
void MainMenuState ::OnUpPressed() 
{
	return;
}
void MainMenuState ::OnDownPressed() 
{
	return;
}
void MainMenuState ::OnLeftPressed() 
{
	return;
}
void MainMenuState ::OnRightPressed() 
{
	return;
}
void MainMenuState ::OnSelectPressed() 
{
	return;
}
void MainMenuState::OnMouseMove(float x, float y)
{
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------


