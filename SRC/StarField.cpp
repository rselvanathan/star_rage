/*
A Speed Line particle, that will be generated to create the illusion of stars stetching out when sprinting.
In order for the lines to appear around the player as well as moving towards 
him/her, the world matrix of the player spaceship is passed in. The particle system generates a line which 
have two points defined, one for the starting point and the other is the decrementing z value. This z value
will stop at a user specified boundary at which the z value will be reset to its starting position again. 

Author : Romesh Selvanathan
Date : 20/03/12
*/
#include "StarField.h"
#include "GameUtil.h"
#include "PreLoader.h"
#include "Texture.h"
#include "Matrix4.h"

/* Constructor */
StarField::StarField(Vec3 position, int MaxParticles)
{
	mPosition = position;
	m_playerWorldMatrix = Matrix4::IDENTITY;
	m_sprint = false;
	count = 0;

	for (int i = 0; i < MaxParticles; i ++)
	{
		SmartPtr<StarParticle> s = new StarParticle();
		mStars.push_back(s);
	}
}
/* Destructor */
StarField::~StarField()
{}

/* Setup the particle Effect */
void StarField::SetupParticles()
{
	texture = PreLoader::GetInstance()->particle->GetTGATexture().texID;	// Set the texture

	// GO thorugh each particle and initialise the values
	for(StarList::iterator it = mStars.begin(); it != mStars.end(); it++)
	{
		(*it)->active = false;
		float x, y, z;
		x = rand() % 20;
		y = rand() % 20;
		z = 0;
		(*it)->position.x = x;
		(*it)->position.y = y;
		(*it)->position.z = z;

		(*it)->velocity.z = float((rand()%20)-15.f) * 0.1;

		(*it)->r = 0.9;
		(*it)->g = 1; 
		(*it)->b = 1;
	}
}

/* Update the Particle Effect */
void StarField::Update(float dt)
{
	// IF sprinting start gradually and then integrate the position through velocity
	// IF position is less than a certain amount reset the particles
	if(m_sprint) {
		for(StarList::iterator it = mStars.begin(); it != mStars.end(); it++)
		{
			(*it)->active = true;
			(*it)->position.z += (*it)->velocity.z * 0.05;
			if((*it)->position.z < -5)
				(*it)->position.z = 0;
		}
	}
	// else if the position of it is less than a certain amount and the particle is still alive decrease the velocity and move it back 
	// creating a smooth gradual decreasing particle effect when sprint it off.
	// When reaching a certain value set the particle off and count how many have reached that certain spot.
	else {
		for(StarList::iterator it = mStars.begin(); it != mStars.end(); it++)
		{
			if((*it)->position.z < 0 && (*it)->active){
				(*it)->position.z -= (*it)->velocity.z * 0.2;
				if((*it)->position.z >= 0) {
					(*it)->active = false;
					count += 1;
					// if count is >= to half of star size then reset the particles
					// NOTE: ccreates a smooth effect as some particles dont remove themselves if count is set to all particles
					// as some dont seem to move to reach 0 at all so this has to be done instead
					if(count >= (mStars.size()/2)){
						count = 0;
						SetupParticles();
					}
				}
			}
		}
	}
}

void StarField::Render()
{
	// Enable Blending
	Graphics::GetInstance()->DisableLighting();
	Graphics::GetInstance()->Disable2DTextures();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();
	// Bind a Texture to the particle effect
	Graphics::GetInstance()->Bind2DTexture(texture);
	// Multiply the matrix with the current matrix in the stack to apply the rotations
	Graphics::GetInstance()->MultiplyToMatrix(m_playerWorldMatrix);
	// After rotations move them in front of the player
	Graphics::GetInstance()->Translate(mPosition.x,mPosition.y,mPosition.z);
	// Loop throught each particl effect
	for(StarList::iterator it = mStars.begin(); it != mStars.end(); it++)
	{
		// Store the Current Matrix
		Graphics::GetInstance()->StoreMatrix();
		// If particle is active draw a line which stretches
		if( (*it)->active) {
			// Is Quite specfic to this class so had to idividually draw this
			// A Line is created with two points. One to determine where it starts from
			// the other will make sure the line is moving towards the viewer in the z direction
			Graphics::GetInstance()->StarDraw(GL_LINE_STRIP);
			Graphics::GetInstance()->Color3((*it)->r, (*it)->g, (*it)->b);
			Graphics::GetInstance()->Vertex3((*it)->position.x, (*it)->position.y, (*it)->position.z);
			Graphics::GetInstance()->Color3(0, 0, 0);
			Graphics::GetInstance()->Vertex3((*it)->position.x, (*it)->position.y, (*it)->position.z * 5 *(*it)->velocity.z);
			Graphics::GetInstance()->EndDraw();
		}
		// Release the Matrix
		Graphics::GetInstance()->ReleaseMatrix();
	}// Done Building Triangle Strip

	// Disable Blending and Texture 2D
	Graphics::GetInstance()->DisableBlend();
	Graphics::GetInstance()->Enable2DTextures();
	Graphics::GetInstance()->EnableLighting();
}


