/*
There are two types of powerups the player can obtain currently. These are the energy powerup and health powerup
and as their names suggest their increase either the energy or health values back up again

Author : Romesh Selvanathan
*/ 
#ifndef __POWERUP_H__
#define __POWERUP_H__

#include "Entity.h"
#include "GLBox.h"

class PowerUp : public Entity
{
public:
	/* Constructor */
	PowerUp(const char* name);
	/* Destructor */ 
	virtual ~PowerUp();

	bool Init(){return true;}
	void Reset(){}

	///////////////////////////////////////GameObject Inherited methods/////////////////////////////////
	virtual void Update(float dt);
	virtual void Render();
	virtual bool CollisionTest(Entity* o);
	virtual void OnCollision(Entity* o);
	bool HandleMessage(const Telegram& msg){return true;} 
	////////////////////////////////////////////////////////////////////////////////////////////////////

	//-------------------------------ITimerListener-------------------------------------
	virtual void OnTimer(float milliseconds){}
	//----------------------------------------------------------------------------------

private:
	GLBox box;		// A 3D openGl box
};

#endif