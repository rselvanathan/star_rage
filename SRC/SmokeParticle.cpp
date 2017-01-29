#include "SmokeParticle.h"
#include "GLParticleEngine.h"
#include "GameUtil.h"
#include "PreLoader.h"
#include "Texture.h"
#include "MovingEntity.h"

/* Constructor */
SmokeParticle::SmokeParticle(int ttl, Vec3 position, int MaxParticles, MovingEntity* o) : GLParticleEngine(ttl, position, MaxParticles)
{
	texture = PreLoader::GetInstance()->smoke->GetTGATexture().texID;
	timeTillNextPlume = 50;

	object = o;

	minV = 0.5f;
	maxV = 2.0f;

	minLife = 0.5f;
	maxLife = 1.0f;

	minScale = 0.5;
	maxScale = 1.0;

	minNum = 50;
	maxNum = 70;

	scale = 300;
	//scale = 1.5f;
}

/* Destructor */
SmokeParticle::~SmokeParticle()
{
	SAFE_DELETE(object);
	GLParticleEngine::~GLParticleEngine();
}

void SmokeParticle::SetupParticles()
{
	// GO thorugh each particle and initialise the values
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		(*it)->active=false;		// particle should be active
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
		(*it)->acceleration.x = 0.0f;	// Acceleration towards certan direction (minmal downwards acceleration)						
		(*it)->acceleration.y = 2.2f;						
		(*it)->acceleration.z = 0.0f;	
		(*it)->scale = 0.0f;
	}
}

void SmokeParticle::Update(float dt)
{
	// Obtain Current object's position and set to this smoke position
	mPosition = object->GetPosition();

	// This will add more particls at an interval creating a more realistis smoke plume effect
	timeTillNextPlume-= 100 *dt;
	if(timeTillNextPlume <= 0){
		timeTillNextPlume = 30;
		AddParticles();
	}

	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if((*it)->active){
			(*it)->position.x += (*it)->velocity.x * dt ;	// Change in Position 
			(*it)->position.y += (*it)->velocity.y * dt ;
			(*it)->position.z += (*it)->velocity.z * dt ;

			(*it)->velocity.x += (*it)->acceleration.x / 10;   // Change in Velocity
			(*it)->velocity.y += (*it)->acceleration.y / 10;
			(*it)->velocity.z += (*it)->acceleration.z / 10;
				
			(*it)->life-=(*it)->fade;							// Reduce Particles Life By 'Fade
			(*it)->r -= (*it)->fade;
			(*it)->g -= (*it)->fade;
			(*it)->b -= (*it)->fade;

			if ((*it)->life<0.0f)								// If Particle Is Burned Out
				{ 
					(*it)->active= false;
				}
		}

	}

	GLParticleEngine::Update(dt);
}

/* Render the Particle Effect */
void SmokeParticle::Render()
{
	// Blend The Effect
	Graphics::GetInstance()->Blend_SRCWithOne_MinusSRCColor();

	GLParticleEngine::Render();
	
	Graphics::GetInstance()->DisableBlend();
}

// A function to obtain a random float between a minimum and maximum
float SmokeParticle::float_rand(float min, float max)
{
	return ((max-min)*((float)rand()/RAND_MAX))+min;
}

// a function to return a random integer beteween a minimum and maximum
int SmokeParticle::int_rand(int min, int max)
{
	return rand()%(max-min)+min;
}

// The random direction the particle should travel
Vec3 SmokeParticle::RandomDirection()
{
	float radians = float_rand(RAD2DEG*80, RAD2DEG*100);

	Vec3 direct = Vec3((float)cos(radians), (float)sin(radians), 0);
	return direct;
}

// Add more particles to smoke effect
void SmokeParticle::AddParticles()
{
	int numParticles = int_rand(minNum, maxNum);
	int count = 0;
	
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		if(!(*it)->active){
			(*it)->active = true;

			float l = float_rand(minLife, maxLife);
			float v = float_rand(minV, maxV);
			Vec3 dir = RandomDirection();

			(*it)->life=l;		// alpha value of particle
			(*it)->fade=float(rand()%100)/1000.0f+0.003f;;	// initial random fade value(life -= fade)	
			(*it)->r= 1.0f;	// the red component of particle
			(*it)->g= 1;	// the green component of particle
			(*it)->b= 1; // the blue component of particle
			(*it)->position.x = (*it)->position.y = (*it)->position.z = 0.0f; // Set the position as the center	
			// Speeding Particles up by * 10
			(*it)->velocity.x = float((rand()%60) -32.f)* 0.03f;	// Random Valocity values
			(*it)->velocity.y = float((rand()%60) -20.0f)*0.05f;	
			(*it)->velocity.z = 0.0f;	
			(*it)->acceleration.x = 0.0f;	// Acceleration towards certan direction (minmal downwards acceleration)						
			(*it)->acceleration.y = 0.08f;						
			(*it)->acceleration.z = 0.0f;	
		}

		count++;
		if(count >= numParticles)
			return;
	}
}
