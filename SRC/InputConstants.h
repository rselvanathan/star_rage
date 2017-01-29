/*
Constants for identifying the inputs used by the game i.e actions

Author : Romesh Selvanathan

Date : October 2012
*/

#ifndef __INPUTCONSTANTS_H__
#define __INPUTCONSTANTS_H__

enum Action {
	ACTION_PAUSE,
	ACTION_GODMODE,
	ACTION_CHEAT_WAVE,
	MENU_MOUSECLICK,
	MENU_UP,
	MENU_DOWN,
	MENU_RIGHT,
	MENU_LEFT,
	MENU_SELECT
};

enum Button_State {
	STATE_MOVE_FORWARD,
	STATE_MOVE_BACKWARD,
	STATE_TURN_LEFT,
	STATE_TURN_RIGHT,
	STATE_SPRINT,
	STATE_SHOOT
};

enum Range {
	RANGE_X,
	RANGE_Y
};

#endif