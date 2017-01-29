/*
This is essentially the brain of the Enemy. This class encapsulated all the Different Enemy behaviours
which they can use, by simply enabling or disabling them. Thi class simply returns a total force
which is used to steer an enemy some where

Author : Romesh Selvanathan
Date : 03/05/12

References : 
Buckland, M. (2005). Programming Game AI by Example. Texas: Wordware.
*/

#ifndef __AISTEERING_H__
#define __AISTEERING_H__

#include "GameUtil.h"
#include "CRandom.h"

class Enemy;
class Entity;
class MovingEntity;

//---------------------------Constants----------------------------------

// maximum number of moves before agent has to return back to starting position
#define WANDER_MAX_MOVES 5
// minimum Wandering Distance per each movement
#define MIN_WANDER_DIST 5.0
// A epsilon value to see if distance is below it ( SO that the enemy can generate a new wander distance)
#define EPSILON_DISTANCE 3.0

//--------------------------------------------------------------------

class AISteering
{
public:
	// Constructor
	AISteering(Enemy* enemy);
	// Destructor
	virtual ~AISteering();
	// Reset all the variables
	void Reset();

	// Calculate the Final Total Force
	Vec3 Calculate();
	// Calcuate the force by priority(give priority to some behaviours compared to others)
	Vec3 CalculatePriority();

	// Set the Target Position
	void SetTarget(const Vec3 t){m_vTarget = t;}
	// Set the target Object
	void SetTargetObject(Entity* o) {m_pTargetObject = o;}

	// Accessor methods
	float GetTest() {return test;}
	const Vec3 GetForce() const {return m_vSteeringForce;}
	const Entity* GetTargetObject() const {return m_pTargetObject;}
	Vec3 GetTargetPos() const {return m_vTarget;}
	Quaternion GetQuatToFaceTarget();
	bool IsEntityFacingPlayer();

	// These switch on the various behaviours off and on
	void FleeOn() {if(!On(flee)) flee = true;}
	void SeekOn() {if(!On(seek)) seek = true;}
	void ArriveOn(float distance) {if(!On(arrive)) arrive = true; m_fArriveDistance = distance;}
	void WanderOn() {if(!On(wander)) wander = true;}
	void EvadeOn(MovingEntity* o) {if(!On(evade)) evade = true; m_pEvadeObject = o;}
	void FleeOff() {if(On(flee)) flee = false;}
	void SeekOff() {if(On(seek)) seek = false;}
	void ArriveOff() {if(On(arrive)) arrive = false;}
	void WanderOff() {if(On(wander)) wander = false;}
	void EvadeOff() {if(On(evade)) evade = false;}

	// To see if a specfifc behaviour is still valid
	bool IsFleeOn() {return On(flee);}
	bool IsSeekOn() {return On(seek);}
	bool IsArriveOn() {return On(arrive);}
	bool IsWanderOn() {return On(wander);}
	bool IsEvadeOn() {return On(evade);}

private:
	// A Random number generator
	CRandom m_RandGenerator;

	// Pointer to the owner
	Enemy* m_pEnemy;

	// Pointer to the Target Object of the enemy
	Entity* m_pTargetObject;
	// Pointer to the object to evade - will be set whenever the evade function is called
	MovingEntity* m_pEvadeObject;

	// The FInal Steering Force returned to the Enemy
	Vec3 m_vSteeringForce;
	// The Target position
	Vec3 m_vTarget;

	// Number of moves covered
	int m_iWanderMovesCount;
	// the wandering start position
	Vec3 m_vWanderStartPosition;
	// the wandering end position
	Vec3 m_vWanderDestPosition;	

	// The flags for the different behaviours
	bool wander;
	bool flee;
	bool arrive;
	bool seek;
	bool evade;

	// Not currently used
	float test;
	float m_fViewDistance;
	float m_fArriveDistance;

	// For the use of Arrive
	enum Decelaration {slow = 3, normal = 2, fast = 2};

	// this function tests if a sepcific bit of m_iflag is set
	//bool On(behaviours bt) {return (m_iFlag & bt) == bt;}
	bool On(bool bH) {return (bH == true);}

	// Accumulate all the forces together
	bool AccumulateForce(Vec3 &sf, Vec3 ForceToAdd);
	// Obtain a Random Position
	Vec3 RandomPosition();

	///////////////////////////Behaviours////////////////////////////
	Vec3 Seek(Vec3 target);
	Vec3 Flee(Vec3 target);
	Vec3 Arrive(Vec3 target, Decelaration type);
	Vec3 Wander();
	Vec3 Pursuit(const MovingEntity* object);
	Vec3 Evade (const MovingEntity* object);
	Vec3 Seperate(const vector<Enemy*> &neighbours);
};

#endif