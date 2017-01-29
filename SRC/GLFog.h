/*
A class that sets up fog for the game world

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/ 
#ifndef __GLFOG_H__
#define __GLFOG_H__

#include "GameUtil.h"

class GLFog
{
public:
	/* Constructor */
	GLFog();
	/* Set up the Color of the fog including the alpha channel */
	void SetColor(float R, float G, float B, float A);
	/* Set the depth of the fog(higher the value the more thick it will be) */
	void SetFogDensity(float Density);
	/* How far you can see for the fog */
	void SetFogDepth(float Start, float End);
	/* Enable and render the fog */
	void Render();

private:
	float r, g, b, a;			// The Color compontents of the fog including th alpha value
	float fogDensity;			// Density of the fog
	float fogStartDepth;		// Starting depth of fog
	float fogEndDepth;			// Ending depth of fog
};

#endif