/*
The Options State. It is a singleton class
This class will allow the user to select an option and change some of the parameters to enable
a better game experience.

Author : Romesh Selvanathan
Date : 10/12
*/

#include "GameUtil.h"
#include "OptionsState.h"
#include "MainMenuState.h"
#include "MouseOptionsState.h"
#include "KeybindsOptionsState.h"
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

OptionsState* OptionsState::mOptionsState;

OptionsState::OptionsState() : GameState(ScreenIDList::GetInstance()->options)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

OptionsState::~OptionsState()
{
	// If the list is not empty destroy all objects in there
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}

	GameState::~GameState();
}

void OptionsState::Init(GameStateManager* mGameState)
{
	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Keybinds", PreLoader::GetInstance()->keybindsButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 300));
		menuEntries.push_back(new MenuEntry("Mouse", PreLoader::GetInstance()->mouseButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 410));
		menuEntries.push_back(new MenuEntry("Menu", PreLoader::GetInstance()->menuButton->GetTGATexture().texID,
			100, WINDOW_HEIGHT - 520));
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

void OptionsState::Resume()
{
	mMessageActive = false;
}

void OptionsState::Update(float dt)
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

	if(m_switchTo_state == KEYBINDS){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(KeybindsOptionsState::GetInstance());	
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(KeybindsOptionsState::GetInstance());
			m_switchTo_state = 0;
		}
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	} else if(m_switchTo_state == MOUSE){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(MouseOptionsState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MouseOptionsState::GetInstance());
			m_switchTo_state = 0;
		}	
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	} else if(m_switchTo_state == MENU){
		// if it is fully blended in change state
		if(alpha >= 1.0f) {
			mGameStateManager->ChangeState(MainMenuState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MainMenuState::GetInstance());
			m_switchTo_state = 0;
		}	
		else {
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f) * 1.5;
		}
	}

	// Update all the buttons - the glow effect when hovering over them
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();

	// the value 0.0xx controls how quickly the glow occurs - higher the number the more times it flashes brightly
	cosAdd += 0.081;
	// using the cosing graph to obtain value between 0 and 1 to create the pulsating glow effect
	mGlow = (float)cos(cosAdd);
}

void OptionsState::Render()
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
	if(m_switchTo_state == KEYBINDS || m_switchTo_state == MOUSE|| m_switchTo_state == MENU) {
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

void OptionsState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Keybinds"){
			m_switchTo_state = KEYBINDS;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Mouse"){
			m_switchTo_state = MOUSE;
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Menu"){
			m_switchTo_state = MENU;
			return;
		}
	}
}

//---------------------------------------IMenuButtonListener-------------------------------------
void OptionsState ::OnMouseClick()
{
	Selected();
}
void OptionsState ::OnUpPressed() 
{
	return;
}
void OptionsState ::OnDownPressed() 
{
	return;
}
void OptionsState ::OnLeftPressed() 
{
	return;
}
void OptionsState ::OnRightPressed() 
{
	return;
}
void OptionsState ::OnSelectPressed() 
{
	return;
}

void OptionsState ::OnMouseMove(float x, float y)
{
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------------------------------------------------------------------------


