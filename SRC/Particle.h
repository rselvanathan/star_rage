/* 

This class represent a Particle. Used for the Billboarding Technqiue 

Author : Romesh Selvanthan

Date : 03/05/12
*/ 

#ifndef __PARTICLE_H__
#define __PARTICLE_H__

#include "GameUtil.h"

struct Particle
{
	Vec3 position;		// position of particle
	Vec3 velocity;		// velocity of particle
	Vec3 acceleration;
	float life;			// life of particle (alpha value practically)
	float fade;			// fade value of particle (amount decreasing for alpha)
	float r, g, b;		// red green blue color components
	float scale;		// size of particle
	bool active;		// is particle active?
};

#endif