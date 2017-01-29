/*
This class represent an asteroid in the game world. This object will travel in a certain direction
and when it reaches the end, it will reset itself on the other side again, creating an illusion of 
as constant stream of asteroid

Author : Romesh Selvanathan

Date : 03/05/12
*/

#ifndef __ASTEROID_H__
#define __ASTEROID_H__

// Have added Ienemylistener in here to test the gui

#include "GameUtil.h"
#include "MovingEntity.h"
#include "CRandom.h"

class Asteroid : public MovingEntity
{
public :
	// Create a asteroid specifying which side it will begin from
	Asteroid(int side);
	virtual ~Asteroid();

	bool Init(){return true;}
	void Reset();

	virtual void Update(float dt);
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);
	virtual bool HandleMessage(const Telegram& msg); 

	//-------------------------------ITimerListener-------------------------------------
	void OnTimer(float milliseconds);
	//----------------------------------------------------------------------------------

	enum {LEFT, RIGHT};		// THe two types of sides

private:
	CRandom r;						// A Random number generator
	int m_iLastCollisionTimeStep;	// This value will create a delay for the collisions
	bool m_bCheckBounce;			// A flag for the delay in collision test

	int m_iSide;					// the current side the asteroid began from
};

#endif