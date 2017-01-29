/*
A Wrapper Class that sets up a openGl light

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "GLLight.h"
#include "GameUtil.h"

/* Constructor */
GLLight::GLLight()
{
	// Initialise all the values
	Ar, Ag, Ab = 0.2f;
	AOn = w = 0;
	aOn = dOn = sOn = 1;

	ar = ag = ab = dr = dg = db = sr = sg = sb = 1.0f;

	shiny = 70.0f;

	x = y = z = 1.0f;

	isSpotlight = false;

	cutoff = 20.0f;
	exponent = 2.0f;

	constant_att = 1.0f;
	linear_att = quadratic_att = 0.0f;
}

/* Reset the Light variables */
void GLLight::Reset()
{
	Ar, Ag, Ab = 0.2f;
	AOn = w = 0;
	aOn = dOn = sOn = 1;

	ar = ag = ab = dr = dg = db = sr = sg = sb = 1.0f;

	shiny = 70.0f;

	x = y = z = 1.0f;

	isSpotlight = false;

	cutoff = 20.0f;
	exponent = 2.0f;
}

/* 
Enabele And disable OpenGL Light ( as well as the Color Material and GL_Normalize wich makes
sure that if an object is scaled then the lighting onto a object does not change
*/
void GLLight::PowerUpLight()
{glEnable(GL_LIGHTING);glEnable(GL_COLOR_MATERIAL);glEnable(GL_NORMALIZE);}

void GLLight::PowerDownLight()
{glDisable(GL_LIGHTING);glDisable(GL_COLOR_MATERIAL);glDisable(GL_NORMALIZE);}

/* Switch on and off a Specific Light from Opengl (for ex GL_LIGHT1) */
void GLLight::SwitchOnLight(GLenum Light)
{light = Light; glEnable(light);}

void GLLight::SwitchOffLight(GLenum Light)
{light = Light; glDisable(light);}

/* Set the world ambient color of the light */
void GLLight::SetWorldAmbientColor(float AR, float AG, float AB, float AON)
{Ar = AR; Ag = AG; Ab = AB; AOn = AON;}

/* Set the Ambient Color of the light */
void GLLight::SetAmbientColor(float R, float G, float B, float On)
{ar = R; ag = G; ab = B; aOn = On;}

/* Set the Diffuse Color of the light */
void GLLight::SetDiffuseColor(float R, float G, float B, float On)
{dr = R; dg = G; db = B; dOn = On;}

/* Set the Specular Color of the light */
void GLLight::SetSpecularColor(float R, float G, float B, float On)
{sr = R; sg = G; sb = B; sOn = On;}

/* Set the shinniness of the specular light */
void GLLight::SetShininess(float Shiny)
{shiny = Shiny;}

/* Set the Position of the light */
void GLLight::SetPosition(float X, float Y, float Z, float W)
{x = X; y = Y; z = Z; w = W;}

/* Activate a spotligtht */
void GLLight::ActivateSpotLight(bool v)
{isSpotlight = v;}

/* Setup A spotlight */
void GLLight::SetupSpotLight(float CutOff, float Exponent)
{cutoff = CutOff; exponent = Exponent;}

void GLLight::SetLightAttenuation(float constant, float linear, float quadratic)
{constant_att = constant; linear_att = linear; quadratic_att = quadratic;}

/* Render the light */
void GLLight::Render()
{
	//GLfloat ambientIntensity[] = {Ar, Ag, Ab, AOn};
	GLfloat ambientColor[] = {ar, ag, ab, aOn};
	GLfloat diffuseColor[] = {dr, dg, db, dOn};
	GLfloat specularColor[] = {sr, sg, sb, dOn};
	GLfloat position[] = {x, y, z, w};
	GLfloat shininess[1] = {shiny};

	//glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientIntensity);

	// Enable GL Materials to allow object to reflect and be shiny
	// THis also makes sure that the color from each object is lighted properly
	glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE, diffuseColor);
	glMaterialfv(GL_FRONT,GL_SPECULAR,specularColor);
	glMaterialfv(GL_FRONT,GL_SHININESS,shininess); 

	// Setup all the Different colors of lights and position the,
	glLightfv(light, GL_AMBIENT, ambientColor);
	glLightfv(light, GL_DIFFUSE, diffuseColor);
	glLightfv(light, GL_SPECULAR, specularColor);
    glLightfv(light, GL_POSITION, position);

	// Setup the Light Attenuation (how the light intensity is depending on distance)
	// Formula to work this out is : attenuation factor = 1 / (GL_CONSTANT_ATTENUATION + GL_LINEAR_ATTENUATION * d + GL_QUADRATIC_ATTENUATION * d²)
	// Default values are constant = 1.0, linear and quadratic = 0.0
	glLightf(light, GL_CONSTANT_ATTENUATION, constant_att);
	glLightf(light, GL_LINEAR_ATTENUATION, linear_att);
	glLightf(light, GL_QUADRATIC_ATTENUATION, quadratic_att);

	// IF Spot light is enabled Render that too
	if(isSpotlight) {
		glLightf(light, GL_SPOT_CUTOFF, cutoff);
		//glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDir);
		glLightf(GL_LIGHT1,  GL_SPOT_EXPONENT, exponent);
	}
}

