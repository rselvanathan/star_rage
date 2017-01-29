/*
Contains All the States for the TurretHead.

Attack the Target - Stay Idle

Author : Romesh Selvanathan

Date : 3/05/12
*/

#include "TurretHeadStates.h"
#include "TurretHead.h"
#include "Telegram.h"
#include "GameUtil.h"
#include "MessageType.h"
#include "AISteering.h"
#include "Enemy.h"

///////////////////////////////////////GLOBAL STATE////////////////////////////////////

GlobalTurretHeadState* GlobalTurretHeadState::Instance()
{
	static GlobalTurretHeadState instance;

	return &instance;
}

void GlobalTurretHeadState::ExecuteState(TurretHead* entity)
{
	// Nothing to do 
}

bool GlobalTurretHeadState::OnMessage(TurretHead* entity, const Telegram& tele)
{
	return true;
}

///////////////////////////////////CHASE STATE//////////////////////////////////////////

AttackState* AttackState::Instance()
{
	static AttackState instance;

	return &instance;
}

void AttackState::EnterState(TurretHead* entity)
{
	// To delay the shot slightly 
	entity->SetIgnoreShoot(true);
}

void AttackState::ExecuteState(TurretHead* entity) 
{
	// obtain the direction the target
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	Vec3 direction = targetPos - entity->GetPosition();
	// if target is far away then stay idle)
	if(direction.Length() > 40)
		entity->GetStateMachine()->ChangeState(IdleState::Instance());
	// Face the target
	entity->FaceTarget();
}

void AttackState::ExitState(TurretHead* entity)
{
	//Do nothing
}

bool AttackState::OnMessage(TurretHead* entity, const Telegram& tele)
{

	return true;
}

//////////////////////////////EVADE STATE ///////////////////////////////////////////////////

IdleState* IdleState::Instance()
{
	static IdleState Instance;

	return &Instance;
}

void IdleState::EnterState(TurretHead* entity)
{
	// Do nothing
}

void IdleState::ExecuteState(TurretHead* entity)
{
	// Obtain the direction to the target
	Vec3 targetPos = entity->GetSteering()->GetTargetObject()->GetPosition();
	Vec3 direction = targetPos - entity->GetPosition();
	// if it is within range change to attacking the target
	if(direction.Length() < 35)
		entity->GetStateMachine()->ChangeState(AttackState::Instance());
}

void IdleState::ExitState(TurretHead* entity)
{
	// Do nothing
}

bool IdleState::OnMessage(TurretHead* entity, const Telegram& tele)
{
	return true;
}