/*
Wrapper class for managing the different input contexts - (For example menus will have different actions
to the game itself)

Author : Romesh Selvanathan

Date : October 2012
*/

#ifndef __INPUTCONTEXT_H__
#define __INPUTCONTEXT_H__

#include "RawInputConstants.h"
#include "InputConstants.h"
#include "InputRangeConverter.h"
#include "GameUtil.h"

class InputContext
{
public:
	explicit InputContext(std::wstring contextfilename);
	~InputContext();

	//Mapping Interface
	// Map the Raw Input to the corresponding Action/State/Axis
	bool MapButtonToAction(RawInputButton button, Action& out) const;
	bool MapButtonToState(RawInputButton button, Button_State& out) const;
	bool MapAxisToRange(RawInputAxis axis, Range& out) const;

	// Map the Action/State/Axis to the Raw Input
	bool MapActionToButton(Action action, RawInputButton& button) const;
	bool MapStateToButton(Button_State state, RawInputButton& button) const;

	//Remapping Interface
	bool RemapButtonToAction(RawInputButton button, Action action);
	bool RemapButtonToState(RawInputButton button, Button_State state);
	bool ChangeSensitivity(int percentage);
	// Change the Value that determines whether it is inverted or not.
	// Return true if it is inverted and return false it is not inverted after changing the value.
	bool SetInverted();

	// Remapping and Mapping the Mouse Values
	// Obtain the Sensitivity value
	double GetSensitivity(Range range) const;
	// Obtain the Sensitivity Value as a percentage
	int GetSensitivityPercentage() const;
	// Obtain the value itself
	int GetInverted() const;
	// Return a boolean instead of the value
	bool IsInverted() const;

	const InputRangeConverter& GetConversions() const {return *p_Conversions;}

	// Writes the remapped values into the context file.
	bool SaveRemappedValues();
	// Load all the values from the file
	bool LoadValues(const std::wstring& contextfilename);
private:
	// Clear all the values from the Maps
	void ClearMaps();

	// Maps to determine an action, state or range from a button press/mouse movement
	std::map<RawInputButton, Action> ActionMap;
	std::map<RawInputButton, Button_State> StateMap;
	std::map<RawInputAxis, Range> RangeMap;
	
	// Map that deals with sensitivity 
	std::map<Range, double> SensitivityMap;
	// The minimum and maximum sensitivity values
	double m_dMinSensitivity, m_dMaxSensitivity;

	// This value will either be 1 or -1 depending on whether the user wants inverted or non-inverted Up and Down Turn
	int m_iInverted;

	// These maps will help allow the keys to be rebound. I am not using the top 2 maps as those are using the
	// buttons as "keys" to find the items ,so when the user wants to change button for a specific action/state then I would have to go
	// thorugh each item in the map to find it, turning it into a sequential search. So instead I create another map
	// where the action and the state are the keys allowing me to easily identify the button and change it with (O(1)) search time
	// instead of (O(n)). This may prove to be problematic with a large number of keys, however for any game of this size with limited number
	// of keys, the memory problem is not as significant. 
	std::map<Action, RawInputButton> RebindActionMap;
	std::map<Button_State, RawInputButton> RebindStateMap;
	// The current sensitivity value, default value is = 1.0f
	double m_dSensitivity;

	std::wstring m_contextfilename;						// The context filename
	InputRangeConverter* p_Conversions;						// Pointer to the Range convertor
};

#endif