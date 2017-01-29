/* 
Input Manager uses the DirectInput API to acces the input devices such as keyboard or
mouse to provide input functionality to the game

Author : Romesh Selvanathan
Date : 03/05/12
*/ 

#include "GameUtil.h"
#include "InputManager.h"
#include "IKeyCharListener.h"

InputManager* InputManager::mInstance;

//---------------------------------------------------------------------------------------------------------------------
/* Constructor */
//---------------------------------------------------------------------------------------------------------------------
InputManager::InputManager()
{
	mouseY = mouseX = 0;

	mapper.AddListener(this);
}

//---------------------------------------------------------------------------------------------------------------------
/* Destructor */
//---------------------------------------------------------------------------------------------------------------------
InputManager::~InputManager()
{
	for(ButtonListeners::iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
		SAFE_DELETE((*it));

	for(MenuButtonListeners::iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
		SAFE_DELETE((*it));

	for(MousePosListeners::iterator it = mMousePosListener.begin(); it != mMousePosListener.end(); ++it)
		SAFE_DELETE((*it));
}

//---------------------------------------------------------------------------------------------------------------------
// Creates an Input Object to moniter the mouse and keyboard input
// @param - window instance, global window handle
//---------------------------------------------------------------------------------------------------------------------
bool InputManager::Init(HWND hwnd, HINSTANCE hInstance)
{
	// Set the current window handle
	wndHandle = hwnd;
	// Initialise the Old Cursor Position
	// Temp values to obtain window properties
	//RECT r_Client;
	//int width, height;
	//// Obtain info on window 
	//GetClientRect(wndHandle, &r_Client);
	//// window widht and height
	//width = r_Client.right - r_Client.left;
	//height = r_Client.bottom - r_Client.top;
	////Set cursor to the middle and update the position of the mouse
	//SetCursorPos(width/2, height/2);
	//GetCursorPos(&mOldMouse);
	//ScreenToClient(hwnd, &mOldMouse);

	// Work out the center of the screen
	// NOTE : the previous code would ask windows for client window size and work out the width and height
	// however this caused bugs on certain machines where the mouse would become stuck at the top left corner of the screen
	int width, height;
	// window widht and height
	width = WINDOW_WIDTH;
	height = WINDOW_HEIGHT;
	// window middle values
	midX = width/2; 
	midY = height/2; 

	SetCursorPos(midX, midY);

	return true;
}

//---------------------------------------------------------------------------------------------------------------------
// Messages from Windows to do with input
//---------------------------------------------------------------------------------------------------------------------
void InputManager::HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam)
{
	// Listen for any Character key presses and send message to the listeners
	switch(msg){
		case WM_CHAR:
			OnCharEntered(wParam);
			break;
		case WM_MOUSEMOVE:
			{
				int x = LOWORD(lParam);
				int y = HIWORD(lParam);

				if(!mMenuButtonListener.empty()){
					for (MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
						(*it)->OnMouseMove(static_cast<float>(x), static_cast<float>(y));
				}

				// Only apply this code when the listener list is not empty (The gameplay state is aimed at here)
				if(!mMousePosListener.empty()) {
					// obtain current mouse values
					GetCursorPos(&mMousePosition);
					// if mouse is in the middle just return as no update is required
					if((abs(mMousePosition.x) == midX) && (abs(mMousePosition.y) == midY)) {
						return;
					}
					// Set Mouse back to the middle
					SetCursorPos(midX, midY);
					// Get the relative Mouse Position, by subracting the current with old mouse Position
					mouseX = static_cast<float>(midX - mMousePosition.x);
					mouseY = static_cast<float>(midY - mMousePosition.y);
					// Fire the mouse position to all the listeners
					//FireMousePosition(mouseX, mouseY);

					mapper.SetRawAxisValue(RawInputAxis::AXIS_MOUSE_X, static_cast<double>(mouseX));
					mapper.SetRawAxisValue(RawInputAxis::AXIS_MOUSE_Y, static_cast<double>(mouseY));
				}
			}
			break;
		case WM_LBUTTONDOWN:
			{
				RawInputButton button;
				// 31 specified the previous key state, where if it is 1 then the key is down before the message
				// is sent otherwise if it is 0 then the key is up
				bool previouslydown = ((lParam & (1 << 31)) != 0);
				button = RawInputButton::BUTTON_LBUTTON;

				// If the raw button listener list is not empty send the button to all the listeners
				if (!mRawButtonListener.empty())
					for(RawButtonListeners::const_iterator it = mRawButtonListener.begin(); it != mRawButtonListener.end(); ++it)
						(*it)->OnRawButtonPress(button);

				mapper.SetRawButtonState(button, true, previouslydown);
			}break;
		case WM_LBUTTONUP:
			{ 
				RawInputButton button;
				button = RawInputButton::BUTTON_LBUTTON;

				mapper.SetRawButtonState(button, false, true);
			}
			break;
		case WM_MBUTTONDOWN:
			{
				RawInputButton button;
				bool previouslydown = ((lParam & (1 << 31)) != 0);
				button = RawInputButton::BUTTON_MBUTTON;

				// If the raw button listener list is not empty send the button to all the listeners
				if (!mRawButtonListener.empty())
					for(RawButtonListeners::const_iterator it = mRawButtonListener.begin(); it != mRawButtonListener.end(); ++it)
						(*it)->OnRawButtonPress(button);

				mapper.SetRawButtonState(button, true, previouslydown);
			}break;
		case WM_MBUTTONUP:
			{
				RawInputButton button;
				button = RawInputButton::BUTTON_MBUTTON;

				mapper.SetRawButtonState(button, false, true);
			}
			break;
		case WM_RBUTTONDOWN:
			{
				RawInputButton button;
				// 31 specified the previous key state, where if it is 1 then the key is down before the message
				// is sent otherwise if it is 0 then the key is up
				bool previouslydown = ((lParam & (1 << 31)) != 0);
				button = RawInputButton::BUTTON_RBUTTON;

				// If the raw button listener list is not empty send the button to all the listeners
				if (!mRawButtonListener.empty())
					for(RawButtonListeners::const_iterator it = mRawButtonListener.begin(); it != mRawButtonListener.end(); ++it)
						(*it)->OnRawButtonPress(button);

				mapper.SetRawButtonState(button, false, previouslydown);
			}break;
		case WM_RBUTTONUP:
			{
				RawInputButton button;
				button = RawInputButton::BUTTON_RBUTTON;

				mapper.SetRawButtonState(button, false, true);
			}
			break;
		case WM_KEYDOWN:
			{
				RawInputButton button;
				// 31 specified the previous key state, where if it is 1 then the key is down before the message
				// is sent otherwise if it is 0 then the key is up
				bool previouslydown = ((lParam & (1 << 31)) != 0);
				if(ConvertWParamToRawButton(wParam, button)) {

					// If the raw button listener list is not empty send the button to all the listeners
					if (!mRawButtonListener.empty())
						for(RawButtonListeners::const_iterator it = mRawButtonListener.begin(); it != mRawButtonListener.end(); ++it)
							(*it)->OnRawButtonPress(button);

					mapper.SetRawButtonState(button, true, previouslydown);
				}
			}
			break;
		case WM_KEYUP:
			{
				RawInputButton button;
				if(ConvertWParamToRawButton(wParam, button))
					mapper.SetRawButtonState(button, false, true);
			}
			break;
	}
}

//--------------------------------------------------------------------------------------------------------------
// Send messages to all listeners on what character press was made
//--------------------------------------------------------------------------------------------------------------
void InputManager::OnCharEntered(WPARAM wParam)
{
	for(KeyListeners::iterator it = mListener.begin(); it != mListener.end(); it++)
		(*it)->OnCharKeyDown(wParam);
}

//-------------------------------------------------------------------------------
// Convert the windows message into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::ConvertWParamToRawButton(WPARAM wParam, RawInputButton& button)
{
	switch(wParam)
	{
	case VK_LBUTTON		:	button = RawInputButton::BUTTON_LBUTTON;	break;
	case VK_RBUTTON		:	button = RawInputButton::BUTTON_RBUTTON;	break;
	case VK_MBUTTON		:	button = RawInputButton::BUTTON_MBUTTON;	break;
	case VK_BACK		:	button = RawInputButton::BUTTON_BACK;		break;
	case VK_TAB			:	button = RawInputButton::BUTTON_TAB;		break;
	case VK_CLEAR		:	button = RawInputButton::BUTTON_CLEAR;		break;
	case VK_RETURN		:	button = RawInputButton::BUTTON_RETURN;		break;
	case VK_SHIFT		:	button = RawInputButton::BUTTON_SHIFT;		break;
	case VK_CONTROL		:	button = RawInputButton::BUTTON_CONTROL;	break;
	case VK_MENU		:	button = RawInputButton::BUTTON_ALT;		break;
	case VK_PAUSE		:	button = RawInputButton::BUTTON_PAUSE;		break;
	case VK_CAPITAL		:	button = RawInputButton::BUTTON_CAPSLOCK;	break;
	case VK_ESCAPE		:	button = RawInputButton::BUTTON_ESCAPE;		break;
	case VK_SPACE		:	button = RawInputButton::BUTTON_SPACEBAR;	break;
	case VK_PRIOR		:	button = RawInputButton::BUTTON_PAGEUP;		break;
	case VK_NEXT		:	button = RawInputButton::BUTTON_PAGEDOWN;	break;
	case VK_END			:	button = RawInputButton::BUTTON_END;		break;
	case VK_HOME		:	button = RawInputButton::BUTTON_HOME;		break;
	case VK_LEFT		:	button = RawInputButton::BUTTON_LEFT;		break;
	case VK_UP			:	button = RawInputButton::BUTTON_UP;			break;
	case VK_RIGHT		:	button = RawInputButton::BUTTON_RIGHT;		break;
	case VK_DOWN		:	button = RawInputButton::BUTTON_DOWN;		break;
	case VK_SELECT		:	button = RawInputButton::BUTTON_SELECT;		break;
	case VK_SNAPSHOT	:	button = RawInputButton::BUTTON_PRINTSCREEN;break;
	case VK_INSERT		:	button = RawInputButton::BUTTON_INSERT;		break;
	case VK_DELETE		:	button = RawInputButton::BUTTON_DELETE;		break;
	case 0x30			:	button = RawInputButton::BUTTON_0;			break;
	case 0x31			:	button = RawInputButton::BUTTON_1;			break;
	case 0x32			:	button = RawInputButton::BUTTON_2;			break;
	case 0x33			:	button = RawInputButton::BUTTON_3;			break;
	case 0x34			:	button = RawInputButton::BUTTON_4;			break;
	case 0x35			:	button = RawInputButton::BUTTON_5;			break;
	case 0x36			:	button = RawInputButton::BUTTON_6;			break;
	case 0x37			:	button = RawInputButton::BUTTON_7;			break;
	case 0x38			:	button = RawInputButton::BUTTON_8;			break;
	case 0x39			:	button = RawInputButton::BUTTON_9;			break;
	case 0x41			:	button = RawInputButton::BUTTON_A;			break;
	case 0x42			:	button = RawInputButton::BUTTON_B;			break;
	case 0x43			:	button = RawInputButton::BUTTON_C;			break;
	case 0x44			:	button = RawInputButton::BUTTON_D;			break;
	case 0x45			:	button = RawInputButton::BUTTON_E;			break;
	case 0x46			:	button = RawInputButton::BUTTON_F;			break;
	case 0x47			:	button = RawInputButton::BUTTON_G;			break;
	case 0x48			:	button = RawInputButton::BUTTON_H;			break;
	case 0x49			:	button = RawInputButton::BUTTON_I;			break;
	case 0x4A			:	button = RawInputButton::BUTTON_J;			break;
	case 0x4B			:	button = RawInputButton::BUTTON_K;			break;
	case 0x4C			:	button = RawInputButton::BUTTON_L;			break;
	case 0x4D			:	button = RawInputButton::BUTTON_M;			break;
	case 0x4E			:	button = RawInputButton::BUTTON_N;			break;
	case 0x4F			:	button = RawInputButton::BUTTON_O;			break;
	case 0x50			:	button = RawInputButton::BUTTON_P;			break;
	case 0x51			:	button = RawInputButton::BUTTON_Q;			break;
	case 0x52			:	button = RawInputButton::BUTTON_R;			break;
	case 0x53			:	button = RawInputButton::BUTTON_S;			break;
	case 0x54			:	button = RawInputButton::BUTTON_T;			break;
	case 0x55			:	button = RawInputButton::BUTTON_U;			break;
	case 0x56			:	button = RawInputButton::BUTTON_V;			break;
	case 0x57			:	button = RawInputButton::BUTTON_W;			break;
	case 0x58			:	button = RawInputButton::BUTTON_X;			break;
	case 0x59			:	button = RawInputButton::BUTTON_Y;			break;
	case 0x5A			:	button = RawInputButton::BUTTON_Z;			break;
	case 0x60			:	button = RawInputButton::BUTTON_NUMPAD0;	break;
	case 0x61			:	button = RawInputButton::BUTTON_NUMPAD1;	break;
	case 0x62			:	button = RawInputButton::BUTTON_NUMPAD2;	break;
	case 0x63			:	button = RawInputButton::BUTTON_NUMPAD3;	break;
	case 0x64			:	button = RawInputButton::BUTTON_NUMPAD4;	break;
	case 0x65			:	button = RawInputButton::BUTTON_NUMPAD5;	break;
	case 0x66			:	button = RawInputButton::BUTTON_NUMPAD6;	break;
	case 0x67			:	button = RawInputButton::BUTTON_NUMPAD7;	break;
	case 0x68			:	button = RawInputButton::BUTTON_NUMPAD8;	break;
	case 0x69			:	button = RawInputButton::BUTTON_NUMPAD9;	break;
	case 0x6A			:	button = RawInputButton::BUTTON_MULTIPLY;	break;
	case 0x6B			:	button = RawInputButton::BUTTON_ADD;		break;
	case 0x6C			:	button = RawInputButton::BUTTON_SEPARATOR;	break;
	case 0x6D			:	button = RawInputButton::BUTTON_SUBTRACT;	break;
	case 0x6E			:	button = RawInputButton::BUTTON_DECIMAL;	break;
	case 0x6F			:	button = RawInputButton::BUTTON_DIVIDE;		break;
	case 0x70			:	button = RawInputButton::BUTTON_F1;			break;
	case 0x71			:	button = RawInputButton::BUTTON_F2;			break;
	case 0x72			:	button = RawInputButton::BUTTON_F3;			break;
	case 0x73			:	button = RawInputButton::BUTTON_F4;			break;
	case 0x74			:	button = RawInputButton::BUTTON_F5;			break;
	case 0x75			:	button = RawInputButton::BUTTON_F6;			break;
	case 0x76			:	button = RawInputButton::BUTTON_F7;			break;
	case 0x77			:	button = RawInputButton::BUTTON_F8;			break;
	case 0x78			:	button = RawInputButton::BUTTON_F9;			break;
	case 0x79			:	button = RawInputButton::BUTTON_F10;		break;
	case 0x7A			:	button = RawInputButton::BUTTON_F11;		break;
	case 0x7B			:	button = RawInputButton::BUTTON_F12;		break;
	case 0x90			:	button = RawInputButton::BUTTON_NUMLOCK;	break;
	case 0xA0			:	button = RawInputButton::BUTTON_LSHIFT;		break;
	case 0xA1			:	button = RawInputButton::BUTTON_RSHIFT;		break;
	case 0xA2			:	button = RawInputButton::BUTTON_LCONTROL;	break;
	case 0xA3			:	button = RawInputButton::BUTTON_RCONTROL;	break;
	default: return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
// Convert Action/States into a String/char
//-------------------------------------------------------------------------------
bool InputManager::ConvertActionToString(Action action, std::string& out)
{
	switch(action)
	{
	case ACTION_PAUSE		:	out = "Pause";	break;
	
	default: return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
// Convert Action/States into a String/char
//-------------------------------------------------------------------------------
bool InputManager::ConvertStateToString(Button_State state, std::string& out)
{
	switch(state)
	{
	case STATE_MOVE_FORWARD		:	out = "Move Forward";	break;
	case STATE_MOVE_BACKWARD	:	out = "Move Backward";	break;
	case STATE_TURN_LEFT		:	out = "Turn Left";		break;
	case STATE_TURN_RIGHT		:	out = "Turn Right";		break;
	case STATE_SPRINT			:	out = "Sprint";			break;
	case STATE_SHOOT			:	out = "Shoot";			break;
	default: return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
// Convert Raw Input into a string/char
//-------------------------------------------------------------------------------
bool InputManager::ConvertRawButtonToString(RawInputButton button, std::string& out)
{
	switch(button)
	{
	case BUTTON_LBUTTON		:	out = "Mouse LButton";		break;
	case BUTTON_RBUTTON		:	out = "Mouse RButton";		break;
	case BUTTON_MBUTTON		:	out = "Mouse MButton";		break;
	case BUTTON_BACK		:	out = "Backspace";			break;
	case BUTTON_TAB			:	out = "Tab";				break;
	case BUTTON_CLEAR		:	out = "Clear";				break;
	case BUTTON_RETURN		:	out = "Return";				break;
	case BUTTON_SHIFT		:	out = "Shift";				break;
	case BUTTON_CONTROL		:	out = "Control";			break;
	case BUTTON_ALT			:	out = "ALT";				break;
	case BUTTON_PAUSE		:	out = "Pause";				break;
	case BUTTON_CAPSLOCK	:	out = "CAPS";				break;
	case BUTTON_ESCAPE		:	out = "ESC";				break;
	case BUTTON_SPACEBAR	:	out = "Space";				break;
	case BUTTON_PAGEUP		:	out = "Page Up";			break;
	case BUTTON_PAGEDOWN	:	out = "Page Down";			break;
	case BUTTON_END			:	out = "End";				break;
	case BUTTON_HOME		:	out = "Home";				break;
	case BUTTON_LEFT		:	out = "Left";				break;
	case BUTTON_UP			:	out = "Up";					break;
	case BUTTON_RIGHT		:	out = "Right";				break;
	case BUTTON_DOWN		:	out = "Down";				break;
	case BUTTON_SELECT		:	out = "Select";				break;
	case BUTTON_PRINTSCREEN	:	out = "Print Screen";		break;
	case BUTTON_INSERT		:	out = "Insert";				break;
	case BUTTON_DELETE		:	out = "Delete";				break;
	case BUTTON_0			:	out = "0";					break;
	case BUTTON_1			:	out = "1";					break;
	case BUTTON_2			:	out = "2";					break;
	case BUTTON_3			:	out = "3";					break;
	case BUTTON_4			:	out = "4";					break;
	case BUTTON_5			:	out = "5";					break;
	case BUTTON_6			:	out = "6";					break;
	case BUTTON_7			:	out = "7";					break;
	case BUTTON_8			:	out = "8";					break;
	case BUTTON_9			:	out = "9";					break;
	case BUTTON_A			:	out = "A";					break;
	case BUTTON_B			:	out = "B";					break;
	case BUTTON_C			:	out = "C";					break;
	case BUTTON_D			:	out = "D";					break;
	case BUTTON_E			:	out = "E";					break;
	case BUTTON_F			:	out = "F";					break;
	case BUTTON_G			:	out = "G";					break;
	case BUTTON_H			:	out = "H";					break;
	case BUTTON_I			:	out = "I";					break;
	case BUTTON_J			:	out = "J";					break;
	case BUTTON_K			:	out = "K";					break;
	case BUTTON_L			:	out = "L";					break;
	case BUTTON_M			:	out = "M";					break;
	case BUTTON_N			:	out = "N";					break;
	case BUTTON_O			:	out = "O";					break;
	case BUTTON_P			:	out = "P";					break;
	case BUTTON_Q			:	out = "Q";					break;
	case BUTTON_R			:	out = "R";					break;
	case BUTTON_S			:	out = "S";					break;
	case BUTTON_T			:	out = "T";					break;
	case BUTTON_U			:	out = "U";					break;
	case BUTTON_V			:	out = "V";					break;
	case BUTTON_W			:	out = "W";					break;
	case BUTTON_X			:	out = "X";					break;
	case BUTTON_Y			:	out = "Y";					break;
	case BUTTON_Z			:	out = "Z";					break;
	case BUTTON_NUMPAD0		:	out = "NUM_0";				break;
	case BUTTON_NUMPAD1		:	out = "NUM_1";				break;
	case BUTTON_NUMPAD2		:	out = "NUM_2";				break;
	case BUTTON_NUMPAD3		:	out = "NUM_3";				break;
	case BUTTON_NUMPAD4		:	out = "NUM_4";				break;
	case BUTTON_NUMPAD5		:	out = "NUM_5";				break;
	case BUTTON_NUMPAD6		:	out = "NUM_6";				break;
	case BUTTON_NUMPAD7		:	out = "NUM_7";				break;
	case BUTTON_NUMPAD8		:	out = "NUM_8";				break;
	case BUTTON_NUMPAD9		:	out = "NUM_9";				break;
	case BUTTON_MULTIPLY	:	out = "NUM_*";				break;
	case BUTTON_ADD			:	out = "NUM_+";				break;
	case BUTTON_SEPARATOR	:	out = "NUM_SEPERATOR";		break;
	case BUTTON_SUBTRACT	:	out = "NUM_-";				break;
	case BUTTON_DECIMAL		:	out = "NUM_.";				break;
	case BUTTON_DIVIDE		:	out = "NUM_/";				break;
	case BUTTON_F1			:	out = "F1";					break;
	case BUTTON_F2			:	out = "F2";					break;
	case BUTTON_F3			:	out = "F3";					break;
	case BUTTON_F4			:	out = "F4";					break;
	case BUTTON_F5			:	out = "F5";					break;
	case BUTTON_F6			:	out = "F6";					break;
	case BUTTON_F7			:	out = "F7";					break;
	case BUTTON_F8			:	out = "F8";					break;
	case BUTTON_F9			:	out = "F9";					break;
	case BUTTON_F10			:	out = "F10";				break;
	case BUTTON_F11			:	out = "F11";				break;
	case BUTTON_F12			:	out = "F12";				break;
	case BUTTON_NUMLOCK		:	out = "NUMLOCK";			break;
	case BUTTON_LSHIFT		:	out = "LSHIFT";				break;
	case BUTTON_RSHIFT		:	out = "RSHIFT";				break;
	case BUTTON_LCONTROL	:	out = "LCTRL";				break;
	case BUTTON_RCONTROL	:	out = "RCTRL";				break;
	case BUTTON_NULL		:	out = " ";					break;
	default: return false;
	}

	return true;
}

//-------------------------------------------------------------------------------
// Convert the Action into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::ObtainButtonFromAction(Action action, RawInputButton& button)
{
	std::wstring context = L"maincontext";
	if(mapper.ObtainButtonFromAction(action, button, context))
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------------------
// Convert the State into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::ObtainButtonFromState(Button_State state, RawInputButton& button)
{
	std::wstring context = L"maincontext";
	if(mapper.ObtainButtonFromState(state, button, context))
		return true;
	else
		return false;
}

//-------------------------------------------------------------------------------
// Convert the Action into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::ObtainActionFromButton(RawInputButton button, Action& action)
{
	std::wstring context = L"maincontext";
	if(mapper.ObtainActionFromButton(button, action, context))
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------------------
// Convert the State into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::ObtainStateFromButton(RawInputButton button, Button_State& state)
{
	std::wstring context = L"maincontext";
	if(mapper.ObtainStateFromButton(button, state, context))
		return true;
	else
		return false;
}

//----------------------------------------------Obtain Data From Default values----------------- ---------------------------------
//-------------------------------------------------------------------------------
// Convert the Action into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::DefaultButtonFromAction(Action action, RawInputButton& button)
{
	std::wstring context = L"defaultcontext";
	if(mapper.ObtainButtonFromAction(action, button, context))
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------------------
// Convert the State into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::DefaultButtonFromState(Button_State state, RawInputButton& button)
{
	std::wstring context = L"defaultcontext";
	if(mapper.ObtainButtonFromState(state, button, context))
		return true;
	else
		return false;
}

//-------------------------------------------------------------------------------
// Convert the Action into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::DefaultActionFromButton(RawInputButton button, Action& action)
{
	std::wstring context = L"defaultcontext";
	if(mapper.ObtainActionFromButton(button, action, context))
		return true;
	else 
		return false;
}

//-------------------------------------------------------------------------------
// Convert the State into a raw button press
//-------------------------------------------------------------------------------
bool InputManager::DefaultStateFromButton(RawInputButton button, Button_State& state)
{
	std::wstring context = L"defaultcontext";
	if(mapper.ObtainStateFromButton(button, state, context))
		return true;
	else
		return false;
}

//--------------------------------------------------------------------------------------------------------
// Return whether or not mouse controls are inverted, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::DefaultIsInverted()
{
	std::wstring context = L"defaultcontext";
	bool inverted;
	mapper.ObtainInvertedMouse(inverted, context);
	if(inverted)
		return true;
	else 
		return false;
}

//--------------------------------------------------------------------------------------------------------
// Return the default sensitivity value by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::DefaultSensitivityPercentage(int& percentage)
{
	std::wstring context = L"defaultcontext";
	if(mapper.ObtainSensitivityPercentage(percentage, context))
		return true;
	else
		return false;
}

//----------------------------------------------Remapping The Button presses------------------------------------------------------
//--------------------------------------------------------------------------------------------------------
// Remap the action to another key press, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::RemapButtonToAction(RawInputButton button, Action action)
{
	std::wstring context = L"maincontext";
	if(mapper.RemapButtonToAction(button, action, context))
		return true;
	else 
		return false;
}
//--------------------------------------------------------------------------------------------------------
// Remap the state to another key press, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::RemapButtonToState(RawInputButton button, Button_State state)
{
	std::wstring context = L"maincontext";
	if(mapper.RemapButtonToState(button, state, context))
		return true;
	else 
		return false;
}


//----------------------------------------------Remapping Mouse Settings----------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
// Change the mouse sensitivty, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::ChangeSensitivity(int percentage)
{
	std::wstring context = L"maincontext";
	if(mapper.ChangeSensitivity(percentage, context))
		return true;
	else 
		return false;
}

//--------------------------------------------------------------------------------------------------------
// Obtain the mouse sensitivty, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::ObtainSensitivityPercentage(int& percentage)
{
	std::wstring context = L"maincontext";
	if(mapper.ObtainSensitivityPercentage(percentage, context))
		return true;
	else 
		return false;
}

//--------------------------------------------------------------------------------------------------------
// Return whether or not mouse controls are inverted, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputManager::IsInverted()
{
	std::wstring context = L"maincontext";
	bool inverted;
	mapper.ObtainInvertedMouse(inverted, context);
	if(inverted)
		return true;
	else 
		return false;
}

//--------------------------------------------------------------------------------------------------------
// Set whether or not mouse controls are inverted, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
void InputManager::SetInverted()
{
	std::wstring context = L"maincontext";
	mapper.SetInverted(context);
}

//--------------------------------------------------------------------------------------------------------
// Save the Remapped values for the given context
//--------------------------------------------------------------------------------------------------------
void InputManager::SaveRemappedOptions()
{
	std::wstring context = L"maincontext";
	mapper.SaveRemappedOptions(context);
}

//-----------------------------------------Fire In-game Button Presses----------------------------------

void InputManager::FirePausePressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnPausePress();
}

void InputManager::FireGodModePressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnGodModePress();
}

void InputManager::FireCheatWavePressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnCheatWavePress();
}

void InputManager::FireMoveForwardPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnMoveForwardPress();
}

void InputManager::FireMoveBackPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnMoveBackPress();
}

void InputManager::FireTurnLeftPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnTurnLeftPress();
}

void InputManager::FireTurnLeftReleased()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnTurnLeftRelease();
}

void InputManager::FireTurnRightPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnTurnRightPress();
}

void InputManager::FireTurnRightReleased()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnTurnRightRelease();
}

void InputManager::FireShootPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnShootPress();
}

void InputManager::FireSprintPressed()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnSprintPress();
}

void InputManager::FireSprintReleased()
{
		for(ButtonListeners::const_iterator it = mButtonListener.begin(); it != mButtonListener.end(); ++it)
			(*it)->OnSprintRelease();
}

//-----------------------------------------Fire Menu Button Presses----------------------------------

void InputManager::FireMouseClicked()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnMouseClick();
}

void InputManager::FireUpPressed()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnUpPressed();
}

void InputManager::FireDownPressed()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnDownPressed();
}

void InputManager::FireRightPressed()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnRightPressed();
}

void InputManager::FireLeftPressed()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnLeftPressed();
}

void InputManager::FireSelectPressed()
{
		for(MenuButtonListeners::const_iterator it = mMenuButtonListener.begin(); it != mMenuButtonListener.end(); ++it)
			(*it)->OnSelectPressed();
}

// Fire Mouse Position
void InputManager::FireMousePosition(double x, double y)
{
		for(MousePosListeners::const_iterator it = mMousePosListener.begin(); it != mMousePosListener.end(); ++it)
			(*it)->OnMouse(x, y);
}

//--------------------------------------------IMapperListener------------------------------------
void InputManager::OnInputUpdate(MappedInput& input)
{
	if(input.Actions.find(Action::ACTION_PAUSE) != input.Actions.end())
		FirePausePressed();

	if(input.Actions.find(Action::ACTION_GODMODE) != input.Actions.end())
		FireGodModePressed();

	if(input.Actions.find(Action::ACTION_CHEAT_WAVE) != input.Actions.end())
		FireCheatWavePressed();

	if(input.States.find(Button_State::STATE_MOVE_BACKWARD) != input.States.end())
		FireMoveBackPressed();

	if(input.States.find(Button_State::STATE_MOVE_FORWARD) != input.States.end())
		FireMoveForwardPressed();

	if(input.States.find(Button_State::STATE_SPRINT) != input.States.end())
		FireSprintPressed();

	if(input.States.find(Button_State::STATE_SHOOT) != input.States.end())
		FireShootPressed();

	if(input.States.find(Button_State::STATE_TURN_LEFT) != input.States.end())
		FireTurnLeftPressed();

	if(input.States.find(Button_State::STATE_TURN_RIGHT) != input.States.end())
		FireTurnRightPressed();

	// Menu Actions

	if(input.Actions.find(Action::MENU_MOUSECLICK) != input.Actions.end())
		FireMouseClicked();

	if(input.Actions.find(Action::MENU_UP) != input.Actions.end())
		FireUpPressed();

	if(input.Actions.find(Action::MENU_DOWN) != input.Actions.end())
		FireDownPressed();

	if(input.Actions.find(Action::MENU_RIGHT) != input.Actions.end())
		FireRightPressed();

	if(input.Actions.find(Action::MENU_LEFT) != input.Actions.end())
		FireLeftPressed();

	if(input.Actions.find(Action::MENU_SELECT) != input.Actions.end())
		FireSelectPressed();

	// Mouse Position
	double mouse_x = input.Ranges[Range::RANGE_X];
	double mouse_y = input.Ranges[Range::RANGE_Y];

	FireMousePosition(mouse_x, mouse_y);
}

void InputManager::OnInputReleaseUpdate(MappedInput& input)
{
	if(input.States.find(Button_State::STATE_TURN_LEFT) != input.States.end())
		FireTurnLeftReleased();

	if(input.States.find(Button_State::STATE_TURN_RIGHT) != input.States.end())
		FireTurnRightReleased();

	if(input.States.find(Button_State::STATE_SPRINT) != input.States.end())
		FireSprintReleased();
}
//-----------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------
// Update the Input Mapper by dispatching the message and Clearing the data in the mapper at every frame
//----------------------------------------------------------------------------------------------
void InputManager::UpdateMapper()
{
	mapper.Dispatch();
	mapper.Clear();
}

//----------------------------------------------------------------------------------------------
// Add a new context to the mapper when required
//----------------------------------------------------------------------------------------------
void InputManager::PushContext(const std::wstring& name)
{
	mapper.PushContext(name);
}

//----------------------------------------------------------------------------------------------
// Remove the current context from the mapper
//----------------------------------------------------------------------------------------------
void InputManager::PopContext()
{
	mapper.PopContext();
}


