/*
This is the Head of the turret. This has its own AI functions which will look at the player when in range and shoot towards him.
The Turret will have a max and minimum pitch whcih determines how much it can look up and down by. For now it does not have a proper
field of view implementation instead it compares the angle obtained from -asin to see if the player is above/below the angle as a result  not shooting.

Author : Romesh Selvanathan

Last Edit : 26/02/2012
*/

#ifndef __TURRETHEAD_H__
#define __TURRETHEAD_H__

/////////////////////////////////////////ATTACKING///////////////////////////////////////////////////////////////////////////////////////////////
#define PERCEPT_DISTANCE 20.0f			// Distance where enemy will start chasing the player from center of enemy
#define SHOOT_DELAY 150					// A Delay before the next shoot method is called
#define ATTACK_DELAY 100				// A delay before the next melee attack is allowed
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "GameUtil.h"
#include "Enemy.h"
#include "StateMachine.h"

class TurretHead : public Enemy
{
public :
	/* Constructor */
	TurretHead(float minPitch, float maxPitch, int turretPlacement, MovingEntity* target);
	/* Destructor */
	virtual ~TurretHead();

	///////////////////////////////////////////GameObject///////////////////////////////
	bool Init(){return true;}
	/* Reset the Turret */
	void Reset();
	virtual void Update(float dt);
	virtual bool HandleMessage(const Telegram& msg);
	////////////////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////ITimerListener//////////////////////////////////////
	void OnTimer(float milliseconds);
	//////////////////////////////////////////////////////////////////////////////////////////

	// Initialize The Fuzzy Module as well as creating the rules for fuzzy Logic
	void InitializeFuzzyModule(){}
	// Use Fuzzy Logic to determine what action to take depending on the distance of the player
	float GetDesirability(float distanceToPlayer){ return 0;}
	/* Update the Enemy according to its current health */ 
	virtual void UpdateEnemy(){}
	/* Shoot Towards a target */
	void ShootBullet(Vec3 direction);
	// Face the target - Very specific method so cannot use AISteering version
	void FaceTarget();

	void SetPitchThreshold(float min, float max) {m_minPitchThreshold = min; m_maxPitchThreshold = max;}
	/* Get Methods */
	float GetTest(){return test;}
	StateMachine<TurretHead>* GetStateMachine() {return m_pStateMachine;}

	/* Where the Turret will be places relative to the big enemy */
	enum{TOP, BOTTOM};

private:
	StateMachine<TurretHead>* m_pStateMachine;

	int m_placement;					// Current placement of the Turret

	bool m_hasShot;						// Has the turret shot recently?
	int m_tillNextShot;					// Time till the next shot is allowed		

	float m_minPitchThreshold, m_maxPitchThreshold;	// The minimum and maximum amount the turret can look up and down by
	float m_turnModifier;				// This value modifies the degrees depending on the placement (if "bottom" angle is multiplied by -1)
	
	float test;							// Debugging
};

#endif