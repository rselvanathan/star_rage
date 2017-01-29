/*
This class will map inputs to their appropriate actions/states. This is done through keeping a list of 
the different "contexts" that are described as text files. These files will contain what button maps onto
what action/state. So for ex: during the menu a different context will be loaded compared to when the game itself
is running.

Author : Romesh Selvanathan

Date : October 2012
*/

#ifndef __INPUTMAPPER_H__
#define __INPUTMAPPER_H__

#include "RawInputConstants.h"
#include "InputConstants.h"
#include "IMapperListener.h"
#include "GameUtil.h"

class InputContext;

// Helper Structure
struct MappedInput
{
	std::set<Action> Actions;
	std::set<Button_State> States;
	std::map<Range, double> Ranges;

	// Helps erase the items in sets
	void RemoveAction(Action action) {Actions.erase(action);}
	void RemoveState(Button_State state) {States.erase(state);}
	void RemoveRange(Range range)
	{
		std::map<Range, double>::iterator it = Ranges.find(range);
		if(it != Ranges.end())
			Ranges.erase(it);
	}
};

class InputMapper
{
public:
	InputMapper();
	~InputMapper();

	// Type Shortcuts
	typedef void (*InputCallback)(MappedInput& inputs);


	// Raw Input Interface
	void Clear();
	void SetRawButtonState(RawInputButton button, bool pressed, bool previouslypressed);
	void SetRawAxisValue(RawInputAxis axis, double value);

	// Remapping Input Interface - talks to the appropriate context
	bool RemapButtonToAction(RawInputButton button, Action action, std::wstring& name);
	bool RemapButtonToState(RawInputButton button, Button_State state, std::wstring& name);
	bool ChangeSensitivity(int percentage, std::wstring& name);
	bool SetInverted(std::wstring& name);
	void SaveRemappedOptions(std::wstring& name);

	// Obtain Raw Button Presses - talks to the appropriate context
	bool ObtainButtonFromAction(Action action, RawInputButton& button, std::wstring& name);
	bool ObtainButtonFromState(Button_State state, RawInputButton& button, std::wstring& name);
	// Obtain the Action or state from a raw button 
	bool ObtainActionFromButton(RawInputButton button, Action& action, std::wstring& name);
	bool ObtainStateFromButton(RawInputButton button, Button_State& state, std::wstring& name);
	// Obtain whether or not the Mouse Control is inverted
	bool ObtainInvertedMouse(bool& isInverted, std::wstring& name);
	bool ObtainSensitivityPercentage(int& percentage, std::wstring& name);

	// Dispatching the INput to all listeners
	void Dispatch() const;
	void DispatchReleasedInput() const;

	// Input callback registration
	void AddListener(IMapperListener* lptr);
	// Remove listener
	void RemoveListener(IMapperListener* lptr);

	//Context management
	void PushContext(const std::wstring& name);
	void PopContext();

private:
	// Shortcut types
	typedef list<IMapperListener*> MapperListeners;

private:
	// Internal Helper function
	void MapAndRemoveButton(RawInputButton button);
	bool MapButtonToAction(RawInputButton button, Action& out) const;
	bool MapButtonToState(RawInputButton button, Button_State& out) const;

	// Map of contexts
	std::map<std::wstring, InputContext*> InputContexts;
	// List of Active Contexts
	std::list<InputContext*> ActiveContexts;
	// List of Listeners
	MapperListeners mMapperListener;

	MappedInput CurrentMappedInput;
	MappedInput ReleasedMappedInput;
};

#endif