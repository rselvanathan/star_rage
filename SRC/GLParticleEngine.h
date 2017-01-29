/* 
This Class is the base Class for any type of Particle Emitter in the game, which allows a
point sprite to be added into the game world. Point Sprites are excellent as they automatically face
the viewer and take up much less CPU processing power as well as memory. However their disadvantage is that you cannot
apply different sizes for each point sprite as they all are defined with a single scale

Author : Romesh Selvanathan
Date : 03/05/12

References : 

Harris, K.R. (2010). OpenGL 1.2 - 2.0 code samples - page 6. [online] Available at: <http://www.codesampler.com/oglsrc/oglsrc_6.htm>. [Accessed: 30 March 2012].
*/
#ifndef __GLGLParticleEngine_H__
#define __GLGLParticleEngine_H__

#include "GameUtil.h"

class ParticleManager;

struct ParticleP
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

class GLParticleEngine 
{
public:
	/* Constructor */
	GLParticleEngine(int ttl, Vec3 position, int MaxParticles);
	/* Destructor */
	virtual ~GLParticleEngine();

	/* Initialise Particles */
	bool Init();
	/* Setup Each Particle */
	virtual void SetupParticles() = 0;
	virtual void Update(float dt);
	virtual void Render();

	const float GetScale() const{return scale;}
	void SetScale(float s) {scale = s;}

	const Vec3 GetPosition() const{return mPosition;}
	void SetPosition(Vec3 v) {mPosition = v;}

	void SetTexture(unsigned int t) {texture = t;}

	const bool IsActive() const{return active;}
	void SetActive(bool a) {active = a;}

protected:
	typedef list < SmartPtr<ParticleP> > ParticleList;
	ParticleList particles;						// List of particles

	float scale;								// scale of the particle effect
	int m_maxParticles;							// maximum number of particles
	unsigned int texture;						// the texture to be used by the particle effect
	int m_timeToLive;							// time to live for the particles
	int setTTL;									// The initial timetolive value;
	Vec3 mPosition;								// Position of the whole Particle Emitter itself
	float m_maxPointSize;						// The maximum point size - This depends on hardware 

	bool active;								// Is the emitter active?
};

#endif