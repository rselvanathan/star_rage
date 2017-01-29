/*
This class will map inputs to their appropriate actions/states. This is done through keeping a list of 
the different "contexts" that are described as text files. These files will contain what button maps onto
what action/state. So for ex: during the menu a different context will be loaded compared to when the game itself
is running.

Author : Romesh Selvanathan

Date : October 2012
*/

#include "InputMapper.h"
#include "InputContext.h"
#include "FileIO.h"
#include "GameUtil.h"


//--------------------------------------------------------------------------------------------------------
// Construct and initialise an input mapper
//--------------------------------------------------------------------------------------------------------
InputMapper::InputMapper()
{
	unsigned count;
	// Contains all the different contexts - The contexts have to be described here otherwise the system will not recognise
	// a context which is not contained within this file.
	std::wifstream infile(L"Resources//ContextList.reso");
	if(!(infile >> count))
		throw std::exception("Failed to read ContextList.txt");

	for (unsigned i = 0; i < count; ++i)
	{
		std::wstring name = AttemptRead<std::wstring>(infile);
		std::wstring file = AttemptRead<std::wstring>(infile);
		InputContexts[name] = new InputContext(file);
	}
}

//--------------------------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------------------------
InputMapper::~InputMapper()
{
	for(std::map<std::wstring, InputContext*>::iterator it = InputContexts.begin(); it != InputContexts.end(); ++it)
		delete it->second;

	for(MapperListeners::iterator it = mMapperListener.begin(); it != mMapperListener.end(); ++it)
		delete (*it);
}

//--------------------------------------------------------------------------------------------------------
// Clear all mapped inputs
//--------------------------------------------------------------------------------------------------------
void InputMapper::Clear()
{
	// Reset the released action and states
	ReleasedMappedInput.Actions.clear();
	ReleasedMappedInput.States.clear();

	CurrentMappedInput.Actions.clear();
	/* States are not cleared as they need to remain set across frames so that they
	do not accidently appear as "off" for a tick or two whole the raw input is still pending*/
}

//--------------------------------------------------------------------------------------------------------
// Set the State of a raw button
//--------------------------------------------------------------------------------------------------------
void InputMapper::SetRawButtonState(RawInputButton button, bool pressed, bool previouslypressed)
{
	Action action;
	Button_State state;

	// the raw button has been pressed recently 
	if(pressed && !previouslypressed)
	{
		// Map the button to an action and add to the currently mapped inputs
		if(MapButtonToAction(button, action))
		{
			CurrentMappedInput.Actions.insert(action);
			return;
		}
	}

	// if a raw button is being pressed
	if(pressed)
	{
		// map the button to a state and add to the currently mapped inputs
		if(MapButtonToState(button, state))
		{
			CurrentMappedInput.States.insert(state);
			return;
		}
	}

	// Map and then remove the button
	MapAndRemoveButton(button);
}

//--------------------------------------------------------------------------------------------------------
// Set the Range of an axis
//--------------------------------------------------------------------------------------------------------
void InputMapper::SetRawAxisValue(RawInputAxis axis, double value)
{
	// Go through each context and map an axis to a range. If there is a range that matches the axis then
	// convert the range depending on the sensitivity value and store the value to be dispatched.
	for(std::list<InputContext*>::const_iterator it = ActiveContexts.begin(); it != ActiveContexts.end(); ++it)
	{
		const InputContext* context = *it;

		Range range;
		if(context->MapAxisToRange(axis, range))
		{
			CurrentMappedInput.Ranges[range] = context->GetConversions().Convert(range, value * context->GetSensitivity(range) * context->GetInverted());
			break;
		}
	}
}

//--------------------------------------------------------------------------------------------------------
// Dispatch input to all registerd listeners
//--------------------------------------------------------------------------------------------------------
void InputMapper::Dispatch() const
{
	MappedInput input = CurrentMappedInput;
	for(MapperListeners::const_iterator it = mMapperListener.begin(); it != mMapperListener.end(); ++it)
		(*it)->OnInputUpdate(input);
}

//--------------------------------------------------------------------------------------------------------
// Dispatch the released input to all registerd listeners
//--------------------------------------------------------------------------------------------------------
void InputMapper::DispatchReleasedInput() const
{
	MappedInput releasedinput = ReleasedMappedInput;
	for(MapperListeners::const_iterator it = mMapperListener.begin(); it != mMapperListener.end(); ++it)
		(*it)->OnInputReleaseUpdate(releasedinput);
}

//--------------------------------------------------------------------------------------------------------
// Add a listener to the dispatch table
//--------------------------------------------------------------------------------------------------------
void InputMapper::AddListener(IMapperListener* lptr)
{
	mMapperListener.push_back(lptr);
}

//--------------------------------------------------------------------------------------------------------
// Remove a listener to the dispatch table
//--------------------------------------------------------------------------------------------------------
void InputMapper::RemoveListener(IMapperListener* lptr)
{
	mMapperListener.remove(lptr);
}

//--------------------------------------------------------------------------------------------------------
// Push an active input context onto the stack
//--------------------------------------------------------------------------------------------------------
void InputMapper::PushContext(const std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		throw std::exception("Invalid input context pushed");

	ActiveContexts.push_front(it->second);
}

//--------------------------------------------------------------------------------------------------------
// Pop the current input context off the stack
//--------------------------------------------------------------------------------------------------------
void InputMapper::PopContext()
{
	if(ActiveContexts.empty())
		throw std::exception("Cannot pop input context, no contexts active!");

	ActiveContexts.pop_front();
}

//--------------------------------------------------------------------------------------------------------
// map a button to an action in the active context
//--------------------------------------------------------------------------------------------------------
bool InputMapper::MapButtonToAction(RawInputButton button, Action& action) const
{
	for(std::list<InputContext*>::const_iterator it = ActiveContexts.begin(); it != ActiveContexts.end(); ++it)
	{
		const InputContext* context = *it;

		if(context->MapButtonToAction(button, action))
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// map a button to a state in the active context
//--------------------------------------------------------------------------------------------------------
bool InputMapper::MapButtonToState(RawInputButton button, Button_State& state) const
{
	for(std::list<InputContext*>::const_iterator it = ActiveContexts.begin(); it != ActiveContexts.end(); ++it)
	{
		const InputContext* context = *it;

		if(context->MapButtonToState(button, state))
			return true;
	}

	return false;
}

//--------------------------------------------------------------------------------------------------------
// eat all input mapped to a given button and a message to the listeners needs to be sent informing that 
// the eaten mapped input has been released. (This makes sure certain button releases are known and can be 
// reacted to appropriatly by the game.)
//--------------------------------------------------------------------------------------------------------
void InputMapper::MapAndRemoveButton(RawInputButton button)
{
	Action action;
	Button_State state;

	if(MapButtonToAction(button, action)){
		CurrentMappedInput.RemoveAction(action);
		ReleasedMappedInput.Actions.insert(action);
	}

	if(MapButtonToState(button, state)) {
		CurrentMappedInput.RemoveState(state);
		ReleasedMappedInput.States.insert(state);
	}

	if(!ReleasedMappedInput.States.empty() || !ReleasedMappedInput.Actions.empty())
		DispatchReleasedInput();
}

//--------------------------------------------------------------------------------------------------------
// Remap the action to another key press, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputMapper::RemapButtonToAction(RawInputButton button, Action action, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->RemapButtonToAction(button, action);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Remap the state to another key press, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputMapper::RemapButtonToState(RawInputButton button, Button_State state, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->RemapButtonToState(button, state);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Change the mouse sensitivty, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ChangeSensitivity(int percentage, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->ChangeSensitivity(percentage);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Set whether or not mouse controls should be inverted, by specifying which context needs to obtain the remapping option
//--------------------------------------------------------------------------------------------------------
bool InputMapper::SetInverted( std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->SetInverted();
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Save the Remapped values for the given context
//--------------------------------------------------------------------------------------------------------
void InputMapper::SaveRemappedOptions(std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return;

	it->second->SaveRemappedValues();
}

//--------------------------------------------------------------------------------------------------------
// Obtain the Raw input button from an action by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainButtonFromAction(Action action, RawInputButton& button, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->MapActionToButton(action, button);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Obtain the Raw input button from an action by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainButtonFromState(Button_State state, RawInputButton& button, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->MapStateToButton(state, button);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Obtain the action button from a raw input button by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainActionFromButton(RawInputButton button, Action& action, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->MapButtonToAction(button, action);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Obtain the state button from a raw input button by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainStateFromButton(RawInputButton button, Button_State& state, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	it->second->MapButtonToState(button, state);
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Obtain whether or not the Mouse Control is inverted by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainInvertedMouse(bool& isInverted, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	isInverted = it->second->IsInverted();
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Obtain what the Sensitivity value is as a percentage by specifying what context to look at
//--------------------------------------------------------------------------------------------------------
bool InputMapper::ObtainSensitivityPercentage(int& percentage, std::wstring& name)
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(name);
	if(it == InputContexts.end())
		return false;

	percentage = it->second->GetSensitivityPercentage();
	return true;
}

/*void InputMapper::Test()
{
	std::map<std::wstring, InputContext*>::iterator it = InputContexts.find(L"maincontext");
	if(it == InputContexts.end())
		throw std::exception("File not found");

	it->second->SaveRemappedValues();
}*/