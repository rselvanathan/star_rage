
/* 
Input Manager uses the DirectInput API to acces the input devices such as keyboard or
mouse to provide input functionality to the game

Author : Romesh Selvanathan
Date : 03/05/12
*/ 

#ifndef __INPUTMANAGER_H__
#define __INPUTMANAGER_H__

#include "GameUtil.h"  
#include "IKeyCharListener.h"
#include "IButtonListener.h"
#include "IMenuButtonListener.h"
#include "IMousePosListener.h"
#include "IMapperListener.h"
#include "IRawButtonListener.h"
#include "InputMapper.h"

class InputManager : public IMapperListener
{
public:
	static InputManager* Instance()
	{
		if(mInstance == NULL)
			mInstance = new InputManager();
		return mInstance;
	}
	~InputManager();

	bool Init(HWND hwnd, HINSTANCE hIntance);

	// Handle Message
	void HandleMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	//Methods to update any listeners
	void OnCharEntered(WPARAM param);

	// Show and Hide Mouse Cursor
	void ShowMouseCursor() {ShowCursor(true);}
	void HideMouseCursor() {ShowCursor(false);}

	//--------------------------------------------------Fire Messages To Listeners--------------------------------------------------

	// Actions
	void FirePausePressed();
	void FireGodModePressed();
	void FireCheatWavePressed();
	// States
	void FireMoveForwardPressed();
	void FireMoveBackPressed();
	void FireTurnLeftPressed();
	void FireTurnLeftReleased();
	void FireTurnRightPressed();
	void FireTurnRightReleased();
	void FireShootPressed();
	void FireSprintPressed();
	void FireSprintReleased();

	// Fire Menu Button Presses
	void FireMouseClicked();
	void FireUpPressed();
	void FireDownPressed();
	void FireRightPressed();
	void FireLeftPressed();
	void FireSelectPressed();

	// Fire Mouse Position
	void FireMousePosition(double x, double y);

	// Fire Raw Button press
	void FireRawButtonPress(RawInputButton button);

	//--------------------------------------------------------------------------------------------------------------------------------

	//-----------------------------------------------String Conversions---------------------------------------------------------------
	// Convert Action/States into a String/char
	bool ConvertActionToString(Action action, std::string& out);
	bool ConvertStateToString(Button_State state, std::string& out);
	// Convert Raw Input into a string/char
	bool ConvertRawButtonToString(RawInputButton button, std::string& out);
	//--------------------------------------------------------------------------------------------------------------------------------

	//----------------------------------------------Conversions Between Raw and Action/State Buttons ---------------------------------
	// Convert the windows message into a raw button press
	bool ConvertWParamToRawButton(WPARAM wParam, RawInputButton& button);
	// Convert the action/state to a button
	bool ObtainButtonFromAction(Action action, RawInputButton& button);
	bool ObtainButtonFromState(Button_State state, RawInputButton& button);
	// Convert the button into state or action
	bool ObtainActionFromButton(RawInputButton button, Action& action);
	bool ObtainStateFromButton(RawInputButton button, Button_State& state);
	//--------------------------------------------------------------------------------------------------------------------------------

	//----------------------------------------------Obtain Data From Default values----------------- ---------------------------------
	// Convert the action/state to a button
	bool DefaultButtonFromAction(Action action, RawInputButton& button);
	bool DefaultButtonFromState(Button_State state, RawInputButton& button);
	// Convert the button into state or action
	bool DefaultActionFromButton(RawInputButton button, Action& action);
	bool DefaultStateFromButton(RawInputButton button, Button_State& state);
	// Obtain whether or not inverted mouse controls is set as default
	bool DefaultIsInverted();
	// Obtain the Default sensitivity value
	bool DefaultSensitivityPercentage(int& percentage);
	//--------------------------------------------------------------------------------------------------------------------------------

	//----------------------------------------------Remapping The Button presses------------------------------------------------------
	bool RemapButtonToAction(RawInputButton button, Action action);
	bool RemapButtonToState(RawInputButton button, Button_State state);
	//--------------------------------------------------------------------------------------------------------------------------------

	//----------------------------------------------Remapping Mouse Settings----------------------------------------------------------
	bool ChangeSensitivity(int percentage);
	bool ObtainSensitivityPercentage(int& percentage);
	bool IsInverted();	
	void SetInverted();
	//--------------------------------------------------------------------------------------------------------------------------------

	// Save all the Remapped values
	void SaveRemappedOptions();

	// Dispatch and clear the Mapper information
	void UpdateMapper();
	// Push a new input context
	void PushContext(const std::wstring& name);
	// Pop the current input context
	void PopContext();

	//--------------------------------------------IMapperListener------------------------------------
	void OnInputUpdate(MappedInput& input);
	void OnInputReleaseUpdate(MappedInput& input);
	//-----------------------------------------------------------------------------------------------

	// Add and remove character key listeners
	void AddKeyListener(IKeyCharListener* lptr) {mListener.push_back(lptr);}
	void RemoveKeyListener(IKeyCharListener* lptr){mListener.remove(lptr);}
	// Add and remove In-game button listeners
	void AddButtonListener(IButtonListener* lptr){mButtonListener.push_back(lptr);}
	void RemoveButtonListener(IButtonListener* lptr){mButtonListener.remove(lptr);}
	// Add and remove Menu button listeners
	void AddMenuButtonListener(IMenuButtonListener* lptr){mMenuButtonListener.push_back(lptr);}
	void RemoveMenuButtonListener(IMenuButtonListener* lptr){mMenuButtonListener.remove(lptr);}
	// Add and remove Mouse position listeners
	void AddMousePosListener(IMousePosListener* lptr){mMousePosListener.push_back(lptr);}
	void RemoveMousePosListener(IMousePosListener* lptr){mMousePosListener.remove(lptr);}
	// Add and remove Raw button listeners
	void AddRawButtonListener(IRawButtonListener* lptr){mRawButtonListener.push_back(lptr);}
	void RemoveRawButtonListener(IRawButtonListener* lptr){mRawButtonListener.remove(lptr);}

private :
	// Shorthands for types
	typedef list<IKeyCharListener*> KeyListeners;
	typedef list<IButtonListener*> ButtonListeners;
	typedef list<IMenuButtonListener*> MenuButtonListeners;
	typedef list<IMousePosListener*> MousePosListeners;
	typedef list<IRawButtonListener*> RawButtonListeners;

private:
	InputManager();
	static InputManager* mInstance;

	POINT mMousePosition;						// Mouse Point used for calculating relative mouse Position
	HWND wndHandle;								// A window handle(To be be used for mouse position)

	float mouseX, mouseY;						// Relative MouseX and Y positions from the center
	int midX, midY;								// middle of the screen

	KeyListeners mListener;						// List of listeners that listen to character key presses (ex HighScore)
	ButtonListeners mButtonListener;			// List of in-game button listeners
	MenuButtonListeners mMenuButtonListener;	// List of menu button listeners
	MousePosListeners mMousePosListener;		// List of mouse position listeners
	RawButtonListeners mRawButtonListener;		// List of Raw button listeners

	InputMapper mapper;							// A input mapping object
};

#endif