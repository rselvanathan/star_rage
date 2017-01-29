/*
Contains All the States for the Hard Enemy.

Chase the Player - Seek a target position - Evade a shot

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __HARDENEMYSTATES_H__
#define __HARDENEMYSTATES_H__

#include "GameUtil.h"
#include "State.h"
#include "Telegram.h"

class HardEnemy;

//////////////////////////////////////////////////////////////////////////////////////////////

class GlobalHardEnemyState : public State<HardEnemy>
{
private:
	GlobalHardEnemyState() {}

public:
	// This is a singleton
	static GlobalHardEnemyState* Instance();

	void EnterState(HardEnemy* entity){}
	void ExecuteState(HardEnemy* entity);
	void ExitState(HardEnemy* entity){}

	bool OnMessage(HardEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class ChasePlayer : public State<HardEnemy>
{
private:
	ChasePlayer() {}

public:
	// This is a singleton
	static ChasePlayer* Instance();

	void EnterState(HardEnemy* entity);
	void ExecuteState(HardEnemy* entity);
	void ExitState(HardEnemy* entity);

	bool OnMessage(HardEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class EvadeShots : public State<HardEnemy>
{
private:
	EvadeShots() {}

public:
	// This is a singleton
	static EvadeShots* Instance();

	void EnterState(HardEnemy* entity);
	void ExecuteState(HardEnemy* entity);
	void ExitState(HardEnemy* entity);

	bool OnMessage(HardEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////////////////

class SeekTarget : public State<HardEnemy>
{
private:
	SeekTarget() {}

public:
	// This is a singleton
	static SeekTarget* Instance();

	void EnterState(HardEnemy* entity);
	void ExecuteState(HardEnemy* entity);
	void ExitState(HardEnemy* entity);

	bool OnMessage(HardEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////


#endif