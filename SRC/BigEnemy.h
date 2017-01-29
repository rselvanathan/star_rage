/*
This is the big enemy which, does not move currently. This enemy will create two turret, one on top
and another at the bottom, and will be stationary. When this enemy dies so will the turrets.

Author : Romesh Selvanathan

Date : 3/05/2012
*/

#ifndef __BIGENEMY_H__
#define __BIGENEMY_H__

#include "GameUtil.h"
#include "Enemy.h"
#include "MovingEntity.h"
#include "StateMachine.h"
#include "IGameWorldListener.h"
#include "TurretHead.h"

class TurretBase;
class TurretHead;

#define HEALTH_POINTS 3					// The Health of the enemy
#define UPDATE_HEALTH_DELAY 60			// A Delay to decrease the health value. Used because when the shoot object goes through the collision sphere
										// of the enemy the updates occur to frequently so had to put this up making the health go down by one at a time

class BigEnemy : public Enemy, public IGameWorldListener
{
public:
	BigEnemy(MovingEntity* target);
	virtual ~BigEnemy();

	bool Init(){return true;}
	void Reset() ;

	///////////////////////////////////////////GameObject///////////////////////////////
	virtual void Update(float dt);
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);
	virtual bool HandleMessage(const Telegram& msg);
	////////////////////////////////////////////////////////////////////////////////////

	/* Update the Enemy according to its current health */ 
	void UpdateEnemy();
	/* Shoot A bullet */ 
	void ShootBullet(Vec3 direction){}

	// Debugging
	float GetTest() {return test;}
	float GetTestTurret() {return mTurretHeadBottom->GetTest();}

	////////////////////////////////////////////IGAMEWORLDLISTENER///////////////////////////
	void OnObjectAdded(GameWorld* world, Entity* object);
	void OnObjectRemoved(GameWorld* world, Entity* object);
	////////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////ITimerListener//////////////////////////////////////
	void OnTimer(float milliseconds);
	//////////////////////////////////////////////////////////////////////////////////////////

private:
	bool m_hasBeenAttacked;				// A boolean to see it enemy has been attacked by the player recently
	int m_tillNextUpdate;				// time till next update is allowed

	// The two turrets the big enemy contains
	SmartPtr<TurretBase> mTurretBase;
	SmartPtr<TurretHead> mTurretHead;

	SmartPtr<TurretBase> mTurretBaseBottom;
	SmartPtr<TurretHead> mTurretHeadBottom;

	float test;
};

#endif