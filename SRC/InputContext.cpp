/*
Wrapper class for managing the different input contexts - (For example menus will have different actions
to the game itself)

Author : Romesh Selvanathan

Date : October 2012
*/

#include "InputContext.h"
#include "GameUtil.h"
#include "FileIO.h"

//--------------------------------------------------------------------------------------------------------
// Construct and initialise an input context given by data in a file
//--------------------------------------------------------------------------------------------------------
InputContext::InputContext(std::wstring contextfilename) : m_contextfilename(contextfilename)
{
	// Read in the MouseRange File which contains the values to be used for the conversions
	// These values will not change during remapping or changing the mouse options for most applications
	std::wifstream mouseRangeFile(L"Resources//MouseRanges.reso", ios::in);
	p_Conversions = new InputRangeConverter(mouseRangeFile);

	// Load the values from the context file
	LoadValues(m_contextfilename);
}

//--------------------------------------------------------------------------------------------------------
// Destructor
//--------------------------------------------------------------------------------------------------------
InputContext::~InputContext()
{
	SAFE_DELETE(p_Conversions);
}

//--------------------------------------------------------------------------------------------------------
// Map a raw button to an action
//--------------------------------------------------------------------------------------------------------
bool InputContext::MapButtonToAction(RawInputButton button, Action& action) const 
{
	// iterate through each item in the map and find the raw button pressed
	std::map<RawInputButton, Action>::const_iterator it = ActionMap.find(button);
	// if no such button exists then return nothing
	if(it == ActionMap.end())
		return false;

	// else return the corresponding action
	action = it->second;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Map a raw button to a state
//--------------------------------------------------------------------------------------------------------
bool InputContext::MapButtonToState(RawInputButton button, Button_State& state) const 
{
	// iterate through each item in the map and find the raw button pressed
	std::map<RawInputButton, Button_State>::const_iterator it = StateMap.find(button);
	// if no such button exists then return nothing
	if(it == StateMap.end())
		return false;

	// else return the corresponding state
	state = it->second;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Map a raw axis to a range
//--------------------------------------------------------------------------------------------------------
bool InputContext::MapAxisToRange(RawInputAxis axis, Range& out) const 
{
	// iterate through each item in the map and find the raw axis 
	std::map<RawInputAxis, Range>::const_iterator it = RangeMap.find(axis);
	// if no such axis exists then return nothing
	if(it == RangeMap.end())
		return false;

	// else return the corresponding range
	out = it->second;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Map an action to a raw button
//--------------------------------------------------------------------------------------------------------
bool InputContext::MapActionToButton(Action action, RawInputButton& button) const
{
	// iterate through each item in the map and find the action
	std::map<Action, RawInputButton>::const_iterator it = RebindActionMap.find(action);
	// if no such action exists then return nothing
	if(it == RebindActionMap.end())
		return false;

	// else return the corresponding button
	button = it->second;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Map a state to a raw button
//--------------------------------------------------------------------------------------------------------
bool InputContext::MapStateToButton(Button_State state, RawInputButton& button) const 
{
	// iterate through each item in the map and find the state
	std::map<Button_State, RawInputButton>::const_iterator it = RebindStateMap.find(state);
	// if no such state exists then return nothing
	if(it == RebindStateMap.end())
		return false;

	// else return the corresponding button
	button = it->second;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Return a Sensitvity value given a range
//--------------------------------------------------------------------------------------------------------
double InputContext::GetSensitivity(Range range) const
{
	std::map<Range, double>::const_iterator it = SensitivityMap.find(range);
	if(it == SensitivityMap.end())
		return 1.0;

	return it->second;
}

//--------------------------------------------------------------------------------------------------------
// Return whether the remapping the key for a specific action was successful
//--------------------------------------------------------------------------------------------------------
bool InputContext::RemapButtonToAction(RawInputButton button, Action action)
{
	// iterate through each item in the map and find the action
	std::map<Action, RawInputButton>::iterator it = RebindActionMap.find(action);
	// if there is no such action then do nothing
	if(it == RebindActionMap.end())
		return false;

	// else remap the key
	it->second = button;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Return whether the remapping the key for a specific state was successful
//--------------------------------------------------------------------------------------------------------
bool InputContext::RemapButtonToState(RawInputButton button, Button_State state)
{
	// iterate through each item in the map and find the state 
	std::map<Button_State, RawInputButton>::iterator it = RebindStateMap.find(state);
	// if there is no such state then do nothing
	if(it == RebindStateMap.end())
		return false;

	// else remap the key
	it->second = button;
	return true;
}

//--------------------------------------------------------------------------------------------------------
// Set the new Sensitivity value 
// Parameter needs to be a percentage value
//--------------------------------------------------------------------------------------------------------
bool InputContext::ChangeSensitivity(int percentage)
{
	// Work out the value by multiplying max sensitivity by percentage, divided by 100 % to obtain final value
	double temp = (static_cast<double>(percentage) * m_dMaxSensitivity) / 100.0f;
	// Clamp this new sensitivity
	Clamp(temp, m_dMinSensitivity, m_dMaxSensitivity);
	// Set the value
	m_dSensitivity = temp;

	for(std::map<Range, double>::iterator it = SensitivityMap.begin(); it != SensitivityMap.end(); it++)
		it->second = m_dSensitivity;

	return true;
}

//--------------------------------------------------------------------------------------------------------
// Change the Value that determines whether it is inverted or not.
// Return true if it is inverted and return false it is not inverted after changing the value.
//--------------------------------------------------------------------------------------------------------
bool InputContext::SetInverted()
{
	if(m_iInverted == -1) {
		m_iInverted = 1;
		return true;
	}
	else 
		m_iInverted = -1;

	return false;
}

//--------------------------------------------------------------------------------------------------------
// Return the Sensitivity value as a percentage 
// This value can be used by settings or any other type of function, which may require the
// sensitivity to be known as a percentage
//--------------------------------------------------------------------------------------------------------
int InputContext::GetSensitivityPercentage() const
{
	// work out the percentage by multiplying the sensitivity by 100 then dividing by the 
	// max sensitivity value
	double temp = (m_dSensitivity * 100.0f) / m_dMaxSensitivity;
	Clamp(temp, 0, 100);

	return static_cast<int>(temp);
}

//--------------------------------------------------------------------------------------------------------
// Return the Inverted value itself - to be used by mapper to change the final mouse value
//--------------------------------------------------------------------------------------------------------
int InputContext::GetInverted() const
{
	return m_iInverted;
}

//--------------------------------------------------------------------------------------------------------
// Return whether or not the Mouse control is inverted
bool InputContext::IsInverted() const
{
	if(m_iInverted == -1) {
		return false;
	}

	return true;
}
//--------------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------------
// Save the remapped values into the file by opening the original file and rewriting everything, that was
// contained in there. In the end reload the values
//--------------------------------------------------------------------------------------------------------
bool InputContext::SaveRemappedValues()
{
	std::ofstream outfile(m_contextfilename, ios::out);

	if(!outfile.is_open()) {
		throw std::exception("Failed to open the required file");
		return false;
	}

	// Obtain how many states there are
	unsigned statecount = RebindStateMap.size();
	outfile << statecount << "\n";
	for (std::map<Button_State, RawInputButton>::const_iterator it = RebindStateMap.begin(); it !=  RebindStateMap.end(); ++it)
	{
		outfile << it->second << " " << it->first << "\n";
	}

	// Obtain how many actions there are
	unsigned actioncount = RebindActionMap.size();
	outfile << actioncount << "\n";
	for (std::map<Action, RawInputButton>::const_iterator it = RebindActionMap.begin(); it !=  RebindActionMap.end(); ++it)
	{
		outfile << it->second << " " << it->first << "\n";
	}

	// Obtain how many ranges there are
	unsigned rangecount = RangeMap.size();
	outfile << rangecount << "\n";
	for (std::map<RawInputAxis, Range>::const_iterator it = RangeMap.begin(); it !=  RangeMap.end(); ++it)
	{
		// Writing the first value here before the second unlike the top two, as these values are not getting changed
		outfile << it->first << " " << it->second << "\n";
	}

	// Obtain how many sensitivity values there are
	unsigned sensitivtycount = SensitivityMap.size();
	outfile << sensitivtycount << "\n";
	// Input the min/max sensitivity values
	outfile << m_dMinSensitivity << " " << m_dMaxSensitivity << "\n";
	for (std::map<Range, double>::const_iterator it = SensitivityMap.begin(); it !=  SensitivityMap.end(); ++it)
	{
		// Writing the first value here before the second unlike the top two, as these values are not getting changed
		// except for the sensitivity, however the sensitivty is the same for both Ranges so I can use the same map
		outfile << it->first << " " << it->second << "\n";
	}

	// Write whether or not the mouse control for up and down are inverted
	outfile << m_iInverted << "\n"; 

	outfile.close();

	// Clear the mapped values
	ClearMaps();
	// Load the context file
	LoadValues(m_contextfilename);

	return true;
}

//--------------------------------------------------------------------------------------------------------
// Load the values form the context files
//--------------------------------------------------------------------------------------------------------
bool InputContext::LoadValues(const std::wstring& contextfilename)
{
	// The file to be loaded
	std::wifstream infile(m_contextfilename);

	// Obtain how many states there are
	unsigned statecount = AttemptRead<unsigned>(infile);
	// For each raw input add the corresponding state to the map
	for (unsigned i = 0; i < statecount; ++i)
	{
		RawInputButton button = static_cast<RawInputButton>(AttemptRead<unsigned>(infile));
		Button_State state = static_cast<Button_State>(AttemptRead<unsigned>(infile));
		StateMap[button] = state;
		// Add the corresponding button to the map
		RebindStateMap[state] = button;
	}

	// Obtain how many actions there are
	unsigned actioncount = AttemptRead<unsigned>(infile);
	// For each raw input add the corresponding action to the map
	for (unsigned i = 0; i < actioncount; ++i)
	{
		RawInputButton button = static_cast<RawInputButton>(AttemptRead<unsigned>(infile));
		Action action = static_cast<Action>(AttemptRead<unsigned>(infile));
		ActionMap[button] = action;
		// Add the corresponding button to the map
		RebindActionMap[action] = button;
	}

	// Obtain how many ranges there are
	unsigned rangecount = AttemptRead<unsigned>(infile);
	// For each raw input axis add the corresponding range to the map
	for (unsigned i = 0; i < rangecount; ++i)
	{
		RawInputAxis axis = static_cast<RawInputAxis>(AttemptRead<unsigned>(infile));
		Range range = static_cast<Range>(AttemptRead<unsigned>(infile));
		RangeMap[axis] = range;
	}

	// Obtain how many sensitivty values there are
	unsigned sensitivtycount = AttemptRead<unsigned>(infile);
	// Obtain the minimum and maximum sensitivty values next
	m_dMinSensitivity = static_cast<double>(AttemptRead<double>(infile));
	m_dMaxSensitivity = static_cast<double>(AttemptRead<double>(infile));
	// For each range add the corresponding sensitivity value to the map
	for (unsigned i = 0; i < sensitivtycount; ++i)
	{
		Range range = static_cast<Range>(AttemptRead<unsigned>(infile));
		double sensitivty = static_cast<double>(AttemptRead<double>(infile));
		SensitivityMap[range] = sensitivty;
		m_dSensitivity = sensitivty;
	}

	// Obtan whether or not the mouse control for up and down are inverted
	m_iInverted = static_cast<int>(AttemptRead<signed>(infile));

	infile.close();

	return true;
}

//--------------------------------------------------------------------------------------------------------
// Clear all the values in the maps
//--------------------------------------------------------------------------------------------------------
void InputContext::ClearMaps()
{
	if(!StateMap.empty())
		StateMap.clear();
	if(!ActionMap.empty())
		ActionMap.clear();
	if(!RangeMap.empty())
		RangeMap.clear();
	if(!SensitivityMap.empty())
		SensitivityMap.clear();
	if(!RebindActionMap.empty())
		RebindActionMap.clear();
	if(!RebindStateMap.empty())
		RebindStateMap.clear();
}