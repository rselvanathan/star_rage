/*
This class represent an asteroid in the game world. This object will travel in a certain direction
and when it reaches the end, it will reset itself on the other side again, creating an illusion of 
as constant stream of asteroid

Author : Romesh Selvanathan

Date : 03/05/12
*/
#include "Asteroid.h"
#include "MovingEntity.h"
#include "GameUtil.h"
#include "BoundingSphere.h"
#include "GameWorld.h"
#include "Model_3DS.h"
#include "CRandom.h"
#include "MessageDispatcher.h"
#include "MessageType.h"
#include "ClassIDList.h"


Asteroid::Asteroid(int side) : MovingEntity("asteroid")
{
	mRadius = 3.2f;
	mBoundingSphere = new BoundingSphere(this, mRadius);

	m_iSide = side;
	
	// initialize the custom random class
	r.Randomize();

	// Obtain random x y and z values
	float x;
	if(side == LEFT)
		x = (10 + static_cast<float>(r.Random(30))) * -1;
	else
		x = 10 + static_cast<float>(r.Random(30));
	float y = 1 + static_cast<float>(r.Random(5));
	float z = 1 + static_cast<float>(r.Random(5));
	
	// Randomly choose if y is positive of negative
	int yRandom = r.Random(2);
	if(yRandom == 1)
		y *= -1;
	// Randomly choose if z is positive of negative
	int zRandom = r.Random(2);
	if(zRandom == 1)
		z *= -1;

	// Set the force
	Vec3 temp = Vec3(x, y, z);
	m_vForce = temp;

	// Set the random max speed
	float randMaxSpeed = 1 + static_cast<float>(r.Random(21));
	m_maxSpeed = randMaxSpeed;

	// The 3D model
	SmartPtr<Model_3DS> model = new Model_3DS();
	model->Load("3DModels//asteroid.3DS");
	SetModel(model);

	m_bCheckBounce = false;
	m_iLastCollisionTimeStep = 0;

	// Allow it to ignore the breaking of velocity so that when collisions occur, the
	// velocity set will continue even if no force is applied
	m_bIgnoreBrake = true;

	// Random mass for object
	m_mass = static_cast<float>(r.Random(501));
	Clamp(m_mass, 200.0f, 500.0f);

	// random elactisity value for asteroid
	m_fElactisity = static_cast<float>(r.Random(101));
	Clamp(m_fElactisity, 20.0f, 100.0f);
	m_fElactisity /= 100.0f;
}

Asteroid::~Asteroid()
{MovingEntity::~MovingEntity();}

void Asteroid::Reset()
{
	m_bCheckBounce = false;
	m_iLastCollisionTimeStep = 0;

	// Obtain random x, z values which have to be a minimum of the wander distance
	float x;
	if(m_iSide == LEFT)
		x = (10 + static_cast<float>(r.Random(30))) * -1;
	else
		x = 10 + static_cast<float>(r.Random(30));
	float y = 1 + static_cast<float>(r.Random(5));
	float z = 1 + static_cast<float>(r.Random(5));

	int yRandom = r.Random(2);
	if(yRandom == 1)
		y *= -1;
	// Randomly choose if z is positive of negative
	int zRandom = r.Random(2);
	if(zRandom == 1)
		z *= -1;

	Vec3 temp = Vec3(x, y, z);
	m_vForce = temp;

	float randMaxSpeed = 1 + static_cast<float>(r.Random(21));
	m_maxSpeed = randMaxSpeed;
}

void Asteroid::Update(float dt)
{
	UpdatePosition(m_vForce, dt);

	yaw +=  5 * dt; if(yaw > 360) yaw = 0;
	pitch += 5 *dt; if(pitch > 360) pitch = 0;
	roll += 5 *dt; if(roll > 360) roll = 0;

	// If the asteroid reaches one of the other ends then reset it again
	if(mPosition.x > 150 || mPosition.x < -150){
		mPosition *= -1;
		if(mPosition.x < -150)
			mPosition.x += 5;
		else
			mPosition.x -= 5;

		// Obtain random x, z values which have to be a minimum of the wander distance
		float x;
		if(m_iSide == LEFT)
			x = (10 + static_cast<float>(r.Random(30))) * -1;
		else
			x = 10 + static_cast<float>(r.Random(30));
		float y = 1 + static_cast<float>(r.Random(5));
		float z = 1 + static_cast<float>(r.Random(5));

		int yRandom = r.Random(2);
		if(yRandom == 1)
			y *= -1;
		// Randomly choose if z is positive of negative
		int zRandom = r.Random(2);
		if(zRandom == 1)
			z *= -1;

		Vec3 temp = Vec3(x, y, z);
		m_vForce = temp;

		float randMaxSpeed = 1 + static_cast<float>(r.Random(21));
		m_maxSpeed = randMaxSpeed;
	}
}

// Test the Collisions with objects
bool Asteroid::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("spaceship") || o->GetType() == GameObjectType("asteroid") ||
		o->GetType() == GameObjectType("smallenemy") || o->GetType() == GameObjectType("hardenemy")) 
		return mBoundingSphere->CollisionTest(o->GetBoundingShape());	
	if(o->GetType() == GameObjectType("bigEnemy"))
		return mBoundingSphere->CollisionWithBox(o->GetBoundingBox());
	return false;
}

void Asteroid::OnCollision(Entity* o)
{
	if(o->GetType() == GameObjectType("spaceship")) {
		MessageDispatcher::GetInstance()->DispatchMessageA(0, GetID(),
			ClassIDList::GetInstance()->class_PlayerManager, msg_playerCollidesAsteroid, NULL);
	}
	
	// If The Collidion should not be ignored then
	if(m_bCheckBounce) {
		// Allow elastic collision to occur
		m_fCollidedMass = o->GetMass();
		m_vCollidedVelocity = mVelocity;
		// Send a message to the other object about the elastic collision
		MessageDispatcher::GetInstance()->DispatchMessageA(2, GetID(), o->GetID(), msg_elasticCollision, m_vCollidedVelocity);
		// Create a delay in collision 
		m_bCheckBounce = false;
		m_iLastCollisionTimeStep = m_fTimeElapsed;
	}
}

bool Asteroid::HandleMessage(const Telegram& msg)
{
	switch (msg.m_message)
	{
	case msg_elasticCollision:
		// Obtain the other object's velocity
		Vec3 object_initVel = *(Vec3*)msg.m_extraInfo;
		// Do a elastic collision
		ElasticCollision(object_initVel);
		break;
	}

	return true;
}

//-------------------------------ITimerListener-------------------------------------
void Asteroid::OnTimer(float milliseconds)
{
	m_fTimeElapsed = milliseconds;

	if(!m_bCheckBounce)
		if(m_fTimeElapsed - m_iLastCollisionTimeStep > 5000)
			m_bCheckBounce = true;
}
//----------------------------------------------------------------------------------