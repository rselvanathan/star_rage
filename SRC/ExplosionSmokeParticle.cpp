/*
Create A Explosion Smoke Particle Effect

Author : Romesh Selvanathan
*/ 

#include "ExplosionSmokeParticle.h"
#include "GLParticleEngine.h"
#include "GameUtil.h"
#include "PreLoader.h"
#include "GameWorld.h"
#include "Texture.h"

/* Constructor */
ExplosionSmokeParticle::ExplosionSmokeParticle(int ttl, Vec3 position, int MaxParticles) 
	: GLParticleEngine(ttl, position, MaxParticles)
{
	//mRadius = 2;
}

/* Destructor */
ExplosionSmokeParticle::~ExplosionSmokeParticle()
{GLParticleEngine::~GLParticleEngine();}

/* Setup the particle Effect */
void ExplosionSmokeParticle::SetupParticles()
{
	texture = PreLoader::GetInstance()->smoke->GetTGATexture().texID;
	scale = 500;
	//scale = 1;

	// GO thorugh each particle and initialise the values
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		(*it)->active=true;		// particle should be active
		(*it)->life=1.0f;		// alpha value of particle
		(*it)->fade=float(rand()%100)/1000.0f+0.003f;;	// initial random fade value(life -= fade)	
		(*it)->r= 1.0f;	// the red component of particle
		(*it)->g= 1.0f;	// the green component of particle
		(*it)->b= 1.0f; // the blue component of particle
		(*it)->position.x = (*it)->position.y = (*it)->position.z = 0.0f; // Set the position as the center	
		// Slowing Particles down by * 0.7
		(*it)->velocity.x = float((rand()%60)-50.0f) * 0.7;	// Random Valocity values
		(*it)->velocity.y = float((rand()%60)-50.0f) * 0.7;	
		(*it)->velocity.z = float((rand()%60)-30.0f) * 0.7;	
		(*it)->acceleration.x = 0.0f;	// Acceleration towards certan direction (in this case imitate wind)						
		(*it)->acceleration.y = 2.8f;						
		(*it)->acceleration.z = 0.0f;	
		(*it)->scale = 0.0f;
	}
}

/* Update the Particle Effect */
void ExplosionSmokeParticle::Update(float dt)
{
	// GO thorugh each particle 
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		// if particle is active 
		if ((*it)->active)
		{
			(*it)->position.x += (*it)->velocity.x *dt/20;	// Change in Position 
			(*it)->position.y += (*it)->velocity.y *dt/20;
			(*it)->position.z += (*it)->velocity.z *dt/20;

			(*it)->velocity.x += (*it)->acceleration.x;	    // Change in Velocity
			(*it)->velocity.y += (*it)->acceleration.y;
			(*it)->velocity.z += (*it)->acceleration.z;

			(*it)->life-=(*it)->fade;							// Reduce Particles Life By 'Fade'
			(*it)->r = (*it)->life;
			(*it)->g = (*it)->life;
			(*it)->b = (*it)->life;
			(*it)->scale += 0.01;
			if((*it)->scale >= 0.5f)
				(*it)->scale = 0.5f;
		}
	}

	GLParticleEngine::Update(dt);
}

/* Render the Particle Effect */
void ExplosionSmokeParticle::Render()
{
	// Add Additive Blending
	Graphics::GetInstance()->AdditiveBlend_Dest_Alpha();

	GLParticleEngine::Render();
	
	Graphics::GetInstance()->DisableBlend();
}

