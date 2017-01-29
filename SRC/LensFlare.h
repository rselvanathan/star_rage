#ifndef __LENSFLARE_H__
#define __LENSFLARE_H__

#include "GameUtil.h"

class Frustum;
class Spaceship;
class Texture;

class LensFlare
{
public:
	LensFlare(SmartPtr<Spaceship> c, SmartPtr<Frustum> f);
	~LensFlare();

	void Update(float dt);

	void RenderBigGlow(float r, float g, float b, float a, Vec3 p, float scale); 
	void RenderStreaks(float r, float g, float b, float a, Vec3 p, float scale);
	void RenderGlow(float r, float g, float b, float a, Vec3 p, float scale);
	void RenderHalo(float r, float g, float b, float a, Vec3 p, float scale);
	void Render();

	void SetPosition(Vec3 pos) {mPosition = pos;}

	bool IsOccluded(Vec3 point);

private:
	Vec3 mPosition;
	Vec3 mCameraPosition;
	Vec3 mCameraViewDirection;
	Vec3 mLightSorceToCamera;

	Vec3 CameraIntersectPoint;
	Vec3 LightSourceToIntersect;

	SmartPtr<Spaceship> p_Camera;
	SmartPtr<Frustum> p_Frustum;

	SmartPtr<Texture> mBigGlow;
	SmartPtr<Texture> mStreaks;
	SmartPtr<Texture> mHardGlow;
	SmartPtr<Texture> mHalo;
};

#endif