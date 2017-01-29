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


#include "GameUtil.h"
#include "MovingEntity.h"
#include "GLParticleEngine.h"
#include "SmokeParticle.h"
#include "ParticleManager.h"
#include "ExplosionFireParticle.h"
#include "ExplosionParticle.h"
#include "ExplosionSmokeParticle.h"

ParticleManager* ParticleManager::mParticleManager;

/* Constructor */
ParticleManager::ParticleManager()
{
	count = 0;
}
/* Destructor */
ParticleManager::~ParticleManager()
{
	if(!smokes.empty()){
		for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++)
		{
			SAFE_DELETE(*it);
		}
	}

	if(!explosions.empty()){
		for(ExplosionList::iterator it = explosions.begin(); it != explosions.end(); it++)
		{
			SAFE_DELETE(*it);
		}
	}

	if(!sparks.empty()){
		for(SparksList::iterator it = sparks.begin(); it != sparks.end(); it++)
		{
			SAFE_DELETE(*it);
		}
	}
}

/* 
Reset the Manager 
If the list isn't empty then iterate and whichever emitter is active set it not active
*/
void ParticleManager::Reset()
{
	count = 0;
	if(!smokes.empty()){
		for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++)
		{
			if((*it)->IsActive()){
				(*it)->SetActive(false);
			}
		}
	}

	if(!explosions.empty()){
		for(ExplosionList::iterator it = explosions.begin(); it != explosions.end(); it++)
		{
			if((*it)->active){
				(*it)->active = false;
			}
		}
	}

	if(!sparks.empty()){
		for(SparksList::iterator it = sparks.begin(); it != sparks.end(); it++)
		{
			if((*it)->IsActive()){
				(*it)->SetActive(false);
			}
		}
	}
} 

// Add a Smoke plume effect at a certain position, with the scale and the object that owns it
void ParticleManager::AddSmoke(Vec3 pos, float scale,MovingEntity* o)
{
	// if the list is not empty then check if a emitter is not active
	if(!smokes.empty()){
		for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++){
			if(!(*it)->IsActive()){
				// Set to active and setup the particles and then return from the function, as 
				// you do not have to continue to search through the list for more. Also if the return is 
				// not called then th enext step is ....
				(*it)->SetActive(true);
				(*it)->SetPosition(pos);
				(*it)->SetScale(scale);
				(*it)->SetGameObject(o);
				(*it)->SetupParticles();
				return;
			}
		}
	}
	// ... to add a new emitter to the list which will be Updated
	SmartPtr<SmokeParticle> sprite = new SmokeParticle(0, pos, 300, o);
	sprite->SetupParticles();
	sprite->SetScale(scale);
	sprite->SetActive(true);
	smokes.push_back(sprite);
	// Debugging
	//count ++;
}

// Remove the smoke effect
void ParticleManager::RemoveSmoke(MovingEntity* o)
{
	// When removing smokes if the object given in as a parameter == the gameobject of a item in the list then switch the smoke off
	if(!smokes.empty()){
		for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++){
			if((*it)->IsActive()){
				if(o == (*it)->GetGameObject())
					(*it)->SetActive(false);
			}
		}
	}
}

// Add the explosion effect At a specific position
void ParticleManager::AddExplosion(Vec3 pos)
{
	// if the list is not empty then check if a emitter is not active
	if(!explosions.empty()){
		for(ExplosionList::iterator it = explosions.begin(); it != explosions.end(); it++){
			if(!(*it)->smoke->IsActive()){
				// Set to active and setup the particles and then return from the function, as 
				// you do not have to continue to search through the list for more. Also if the return is 
				// not called then th enext step is ....
				(*it)->smoke->SetActive(true);
				(*it)->fire->SetActive(true);
				(*it)->spark->SetActive(true);
				(*it)->fire->SetPosition(pos);
				(*it)->spark->SetPosition(pos);
				(*it)->smoke->SetPosition(pos);

				(*it)->fire->SetupParticles();
				(*it)->spark->SetupParticles();
				(*it)->smoke->SetupParticles();
				return;
			}
		}
	}
	// ... to add a new emitter to the list which will be Updated
	SmartPtr<ExplosionFireParticle> fire = new ExplosionFireParticle(60, pos, 100);
	fire->SetupParticles();

	SmartPtr<ExplosionParticle> spark = new ExplosionParticle(20, pos, 30);
	spark->SetupParticles();

	SmartPtr<ExplosionSmokeParticle> smoke = new ExplosionSmokeParticle(100, pos, 100);
	smoke->SetupParticles();

	SmartPtr<Explosion> exp = new Explosion();
	exp->fire = fire;
	exp->fire->SetActive(true);
	exp->smoke = smoke;
	exp->smoke->SetActive(true);
	exp->spark = spark;
	exp->spark->SetActive(true);

	explosions.push_back(exp);
	// Debugging
	//count ++;
}

// Add the Spark effect at a speficic position
void ParticleManager::AddSpark(Vec3 pos)
{
	// if the list is not empty then check if a emitter is not active
	if(!sparks.empty()){
		for(SparksList::iterator it = sparks.begin(); it != sparks.end(); it++){
			if(!(*it)->IsActive()){
				// Set to active and setup the particles and then return from the function, as 
				// you do not have to continue to search through the list for more. Also if the return is 
				// not called then th enext step is ....
				(*it)->SetActive(true);
				(*it)->SetPosition(pos);
				(*it)->SetupParticles();
				return;
			}
		}
	}
	// ... to add a new emitter to the list which will be Updated
	SmartPtr<ExplosionParticle> sprite = new ExplosionParticle(40, pos, 10);
	sprite->SetupParticles();
	sprite->SetActive(true);
	sparks.push_back(sprite);
	// Debugging
	count++;
}

/* Update all the active emitters Particle Emitters */
void ParticleManager::Update(float dt)
{
	for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++){
		if((*it)->IsActive()){
			(*it)->Update(dt);
		}
	}

	for(ExplosionList::iterator it = explosions.begin(); it != explosions.end(); it++){
		if((*it)->smoke->IsActive()) 
			(*it)->smoke->Update(dt);
		if((*it)->fire->IsActive())
			(*it)->fire->Update(dt);
		if((*it)->spark->IsActive())
			(*it)->spark->Update(dt);
	}

	for(SparksList::iterator it = sparks.begin(); it != sparks.end(); it++){
		if((*it)->IsActive()){
			(*it)->Update(dt);
		}
	}
}

/* Render all the active Particle Emitters */
void ParticleManager::Render()
{
	for(SmokeList::iterator it = smokes.begin(); it != smokes.end(); it++){
		if((*it)->IsActive()){
			(*it)->Render();
		}
	}

	for(ExplosionList::iterator it = explosions.begin(); it != explosions.end(); it++){
		if((*it)->smoke->IsActive()) 
			(*it)->smoke->Render();
		if((*it)->fire->IsActive())
			(*it)->fire->Render();
		if((*it)->spark->IsActive())
			(*it)->spark->Render();
	}

	for(SparksList::iterator it = sparks.begin(); it != sparks.end(); it++){
		if((*it)->IsActive()){
			(*it)->Render();
		}
	}
}