/*
This is the big enemy which, does not move currently. This enemy will create two turret, one on top
and another at the bottom, and will be stationary. When this enemy dies so will the turrets.

Author : Romesh Selvanathan

Date : 3/05/2012
*/

#include "BigEnemy.h"
#include "GameUtil.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "Enemy.h"
#include "Model_3DS.h"
#include "BoundingSphere.h"
#include "BoundingBox.h"
#include "GameWorld.h"
#include "Shoot.h"
#include "TurretBase.h"
#include "TurretHead.h"
#include "ParticleManager.h"

// Constructor
BigEnemy::BigEnemy(MovingEntity* target) : Enemy("bigEnemy"),
				 m_hasBeenAttacked(false),
				 m_tillNextUpdate(0)
{
	// Set all the variabls
	m_worldMatrix.Identity();
	m_orientation.Identity();
    
    mRight = Vec3(1.0f, 0.0f, 0.0f);
    mUp = Vec3(0.0f, 1.0f, 0.0f);
    mForward = Vec3(0.0f, 0.0f, -1.0f);
	 m_eulerOrient = Vec3(0.0f, 0.0f, 0.0f);

	mHealth = HEALTH_POINTS;
	mRadius = 5;

	mBoundingBox = new BoundingBox(this, Vec3(-11, -2, -3), Vec3(11, 2, 3)); 

	// Load the 3D model
	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//Enemy//FinalCapital//CapitalShip.3DS");
	SetModel(model);

	// Set the scale of this object
	mScale = 2.f;

	// Set up the two turrets
	mTurretBase = new TurretBase(TurretBase::TOP);
	mTurretHead = new TurretHead(-50, 10, TurretHead::TOP, target);

	mTurretBaseBottom = new TurretBase(TurretBase::BOTTOM);
	mTurretHeadBottom = new TurretHead(-40, 10, TurretHead::BOTTOM, target);
}

// Destructor
BigEnemy::~BigEnemy()
{Enemy::~Enemy();}

// Reset the enemy
void BigEnemy::Reset()
{
	m_hasBeenAttacked=false;
	m_tillNextUpdate=UPDATE_HEALTH_DELAY;
	mHealth=HEALTH_POINTS;

	// Reset all other common values
	Enemy::Reset();
}

void BigEnemy::Update(float dt)
{
	// Update the position of the Entity using basic physics from F=ma -> v = u + at -> S = S + vt
	UpdatePosition(Vec3(0, 0, 0), dt);
	// Update the Bounding Box
	mBoundingBox->UpdateBoundingBox();
}

/* Update the enemy according to its current health value */ 
void BigEnemy::UpdateEnemy()
{
	switch (mHealth)
	{
			// Turn Orange for the 1st attack
	case 2: mRed = 1; mGreen = 0.5f; mBlue = 0.0f; break;
			// Turn Red for the second attack
	case 1: mRed = 1; mGreen = 0.0f; mBlue = 0.0f; 
			// Add a Smoke plume effect
			ParticleManager::GetInstance()->AddSmoke(mPosition, 300, this);
			break;
	case 0: // Remove this object
			ParticleManager::GetInstance()->AddExplosion(mPosition);
			ParticleManager::GetInstance()->RemoveSmoke(this);
			mWorld->RemoveGameObject(this);
			break;
	}
}

bool BigEnemy::HandleMessage(const Telegram& msg)
{
	return true;
}

// Collision Test 
bool BigEnemy::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("bullet"))
		return mBoundingBox->CollideWithSphere(o->GetPosition(), o->GetRadius());
	return false;	
}

void BigEnemy::OnCollision(Entity* o)
{
	// IF collision occurs with a bullet and it has not been attacked recently (a delay for the collision 
	// detection) then decrease the health and update the enemy. Also set the collision delay 
	if(o->GetType() == GameObjectType("bullet") && !m_hasBeenAttacked) {
		mHealth -= 1;
		UpdateEnemy();
		m_hasBeenAttacked = true;
		m_tillNextUpdate = m_fTimeElapsed;
	}
}

///////////////////////////////////////////////////////////IGAMEWORLDLISTENER////////////////////////////////////////
void BigEnemy::OnObjectAdded(GameWorld* world, Entity* object)
{
	if(object == this){
		// When the object added is this object specificlly then Add the Turret into the game world
		// Im doing it this way instead of creating it in the game play state or as a seperate entity as 
		// the big enemy own this turret. So it should "contain" the turret in the class. Because of the way my 
		// game is designed I am adding the turrets as soon as the game world has added this enemy into its list.
		// This is for acces and optimisation reasons so that this is only perfromed once when the enemy is added to the game world.
		mTurretBase->SetPosition(Vec3(mPosition.x -2.0f, mPosition.y + 1.5f, mPosition.z));
		mTurretHead->Reset();
		mTurretHead->SetPosition(Vec3(mPosition.x - 1.9f , mPosition.y + 1.7f, mPosition.z));
		mTurretBaseBottom->SetPosition(Vec3(mPosition.x -2.0f, mPosition.y - 1.3f, mPosition.z));
		mTurretHeadBottom->Reset();
		mTurretHeadBottom->SetPosition(Vec3(mPosition.x - 2.1f , mPosition.y - 1.5f, mPosition.z));
		world->AddGameObject(mTurretBase);
		world->AddGameObject(mTurretHead);
		world->AddGameObject(mTurretBaseBottom);
		world->AddGameObject(mTurretHeadBottom);
	}
}

void BigEnemy::OnObjectRemoved(GameWorld* world, Entity* object)
{
	if(object == this){
		// WHen this enemy is removed from the list then remove the turrets aswel and remove
		// tthis object from being a listener of the game world (it will be readded in the 
		// enemy generator class when it is ready to be added to the world again)
		world->RemoveGameObject(mTurretBase);
		world->RemoveGameObject(mTurretHead);
		world->RemoveGameObject(mTurretBaseBottom);
		world->RemoveGameObject(mTurretHeadBottom);
	}
}

//------------------------------ITimerListener--------------------------------

// Update all the time values here 
void BigEnemy::OnTimer(float milliseconds)
{
	// Obtain the current time
	m_fTimeElapsed = milliseconds;

	if(m_hasBeenAttacked)
		if(milliseconds - m_tillNextUpdate >= 1)
			m_hasBeenAttacked = false;

	mTurretHead->OnTimer(milliseconds);
	mTurretHeadBottom->OnTimer(milliseconds);
}

//------------------------------------------------------------------------------