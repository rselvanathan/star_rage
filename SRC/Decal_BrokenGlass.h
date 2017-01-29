/*
The Screen Decal Blood splatter effect with a broken glass.

Author : Romesh Selvanathan
Date : 2/04/12
*/

#ifndef __DECAL_BROKENGLASS_H__
#define __DECAL_BROKENGLASS_H__

#include "ScreenDecal.h"
#include "PreLoader.h"
#include "Texture.h"

class Decal_BrokenGlass : public ScreenDecal
{
public:
	Decal_BrokenGlass(float timeTillDead, float alpha) 
		: ScreenDecal(timeTillDead, alpha)
	{
		// Simply set the texture for this decal
		SetTexture(PreLoader::GetInstance()->decal_glass->GetTGATexture().texID);
	}
};

#endif