/*
Contains All the States for the Hard Enemy.

Chase the Player - Seek a target - Evade a shot

Author : Romesh Selvanathan

Date : 3/05/12
*/

#include "HardEnemyStates.h"
#include "HardEnemy.h"
#include "Telegram.h"
#include "GameUtil.h"
#include "MessageType.h"
#include "Enemy.h"

///////////////////////////////////////GLOBAL STATE////////////////////////////////////

GlobalHardEnemyState* GlobalHardEnemyState::Instance()
{
	static GlobalHardEnemyState instance;

	return &instance;
}

void GlobalHardEnemyState::ExecuteState(HardEnemy* entity)
{
	// If target object is a health power up then Seek it rather then Chasing (Chase does arrive not seek, so it will stop
	// just before it reaches the target)
	if(entity->GetTargetObject()->GetType() == GameObjectType("poweruphealth") || entity->GetTargetObject()->GetType() == GameObjectType("powerupenergy")){
			entity->GetSteering()->SetTarget(entity->GetTargetObject()->GetPosition());
			entity->GetStateMachine()->ChangeState(SeekTarget::Instance());
	}
}

bool GlobalHardEnemyState::OnMessage(HardEnemy* entity, const Telegram& tele)
{
	return false;
}

///////////////////////////////////CHASE STATE//////////////////////////////////////////

ChasePlayer* ChasePlayer::Instance()
{
	static ChasePlayer instance;

	return &instance;
}

void ChasePlayer::EnterState(HardEnemy* entity)
{
	// Set The Arrive behaviour with a specific distance to stop
	entity->GetSteering()->ArriveOn(6);
	// Used to delay the shooting slightly
	entity->SetIgnoreShoot(true);
}

void ChasePlayer::ExecuteState(HardEnemy* entity) 
{
	// Obtain the target position
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	// obtain the direction in which the target is
	Vec3 direction = targetPos - entity->GetPosition();
	// obtain the distance to the target
	float distance = direction.Length();
	// normalize the direction
	if(direction.Length() > 0)
		direction.Normalize();
	// if the distance is less than specific amount and the shoot delay is not active
	// then shoot toward the target
	if(distance < 30)
		if(!entity->IgnoreShot())
			entity->ShootBullet(direction);
}

void ChasePlayer::ExitState(HardEnemy* entity)
{
	// Set the arrive behaviour off
	entity->GetSteering()->ArriveOff();
}

bool ChasePlayer::OnMessage(HardEnemy* entity, const Telegram& tele)
{
	// If the player shoots at this stage try to evade the shot
	if(tele.m_message == msg_playerNormalShot) {
		entity->SetObjectToEvade((MovingEntity*)tele.m_extraInfo);
		entity->GetStateMachine()->ChangeState(EvadeShots::Instance());
	}

	return true;
}

//////////////////////////////EVADE STATE ///////////////////////////////////////////////////

EvadeShots* EvadeShots::Instance()
{
	static EvadeShots Instance;

	return &Instance;
}

void EvadeShots::EnterState(HardEnemy* entity)
{
	// Set The Evade behaviour by providing the object to evade
	entity->GetSteering()->EvadeOn(entity->GetObjectToEvade());
	// Set the maximum speed
	entity->SetMaxSpeed(20);
}

void EvadeShots::ExecuteState(HardEnemy* entity)
{
	// If evade is not active then chase the player again
	if(!entity->GetSteering()->IsEvadeOn())
		entity->GetStateMachine()->ChangeState(ChasePlayer::Instance());
}

void EvadeShots::ExitState(HardEnemy* entity)
{
	// Switch off the evade behaviour
	entity->GetSteering()->EvadeOff();
	// Set the speed back to normal
	entity->SetMaxSpeed(2);
}

bool EvadeShots::OnMessage(HardEnemy* entity, const Telegram& tele)
{
	// No message read
	return false;
}

///////////////////////////////////SEEK STATE////////////////////////////////////////////////

SeekTarget* SeekTarget::Instance()
{
	static SeekTarget Instance;

	return &Instance;
}

void SeekTarget::EnterState(HardEnemy* entity)
{
	// Switch the arrive behaviour on with a distance to stop before reaching the target
	entity->GetSteering()->ArriveOn(1);
	// Set the Speed
	entity->SetMaxSpeed(6);
}

void SeekTarget::ExecuteState(HardEnemy* entity)
{

	// Obtain the distance and direction for seeking the target
	Vec3 seekPos = entity->GetSteering()->GetTargetPos() - entity->GetPosition();
	float seekDist = seekPos.Length();
	// If the seek position if less than 0 then change the state back to chasing the player
	if(seekDist <= 0)
		entity->GetStateMachine()->ChangeState(ChasePlayer::Instance());

	// If the current target is the player again during this state, go back to chasing
	if(entity->GetTargetObject()->GetType() == GameObjectType("spaceship")) 
		entity->GetStateMachine()->ChangeState(ChasePlayer::Instance());
}

void SeekTarget::ExitState(HardEnemy* entity)
{
	// reset the speed and switch off the arrive behaviour
	entity->SetMaxSpeed(2);
	entity->GetSteering()->ArriveOff();
}

bool SeekTarget::OnMessage(HardEnemy* entity, const Telegram& tele)
{
	return false;
}
