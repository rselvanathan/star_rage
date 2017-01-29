/*
A Speed Line particle, that will be generated to create the illusion of stars stetching out when sprinting.
In order for the lines to appear around the player as well as moving towards 
him/her, the world matrix of the player spaceship is passed in. The particle system generates a line which 
have two points defined, one for the starting point and the other is the decrementing z value. This z value
will stop at a user specified boundary at which the z value will be reset to its starting position again. 

Author : Romesh Selvanathan
Date : 20/03/12
*/

#ifndef __STARFIELD_H__
#define __STARFIELD_H__

#include "GameUtil.h"
#include "Matrix4.h"

// A Single Star particle
struct StarParticle
{
	bool active;					// Is Star Particle Active
	Vec3 position;					
	Vec3 velocity;
	Vec3 rotation;

	float r , g , b;				// Color of Particle

};

class StarField
{
public:
	/* Constructor */
	StarField(Vec3 position, int MaxParticles);
	/* Destructor */
	~StarField();

	void Reset();

	/* Setup the particle Effect */
	void SetupParticles();
	/* Update the Particle Effect */
	void Update(float dt);
	/* Render Particle Effect */
	void Render();

	// Set the player matrix
	void SetMatrix(Matrix4 m) {m_playerWorldMatrix = m;} 
	// Is Sprint enabled?
	void SetSprint(bool b) {m_sprint = b;}
private:
	Vec3 mPosition;											// The Position of the Particle Effect
	Matrix4 m_playerWorldMatrix;							// THe World Matrix of the SPaceship
	bool m_sprint;											// Is player Sprinting?
	int count;												// Used to count how many have reached 0 in terms of position to reset the particles

	unsigned int texture;									// Texture not used currently

	typedef vector<SmartPtr<StarParticle>> StarList;		// List of Particles
	StarList mStars;
};

#endif