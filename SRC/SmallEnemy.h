/*
This is the small enemy that will be available in swarms. These enemies
can communicate with each other and will flee the player when low in health. However
This also depdns on the persoanlity value, as some may tend to be really aggressive. 

Author : Romesh Selvanathan

Date : 3/05/2012
*/

#ifndef __SMALLENEMY_H__
#define __SMALLENEMY_H__

#include "GameUtil.h"
#include "Enemy.h"
#include "MovingEntity.h"
#include "StateMachine.h"
#include "IGameWorldListener.h"

const int HEALTH_POINTS = 3;

class SmallEnemy : public Enemy, public IGameWorldListener 
{
public:
	SmallEnemy(MovingEntity* target);
	virtual ~SmallEnemy();

	bool Init(){return true;}
	void Reset();

	virtual void Update(float dt);
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);
	virtual bool HandleMessage(const Telegram& msg); 

	//-----------------------------------ITimerListener---------------------------------------
	void OnTimer(float milliseconds);
	//-----------------------------------IGameWorldListener----------------------------------
	void OnObjectAdded(GameWorld* world, Entity* object);
	void OnObjectRemoved(GameWorld* world, Entity* object);
	//---------------------------------------------------------------------------------------

	/* Update the Enemy according to its current health */ 
	void UpdateEnemy();
	/* Shoot A bullet */ 
	void ShootBullet(Vec3 direction);
	/* Call for Help */
	void ShoutForHelp();

	float GetTest() {return test;}
	void SetTest(float v) {test = v;}
	StateMachine<SmallEnemy>* GetStateMachine() {return m_pStateMachine;}
	float HasBeenAttacked() {return m_hasBeenAttacked;}

private:
	StateMachine<SmallEnemy>* m_pStateMachine; // The State machine allowing this enemy to change states

	float test;

	bool m_bCalledHelp;					// A flag to see if the agent has already called for help 
	float m_tillNextCall;				// A time till the next call is allowed

	bool m_hasShot;						// A boolean to see if enemy has shot recently
	float m_tillNextShot;					// time till next shot is allowed

	bool m_bEnergyUsed;					// A Flag to see if energy has been used recently
	float m_fTillEnergyRecharge;			// Timer to see how long it should be

	bool m_hasBeenAttacked;				// A boolean to see it enemy has been attacked by the player recently
	float m_tillNextUpdate;				// time till next update is allowed
		
	bool m_attackChase;					// A boolean to see if enemy has chased the player completly yet
	float m_tillNextChase;				// time allowed till the enemy stops chasing the player
};

#endif