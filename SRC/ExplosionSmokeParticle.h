/*
Create A Explosion Smoke Particle Effect

Author : Romesh Selvanathan
*/ 

#ifndef __EXPLOSIONSMOKEPARTICLE_H__
#define __EXPLOSIONSMOKEPARTICLE_H__

#include "GLParticleEngine.h"

class ExplosionSmokeParticle : public GLParticleEngine
{
public:
	/* Constructor */
	ExplosionSmokeParticle(int ttl, Vec3 position, int MaxParticles);
	/* Destructor */
	virtual ~ExplosionSmokeParticle();

	/* Setup the particle Effect */
	void SetupParticles();
	/* Update the Particle Effect */
	virtual void Update(float dt);
	virtual void Render();
private:
};

#endif