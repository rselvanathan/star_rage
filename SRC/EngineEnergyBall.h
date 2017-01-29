/*
This object represents the engine energy ball that lights up behind the player spaceship, whenever the 
spaceship accelerates. It is similar achieved by placing 5 Quads in varying degrees facing the user.
Then simply blending the Quads with a texture a energy engine will be created. The velocity of the
player spaceship determines the alpha value of this object, making it dissapear or light up slowly when the 
player accelerates.

Author : Romesh Selvanathan

Date: 3/05/12
*/
#ifndef __ENGINEENERGYBALL_H__
#define __ENGINEENERGYBALL_H__

#include "GameUtil.h"
#include "Entity.h"
#include "GLBox.h"

class EngineEnergyBall : public Entity
{
public:
	EngineEnergyBall(const char* mType, float width, float height, Vec3 position);
	virtual ~EngineEnergyBall();

	void Update(float dt){}
	void Render();
	bool HandleMessage(const Telegram& msg){return true;}

	/* This will determine the alpha value of the Energy Ball as only when the player/enemy accelerate should it appear
	and dissapear when the entity stops */
	void SetLife(Vec3 velocity);

private:
	GLBox Quad0;
	GLBox Quad20;
	GLBox Quad40;
	GLBox Quad60;
	GLBox Quad80;

	// The Relative Position of this Engine Energy from its owner
	Vec3 m_vRealtivePosition;
	// This is the texture that will be assigned to the different quads
	unsigned int texture;

	GLUquadricObj* quadratic;	// DEBUGGING
};

#endif