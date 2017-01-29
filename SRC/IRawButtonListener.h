/*
An interface class that allows classes to listen to raw button presses 

Author : Romesh Selvanathan
Date : 10/12
*/
#ifndef __IRAWBUTTONLISTENER__
#define __IRAWBUTTONLISTENER__

enum RawInputButton;

class IRawButtonListener
{
public:
	virtual void OnRawButtonPress(RawInputButton button) = 0;
};

#endif 