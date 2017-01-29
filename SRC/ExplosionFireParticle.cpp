/*
Create A Explosion Fire Particle Effect

Author : Romesh Selvanathan
*/ 
#include "ExplosionFireParticle.h"
#include "GLParticleEngine.h"
#include "GameUtil.h"
#include "PreLoader.h"
#include "GameWorld.h"
#include "Texture.h"

/* Constructor */
ExplosionFireParticle::ExplosionFireParticle(int ttl, Vec3 position, int MaxParticles) 
	: GLParticleEngine(ttl, position, MaxParticles)
{
}
/* Destructor */
ExplosionFireParticle::~ExplosionFireParticle()
{GLParticleEngine::~GLParticleEngine();}

/* Setup the particle Effect */
void ExplosionFireParticle::SetupParticles()
{
	texture = PreLoader::GetInstance()->fire->GetTGATexture().texID;	// Set the texture
	scale = 500;
	//scale = 1;

	// GO thorugh each particle and initialise the values
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		(*it)->active=true;		// particle should be active
		(*it)->life=1.0f;		// alpha value of particle
		(*it)->fade=float(rand()%100)/1000.0f+0.003f;;	// initial random fade value(life -= fade)	
		(*it)->r= 1.0f;	// the red component of particle
		(*it)->g= 1;	// the green component of particle
		(*it)->b= 1; // the blue component of particle
		(*it)->position.x = (*it)->position.y = (*it)->position.z = 0.0f; // Set the position as the center	
		// Speeding Particles up by * 10
		(*it)->velocity.x = float((rand()%60)-32.0f) * 0.7;	// Random Valocity values
		(*it)->velocity.y = float((rand()%60)-30.0f) * 0.7;	
		(*it)->velocity.z = float((rand()%60)-30.0f) * 0.7;	
		(*it)->acceleration.x = 0.3f;	// Acceleration towards certan direction (minmal downwards acceleration)						
		(*it)->acceleration.y = 0.8f;						
		(*it)->acceleration.z = 0.0f;	
		(*it)->scale = 1;				
	}
}

/* Update the Particle Effect */
void ExplosionFireParticle::Update(float dt)
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
		}

	}

	GLParticleEngine::Update(dt);
}

/* Render the Particle Effect */
void ExplosionFireParticle::Render()
{
	// Add Additive Blending
	Graphics::GetInstance()->AdditiveBlend_Dest_Alpha();

	GLParticleEngine::Render();
	
	Graphics::GetInstance()->DisableBlend();
}

