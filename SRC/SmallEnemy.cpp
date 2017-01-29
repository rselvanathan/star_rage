/*
This is the small enemy that will be available in swarms. These enemies
can communicate with each other and will flee the player when low in health. However
This also depdns on the persoanlity value, as some may tend to be really aggressive. 

Author : Romesh Selvanathan

Date : 3/05/2012
*/

#include "SmallEnemy.h"
#include "GameUtil.h"
#include "Enemy.h"
#include "Model_3DS.h"
#include "BoundingSphere.h"
#include "GameWorld.h"
#include "MovingEntity.h"
#include "Shoot.h"
#include "ParticleManager.h"
#include "ShootManager.h"
#include "ClassIDList.h"
#include "StateMachine.h"
#include "AISteering.h"
#include "Enemy_Targeting.h"
#include "SmallEnemyStates.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "PreLoader.h"

#include "PowerUpManager.h"
#include "FuzzyOperators.h"

// Constructor that will set all the initial values
SmallEnemy::SmallEnemy(MovingEntity* target) : Enemy("smallenemy"),
				 m_hasShot(false),
				 m_tillNextShot(0),
				 m_bEnergyUsed(false),
				 m_fTillEnergyRecharge(0),
				 m_hasBeenAttacked(false),
				 m_tillNextUpdate(0),
				 m_attackChase(false),
				 m_tillNextChase(0)
{
	mRadius = 0.7;
	mBoundingSphere = new BoundingSphere(this, mRadius);

	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//Enemy//SmallEnemy//SmallEnemy.3ds");
	SetModel(model);

	test = 0;
	mHealth = HEALTH_POINTS;
	speedMultiplier = 1;

	mScale = 0.06f;
	m_mass = 1.0f;
	m_maxForce = 10;

	m_pStateMachine = new StateMachine<SmallEnemy>(this);

	m_pSteering = new AISteering(this);
	m_pSteering->SetTargetObject(target);

	m_pTargeting = new Enemy_Targeting(this);

	m_pMainTarget = target;
	m_pTargetObject = target;

	m_fElactisity = 0.4f;
}

// Destructor
SmallEnemy::~SmallEnemy()
{
	Enemy::~Enemy();
}

// Reset all the properties of this class
void SmallEnemy::Reset()
{
	 m_bCalledHelp = false;
	 m_tillNextCall = 0;
	 m_hasShot=false;
	 m_tillNextShot=0;
	 m_bEnergyUsed=false;
     m_fTillEnergyRecharge=0;
	 m_hasBeenAttacked=false;
	 m_tillNextUpdate=0;
	 m_attackChase=false;
	 m_tillNextChase=0;
	 mHealth=HEALTH_POINTS;
	 speedMultiplier=1;

	 m_fTimeElapsed = 0;

	 test = 0.0f;

	 m_maxSpeed = 2;
	 m_fEnergy = 100.f;
	 
	 m_pSteering->Reset();

	 // Reset the current state to the wander state again
	 m_pStateMachine->SetCurrentState(WanderState::Instance());
	 m_pStateMachine->SetPreviousState(WanderState::Instance());
	 m_pStateMachine->SetGlobalState(GlobalSmallEnemyState::Instance());
	 // Enter the current state
	 m_pStateMachine->GetCurrentState()->EnterState(this);
	 // Clear the Potecntial Targets
	 m_potentialTargets.clear();
	 m_potentialTargets.push_back(m_pMainTarget);
	 // If there are active powerups currently then add them into potentialTargets list
	 // This is necessary as when the enemy is reset back into the world, during gameplay, there maybe some active powerups
	 // so the agent has to add them into his potential targets
	 if(!PowerUpManager::GetInstance()->GetActivePowerUps().empty())
		for(int i = 0; i < PowerUpManager::GetInstance()->GetActivePowerUps().size(); i++)
			 m_potentialTargets.push_back((Entity*)PowerUpManager::GetInstance()->GetActivePowerUps()[i]);

	 // Reset all other common values
	 Enemy::Reset();
}

void SmallEnemy::Update(float dt)
{
	// If energy is not being used add the value back up again
	if(!m_bEnergyUsed){
		m_fEnergy += 2 * dt; 
		Clamp(m_fEnergy, 0, 100);
	}
	// Update the targeting system
	m_pTargeting->Update();
	// Update the State Machine for this Agent
	m_pStateMachine->Update();

	// Calculate the Force depending on current behaviour
	m_vForce = m_pSteering->Calculate();

	// Obtain the Orientation axis 
	m_orientation.ExtractOrientationAxis(mRight, mUp, mForward);

	// Obtain the Quaternion to face the target
	m_rotation = m_pSteering->GetQuatToFaceTarget();

	// Update the Orientation of the Entity(Not Local orientation, but the objects world orientation)
	UpdateOrientation(dt);
	// Update the position of the Entity using basic physics from F=ma -> v = u + at -> S = S + vt
	UpdatePosition(m_vForce, dt);
}

bool SmallEnemy::HandleMessage(const Telegram& msg)
{
	if(msg.m_message == msg_elasticCollision)
	{
		// Obtain the other object's velocity
		Vec3 object_initVel = *(Vec3*)msg.m_extraInfo;
		// Do a elastic collision
		ElasticCollision(object_initVel);
		return true;
	}
	// Allow the state machine to handle the message
	if(m_pStateMachine->HandleMessage(msg))
		return true;
	return false;
}

/* Update the enemy according to its current health value */ 
void SmallEnemy::UpdateEnemy()
{
	switch (mHealth)
	{
			// Turn Orange for the 1st attack
	case 2: mRed = 1; mGreen = 0.5f; mBlue = 0.0f; break;
			// Turn Red for the second attack
	case 1: mRed = 1; mGreen = 0.0f; mBlue = 0.0f; 
			// Add a smoke plume effect
			ParticleManager::GetInstance()->AddSmoke(mPosition, 300, this);
			break;
	case 0: // Remove this object
			ParticleManager::GetInstance()->AddExplosion(mPosition);
			ParticleManager::GetInstance()->RemoveSmoke(this);
			mWorld->RemoveGameObject(this);
			break;
	}
}

/* Shoot A bullet */ 
void SmallEnemy::ShootBullet(Vec3 direction)
{
	// If the shot did not complete recently
	if(!m_hasShot && m_fEnergy > 0){
		// Preset an acceleration
		float bullet_acceleration = 15;
		// Determine velocity
		Vec3 velocity =  direction * bullet_acceleration;
		// decrease the y position to position the bullet properly
		Vec3 shootPosition = mPosition;
		shootPosition.y -= 0.15f;
		// Create new Shoot Object with enemy position and the velocity worked out
		ShootManager::GetInstance()->AddShot("enemybullet", shootPosition, velocity, 200, Vec3(0.8, 0.0, 0.0), m_worldMatrix, 0.5f);
		// enemy has shot recently
		m_hasShot = true;
		m_tillNextShot = m_fTimeElapsed;
		// Energy has been used
		m_bEnergyUsed = true;
		m_fTillEnergyRecharge = m_fTimeElapsed;

		m_fEnergy -= 10;
		Clamp(m_fEnergy, 0, 100);
	}
}

void SmallEnemy::ShoutForHelp()
{
	if(!m_bCalledHelp){
		MessageDispatcher::GetInstance()->DispatchMessageToGroup(0, this, GameObjectType("smallenemy"), 
			msg_enemyShoutHelp, mPosition);

		m_bCalledHelp = true;
		m_tillNextCall = m_fTimeElapsed;
	}
}

// Perform a collision test with all the required objects
bool SmallEnemy::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("spaceship") || o->GetType() == GameObjectType("bullet") ||
		o->GetType() == GameObjectType("poweruphealth") || o->GetType() == GameObjectType("powerupenergy") ||
		o->GetType() == GameObjectType("asteroid")) 
		return mBoundingSphere->CollisionTest(o->GetBoundingShape());
	return false;	
}

void SmallEnemy::OnCollision(Entity* o)
{
	// If collision occurs with a bullet then reduce health
	if(o->GetType() == GameObjectType("bullet") && !m_hasBeenAttacked) {
		mHealth -= 1;
		UpdateEnemy();
		// Set the bullet reading delay for collision test
		m_hasBeenAttacked = true;
		m_tillNextUpdate = m_fTimeElapsed;
	}

	// If collision occurs with a power up health then increase the health
	if(o->GetType() == GameObjectType("poweruphealth")){
		// If health was low then switch off the current smoke
		if(mHealth == 1)
			ParticleManager::GetInstance()->RemoveSmoke(this);
		mHealth += 1;
		Clamp(mHealth, 0, MAX_HEALTH);
		UpdateEnemy();
	}

	// If collision occurs with a power up energy then increase the energy
	if(o->GetType() == GameObjectType("powerupenergy")){
		m_fEnergy += 50;
		Clamp(m_fEnergy, 0, MAX_ENERGY);
		UpdateEnemy();
	}

	// If collision occurs with a asteroid then allow elastic collision to occur
	if(o->GetType() == GameObjectType("asteroid") && !m_bIgnoreCollision) {
		m_fCollidedMass = o->GetMass();
		m_vCollidedVelocity = mVelocity;
		// Send a message to the asteroid that this object has collided with it
		MessageDispatcher::GetInstance()->DispatchMessageA(5, GetID(), o->GetID(), msg_elasticCollision, m_vCollidedVelocity);
		// Create a delay in collision testing again
		m_bIgnoreCollision = true;
		m_fTimeToIgnoreCollision = m_fTimeElapsed;
	}
}

//-----------------------------------ITimerListener---------------------------------------
// Update all the time values
void SmallEnemy::OnTimer(float milliseconds)
{
	m_fTimeElapsed = milliseconds;

	if(m_hasShot) 
		if(milliseconds - m_tillNextShot >=2000)
			m_hasShot = false;

	if(m_hasBeenAttacked)
		if(milliseconds - m_tillNextUpdate >= 50)
			m_hasBeenAttacked = false;

	if(m_bEnergyUsed)
		if(milliseconds - m_fTillEnergyRecharge >= 3000)
			m_bEnergyUsed = false;

	if(m_bCalledHelp)
		if(milliseconds - m_tillNextCall > 2000)
			m_bCalledHelp = false;
	
	if(m_bIgnoreCollision)
		if(m_fTimeElapsed - m_fTimeToIgnoreCollision > 50)
			m_bIgnoreCollision = false;

	if(m_bIgnoreShot)
		if(m_fTimeElapsed - m_fTimeIgnoreShoot > 100)
			m_bIgnoreShot = false;
}
//-----------------------------------IGameWorldListener----------------------------------

void SmallEnemy::OnObjectAdded(GameWorld* world, Entity* object)
{
	// If an object added is an power up health then add it to the potential targets list
	if(object->GetType() == GameObjectType("poweruphealth") || object->GetType() == GameObjectType("powerupenergy"))
		m_potentialTargets.push_back(object);
}

void SmallEnemy::OnObjectRemoved(GameWorld* world, Entity* object)
{
	if(object->GetType() == GameObjectType("poweruphealth") || object->GetType() == GameObjectType("powerupenergy")){
		// Remove the object from the potential target list
		// First find the object in the list
		PotentialTargetList::iterator it = find(m_potentialTargets.begin(), m_potentialTargets.end(), object);
		// if it does exist then remove it
		if(it != m_potentialTargets.end())
			m_potentialTargets.erase(remove(m_potentialTargets.begin(), m_potentialTargets.end(), object));
	}
}
//----------------------------------------------------------------------------------------