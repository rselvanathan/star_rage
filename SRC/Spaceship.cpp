/*
The Player spaceship

Author : Romesh Selvanathan

Date : 3/05/12
*/

#include "Spaceship.h"
#include "MovingEntity.h"
#include "GameUtil.h"
#include "BoundingSphere.h"
#include "Shoot.h"
#include "GameWorld.h"
#include "Model_3DS.h"
#include "Camera.h"
#include "StarField.h"
#include "ParticleManager.h"
#include "ShootManager.h"
#include "ClassIDList.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "Camera.h"
#include "EngineEnergyBall.h"

/* Constructor */ 
Spaceship::Spaceship() : MovingEntity("spaceship", ClassIDList::GetInstance()->ent_player)
{			
    m_eulerOrient = Vec3(0.0f, 180.f, 0.0f);
	eulerOrientElapsed = Vec3(0.0f, 180.f, 0.0f);

	mRadius = 0.7;
	mBoundingSphere = new BoundingSphere(this, mRadius);

	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//Spaceship//player_spaceship.3ds");
	SetModel(model);
	yaw = 90;
	mScale = 0.16f;

	m_timeTillNextShot = 0;
	m_timeTillEnemyShoot = 0;
	hasShot = false;
	enemyShot = false;
	energyDepleted = false;
	m_sprint = false;

	m_pCamera = new Camera();
	m_pCamera->Reset();

	m_pEngineEnergy = new EngineEnergyBall("spaceshipengine", 0.15, 0.15, Vec3(0, -0.15, -0.75));
	GameWorld::GetInstance()->AddBlendedGameObject(m_pEngineEnergy);
}

// Reset all the values
void Spaceship::Reset()
{
	mVelocity = Vec3(0, 0,0);

	m_worldMatrix.Identity();
	m_orientation.Identity();
	m_rotation.Identity();
    
    mRight = Vec3(1.0f, 0.0f, 0.0f);
    mUp = Vec3(0.0f, 1.0f, 0.0f);
    mForward = Vec3(0.0f, 0.0f, -1.0f);
    
    m_eulerOrient = Vec3(0.0f, 180.f, 0.0f);
	eulerOrientElapsed = Vec3(0.0f, 180.f, 0.0f);

	yaw = 90;
	mScale = 0.16f;

	m_timeTillNextShot = 0;
	m_timeTillEnemyShoot = 0;
	hasShot = false;
	enemyShot = false;
	energyDepleted = false;
	m_sprint = false;

	// Reset the camera and set its look at values.
	m_pCamera->Reset();
	m_pCamera->SetLookAt(Vec3(0.0f, 0.5f, 1.f),
		Vec3(0.0f, 0.0f, -1.0f), Vec3(0.0f, 1.0f, 0.0f));

	m_mass = 1;
	m_maxForce = 60;
	m_maxSpeed = 10;

	m_fdeltaTime = 0;
}

/* Destructor */ 
Spaceship::~Spaceship()
{
	MovingEntity::~MovingEntity();
}

/* Update the Spaceship */ 
void Spaceship::Update(float dt)
{
	// Obtain the Orientation axis 
	m_orientation.ExtractOrientationAxis(mRight, mUp, mForward);

	// Update the Orientation of the Entity(Not Local orientation, but the objects world orientation)
	UpdateOrientation(dt);
	// Update the position of the Entity using basic physics from F=ma -> v = u + at -> S = S + vt
	UpdatePosition(m_vForce, dt);
	// Reset the Force every frame - without this due to the control methods the object will constantly move
	// without ever stopping so have to clear the values every fram
	m_vForce = Vec3(0, 0, 0);

	// Update the Engine Energy Ball
	m_pEngineEnergy->SetWorldMatrix(m_worldMatrix);
	m_pEngineEnergy->SetLife(mVelocity);

	// Update the camera
	m_pCamera->SetTargetMat(m_worldMatrix);
	m_pCamera->SetLookAt(mPosition);
	m_pCamera->Update(dt);
	m_pCamera->SetSprint(m_sprint);
}

void Spaceship::Render()
{
	Graphics::GetInstance()->MultiplyToMatrix(m_worldMatrix);
	// This makes the spaceship appear the rigth size, needs to be improved
	Graphics::GetInstance()->Scale(mScale, mScale, mScale);
	// Needs to be here as spaceship does its own positional updates meaning the render function
	// of gameobject cannot be used here. Have to specify my own version of rendering the 3D model 
	// for the spaceship itself due to the way it functions compared to normal game objects
	mModel->rot.x = pitch;
	mModel->rot.y = yaw;
	mModel->rot.z = roll;
	mModel->color.x = mRed;
	mModel->color.y = mGreen;
	mModel->color.z = mBlue;
	mModel->scale = mScale;
	mModel->Draw();	
}

/////////////////////////////////////////IPlayerListener//////////////////////////////////////
// LAST EDIT - 25/01/12
void Spaceship::OnEnergyChanged(float energy)
{
	if(energy == 0)
		energyDepleted = true;
	else 
		energyDepleted = false;
}

/////////////////////////////////////////Testing////////////////////////////////////

//-------------------------------------Player Control Methods-------------------------------------
void Spaceship::MoveForward(Vec3 force)
{
	m_vForce = force;
}

void Spaceship::MoveBackwards(Vec3 force)
{
	m_vForce = force;
}
//------------------------------------------------------------------------------------------------

/* EDITED AND ADDED 13/01/2012 - Does the rotations now instead of orient */
void Spaceship::SetPitch(float pitchDegrees)
{
	m_eulerOrient.x += pitchDegrees; 

    if (m_eulerOrient.x > 360.0f)
        m_eulerOrient.x -= 360.0f;

    if (m_eulerOrient.x < -360.0f)
        m_eulerOrient.x += 360.0f;

	eulerOrientElapsed.x = m_eulerOrient.x;

	m_pCamera->SetRotateX(pitchDegrees);
}

void Spaceship::SetHeading(float headingDegrees)
{

	m_eulerOrient.y += headingDegrees; 

	if (m_eulerOrient.y > 360.0f)
        m_eulerOrient.y -= 360.0f;

    if (m_eulerOrient.y < -360.0f)
        m_eulerOrient.y += 360.0f;

	eulerOrientElapsed.y = m_eulerOrient.y;

	m_pCamera->SetRotateY(headingDegrees);
}

void Spaceship::SetRoll(float rollDegrees)
{

	m_eulerOrient.z += rollDegrees; 

	if (m_eulerOrient.z > 360.0f)
        m_eulerOrient.z -= 360.0f;

    if (m_eulerOrient.z < -360.0f)
        m_eulerOrient.z += 360.0f;

	eulerOrientElapsed.z = m_eulerOrient.z;

	m_pCamera->SetRotateZ(rollDegrees);
}

//LEGACY - was used before the different sets angles above
void Spaceship::orient(float headingDegrees, float pitchDegrees, float rollDegrees)
{
    // orient() changes the direction the entity is facing. This directly
    // affects the orientation of the entity's right, up, and forward vectors.
    // orient() is usually called in response to the user's input if the entity
    // is able to be moved by the user.

    m_eulerOrient.x += pitchDegrees;
    m_eulerOrient.y += headingDegrees;
    m_eulerOrient.z += rollDegrees;

	m_pCamera->SetRotate(headingDegrees, pitchDegrees, rollDegrees);

    if (m_eulerOrient.x > 360.0f)
        m_eulerOrient.x -= 360.0f;

    if (m_eulerOrient.x < -360.0f)
        m_eulerOrient.x += 360.0f;

    if (m_eulerOrient.y > 360.0f)
        m_eulerOrient.y -= 360.0f;

    if (m_eulerOrient.y < -360.0f)
        m_eulerOrient.y += 360.0f;

    if (m_eulerOrient.z > 360.0f){
        m_eulerOrient.z -= 360.0f;

	}

    if (m_eulerOrient.z < -360.0f)
        m_eulerOrient.z += 360.0f;
}

// LAST EDIT - 25/01/12 - When energy is depleted cant shoot anymore
void Spaceship::ShootBullet()
{
	// Only shoot if time limit has been met
	if(!hasShot && !energyDepleted) {
		// Get the current direction the bullet should fly towards
		Vec3 direction = mForward * -1;
		//Vec3 direction = m_right;
		// Obtain the normalized vector
		direction.Normalize();
		// Preset an acceleration
		float bullet_acceleration = 30.f;
		// Determine velocity
		Vec3 velocity = direction * bullet_acceleration;
		// Create new Shoot Object with camera poisition and the velocity worked out
		Entity* temp;
		ShootManager::GetInstance()->AddShot(temp, "bullet", mPosition, velocity, 200, Vec3(1, 1, 1), m_worldMatrix, 1);

		// Send a message to the PlayerManager
		MessageDispatcher::GetInstance()->DispatchMessageA(0, GetID(), ClassIDList::GetInstance()->class_PlayerManager,
			msg_playerNormalShot, temp);

		// Send a message to the Hard enemy
		MessageDispatcher::GetInstance()->DispatchMessageA(0, GetID(), ClassIDList::GetInstance()->ent_hardEnemy,
			msg_playerNormalShot, temp);

		hasShot = true;
		m_timeTillNextShot = m_fTimeElapsed;
	}
}

// Collision Tests
bool Spaceship::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("enemybullet") ||  o->GetType() == GameObjectType("poweruphealth")
		|| o->GetType() == GameObjectType("powerupenergy")) 
		return mBoundingSphere->CollisionTest(o->GetBoundingShape());
	return false;
}

void Spaceship::OnCollision(Entity* o)
{
	// if collision occurs with the enemy bullet then inform the listeners
	if(o->GetType() == GameObjectType("enemybullet") && !enemyShot) {
		MessageDispatcher::GetInstance()->DispatchMessageA(0, ClassIDList::GetInstance()->ent_player,
			ClassIDList::GetInstance()->class_PlayerManager, msg_playerHasBeenAttacked, NULL);
		enemyShot = true;
		m_timeTillEnemyShoot = m_fTimeElapsed;
	}
	// If Collision with power up health occurs
	if(o->GetType() == GameObjectType("poweruphealth") && !m_bIgnoreCollision) {
		// Send a Message to the PlayerManager class 
		MessageDispatcher::GetInstance()->DispatchMessageA(0, ClassIDList::GetInstance()->ent_player,
			ClassIDList::GetInstance()->class_PlayerManager, msg_playerPickedHealth, NULL);
		m_bIgnoreCollision = true;
		m_fTimeToIgnoreCollision = m_fTimeElapsed;
	}
	// If Collision with the power up energy occurs
	if(o->GetType() == GameObjectType("powerupenergy") && !m_bIgnoreCollision) {
		// Send a Message to the PlayerManager class 
		MessageDispatcher::GetInstance()->DispatchMessageA(0, ClassIDList::GetInstance()->ent_player,
			ClassIDList::GetInstance()->class_PlayerManager, msg_playerPickedEnergy, NULL);
		m_bIgnoreCollision = true;
		m_fTimeToIgnoreCollision = m_fTimeElapsed;
	}
}

bool Spaceship::HandleMessage(const Telegram& msg)
{
	switch(msg.m_message){
	case msg_playerSprint_On :
		{
			// IF the sprint is switched on then increase the speed and send a message
			m_sprint = true;
			m_maxSpeed = 20;
			m_vForce = Vec3(0, 0, 6);
			// Send a message to the player manager
			MessageDispatcher::GetInstance()->DispatchMessageA(0, ClassIDList::GetInstance()->ent_player,
			ClassIDList::GetInstance()->class_PlayerManager, msg_playerSprint_On, NULL);
			return true;
		}
		break;
	case msg_playerSprint_Off :
		{
			// if sprint is switched off then retunr back to normal
			m_sprint = false;
			m_maxSpeed = 10;
			return true;
		}
		break;
	}
}

//--------------------------------ITimerListener----------------------------
void Spaceship::OnTimer(float milliseconds)
{
	m_fTimeElapsed = milliseconds;

	// A Time Limit till next shot can be fired
	if (hasShot)
		if(m_fTimeElapsed - m_timeTillNextShot > 500)
			hasShot = false;

	if(enemyShot)
		if(m_fTimeElapsed - m_timeTillEnemyShoot > 50)
			enemyShot = false;

	if(m_bIgnoreCollision)
		if(m_fTimeElapsed - m_fTimeToIgnoreCollision > 5)
			m_bIgnoreCollision = false;
}
//--------------------------------------------------------------------------

