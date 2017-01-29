/* 
This class is Particle Emitter Manager class that manages all the particles created and adds more if needed. This basically
solves the memory issues for the Particle Emitters. What this class does is Add a New Particle Emitter whenever required, however
if particle emitters are already in the list and not active it will reuse those instead of creating a new one. This class will be 
pointed by every game object through the game world, so whenever a game object is added this class will be Set to that GameObject.

I am going to keep a seperate list for each Particle Effect that I am trying to achieve as this will be easier to manage, update and render.
This class is also a singleton class.

Author : Romesh Selvanathan
Date : 20/02/2012
*/

#ifndef __PARTICLEMANAGER_H__
#define __PARTICLEMANAGER_H__

#include "GameUtil.h"

class MovingEntity;
class SmokeParticle;
class ExplosionFireParticle;
class ExplosionParticle;
class ExplosionSmokeParticle;

// Structure that contains three seperate Emitters to combine into 
// a Single Explosion effect
struct Explosion
{
	SmartPtr<ExplosionFireParticle> fire;
	SmartPtr<ExplosionParticle> spark;
	SmartPtr<ExplosionSmokeParticle> smoke;
	bool active;
};

class ParticleManager
{
public:
	static ParticleManager* GetInstance()
	{
		if(mParticleManager == NULL)
			mParticleManager = new ParticleManager();
		return mParticleManager;
	} 
	/* Destructor */
	~ParticleManager();
	/* Reset the Manager */
	void Reset();
	/* Update all the active emitters Particle Emitters */
	void Update(float dt);
	/* Render all the active Particle Emitters */
	void Render();

	/* Add Specific Particle Effects to the world */
	// Add a Smoke plume effect at a certain position, with the scale and the object that owns it
	void AddSmoke(Vec3 pos, float scale, MovingEntity* o);
	// Remove the smoke effect
	void RemoveSmoke(MovingEntity* o);
	// Add the explosion effect At a specific position
	void AddExplosion(Vec3 pos);
	// Add the Spark effect at a speficic position
	void AddSpark(Vec3 pos);

	/* Debugging */
	int GetCount() {return count;}

private:
	static ParticleManager* mParticleManager;
	/* Constructor */
	ParticleManager();

	typedef vector<SmartPtr<SmokeParticle>> SmokeList;
	SmokeList smokes;									// List of available Smoke Plume Particle Effects

	typedef vector<SmartPtr<Explosion>> ExplosionList;		
	ExplosionList explosions;							// List of available Explosion effects

	typedef vector<SmartPtr<ExplosionParticle>> SparksList;		
	SparksList sparks;									// List of available  sparks

	int count;											// Debugging
};

#endif