/* 
The Drawing portion and Setting up the Point sprite porion has been referenced directly from
:
Harris, K.R. (2010). OpenGL 1.2 - 2.0 code samples - page 6. [online] Available at: <http://www.codesampler.com/oglsrc/oglsrc_6.htm>. [Accessed: 30 March 2012].

Author : Romesh Selvanathan
*/


#include "GLParticleEngine.h"
#include "GameUtil.h"
#include "GLTexture.h"

// GL_ARB_point_parameters
PFNGLPOINTPARAMETERFARBPROC  glPointParameterfARB = NULL;
PFNGLPOINTPARAMETERFVARBPROC glPointParameterfvARB = NULL;

/* Constructor */
GLParticleEngine::GLParticleEngine(int ttl, Vec3 position, int MaxParticles)
{
	mPosition = position;
	m_timeToLive = ttl;
	setTTL = ttl;
	m_maxParticles = MaxParticles;
	scale = 200;
	active = true;

	glPointParameterfARB  = (PFNGLPOINTPARAMETERFARBPROC)wglGetProcAddress("glPointParameterfARB");
    glPointParameterfvARB = (PFNGLPOINTPARAMETERFVARBPROC)wglGetProcAddress("glPointParameterfvARB");

	Init();
}

/* Destructor */
GLParticleEngine::~GLParticleEngine()
{
	// Create each particle and add them to the list
	for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++) {
		SAFE_DELETE(*it);
	}
}

/* Initialise Particle Engine */ 
bool GLParticleEngine::Init()
{
	// Create each particle and add them to the list
	for(int loop=0;loop < m_maxParticles; loop++) {
		SmartPtr<ParticleP> particle = new ParticleP();
		particles.push_back(particle);
	}

	glGetFloatv(GL_POINT_SIZE_MAX_ARB, &m_maxPointSize);
	glPointSize(m_maxPointSize);

	return true;
}

void GLParticleEngine::Update(float dt)
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

/* 
Render the Particle Effect

Reference:
Harris, K.R. (2010). OpenGL 1.2 - 2.0 code samples - page 6. [online] Available at: <http://www.codesampler.com/oglsrc/oglsrc_6.htm>. [Accessed: 30 March 2012].
*/
void GLParticleEngine::Render()
{
	// Store Current Matrix
	glPushMatrix();
	// Enable 2D Textures
	glEnable(GL_TEXTURE_2D);
	// Switch Depth mask off So that the particle effect will still have depth but not effect the particle emitter itself causing
	// weird artifacts like the edges being visible while blending is turned on
	glDepthMask(GL_FALSE);
	// This is how the sprite size will be modfied from the distance from viewer
	float quadratic[] =  { 1.0f, 0.0f, 1.0f };
    glPointParameterfvARB( GL_POINT_DISTANCE_ATTENUATION_ARB, quadratic );
	// This allows fading of points instead of shrinking past a defined threshold size. The 
	// GL_POINT_FADE_THRESHOLD_SIZE_ARB is not clamped to minmium or maximum point sizes
	glPointParameterfARB( GL_POINT_FADE_THRESHOLD_SIZE_ARB, 60.0f );

	glPointParameterfARB( GL_POINT_SIZE_MIN_ARB, 1.0f );
    glPointParameterfARB( GL_POINT_SIZE_MAX_ARB, m_maxPointSize );
	// Specity point sprite texture coordinate replacement mode for each texture unit
	glTexEnvf( GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE );
	// Enable Point SPrites
	glEnable( GL_POINT_SPRITE_ARB );
	// Scale of each point
	glPointSize( scale ); 
	// The texture for this specific emitter
	glBindTexture( GL_TEXTURE_2D, texture);
	// Position the Emitter
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	// Render Each Particle
	glBegin( GL_POINTS );
    {
       for(ParticleList::iterator it = particles.begin(); it != particles.end(); it++)
        {
			if((*it)->active){
				glColor4f( (*it)->r,(*it)->g, (*it)->b, (*it)->life);

				glVertex3f((*it)->position.x,(*it)->position.y,(*it)->position.z);
			}
        }
    }
	glEnd();

	glDisable( GL_POINT_SPRITE_ARB );
	glDisable(GL_TEXTURE_2D);
	glDepthMask(GL_TRUE);

	glPopMatrix();
}