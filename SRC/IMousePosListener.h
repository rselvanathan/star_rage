/*
An interface class that allows classes to listen to button presses which will be active only during the Menu screens

Author : Romesh Selvanathan
Date : 10/12
*/
#ifndef __IMOUSEPOSLISTENER__
#define __IMOUSEPOSLISTENER__

class IMousePosListener
{
public:	
	virtual void OnMouse(int x, int y) = 0;
};

#endif 