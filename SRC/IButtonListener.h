/*
An interface class that allows classes to listen to button presses active only during gameplay

Author : Romesh Selvanathan
Date : 10/12
*/
#ifndef __IBUTTONLISTENER__
#define __IBUTTONLISTENER__

class IButtonListener
{
public:	// Actions
	virtual void OnPausePress() {return;}
	virtual void OnGodModePress() {return;}
	virtual void OnCheatWavePress() {return;}
	// States
	virtual void OnMoveForwardPress(){return;}
	virtual void OnMoveBackPress(){return;}
	virtual void OnTurnLeftPress(){return;}
	virtual void OnTurnLeftRelease() {return;}
	virtual void OnTurnRightPress(){return;}
	virtual void OnTurnRightRelease(){return;}
	virtual void OnShootPress(){return;}
	virtual void OnSprintPress(){return;}
	virtual void OnSprintRelease() {return;}
};

#endif 