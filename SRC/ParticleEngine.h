/* 
A Base class that will be used to create different particle effects - It is a type of game object as it
will be added to the game world just like any other object. The Particle Effect Engine just creates a lot
of Particles that all have their own velocity and acceleration to create different Effects. 

This Particle Engine uses the Billboarding effect to accomplish the effect appearing in the 3D world.
Billboarding is the specidal effect used to make sure that 2D object always face the viewer at all times.
For now I am using the most simple solution to this problem , bu will change the billboarding effect solution
to a more complex one later on.

The Particle Engine is made out of a triangle Strip which together create a quad. So this means that a texture
is put on the triangle strip to create the particle effect. I am also using blending to allow the particle effect
to blend with its background and environment, which just makes sure that the edges of the textures are not shown

Author : Romesh Selvanathan

References : 

Molofee, J. (n.d). "Particle Engine using Triangle Strips". [online]. Available at: <http://nehe.gamedev.net/tutorial/particle_engine_using_triangle_strips/21001/>. [Accessed: 03/05/2012]

Fernandes, A. (n.d). Billboarding Tutorial. [online]. Availabe at: <http://www.lighthouse3d.com/opengl/billboarding/index.php?billInt> [ Accessed: 23 April 2012]
*/
#ifndef __PARTICLEENGINE_H__
#define __PARTICLEENGINE_H__

#include "GameUtil.h"
#include "Particle.h"
#include "MovingEntity.h"

class ParticleEngine
{
public:
	/* Constructor */
	ParticleEngine(int ttl, Vec3 position, int MaxParticles);
	/* Destructor */
	virtual ~ParticleEngine();

	/* Initialise Particles */
	bool Init();
	/* Setup Each Particle */
	virtual void SetupParticles() = 0;
	// Update the Particle Effect
	virtual void Update(float dt);
	// Render the Particle Effect
	virtual void Render();

	// Create the Billboarding Effect
	void BillboardPoint();
	void TrueBillBoardPoint();
	void SetCam(Vec3 cam) {camera = cam;}

	const float GetScale() const{return scale;}
	void SetScale(float s) {scale = s;}

	const Vec3 GetPosition() const{return mPosition;}
	void SetPosition(Vec3 v) {mPosition = v;}

	void SetTexture(unsigned int t) {texture = t;}

	const bool IsActive() const{return active;}
	void SetActive(bool a) {active = a;}

protected:
	typedef list < SmartPtr<Particle> > ParticleList;
	ParticleList particles;						// List of particles

	Vec3 camera;
		
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