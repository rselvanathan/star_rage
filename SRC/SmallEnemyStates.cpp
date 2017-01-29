/*
Contains All the States for the Small Enemy.

Chase the Player - Seek a target position - Wander - Flee From a target

Author : Romesh Selvanathan

Date : 3/05/12
*/
#include "SmallEnemyStates.h"
#include "SmallEnemy.h"
#include "Telegram.h"
#include "GameUtil.h"
#include "MessageType.h"
#include "AISteering.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "Enemy.h"

///////////////////////////////////////GLOBAL STATE////////////////////////////////////

GlobalSmallEnemyState* GlobalSmallEnemyState::Instance()
{
	static GlobalSmallEnemyState instance;

	return &instance;
}

void GlobalSmallEnemyState::ExecuteState(SmallEnemy* entity)
{
	// If target object is a health power up then Seek it rather then Chasing (Chase does arrive not seek, so it will stop
	// just before it reaches the target)
	if(entity->GetTargetObject()->GetType() == GameObjectType("poweruphealth") || entity->GetTargetObject()->GetType() == GameObjectType("powerupenergy")){
			entity->GetSteering()->SetTarget(entity->GetTargetObject()->GetPosition());
			entity->GetStateMachine()->ChangeState(SeekState::Instance());
	}
}

bool GlobalSmallEnemyState::OnMessage(SmallEnemy* entity, const Telegram& tele)
{
	return false;
}

///////////////////////////////////CHASE STATE//////////////////////////////////////////

ChasePlayerState* ChasePlayerState::Instance()
{
	static ChasePlayerState instance;

	return &instance;
}

void ChasePlayerState::EnterState(SmallEnemy* entity)
{
	// Set The Arrive behaviour with a specific distance to stop
	entity->GetSteering()->ArriveOn(4);
	// Used to delay the shooting slightly
	entity->SetIgnoreShoot(true);
}

void ChasePlayerState::ExecuteState(SmallEnemy* entity) 
{
	// Obtain the target position
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	// obtain the direction in which the target is
	Vec3 direction = targetPos - entity->GetPosition();
	// obtain the distance to the target
	float distance = direction.Length();
	// if distance is greater than a certain range and enemy is healthy then wander
	if(distance > 37 && entity->GetHealth() > 1 )
		entity->GetStateMachine()->ChangeState(WanderState::Instance());
	// if the health is low then flee
	else if( entity->GetHealth() <= 1)
		entity->GetStateMachine()->ChangeState(FleeState::Instance());
	// If the Enery is low flee
	else if(entity->GetEnergy() <= 10)
		entity->GetStateMachine()->ChangeState(FleeState::Instance());

	// Normalize the direction and if target is within range shoot
	if(direction.Length() > 0)
		direction.Normalize();
	if(distance < 30)
		if(!entity->IgnoreShot())
			entity->ShootBullet(direction);
}

void ChasePlayerState::ExitState(SmallEnemy* entity)
{
	// Set the arrive behaviour off
	entity->GetSteering()->ArriveOff();
	entity->SetMaxSpeed(2);
}

bool ChasePlayerState::OnMessage(SmallEnemy* entity, const Telegram& tele)
{
	return false;
}

//////////////////////////////EVADE STATE ///////////////////////////////////////////////////

WanderState* WanderState::Instance()
{
	static WanderState Instance;

	return &Instance;
}

void WanderState::EnterState(SmallEnemy* entity)
{
	// Set the wander behaviour on
	entity->GetSteering()->WanderOn();
}

void WanderState::ExecuteState(SmallEnemy* entity)
{
	// Obtain target Position
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	// obtain direction to that position
	Vec3 direction = targetPos - entity->GetPosition();
	// obtain the distance
	float distance = direction.Length();
	// if the distance to current target object is less than a certain amount and 
	// the enemy is healthy then chase the target
	if(distance < 25 && entity->GetHealth() > 1 )
		entity->GetStateMachine()->ChangeState(ChasePlayerState::Instance());
	// otherwise Flee from it
	else if(distance < 35  && entity->GetHealth() == 1)
		entity->GetStateMachine()->ChangeState(FleeState::Instance());

	// If the health is now low and the entity has been attacked then chase the player
	if(entity->GetHealth() > 1 && entity->HasBeenAttacked())
		entity->GetStateMachine()->ChangeState(ChasePlayerState::Instance());
}

void WanderState::ExitState(SmallEnemy* entity)
{
	// switch off the wander behaviour
	entity->GetSteering()->WanderOff();
}

bool WanderState::OnMessage(SmallEnemy* entity, const Telegram& tele)
{
	// During wandering if an enemy shouts for help, go to that position
	// where help was called from
	switch(tele.m_message)
	{
	case msg_enemyShoutHelp:
		{
			if(tele.m_sender != entity->GetID()){
				entity->GetSteering()->SetTarget(*(Vec3*)tele.m_extraInfo);
				entity->GetStateMachine()->ChangeState(SeekState::Instance());
			}
		}
	}
	return true;
}

//////////////////////////////////////FLEE STATE///////////////////////////////////////

FleeState* FleeState::Instance()
{
	static FleeState Instance;

	return &Instance;
}

void FleeState::EnterState(SmallEnemy* entity)
{
	// If health is low then shout for help, before entering this state
	if(entity->GetHealth() == 1)
		entity->ShoutForHelp();

	// Set the Flee State on
	entity->GetSteering()->FleeOn();
	// Increase the speed
	entity->SetMaxSpeed(10);
}

void FleeState::ExecuteState(SmallEnemy* entity)
{
	// OBtain the opposite direction to the current target objects position
	Vec3 newDirection = entity->GetPosition() - entity->GetSteering()->GetTargetObject()->GetPosition();
	// normalize that direction
	newDirection.Normalize();
	// Multiply that position by an amount to get as far away as possible
	Vec3 newPos = newDirection * 150;
	// Set the new target position
	entity->GetSteering()->SetTarget(newPos);

	// Obtain the distance to the target object
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	Vec3 direction = targetPos - entity->GetPosition();
	float distance = direction.Length();
	// if it is greater than some amount, then wander again
	if(distance > 30 )
		entity->GetStateMachine()->ChangeState(WanderState::Instance());
}

void FleeState::ExitState(SmallEnemy* entity)
{
	// Switch fleeing off and reset the speed
	entity->GetSteering()->FleeOff();
	entity->SetMaxSpeed(2);
}

bool FleeState::OnMessage(SmallEnemy* entity, const Telegram& tele)
{
	return false;
}

///////////////////////////////////SEEK STATE////////////////////////////////////////////////

SeekState* SeekState::Instance()
{
	static SeekState Instance;

	return &Instance;
}

void SeekState::EnterState(SmallEnemy* entity)
{
	// Switch the arrive behaviour on with a distance to stop before reaching the target
	entity->GetSteering()->ArriveOn(1);
	// Set the Speed
	entity->SetMaxSpeed(5);
}

void SeekState::ExecuteState(SmallEnemy* entity)
{

	// Obtain the distance and direction for seeking the target
	Vec3 seekPos = entity->GetSteering()->GetTargetPos() - entity->GetPosition();
	float seekDist = seekPos.Length();
	// If the seek position if less than 0 then change the state back wandering
	if(seekDist <= 0)
		entity->GetStateMachine()->ChangeState(WanderState::Instance());

	// If the current target is the player 
	if(entity->GetTargetObject()->GetType() == GameObjectType("spaceship")) {
		// Work out the direction and distance to the player 
		Vec3 direction = entity->GetTargetObject()->GetPosition() - entity->GetPosition();
		float distance = direction.Length();
		// if its within a range and enemy is healthy then chase the player
		if(distance < 20 && entity->GetHealth() > 1)
			entity->GetStateMachine()->ChangeState(ChasePlayerState::Instance());
		// otherwise flee
		else if(distance < 35 && entity->GetHealth() == 1)
			entity->GetStateMachine()->ChangeState(FleeState::Instance());
	}
}

void SeekState::ExitState(SmallEnemy* entity)
{
	// reset the speed and switch off the arrive behaviour
	entity->SetMaxSpeed(2);
	entity->GetSteering()->ArriveOff();
}

bool SeekState::OnMessage(SmallEnemy* entity, const Telegram& tele)
{
	return false;
}