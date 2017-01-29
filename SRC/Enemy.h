/*
This is the base class for all the enemies. This class constains some important properties
for any type of enemy, such as health and energy. It also contains the pointers to the AISteering
and the Targeting class.
The class finally provides the different utlity functions that determine what an object prefers,
depending on some properties.

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __ENEMY_H__
#define __ENEMY_H__

#include "GameUtil.h"
#include "MovingEntity.h"
#include "ITimerListener.h"
#include "Enemy_Targeting.h"
#include "CRandom.h"

class AISteering;

const float MAX_ENERGY = 100.0f;	// The maximum energy for any enemy
const int MAX_HEALTH = 3;			// The maximum health for any enemy

class Enemy : public MovingEntity
{
private:
	typedef vector<Entity*> PotentialTargetList;

public:
	// Constrctor
	Enemy(const char* typeName);
	// Constructor with a specific ID
	Enemy(const char* typeName, int& ID);
	// Destructor
	virtual ~Enemy();

	// Virtual functions for the inheriting enemies to define
	virtual bool Init() = 0;
	virtual void Reset() = 0;

	virtual void Update(float dt) = 0;
	// Render the enemy
	virtual void Render();
	virtual bool HandleMessage(const Telegram& msg) = 0;

	//////////////////////////////////////ITimerListener//////////////////////////////////////
	virtual void OnTimer(float milliseconds) = 0;
	//////////////////////////////////////////////////////////////////////////////////////////

	/* Update the Enemy according to its current health */ 
	virtual void UpdateEnemy() = 0;
	/* Shoot A bullet */ 
	virtual void ShootBullet(Vec3 direction) = 0;

	// References From Programming Game AI by example - Chapter 9
	//---------------------------------Decision Methods-------------------------------
	// This method will return how much a health pot is desired depending on the current health of the enemy
	// and the distance to the nearest health power up. So desirability is directtly propertioanl to how much health
	// is left and indirectly proportional to how far the nearest health power up is. A constant tweaker value
	// will be used to determine how much it will seek it by and the personality value will determine if it is a
	// safe or reckless enemy
	float CalcHealthDesirability(float distance, float maxDistance);
	// This method is similar to the above, where it determines whether the player should be the current target or not.
	// This will depend on the current health and the distance from the player itself (May change). Again tweaker and personaliy
	// are used here.
	float CalcPlayerDesirability(float distance, float maxDistance);
	// This method again similar to the above but determines whether the enemy needs a energy powerup or not
	float CalcEnergyDesirability(float distance, float maxDistance);
	//--------------------------------------------------------------------------------

	// Generate the personality of the Enemy
	void GeneratePersonality();

	const vector<Entity*> GetPotentialTargetArr() const {return m_potentialTargets;}
	const int GetHealth() const {return mHealth;}
	const bool IgnoreShot() const {return m_bIgnoreShot;}
	const float GetEnergy() const {return m_fEnergy;}
	AISteering* GetSteering() const {return m_pSteering;}
	Entity* GetTargetObject() const {return m_pTargetObject;}
	MovingEntity* GetMainTarget() const {return m_pMainTarget;}

	void SetTargetObject(Entity* o) {m_pTargetObject = o;}
	void SetHealth(int health) {mHealth = health;}
	void SetIgnoreShoot(bool value) {m_bIgnoreShot = value; m_fTimeIgnoreShoot = m_fTimeElapsed;}

protected:
	// The Steering Behaviour for each enemy
	AISteering* m_pSteering;
	// The Targeting System for the AI
	Enemy_Targeting* m_pTargeting;
	// This is the current target object of the enemy
	Entity* m_pTargetObject;
	// This is the main target object of the Enemy i.e player, which will usually stay constant
	// This is used when resetting the game so that the pointer to the main target object does not change, when the potential
	// targets list is reset - otherwise have to change the reset method where the spaceship is provided every time reset is called
	MovingEntity* m_pMainTarget;
	// Define A list of potential targets - This will keep track of all the potential targets the enemy
	// can lock onto - this will be used for decision making
	PotentialTargetList m_potentialTargets;
	// Current Health of the enemy
	int mHealth;
	// Current Energy of the player used for shooting
	float m_fEnergy;
	// the speed multiplier 
	float speedMultiplier;	
	// AI Personality values that determine how much it seeks a object, i.e an AI with higher healthbias, will more likely always look out for a
	// health pack rather than a player
	float m_fHealthBias, m_fEnergyBias, m_fPlayerBias;
	// Random Number generator
	CRandom random;

	// A Quick Fix to make sure the shot is not fired if as soon as the player is in range
	bool m_bIgnoreShot;
	float m_fTimeIgnoreShoot;
};

#endif