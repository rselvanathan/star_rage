/*
Create A Explosion Spark Particle Effect

Author : Romesh Selvanathan
*/ 
#ifndef __EXPLOSIONPARTICLE_H__
#define __EXPLOSIONPARTICLE_H__

#include "GLParticleEngine.h"

class ExplosionParticle : public GLParticleEngine
{
public:
	/* Constructor */
	ExplosionParticle(int ttl, Vec3 position, int MaxParticles);
	/* Destructor */
	virtual ~ExplosionParticle();

	/* Setup the particle Effect */
	void SetupParticles();
	/* Update the Particle Effect */
	virtual void Update(float dt);
	virtual void Render();
private:
};

#endif