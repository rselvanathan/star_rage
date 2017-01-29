/*
Contains All the States for the Small Enemy.

Chase the Player - Seek a target position - Wander - Flee From a target

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __SMALLENEMYSTATES_H__
#define __SMALLENEMYSTATES_H__

#include "GameUtil.h"
#include "State.h"
#include "Telegram.h"

class SmallEnemy;

//////////////////////////////////////////////////////////////////////////////////////////////

class GlobalSmallEnemyState : public State<SmallEnemy>
{
private:
	GlobalSmallEnemyState() {distance = MaxFloat;}
	float distance;
public:
	// This is a singleton
	static GlobalSmallEnemyState* Instance();

	void EnterState(SmallEnemy* entity){}
	void ExecuteState(SmallEnemy* entity);
	void ExitState(SmallEnemy* entity){}

	bool OnMessage(SmallEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class ChasePlayerState : public State<SmallEnemy>
{
private:
	ChasePlayerState() {}

public:
	// This is a singleton
	static ChasePlayerState* Instance();

	void EnterState(SmallEnemy* entity);
	void ExecuteState(SmallEnemy* entity);
	void ExitState(SmallEnemy* entity);

	bool OnMessage(SmallEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class WanderState : public State<SmallEnemy>
{
private:
	WanderState() {}

public:
	// This is a singleton
	static WanderState* Instance();

	void EnterState(SmallEnemy* entity);
	void ExecuteState(SmallEnemy* entity);
	void ExitState(SmallEnemy* entity);

	bool OnMessage(SmallEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class FleeState : public State<SmallEnemy>
{
private:
	FleeState() {}

public:
	// This is a singleton
	static FleeState* Instance();

	void EnterState(SmallEnemy* entity);
	void ExecuteState(SmallEnemy* entity);
	void ExitState(SmallEnemy* entity);

	bool OnMessage(SmallEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

class SeekState : public State<SmallEnemy>
{
private:
	SeekState() {}

public:
	// This is a singleton
	static SeekState* Instance();

	void EnterState(SmallEnemy* entity);
	void ExecuteState(SmallEnemy* entity);
	void ExitState(SmallEnemy* entity);

	bool OnMessage(SmallEnemy*, const Telegram&); 
};

//////////////////////////////////////////////////////////////////////////////////////////////

#endif