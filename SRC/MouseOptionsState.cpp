/*
The Mouse Options State. It is a singleton class
This will allow the user to specify the mouse sensitivity as well as if the user wishes to play
with inverted Y rotations or not.

Author : Romesh Selvanathan
Date : 10/12
*/

#include "GameUtil.h"
#include "GameStateManager.h"
#include "MouseOptionsState.h"
#include "GameState.h"
#include "OptionsState.h"
#include "MessageBox.h"
#include "GLText.h"
#include "MenuEntry.h"
#include "MenuTickBox.h"
#include "MenuScrollBar.h"
#include "PreLoader.h"
#include "Texture.h"
#include "GL2DBox.h"
#include "InputManager.h"
#include "ClassManager.h"
#include "ScreenIDList.h"
#include "ScreenMessageTypes.h"
#include "Telegram.h"

MouseOptionsState* MouseOptionsState::mMouseOptionsState;

MouseOptionsState::MouseOptionsState() : GameState(ScreenIDList::GetInstance()->mouse_options)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

MouseOptionsState::~MouseOptionsState()
{
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}
	GameState::~GameState();
}

void MouseOptionsState::Init(GameStateManager* mGameState)
{
	mInactive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;
	mMessageActive = false;
	m_iMessageBoxState = 0;
	m_iMessageBoxChoice = 0;
	temp_Inverted = InputManager::Instance()->IsInverted();

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	mGameStateManager = mGameState;

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->background->GetTGATexture().texID);

	// Set up the button
	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Options",PreLoader::GetInstance()->optionsButton->GetTGATexture().texID, 
			WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT - 900));
		menuEntries.push_back(new MenuEntry("Default", PreLoader::GetInstance()->defaultButton->GetTGATexture().texID,
			WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT - 900));
		menuEntries.push_back(new MenuEntry("Save", PreLoader::GetInstance()->saveButton->GetTGATexture().texID,
			WINDOW_WIDTH/2 + 80, WINDOW_HEIGHT - 900));
	}

	// Setup the tick box
	p_TickBox = new MenuTickBox(WINDOW_WIDTH/2 + 100, WINDOW_HEIGHT/2 + 300);
	p_TickBox->SetTicked(temp_Inverted);

	// Temporary sensitivity value
	int sensitivity_percentage;
	InputManager::Instance()->ObtainSensitivityPercentage(sensitivity_percentage);
	// Setup the Scroll bar
	p_ScrollBar = new MenuScrollBar(WINDOW_WIDTH/2 - 400, WINDOW_HEIGHT/2 + 50, 900, sensitivity_percentage);

	// Setup the Text
	p_Text_Inverted = new GLText();
	p_Text_Inverted->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	p_Text_Inverted->SetColor3f(1, 1, 1);
	p_Text_Inverted->SetPosition(WINDOW_WIDTH/2 - 400, WINDOW_HEIGHT/2 + 300);

	p_Text_Sense = new GLText();
	p_Text_Sense->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	p_Text_Sense->SetColor3f(1, 1, 1);
	p_Text_Sense->SetPosition(WINDOW_WIDTH/2 - 400, WINDOW_HEIGHT/2 + 150);
}

void MouseOptionsState::Resume()
{
	if (m_iMessageBoxState == MessageBoxScreen::OPTIONS) {
		if (m_iMessageBoxChoice == MessageBoxChoice::OK) {
			Save();
			mInactive = true;
		}
		else
			mInactive = true;
	}
	else if (m_iMessageBoxState == MessageBoxScreen::SAVE) {
		if (m_iMessageBoxChoice == MessageBoxChoice::OK)
			Save();
		else
		{/*Do Nothing*/}
	}

	mMessageActive = false;
	m_iMessageBoxState = 0;
	m_iMessageBoxChoice = 0;
}

bool MouseOptionsState::HandleMessage(const Telegram& msg)
{
	switch(msg.m_message) {
	case ScreenMessageType::msg_messageBox_ok :
		{
			m_iMessageBoxChoice = OK;
			return true;
		}
		break;
	case ScreenMessageType::msg_messageBox_cancel :
		{
			m_iMessageBoxChoice = CANCEL;
			return true;
		}
		break;
	}

	return false;
}

void MouseOptionsState::Selected()
{
	// Go thorught each button in the list and see if it has been clicked and depending on the button
	// perform the desired effect
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++){
		if((*it)->IsClicked() && (*it)->GetName() == "Options"){
			if(CheckChanges()) {
				mMessageActive = true;
				m_iMessageBoxState = MessageBoxScreen::OPTIONS;
				return;
			} else {
				mInactive = true;
				return;
			}
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Default"){
			Default();
			return;
		}
		if((*it)->IsClicked() && (*it)->GetName() == "Save"){
			mMessageActive = true;
			m_iMessageBoxState = MessageBoxScreen::SAVE;
			return;
		}
	}

	// Check whether the tick box has been selected and if it has check whether or not it has been ticked
	if(p_TickBox->IsClicked());
		SetInverted (p_TickBox->IsTicked());
	
	p_ScrollBar->IsClicked();
}

void MouseOptionsState::CallMessageBox()
{
	// Add the message box and make this a state inactive
	mGameStateManager->SetMessageActive(true);
	mGameStateManager->AddState(MessageBoxState::GetInstance(MessageBoxState::MOUSE));
	InputManager::Instance()->RemoveMenuButtonListener(this);
	InputManager::Instance()->AddMenuButtonListener(MessageBoxState::GetInstance(MessageBoxState::MOUSE));
}

void MouseOptionsState::SetInverted(bool value)
{
	// If the tick box has been ticked then set the temporary value accordingly
	temp_Inverted = value;
}

void MouseOptionsState::Default()
{
	// Obtain the default Sensitivity and set the value 
	int percentage;
	InputManager::Instance()->DefaultSensitivityPercentage(percentage);
	p_ScrollBar->SetValue(percentage);
	// Obtain the original value 
	temp_Inverted = InputManager::Instance()->DefaultIsInverted();
	// Set whether or not it is inverted
	p_TickBox->SetTicked(temp_Inverted);
}

void MouseOptionsState::Save()
{
	// Obtain the original value
	bool isInverted = InputManager::Instance()->IsInverted();

	// if the current is not equal to the original then set the new value;
	if(temp_Inverted != isInverted)
		// Set whether or not the controls should be inverted
		InputManager::Instance()->SetInverted();
	
	// Set the new Percentage value
	int percentage = p_ScrollBar->GetValue();
	InputManager::Instance()->ChangeSensitivity(percentage);

	// Save the Remapped Options
	InputManager::Instance()->SaveRemappedOptions();
}

bool MouseOptionsState::CheckChanges()
{
	// Obtain the original value
	bool isInverted = InputManager::Instance()->IsInverted();

	// if the current is not equal to the original then return true;
	if(temp_Inverted != isInverted)
		return true;

	// Obtain the current value
	int percentage;
	InputManager::Instance()->ObtainSensitivityPercentage(percentage);

	// If the current scroll bar value is not the same as the current value then return true
	if(p_ScrollBar->GetValue() != percentage)
		return true;

	return false;
}

void MouseOptionsState::Update(float dt)
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
			mGameStateManager->ChangeState(OptionsState::GetInstance());
			// Remove this from the Button Listener list so it does not get updated by messages while it is inactive
			// Without this the inactive screens will be updated which will cause issues
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(OptionsState::GetInstance());
		}
		else 
			// else do the alpha calculation to fade out
			alpha += (float)(0.015f);
	}

	// Activate the message box
	if(mMessageActive) {
		CallMessageBox();
	}

	// Update the tick box
	p_TickBox->Update();

	// Update the Scroll bar
	p_ScrollBar->Update();

	// Update every button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();
}

void MouseOptionsState ::Render()
{
	// CLear the screen
	Graphics::GetInstance()->ClearScreen();

	// Draw background
	background.Render();

	// Render the TickBox
	p_TickBox->Render();

	// Render the Scroll Bar
	p_ScrollBar->Render();

	// Render the button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();

	p_Text_Inverted->print(" Inverted Up/Down Turn ");
	p_Text_Sense->print(" Mouse Sensitivity ");

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
void MouseOptionsState ::OnMouseClick()
{
	Selected();
}
void MouseOptionsState ::OnUpPressed() {return;}
void MouseOptionsState ::OnDownPressed() {return;}
void MouseOptionsState ::OnLeftPressed() {return;}
void MouseOptionsState ::OnRightPressed() {return;}
void MouseOptionsState ::OnSelectPressed() 
{
}
void MouseOptionsState ::OnMouseMove(float x, float y)
{
	// Update The Mouse Position
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);

	p_TickBox->UpdateMousePosition(x, y);

	p_ScrollBar->UpdateMousePosition(x, y);
}
//----------------



