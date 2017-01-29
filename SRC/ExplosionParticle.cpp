/*
Create A Spark Fire Particle Effect

Author : Romesh Selvanathan
*/ 

#include "ExplosionParticle.h"
#include "GLParticleEngine.h"
#include "GameUtil.h"
#include "PreLoader.h"
#include "GameWorld.h"
#include "Texture.h"

/* Constructor */
ExplosionParticle::ExplosionParticle(int ttl, Vec3 position, int MaxParticles) 
	: GLParticleEngine(ttl, position, MaxParticles)
{
	//mRadius = 2;
}

/* Destructor */
ExplosionParticle::~ExplosionParticle()
{GLParticleEngine::~GLParticleEngine();}

/* Setup the particle Effect */
void ExplosionParticle::SetupParticles()
{
	texture = PreLoader::GetInstance()->particle->GetTGATexture().texID;	// Set the texture
	scale = 200;
	//scale = 0.4f;

	// GO thorugh each particle and initialise the values
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		(*it)->active=true;		// particle should be active
		(*it)->life=1.0f;		// alpha value of particle
		(*it)->fade=float(rand()%100)/1000.0f+0.003f;;	// initial random fade value(life -= fade)	
		(*it)->r= 1.0f;	// the red component of particle
		(*it)->g= 0.5f;	// the green component of particle
		(*it)->b= 0.0f; // the blue component of particle
		(*it)->position.x = (*it)->position.y = (*it)->position.z = 0.0f; // Set the position as the center	
		// Speeding Particles up by * 10
		(*it)->velocity.x = float((rand()%60)-32.0f) * 10;	// Random Valocity values
		(*it)->velocity.y = float((rand()%60)-30.0f) * 10;	
		(*it)->velocity.z = float((rand()%60)-30.0f) * 10;	
		(*it)->acceleration.x = 0.0f;	// Acceleration towards certan direction (minmal downwards acceleration)						
		(*it)->acceleration.y = -0.2f;						
		(*it)->acceleration.z = 0.0f;							
	}
}

/* Update the Particle Effect */
void ExplosionParticle::Update(float dt)
{
	// GO thorugh each particle 
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		// if particle is active 
		if ((*it)->active)
		{
			(*it)->position.x += (*it)->velocity.x * dt / 15;	// Change in Position 
			(*it)->position.y += (*it)->velocity.y * dt / 15;
			(*it)->position.z += (*it)->velocity.z * dt / 15;

			(*it)->velocity.x += (*it)->acceleration.x * dt / 10;	// Change in Velocity
			(*it)->velocity.y += (*it)->acceleration.y * dt / 10;
			(*it)->velocity.z += (*it)->acceleration.z * dt / 10;
				
			(*it)->life-=(*it)->fade;							// Reduce Particles Life By 'Fade'
		}
	}

	GLParticleEngine ::Update(dt);
}

/* Render the Particle Effect */
void ExplosionParticle::Render()
{
	// Add Additive Blending
	Graphics::GetInstance()->AdditiveBlend_Dest_Alpha();

	GLParticleEngine::Render();
	
	Graphics::GetInstance()->DisableBlend();
}


