/*
There are two types of powerups the player can obtain currently. These are the energy powerup and health powerup
and as their names suggest their increase either the energy or health values back up again

Author : Romesh Selvanathan
*/ 

#include "PowerUp.h"
#include "Entity.h"
#include "GameUtil.h"
#include "BoundingSphere.h"
#include "PreLoader.h"
#include "GLTexture.h"
#include "GameWorld.h"
#include "ParticleEngine.h"
#include "Texture.h"

/* Constructor */ 
PowerUp::PowerUp(const char* name) : Entity(name)//"Square")
{
	// Set the Bounding Sphere up
	mRadius = 0.3;
	mBoundingSphere = new BoundingSphere(this, mRadius);

	// Set the 3D opengl box up
	box.Init(0.2, 0.2, 0.2);

	// depending on the name given to the powerup change the texture of it
	if(name == "poweruphealth")
		box.SetTexture(PreLoader::GetInstance()->powerup_health->GetTGATexture().texID);
	else if (name == "powerupenergy")
		box.SetTexture(PreLoader::GetInstance()->powerup_energy->GetTGATexture().texID);
}

/* Destructor */ 
PowerUp::~PowerUp()
{Entity::~Entity();}

/* Update the object */
void PowerUp::Update(float dt)
{
	// Rotate the object
	// Each yaw pitch and roll have different rotation values
	yaw += 100. * dt;
	pitch += 51.f * dt;
	roll += 25.f * dt;
	// reset them when they reach 360
	if(yaw > 360)
		yaw = 0;
	if(pitch > 360)
		pitch = 0;
	if(roll > 360)
		roll = 0;

	// Update the box position color and rotation
	box.SetRotation(yaw, pitch, roll);
	box.SetColor(1,1, 1, 0);
	box.SetPosition(mPosition.x, mPosition.y, mPosition.z);
}

/* Render The object */
void PowerUp::Render()
{
	// render the box
	box.Render();
}

/* Test Collisions to see if it has collided */
bool PowerUp::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("spaceship") || o->GetType() == GameObjectType("smallenemy")) 
		return mBoundingSphere->CollisionTest(o->GetBoundingShape());
	return false;
}

/* If Collision does occur then */
void PowerUp::OnCollision(Entity* o)
{
	// remove the object from the world
	mWorld->RemoveGameObject(this);
}