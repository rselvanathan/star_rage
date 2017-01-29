/*
An interface class that allows classes to listen to button presses which will be active only during the Menu screens

Author : Romesh Selvanathan
Date : 10/12
*/
#ifndef __IMENUBUTTONLISTENER__
#define __IMENUBUTTONLISTENER__

class IMenuButtonListener
{
public:	
	virtual void OnMouseClick() = 0;
	virtual void OnUpPressed() = 0;
	virtual void OnDownPressed() = 0;
	virtual void OnLeftPressed() = 0;
	virtual void OnRightPressed() = 0;
	virtual void OnSelectPressed() = 0;

	virtual void OnMouseMove(float x, float y) = 0;
};

#endif 