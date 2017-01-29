/*
This class represent the laser beam shot that has been done. THe laser beam is created simplt by placing
4 quads in a star shaped form. You simply add a texture to each quad and have allow additive blending,
which will blend of the the quads together to create the laser beam effect. The laser beam will also require 
the world matrix from whatever object that is firing it, resulting in the laser beam simply using the
same orientation as the current forward vector of the initiator to fly in the same direction forward. This
class also will light up objects near it, as it contains a GLLight class. So when the shoot is near another
object the light will illuminate the surroundings

Author : Romesh Selvanathan
Date : 03/05/12
*/

#ifndef __SHOOT_H__
#define __SHOOT_H__

#include "GameUtil.h"
#include "MovingEntity.h"
#include "GLLight.h"
#include "GLBox.h"
#include "Texture.h"
#include "Matrix4.h"

class Shoot : public MovingEntity
{
public:
	Shoot(const char* mType, Vec3 position, Vec3 velocity, int TimeToLive, Vec3 mColor);
	virtual ~Shoot() {MovingEntity::~MovingEntity();}

	virtual bool Init(){return true;}

	// Over load this function as both the enemies and player share this class and only differ in texture. So when the type is different change the texture
	// rather than constantly polling for the type in the update function
	virtual void SetGameObjectType(char const* const type);
	virtual void Update(float dt);
	virtual void Render();
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);

	// Set the initator's world matrix
	void SetPlayerWorldMatrix (Matrix4 m) {m_playerWorldMatrix = m;}

	void SetTimeToLive(int t){mTimeToLive = t;}
	int GetTimeToLive(){return mTimeToLive;}

	//-------------------------------ITimerListener-------------------------------------
	virtual void OnTimer(float milliseconds){}
	//----------------------------------------------------------------------------------
private:
	GLUquadricObj* quadratic;		// quadratic used to init a glu quadratic used to draw a glu sphere - LEGACY not used anymore
	int mTimeToLive;
	GLLight mLight;
	Matrix4 m_playerWorldMatrix;	// The World Matrix of the initiator 

	// The 4 differnt quads to form a star shape
	GLBox mQuad;					
	GLBox mQuad2;
	GLBox mQuad4;
	GLBox mQuad5;

	// The texture to be applied
	unsigned int texture;
};

#endif