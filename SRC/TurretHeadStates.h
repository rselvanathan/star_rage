/*
Contains All the States for the TurretHead.

Attack the Target - Stay Idle

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __TURRETHEADSTATES_H__
#define __TURRETHEADSTATES_H__

#include "GameUtil.h"
#include "State.h"
#include "Telegram.h"

class TurretHead;

//////////////////////////////////////////////////////////////////////////////////////////////

class GlobalTurretHeadState : public State<TurretHead>
{
private:
	GlobalTurretHeadState() {}

public:
	// This is a singleton
	static GlobalTurretHeadState* Instance();

	void EnterState(TurretHead* entity){}
	void ExecuteState(TurretHead* entity);
	void ExitState(TurretHead* entity){}

	bool OnMessage(TurretHead*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class AttackState : public State<TurretHead>
{
private:
	AttackState() {}

public:
	// This is a singleton
	static AttackState* Instance();

	void EnterState(TurretHead* entity);
	void ExecuteState(TurretHead* entity);
	void ExitState(TurretHead* entity);

	bool OnMessage(TurretHead*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class IdleState : public State<TurretHead>
{
private:
	IdleState() {}

public:
	// This is a singleton
	static IdleState* Instance();

	void EnterState(TurretHead* entity);
	void ExecuteState(TurretHead* entity);
	void ExitState(TurretHead* entity);

	bool OnMessage(TurretHead*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

#endif