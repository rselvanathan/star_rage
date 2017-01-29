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


#include "ParticleEngine.h"
#include "Particle.h"
#include "GameUtil.h"
#include "GLTexture.h"
#include "GameWorld.h"

/* Constructor */
ParticleEngine::ParticleEngine(int ttl, Vec3 position, int MaxParticles) 
{
	mPosition = position;
	m_timeToLive = ttl;
	setTTL = ttl;
	m_maxParticles = MaxParticles;
	scale = 200;
	active = true;

	Init();
}

/* Destructor */
ParticleEngine::~ParticleEngine()
{
	// Create each particle and add them to the list
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++) {
		SAFE_DELETE(*it);
	}
}

/* Initialise Particle Engine */ 
bool ParticleEngine::Init()
{
	// Create each particle and add them to the list
	for(int loop=0;loop < m_maxParticles; loop++) {
		SmartPtr<Particle> particle = new Particle();
		particles.push_back(particle);
	}
	return true;
}

void ParticleEngine::Update(float dt)
{
	// If Time is not 0 then decrease time till the particle emitter will be turned off 
	if(m_timeToLive != 0){
		// Decrease its time to live and when = 0 remove it from the particle manager
		m_timeToLive -= 100 * dt;
		if(m_timeToLive <= 0){
			m_timeToLive = setTTL;
			active = false;
		}
	}
}

/* Render the Particle Effect */
void ParticleEngine::Render()
{
	// Store Current Matrix
	glPushMatrix();
	// Allow Blending and Enable Texture 2D
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	// GL_DEPTH MAsk makes sure th eblending such as Additive blending still works while depth testing is switchd on without
	// causing issues
	glDepthMask (GL_FALSE);
	// Bind a Texture to the particle effect
	glBindTexture(GL_TEXTURE_2D, texture);
	// Move the Particle Effect to a certain position
	glTranslatef(mPosition.x,mPosition.y,mPosition.z);
	//glScalef(scale, scale, scale);
	// Loop throught each particl effect
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
	{
		// if the particle is active
		if ((*it)->active)
		{
			// Store the Current Matrix
			glPushMatrix();
			// Billboard the Particle Effect
			BillboardPoint();
			// Scale each Particle
			//glScalef((*it)->scale, (*it)->scale, (*it)->scale);
			// Apply the Color to each Particle
			glColor4f((*it)->r,(*it)->g,(*it)->b,(*it)->life);
			glScalef(scale, scale, scale);
			// Begin Drawing the Triangle Strip
			glBegin(GL_TRIANGLE_STRIP);						// Build Quad From A Triangle Strip
				glTexCoord2d(1,1); glVertex3f((*it)->position.x+0.5f,			// Top Right
											  (*it)->position.y+0.5f,
											  (*it)->position.z);
				glTexCoord2d(0,1); glVertex3f((*it)->position.x-0.5f,			// Top Left
											  (*it)->position.y+0.5f,
											  (*it)->position.z);
				glTexCoord2d(1,0); glVertex3f((*it)->position.x+0.5f,			// Bottom Right
											  (*it)->position.y-0.5f,
											  (*it)->position.z);
				glTexCoord2d(0,0); glVertex3f((*it)->position.x-0.5f,			// Bottom Left
											  (*it)->position.y-0.5f,
											  (*it)->position.z);
			glEnd();
			// Reset back to previous matrix
			glPopMatrix();
		}
	}// Done Building Triangle Strip

	// Disable Blending and Texture 2D
	glDepthMask (GL_TRUE);
	BLEND_TEX_OFF;
	glPopMatrix();
}

// Create the Billboarding Effect
//Fernandes, A. (n.d). Billboarding Tutorial. [online]. Availabe at: <http://www.lighthouse3d.com/opengl/billboarding/index.php?billInt> [ Accessed: 23 April 2012]
void ParticleEngine::BillboardPoint()
{
	// An array to hold the 4x4 matrix (4x4 = 16) of model view
	float modelview[16];
	// two temp values
	int i,j;

	// get the current modelview matrix
	glGetFloatv(GL_MODELVIEW_MATRIX , modelview);

	// This resets all the scaling and rotations
	for( i=0; i<3; i++ ) 
	    for( j=0; j<3; j++ ) {
		if ( i==j )
		    modelview[i*4+j] = 1.0;
		else
		    modelview[i*4+j] = 0.0;
	    }

	// set the modelview with no rotations and scaling
	glLoadMatrixf(modelview);
}

// Create the Billboarding Effect
// Fernandes, A. (n.d). Billboarding Tutorial. [online]. Availabe at: <http://www.lighthouse3d.com/opengl/billboarding/index.php?billInt> [ Accessed: 23 April 2012]
void ParticleEngine::TrueBillBoardPoint()
{
	//float lookAt[3]={0,0,1}, objToCamProj[3],upAux[3],angleCosine;
	Vec3 lookAt = Vec3(0, 0, 1);
	Vec3 objToCamProj, upAux;
	float angleCosine;

// objToCamProj is the vector in world coordinates from the local origin to the camera
// projected in the XZ plane
	objToCamProj.x = camera.x - mPosition.x;
	objToCamProj.y = 0;
	objToCamProj.z = camera.x - mPosition.x;


// normalize both vectors to get the cosine directly afterwards
	objToCamProj.Normalize();

// easy fix to determine wether the angle is negative or positive
// for positive angles upAux will be a vector pointing in the 
// positive y direction, otherwise upAux will point downwards
// effectively reversing the rotation.

	//mathsCrossProduct(upAux,lookAt,objToCamProj);
	upAux = lookAt.Cross(objToCamProj);

// compute the angle
	angleCosine = ((lookAt.x * objToCamProj.x) + (lookAt.y * objToCamProj.y) + (lookAt.z * objToCamProj.z));

// perform the rotation. The if statement is used for stability reasons
// if the lookAt and v vectors are too close together then |aux| could
// be bigger than 1 due to lack of precision
	if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
		glRotatef(acos(angleCosine)*180/3.14,upAux.x, upAux.y, upAux.z);	
}

