/*
A Wrapper Class that sets up a openGl light

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#ifndef __GLLIGHT_H__
#define __GLLIGHT_H__

#include "GameUtil.h"

class GLLight
{
public:
	/* Constructor */
	GLLight();
	/* Reset the Light variables */
	void Reset();

	/* Enabele And disable OpenGL Light */
	void PowerUpLight();
	void PowerDownLight();

	/* Switch on and off a Specific Light from Opengl (for ex GL_LIGHT1) */
	void SwitchOnLight(GLenum Light);
	void SwitchOffLight(GLenum Light);

	/* Set the world ambient color of the light */
	void SetWorldAmbientColor(float AR, float AG, float AB, float AON);
	/* Set the Ambient Color of the light */
	void SetAmbientColor(float R, float G, float B, float On);
	/* Set the Diffuse Color of the light */
	void SetDiffuseColor(float R, float G, float B, float On);
	/* Set the Specular Color of the light */
	void SetSpecularColor(float R, float G, float B, float On);
	/* Set the shinniness of the specular light */
	void SetShininess(float Shiny);
	/* Set the Position of the light */
	void SetPosition(float X, float Y, float Z, float W);
	/* Activate a spotligtht */
	void ActivateSpotLight(bool v);
	/* Setup A spotlight */
	void SetupSpotLight(float CutOff, float Exponent);
	/* Setup The Light Attenuation (Intensity depending on distance) */
	void SetLightAttenuation(float constant, float linear, float quadratic);
	/* Render the light */
	void Render();

private:

	// World Ambient Color
	float Ar, Ag, Ab, AOn;
	// Ambient, Diffuse, Specular Coloring
	float ar, ag, ab, aOn;
	float dr, dg, db, dOn;
	float sr, sg, sb, sOn;
	// How shiny specular lighting should be
	float shiny;
	//Position of Light
	float x, y, z, w;
	// Is Spot light active?
	bool isSpotlight;
	// Spot light variables
	float cutoff;
	float exponent;
	// Light Attenuation value
	float constant_att;
	float linear_att;
	float quadratic_att;
	// Light to be switched on
	GLenum light;

};

#endif