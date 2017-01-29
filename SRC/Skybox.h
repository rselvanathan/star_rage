/*
Set up a basic Skybox with 6 Textures on each side, that will follow a spefcific target whenever it 
moves or rotates.

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "GameUtil.h"
class Texture;
class Spaceship;

class Skybox
{
public:
	Skybox();
	~Skybox();

	void Render(float x, float y, float z, float width, float height, float length);
private:
	SmartPtr<Texture> mTextures[6];

	float x;	
	float y;	
};

#endif