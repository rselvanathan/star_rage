/*
This class represent the laser beam shot that has been done. THe laser beam is created simplt by placing
6 quads in a star shaped form. You simply add a texture to each quad and have allow additive blending,
which will blend of the the quads together to create the laser beam effect. The laser beam will also require 
the world matrix from whatever object that is firing it, resulting in the laser beam simply using the
same orientation as the current forward vector of the initiator to fly in the same direction forward.

Author : Romesh Selvanathan
Date : 03/05/12
*/
#include "Shoot.h"
#include "MovingEntity.h"
#include "GameUtil.h"
#include "GameWorld.h"
#include "BoundingSphere.h"
#include "Graphics.h"
#include "GLLight.h"
#include "Texture.h"
#include "ExplosionParticle.h"
#include "ParticleManager.h"
#include "PreLoader.h"

// Constructor
Shoot::Shoot(const char* mType , Vec3 position, Vec3 velocity, int TimeToLive, Vec3 Color) : MovingEntity(mType, position, velocity,
	Vec3(0, 0, 0), 1), mTimeToLive(TimeToLive)
{
	// Initialise glu shape  - LEGACY
	quadratic = gluNewQuadric();
	gluQuadricNormals(quadratic, GL_SMOOTH);
	gluQuadricDrawStyle(quadratic, GLU_FILL);

	mRadius = 0.3;
	mBoundingSphere = new BoundingSphere(this, mRadius);

	mRed = Color.x;
	mGreen = Color.y;
	mBlue = Color.z;

	mQuad.Init(0.5, 0, 1);

	mQuad2.Init(0, 1, 0.5);
	mQuad2.SetRotation(0, 90, 0);

	mQuad4.Init(0, 1, 0.5);
	mQuad4.SetRotation(0, 90, 10);

	mQuad5.Init(0, 1, 0.5);
	mQuad5.SetRotation(0, 90, -10);

	// Set the colors and Attenuation of the light
	mLight.SetAmbientColor(0.0, 0.0, 0.0, 0.0);
	mLight.SetDiffuseColor(mRed, mGreen, mBlue, 1.0);
	mLight.SetSpecularColor(mRed, mGreen, mBlue, 1.0);
	mLight.SetShininess(128);
	mLight.SetLightAttenuation(1.0, 0.2f, 0.1);

	m_playerWorldMatrix.Identity();
}  

// Over load this function as both the enemies and player share this class and only differ in texture. So when the type is different change the texture
// rather than constantly polling for the type in the update function
void Shoot::SetGameObjectType(char const* const type)
{
	MovingEntity::SetGameObjectType(type);
	if(type == "enemybullet")
		texture = PreLoader::GetInstance()->laser_enemy->GetTGATexture().texID;
	else 
		texture = PreLoader::GetInstance()->laser->GetTGATexture().texID;

	mQuad.SetTexture(texture);
	mQuad2.SetTexture(texture);
	mQuad4.SetTexture(texture);
	mQuad5.SetTexture(texture);
}

// Update the bullet using Simple physics - without force being taken into account
void Shoot::Update(float dt)
{
	mVelocity = mVelocity + (mAcceleration*dt);
	mPosition = mPosition + (mVelocity *dt);

	mTimeToLive -= 5 *dt;
	if(mTimeToLive < 0 ) {mTimeToLive = 0;}
	if(mTimeToLive <= 0){
		if(mWorld) mWorld->RemoveBlendedGameObject(this); 
		// Switch off the light when the object is removed from the world
		mLight.SwitchOffLight(GL_LIGHT2);
	}
}

void Shoot::Render()
{
	// PowerUp and Switch on the light
	mLight.PowerUpLight();
	mLight.SwitchOnLight(GL_LIGHT2);
	// Allow light to follow the object
	mLight.SetPosition(mPosition.x, mPosition.y, mPosition.z, 1);
	mLight.Render();

	MovingEntity::Render();

	// Enable Additive Blending
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	Graphics::GetInstance()->DisableDepthMask();
	Graphics::GetInstance()->DisableBackFaceCull();
	Graphics::GetInstance()->StoreMatrix();
		// IF the  world matrix is set to default
		if(GetType() == GameObjectType("bullet") || GetType() == GameObjectType("enemybullet")){
			// Change the position of the matrix to 0, as I do not want these coordinates to mess with
			// the position given by the constructor itself
			// NOTE: IF not 0 it will caus the shoot to behave lyk a third person camera and rotate around
			// the spaceship as it rotates.
			m_playerWorldMatrix[3][0] = 0.0f;
			m_playerWorldMatrix[3][1] = 0.0f;
			m_playerWorldMatrix[3][2] = 0.0f;
			// Multiply the matrix with the current matrix in the stack to apply the rotations
			Graphics::GetInstance()->MultiplyToMatrix(m_playerWorldMatrix);
		}
		// Render all the 4 quads
		mQuad.Render();
		mQuad2.Render();
		mQuad4.Render();
		mQuad5.Render();
		// Switch off the Additive blending
	Graphics::GetInstance()->ReleaseMatrix();
	Graphics::GetInstance()->AllowBackFaceCull();
	Graphics::GetInstance()->AllowDepthMask();
	Graphics::GetInstance()->DisableBlend();
}

// Collision tests with objects
bool Shoot::CollisionTest(Entity* o)
{
	if(o->GetType() == GameObjectType("spaceship") && this->GetType() == GameObjectType("bullet") || 
		o->GetType() == GameObjectType("enemy") && this->GetType() == GameObjectType("enemybullet") || 
		o->GetType() == GameObjectType("bigEnemy") && this->GetType() == GameObjectType("enemybullet"))
		return false;
	if(o->GetType() == GameObjectType("asteroid")) 
		return mBoundingSphere->CollisionTest(o->GetBoundingShape());
	return false;
}

void Shoot::OnCollision(Entity* o)
{
	// Add a spark particle effect whenever collision occurs
	ParticleManager::GetInstance()->AddSpark(mPosition);

	// Swithc off the light and remove this object
	mLight.SwitchOffLight(GL_LIGHT2);
	mWorld->RemoveBlendedGameObject(this);
}
