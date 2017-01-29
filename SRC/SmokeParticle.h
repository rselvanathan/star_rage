
#ifndef __SMOKEPARTICLE_H__
#define __SMOKEPARTICLE_H__

#include "GameUtil.h"
#include "GLParticleEngine.h"

class MovingEntity;

class SmokeParticle : public GLParticleEngine
{
public:
	/* Constructor */
	SmokeParticle(int ttl, Vec3 position, int MaxParticles, MovingEntity* o);
	/* Destructor */
	virtual ~SmokeParticle();

	///* Initialise Particles */
	//bool Init();
	/* Setup Each Particle */
	void SetupParticles();
	virtual void Update(float dt);
	virtual void Render();

	MovingEntity* GetGameObject(){return object;}
	void SetGameObject(MovingEntity* o) {object = o;}

protected:
	float float_rand(float min, float max);
	int int_rand(int min, int max);
	Vec3 RandomDirection();
	void AddParticles();

	float minV, maxV;
	float minLife, maxLife;
	float minScale, maxScale;
	float minNum, maxNum;

	int timeTillNextPlume;

	MovingEntity* object;							// Pointer to what object this smoke will come off at
};

#endif