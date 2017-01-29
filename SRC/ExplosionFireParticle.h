/*
Create A Explosion Fire Particle Effect

Author : Romesh Selvanathan
*/ 
#ifndef __EXPLOSIONFIREPARTICLE_H__
#define __EXPLOSIONFIREPARTICLE_H__

#include "GLParticleEngine.h"

class ExplosionFireParticle : public GLParticleEngine
{
public:
	/* Constructor */
	ExplosionFireParticle(int ttl, Vec3 position, int MaxParticles);
	/* Destructor */
	virtual ~ExplosionFireParticle();

	/* Setup the particle Effect */
	void SetupParticles();
	/* Update the Particle Effect */
	virtual void Update(float dt);
	virtual void Render();
private:
};

#endif