/*
The Screen Decal Blood splatter effect.

Author : Romesh Selvanathan
Date : 2/04/12
*/

#ifndef __DECAL_BLOOD_H__
#define __DECAL_BLOOD_H__

#include "ScreenDecal.h"
#include "PreLoader.h"
#include "Texture.h"

class Decal_Blood : public ScreenDecal
{
public:
	Decal_Blood(float timeTillDead, float alpha) 
		: ScreenDecal(timeTillDead, alpha)
	{
		// Simply set the texture for this decal
		SetTexture(PreLoader::GetInstance()->decal_blood->GetTGATexture().texID);
	}
};

#endif