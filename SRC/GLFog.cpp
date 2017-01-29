/*
A class that sets up fog for the game world

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/ 

#include "GLFog.h"
#include "GameUtil.h"

/* Constructor */
GLFog::GLFog()
{
	// Initialise values
	r = g = b = 0.5f;
	a = 1.0f;

	fogDensity = fogStartDepth = fogEndDepth = 0.0f;
}

/* Set up the Color of the fog including the alpha channel */
void GLFog::SetColor(float R, float G, float B, float A)
{r = R; g = G; b = B; a = A;}

/* Set the depth of the fog(higher the value the more thick it will be) */
void GLFog::SetFogDensity(float Density)
{fogDensity = Density;}

/* How far you can see for the fog */
void GLFog::SetFogDepth(float Start, float End)
{fogStartDepth = Start; fogEndDepth = End;}

/* Enable and render the fog */
void GLFog::Render()
{
	GLfloat color[] = {r, g, b, a};

	glFogi(GL_FOG_MODE, GL_EXP2);				 // Fog Mode
	glFogfv(GL_FOG_COLOR, color);				 // Set Fog Color
	glFogf(GL_FOG_DENSITY, fogDensity);          // How Dense Will The Fog Be
	glHint(GL_FOG_HINT, GL_DONT_CARE);           // Fog Hint Value
	glFogf(GL_FOG_START, fogStartDepth);         // Fog Start Depth
	glFogf(GL_FOG_END, fogEndDepth);             // Fog End Depth
	glEnable(GL_FOG);                            // Enables GL_FOG	
}

