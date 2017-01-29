/*
The Keybinds Options State. It is a singleton class
This will allow the user to specify the keybinds. 
In order to change keybinds the user has to click on a keybind entry on screen, which will prompt the user to press a key to rebind the action/state.
After this is done then the user can permantly save the changes or default back the changes. If the user does not save the changes then the screen
will ask whether to save the changes before exiting this screen.

Author : Romesh Selvanathan
Date : 10/12
*/

#include "GameUtil.h"
#include "GameStateManager.h"
#include "KeybindsOptionsState.h"
#include "GameState.h"
#include "OptionsState.h"
#include "MessageBox_Keybind.h"
#include "MessageBox.h"
#include "GLText.h"
#include "MenuEntry.h"
#include "KeybindEntry.h"
#include "PreLoader.h"
#include "Texture.h"
#include "GL2DBox.h"
#include "InputManager.h"
#include "ClassManager.h"
#include "ScreenIDList.h"
#include "ScreenMessageTypes.h"
#include "Telegram.h"

KeybindsOptionsState* KeybindsOptionsState::mKeybindsOptionsState;

KeybindsOptionsState::KeybindsOptionsState() : GameState(ScreenIDList::GetInstance()->keybind_options)
{
	ClassManager::GetInstance()->RegisterScreen(this);
}

KeybindsOptionsState::~KeybindsOptionsState()
{
	SAFE_DELETE(p_Description);
	// If the list is not empty destroy all objects in there
	if(!menuEntries.empty()){
		for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
			SAFE_DELETE((*it));
	}
	GameState::~GameState();
	for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
		SAFE_DELETE((*it).second);
}

void KeybindsOptionsState::Init(GameStateManager* mGameState)
{
	if (!m_mActionMap.empty())
		m_mActionMap.clear();
	if (!m_mStateMap.empty())
		m_mStateMap.clear();
	if (!m_vKeybindEntryMap.empty()) {
		for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
			SAFE_DELETE((*it).second);
		m_vKeybindEntryMap.clear();
	}
	
	mInactive = false;
	alpha = 0.0f;
	mJustSwitched = true;
	switchAlpha = 1.0f;
	mMessageActive = false;
	m_iMessageBoxState = 0;
	m_iMessageBoxChoice = 0;

	p_CurrentClicked = NULL;

	// Setup the Flash box
	flash.Init(WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);

	mGameStateManager = mGameState;

	// Setup the background box
	background.Init(WINDOW_WIDTH, WINDOW_HEIGHT/2, 0, WINDOW_HEIGHT/2);
	background.SetTexture(PreLoader::GetInstance()->background->GetTGATexture().texID);

	// Initalise the buttons
	if(menuEntries.empty()){
		menuEntries.push_back(new MenuEntry("Options",PreLoader::GetInstance()->optionsButton->GetTGATexture().texID, 
			WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT - 900));
		menuEntries.push_back(new MenuEntry("Default", PreLoader::GetInstance()->defaultButton->GetTGATexture().texID,
			WINDOW_WIDTH/2 - 50, WINDOW_HEIGHT - 900));
		menuEntries.push_back(new MenuEntry("Save", PreLoader::GetInstance()->saveButton->GetTGATexture().texID,
			WINDOW_WIDTH/2 + 80, WINDOW_HEIGHT - 900));
	}

	p_Description = new GLText();
	p_Description->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");

	// Count from the first action to the last action that is allowed to be remapped
	for(int i = Action::ACTION_PAUSE; i <= Action::ACTION_PAUSE; ++i)
	{
		RawInputButton temp_button;
		InputManager::Instance()->ObtainButtonFromAction(static_cast<Action>(i), temp_button);
		m_mActionMap[static_cast<Action>(i)] = temp_button;
	}
	// Iterate from the First state to the last state that is allowed to be remapped
	for(int i = Button_State::STATE_MOVE_FORWARD; i <= Button_State::STATE_SHOOT; ++i)
	{
		RawInputButton temp_button;
		InputManager::Instance()->ObtainButtonFromState(static_cast<Button_State>(i), temp_button);
		m_mStateMap[static_cast<Button_State>(i)] = temp_button;
	}

	// Used to position each of the key bind entries with a initial value
	int displacement_count = 4;
	// Temporary size value
	int temp_size;
	// count how many items are in the action map
	temp_size = m_mActionMap.size();
	// for every item in the action map
	for(int i = 0; i < temp_size; ++i){
		std::string action;
		std::string button;
		// Obtain the Action as a string and save it in the structure 
		InputManager::Instance()->ConvertActionToString(static_cast<Action>(i), action);
		// Then obtain the corresponding button to the action
		RawInputButton temp_button = m_mActionMap[static_cast<Action>(i)];
		// Convert thhe button into a string and save it in the structure 
		InputManager::Instance()->ConvertRawButtonToString(temp_button, button);
		KeybindEntry* entry = new KeybindEntry(action, button,PreLoader::GetInstance()->keybinds_background->GetTGATexture().texID,
			PreLoader::GetInstance()->keybinds_background_mask->GetTGATexture().texID, WINDOW_WIDTH/2 - 450, WINDOW_HEIGHT - (50 *displacement_count), static_cast<Action>(i));

		m_vKeybindEntryMap[action] = entry;
		// incremented by x where the higher the x the more distance each entry will have from its former entry
		displacement_count += 2;
	}

	// count how many items are in the state map
	temp_size = m_mStateMap.size();
	// for every item in the state map
	for(int i = 0; i < temp_size; ++i){
		std::string state;
		std::string button;
		InputManager::Instance()->ConvertStateToString(static_cast<Button_State>(i), state);
		// Then obtain the corresponding button to the state
		RawInputButton temp_button = m_mStateMap[static_cast<Button_State>(i)];
		// Convert thhe button into a string and save it in the structure
		InputManager::Instance()->ConvertRawButtonToString(temp_button, button);

		KeybindEntry* entry = new KeybindEntry(state, button,PreLoader::GetInstance()->keybinds_background->GetTGATexture().texID,
			PreLoader::GetInstance()->keybinds_background_mask->GetTGATexture().texID, WINDOW_WIDTH/2 - 450, WINDOW_HEIGHT - (50 *displacement_count), static_cast<Button_State>(i));

		m_vKeybindEntryMap[state] = entry;
		// incremented by x where the higher the x the more distance each entry will have from its former entry
		displacement_count += 2;
	}
}

void KeybindsOptionsState::Resume()
{
	if(m_iMessageBoxState == MessageBoxScreen::KEYBIND) {
		// As soon as this screen is back rebind the selected entry
		RebindKey(MessageBox_KeybindState::GetInstance()->GetButton());
	}
	else if (m_iMessageBoxState == MessageBoxScreen::OPTIONS) {
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

bool KeybindsOptionsState::HandleMessage(const Telegram& msg)
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

void KeybindsOptionsState::Selected()
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

	for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
		if((*it).second->IsClicked()) {
			// Keep track of the clicked entry
			p_CurrentClicked = (*it).second;
			mMessageActive = true;
			m_iMessageBoxState = MessageBoxScreen::KEYBIND;
			return;
		}
}

void KeybindsOptionsState::CallMessageBox(MessageBoxScreen message)
{
	// Depending on the message type call up the right message Box state;
	switch (message) {
	case MessageBoxScreen::KEYBIND :
		{	
			// Add the message box and make this a state inactive
			mGameStateManager->SetMessageActive(true);
			mGameStateManager->AddState(MessageBox_KeybindState::GetInstance());
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddRawButtonListener(MessageBox_KeybindState::GetInstance());
		}
		break;
	case MessageBoxScreen::SAVE :
		{
			// Add the message box and make this a state inactive
			mGameStateManager->SetMessageActive(true);
			mGameStateManager->AddState(MessageBoxState::GetInstance(MessageBoxState::KEYBINDS));
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MessageBoxState::GetInstance(MessageBoxState::KEYBINDS));
		}
		break;
	case MessageBoxScreen::OPTIONS :
		{
			// Add the message box and make this a state inactive
			mGameStateManager->SetMessageActive(true);
			mGameStateManager->AddState(MessageBoxState::GetInstance(MessageBoxState::KEYBINDS));
			InputManager::Instance()->RemoveMenuButtonListener(this);
			InputManager::Instance()->AddMenuButtonListener(MessageBoxState::GetInstance(MessageBoxState::KEYBINDS));
		}
		break;
	}
}

void KeybindsOptionsState::RebindKey(RawInputButton button)
{
	std::string newButton_name;
	std::string action_name;

	// If the current Clicked entry is an action
	if(p_CurrentClicked->IsAction()) {
		// Obtain the action 
		Action action = p_CurrentClicked->GetAction();
		// Convert the new Button to a string
		InputManager::Instance()->ConvertRawButtonToString(button, newButton_name);
		// set the name of the button to the current clicked entry
		p_CurrentClicked->SetButtonName(newButton_name);

		// Both the Action map and state map have to be checked for rebinding as an action/state may have a button that
		// another action/state already posses and vice versa. So both maps have to be checked before replacing a button

		// If there is such a action with this button already binded and this action is not the same as the current selected one
		for(ActionMapType::iterator action_it = m_mActionMap.begin(); action_it != m_mActionMap.end(); ++action_it)
		{
			if((*action_it).second == button && (*action_it).first != action) {
				// Obtain the string for the NULL BUTTON press
				InputManager::Instance()->ConvertRawButtonToString(RawInputButton::BUTTON_NULL, newButton_name);
				// Obtain the action string
				InputManager::Instance()->ConvertActionToString((*action_it).first, action_name);
				// replace the action with the null button
				(*action_it).second = RawInputButton::BUTTON_NULL;
				// find the key bind entry with this action name and change the button text to the new Null Button text
				m_vKeybindEntryMap[action_name]->SetButtonName(newButton_name);
				// Break the for loop
				break;
			}
		}

		// If there is such a state with this button already binded
		for(StateMapType::iterator state_it = m_mStateMap.begin(); state_it != m_mStateMap.end(); ++state_it)
		{
			if((*state_it).second == button) {
				// Obtain the string for the NULL BUTTON press
				InputManager::Instance()->ConvertRawButtonToString(RawInputButton::BUTTON_NULL, newButton_name);
				// Obtain the state string
				InputManager::Instance()->ConvertStateToString((*state_it).first, action_name);
				// replace the action with the null button
				(*state_it).second = RawInputButton::BUTTON_NULL;
				// find the key bind entry with this action name and change the button text to the new Null Button text
				m_vKeybindEntryMap[action_name]->SetButtonName(newButton_name);
				// Break the for loop
				break;
			}
			
		}

		// Rebind the action with the new button
		m_mActionMap[action] = button;
	}
	// If it is not a action
	else {							 
		// Obtain the state
		Button_State state = p_CurrentClicked->GetState();
		// Convert the new Button to a string
		InputManager::Instance()->ConvertRawButtonToString(button, newButton_name);
		// set the name of the button to the current clicked entry
		p_CurrentClicked->SetButtonName(newButton_name);

		// Both the Action map and state map have to be checked for rebinding as the a state may have a button that
		// a action already posses and vice versa. So both maps have to be checked before replacing a button

		// If there is such a action with this button already binded
		for(ActionMapType::iterator action_it = m_mActionMap.begin(); action_it != m_mActionMap.end(); ++action_it)
		{
			if((*action_it).second == button) {
				// Obtain the string for the NULL BUTTON press
				InputManager::Instance()->ConvertRawButtonToString(RawInputButton::BUTTON_NULL, newButton_name);
				// Obtain the action string
				InputManager::Instance()->ConvertActionToString((*action_it).first, action_name);
				// replace the action with the null button
				(*action_it).second = RawInputButton::BUTTON_NULL;
				// find the key bind entry with this action name and change the button text to the new Null Button text
				m_vKeybindEntryMap[action_name]->SetButtonName(newButton_name);
				// Break the for loop
				break;
			}
		}

		// If there is such a state with this button already binded
		for(StateMapType::iterator state_it = m_mStateMap.begin(); state_it != m_mStateMap.end(); ++state_it)
		{
			if((*state_it).second == button && (*state_it).first != state) {
				// Obtain the string for the NULL BUTTON press
				InputManager::Instance()->ConvertRawButtonToString(RawInputButton::BUTTON_NULL, newButton_name);
				// Obtain the state string
				InputManager::Instance()->ConvertStateToString((*state_it).first, action_name);
				// replace the action with the null button
				(*state_it).second = RawInputButton::BUTTON_NULL;
				// find the key bind entry with this action name and change the button text to the new Null Button text
				m_vKeybindEntryMap[action_name]->SetButtonName(newButton_name);
				// Break the loop
				break;
			}
			
		}

		// Rebind the state with the new button
		m_mStateMap[state] = button;
	}
}

void KeybindsOptionsState::Default()
{
	// Temporary button and name
	RawInputButton button;
	std::string button_name;
	std::string action_name;

	// Iterate through each action
	for(ActionMapType::iterator action_it = m_mActionMap.begin(); action_it != m_mActionMap.end(); ++action_it)
	{
		// Obtain the Button from default values
		InputManager::Instance()->DefaultButtonFromAction((*action_it).first, button);
		// Convert the button to a string
		InputManager::Instance()->ConvertRawButtonToString(button, button_name);
		// convert the action to a string
		InputManager::Instance()->ConvertActionToString((*action_it).first, action_name);
		// change the button
		(*action_it).second = button;
		// Find the action in the entries list and change the button name
		m_vKeybindEntryMap[action_name]->SetButtonName(button_name);
	}
	// Iterate through each state
	for(StateMapType::iterator state_it = m_mStateMap.begin(); state_it != m_mStateMap.end(); ++state_it)
	{
		// Temporary button and name
		RawInputButton button;
		std::string button_name;
		// Obtain the Button from default values
		InputManager::Instance()->DefaultButtonFromState((*state_it).first, button);
		// Convert the button to a string
		InputManager::Instance()->ConvertRawButtonToString(button, button_name);
		// convert the state to a string
		InputManager::Instance()->ConvertStateToString((*state_it).first, action_name);
		// change the button
		(*state_it).second = button;
		// Find the action in the entries list and change the button name
		m_vKeybindEntryMap[action_name]->SetButtonName(button_name);
	}
}

void KeybindsOptionsState::Save()
{
	// Iterate through each action
	for(ActionMapType::iterator action_it = m_mActionMap.begin(); action_it != m_mActionMap.end(); ++action_it)
	{
		if(!InputManager::Instance()->RemapButtonToAction((*action_it).second, (*action_it).first))
			std::exception("File could not be found to save Keys");
	}

	// Iterate through each state
	for(StateMapType::iterator state_it = m_mStateMap.begin(); state_it != m_mStateMap.end(); ++state_it)
	{
		if(!InputManager::Instance()->RemapButtonToState((*state_it).second, (*state_it).first))
			std::exception("File could not be found to save Keys");
	}

	// Save the Remapped values
	InputManager::Instance()->SaveRemappedOptions();
}

bool KeybindsOptionsState::CheckChanges()
{
	// The original button that is mapped
	RawInputButton original_button;
	// THis value will count how many items there are in both the action and state map.
	int count_total = 0;
	// This value will count how many buttons are the same from the original and the maps contained here
	int count_buttons = 0;

	count_total = m_mActionMap.size() + m_mStateMap.size();

	// Iterate through each action
	for(ActionMapType::iterator action_it = m_mActionMap.begin(); action_it != m_mActionMap.end(); ++action_it)
	{
		// Obtain the original button that is mapped
		InputManager::Instance()->ObtainButtonFromAction((*action_it).first, original_button);
		// Check whether the two are the same
		if(original_button == (*action_it).second)
			count_buttons++;
	}

	// Iterate through each state
	for(StateMapType::iterator state_it = m_mStateMap.begin(); state_it != m_mStateMap.end(); ++state_it)
	{
		// Obtain the original button that is mapped
		InputManager::Instance()->ObtainButtonFromState((*state_it).first, original_button);
		// Check whether the two are the same
		if((*state_it).second == original_button)
			count_buttons++;
	}

	// If the buttons are not equal then return false meaning a change occured
	if(count_buttons < count_total)
		return true;
	else
		return false;
}

void KeybindsOptionsState::Update(float dt)
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
		if (m_iMessageBoxState == MessageBoxScreen::KEYBIND) CallMessageBox(MessageBoxScreen::KEYBIND);
		else if (m_iMessageBoxState == MessageBoxScreen::SAVE || m_iMessageBoxState == MessageBoxScreen::OPTIONS) 
			CallMessageBox(MessageBoxScreen::SAVE);
	}

	// Update every button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Update();

	// Update every Keybind entry
	for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
		(*it).second->Update();
}

void KeybindsOptionsState ::Render()
{
	// CLear the screen
	Graphics::GetInstance()->ClearScreen();

	// Draw background
	background.Render();

	for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
		(*it).second->Render();

	// Render the button
	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->Render();

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

	p_Description->SetColor3f(1, 1, 1);
	p_Description->SetPosition(WINDOW_WIDTH/2 - 180, WINDOW_HEIGHT - 100);
	p_Description->print(" Click a Button To Rebind ");
}

//---------------------------------------IMenuButtonListener-------------------------------------
void KeybindsOptionsState ::OnMouseClick()
{
	Selected();
}
void KeybindsOptionsState ::OnUpPressed() {return;}
void KeybindsOptionsState ::OnDownPressed() {return;}
void KeybindsOptionsState ::OnLeftPressed() {return;}
void KeybindsOptionsState ::OnRightPressed() {return;}
void KeybindsOptionsState ::OnSelectPressed() 
{
	return;
}
void KeybindsOptionsState::OnMouseMove(float x, float y)
{
	for(KeybindEntryMapType::iterator it = m_vKeybindEntryMap.begin(); it != m_vKeybindEntryMap.end(); ++it)
		(*it).second->UpdateMousePosition(x, y);

	for(MenuEntryList::iterator it = menuEntries.begin(); it != menuEntries.end(); it++)
		(*it)->UpdateMousePosition(x, y);
}
//-----------------------------