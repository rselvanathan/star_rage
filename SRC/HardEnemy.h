/*
This is the hard enemy that is a more difficult enemy as it can evade. 

Author : Romesh Selvanathan

Date : 3/05/2012
*/
#ifndef __HARDENEMY_H__
#define __HARDENEMY_H__

#include "GameUtil.h"
#include "Enemy.h"
#include "StateMachine.h"
#include "AISteering.h"
#include "IGameWorldListener.h"

class HardEnemy : public Enemy, public IGameWorldListener 
{
public:
	HardEnemy(MovingEntity* target);
	virtual ~HardEnemy();

	bool Init(){return true;}
	void Reset();

	virtual void Update(float dt);
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);
	virtual bool HandleMessage(const Telegram& msg); 

	//------------------------------------ITimerListener--------------------------------------
	void OnTimer(float milliseconds);
	//------------------------------------IGameWorldListener----------------------------------
	void OnObjectAdded(GameWorld* world, Entity* object);
	void OnObjectRemoved(GameWorld* world, Entity* object);
	//----------------------------------------------------------------------------------------

	/* Update the Enemy according to its current health */ 
	void UpdateEnemy();
	/* Shoot A bullet */ 
	void ShootBullet(Vec3 direction);

	float GetTest() {return test;}
	StateMachine<HardEnemy>* GetStateMachine() {return m_pStateMachine;}
	MovingEntity* GetObjectToEvade() const {return m_pObjectToEvade;} 

	void SetObjectToEvade(MovingEntity* o) {m_pObjectToEvade = o;}
private:
	StateMachine<HardEnemy>* m_pStateMachine;	// The State machine allowing this enemy to change states
	MovingEntity* m_pObjectToEvade;				// Pointer to the object this enemy should evade

	float test;

	bool m_hasShot;						// A boolean to see if enemy has shot recently
	float m_tillNextShot;					// time till next shot is allowed

	bool m_hasBeenAttacked;				// A boolean to see it enemy has been attacked by the player recently
	float m_tillNextUpdate;				// time till next update is allowed

	bool m_bEnergyUsed;					// A Flag to see if energy has been used recently
	float m_fTillEnergyRecharge;			// Timer to see how long it should be
};

#endif