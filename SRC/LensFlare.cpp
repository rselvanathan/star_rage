#include "GameUtil.h"
#include "Spaceship.h"
#include "Frustum.h"
#include "LensFlare.h"
#include "Graphics.h"
#include "Texture.h"

LensFlare::LensFlare(SmartPtr<Spaceship> c, SmartPtr<Frustum> f) :
	mPosition(Vec3(0, 0, 0)),
	mCameraPosition(Vec3(0, 0, 0)),
	mCameraViewDirection(Vec3(0, 0, 0)), 
	mLightSorceToCamera(Vec3(0, 0, 0)),
	CameraIntersectPoint(Vec3(0, 0, 0)), 
	LightSourceToIntersect(Vec3(0, 0, 0))
{
	p_Camera = c;
	p_Frustum = f;

	mBigGlow = new Texture();
	mBigGlow->LoadTGA("..//..//ASSETS//Lensflare//BigGlow.tga", Texture::MIPMAPPED);

	mStreaks = new Texture();
	mStreaks->LoadTGA("..//..//ASSETS//Lensflare//Streaks.tga", Texture::MIPMAPPED);

	mHardGlow = new Texture();
	mHardGlow->LoadTGA("..//..//ASSETS//Lensflare//HardGlow.tga", Texture::MIPMAPPED);

	mHalo = new Texture();
	mHalo->LoadTGA("..//..//ASSETS//Lensflare//Halo.tga", Texture::MIPMAPPED);
}

LensFlare::~LensFlare()
{
	SAFE_DELETE(p_Camera);
	SAFE_DELETE(p_Frustum);
}

void LensFlare::Update(float dt)
{
	mCameraPosition = p_Camera->GetPosition();
//	mCameraViewDirection = p_Camera->getForwardVector();
	float Length = 0.0f;

	// As the lens flare has been rotated 90 degrees x will giv the lens flare's z position
	mPosition.z = mCameraPosition.x - 50.0f;

	/*if(p_Frustum->SphereInFrustum(mPosition.x, mPosition.y, mPosition.z, 5.f)){
		mLightSorceToCamera = mCameraPosition - mPosition;
		Length = mLightSorceToCamera.Length();
		CameraIntersectPoint = mCameraViewDirection * Length;
		CameraIntersectPoint += mCameraPosition;
		LightSourceToIntersect = CameraIntersectPoint - mLightSorceToCamera;
		Length = LightSourceToIntersect.Length();
		LightSourceToIntersect.Normalize();
	}*/
}

void LensFlare::Render()
{
	//if(!IsOccluded(mPosition))
	//{
	float Length = 0.0f;
	Vec3 pt;

	if(p_Frustum->SphereInFrustum(mPosition.x, mPosition.y, mPosition.z, 20.f)){
		mLightSorceToCamera = mCameraPosition - mPosition;
		Length = mLightSorceToCamera.Length();
		CameraIntersectPoint = mCameraViewDirection * Length;
		CameraIntersectPoint += mCameraPosition;
		LightSourceToIntersect = CameraIntersectPoint - mLightSorceToCamera;
		Length = LightSourceToIntersect.Length();
		LightSourceToIntersect.Normalize();

		glEnable(GL_BLEND);	
		glDisable(GL_LIGHTING);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);							
		glDisable(GL_DEPTH_TEST);									
		glEnable(GL_TEXTURE_2D);								
	
		//if(!IsOccluded(mPosition))
		//{
		// Rotate all of it by 90 degrees so that is face the + x axis
			Graphics::GetInstance()->StoreMatrix();
			Graphics::GetInstance()->RotateY(90);
			RenderBigGlow(0.60f, 0.60f, 0.8f, 1.0f, mPosition, 4.0f);
			RenderStreaks(0.60f, 0.60f, 0.8f, 1.0f, mPosition, 4.0f);
			RenderGlow(0.8f, 0.8f, 1.0f, 0.5f, mPosition, 0.9f);
		
			pt = LightSourceToIntersect * (Length * 0.1f);				// Lets compute a point that is 20%
			pt += mPosition;										// away from the light source in the
																	// direction of the intersection point.
			RenderGlow(0.9f, 0.6f, 0.4f, 0.5f, pt, 0.6f);				// Render the small Glow

			pt = LightSourceToIntersect * (Length * 0.15f);			// Lets compute a point that is 30%
			pt += mPosition;							
			RenderHalo(0.8f, 0.5f, 0.6f, 0.5f, pt, 1.7f);				// Render the a Halo

			Graphics::GetInstance()->ReleaseMatrix();
			
	//	}
		glDisable(GL_BLEND );	
		glEnable(GL_LIGHTING);
		glEnable(GL_DEPTH_TEST);									
		glDisable(GL_TEXTURE_2D);									

	}
}

void LensFlare::RenderBigGlow(float r, float g, float b, float a, Vec3 p, float scale)
{
	Vec3 pos[4];

	pos[0].x = (p.x - scale);
	pos[0].y = (p.y - scale);

	pos[1].x = (p.x - scale);
	pos[1].y = (p.y + scale);

	pos[2].x = (p.x + scale);
	pos[2].y = (p.y - scale);

	pos[3].x = (p.x + scale);
	pos[3].y = (p.y + scale);

	Graphics::GetInstance()->StoreMatrix();
	//Graphics::GetInstance()->RotateY(90);
	Graphics::GetInstance()->Translate(p.x, p.y, p.z);
	Graphics::GetInstance()->RotateY(p_Camera->GetOrientation().y);
	Graphics::GetInstance()->RotateX(p_Camera->GetOrientation().x);
	Graphics::GetInstance()->Bind2DTexture(mBigGlow->GetTGATexture().texID);
	Graphics::GetInstance()->Color4(r, g, b, a);

	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0.0f, 0.0f);	glVertex2f(pos[0].x, pos[0].y);
		glTexCoord2f(0.0f, 1.0f);   glVertex2f(pos[1].x, pos[1].y);
		glTexCoord2f(1.0f, 0.0f);   glVertex2f(pos[2].x, pos[2].y);
		glTexCoord2f(1.0f, 1.0f);   glVertex2f(pos[3].x, pos[3].y);
	glEnd();

	Graphics::GetInstance()->ReleaseMatrix();
}

void LensFlare::RenderStreaks(float r, float g, float b, float a, Vec3 p, float scale)
{
	Vec3 q[4];
	
	// Basically we are just going to make a 2D box
	// from four points we don't need a z coord because
	// we are rotating the camera by the inverse so the 
	// texture mapped quads will always face us.
	q[0].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[0].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.
		
	q[1].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[1].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	q[2].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[2].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.
		
	q[3].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[3].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	Graphics::GetInstance()->StoreMatrix();
	Graphics::GetInstance()->Translate(p.x, p.y, p.z);
	//Graphics::GetInstance()->RotateY(90);
	Graphics::GetInstance()->RotateY(p_Camera->GetOrientation().y);
	Graphics::GetInstance()->RotateX(p_Camera->GetOrientation().x);
	Graphics::GetInstance()->Bind2DTexture(mStreaks->GetTGATexture().texID);
	Graphics::GetInstance()->Color4(r, g, b, a);
	
	glBegin(GL_TRIANGLE_STRIP);										// Draw the Big Glow on a Triangle Strip
		glTexCoord2f(0.0f, 0.0f);					
		glVertex2f(q[0].x, q[0].y);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(q[1].x, q[1].y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(q[2].x, q[2].y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(q[3].x, q[3].y);
	glEnd();	

	Graphics::GetInstance()->ReleaseMatrix();
}

void LensFlare::RenderGlow(float r, float g, float b, float a, Vec3 p, float scale)
{
	Vec3 q[4];
	
	// Basically we are just going to make a 2D box
	// from four points we don't need a z coord because
	// we are rotating the camera by the inverse so the 
	// texture mapped quads will always face us.
	q[0].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[0].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.

	q[1].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[1].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	q[2].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[2].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.
		
	q[3].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[3].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	Graphics::GetInstance()->StoreMatrix();
	Graphics::GetInstance()->Translate(p.x, p.y, p.z);
	//Graphics::GetInstance()->RotateY(90);
	Graphics::GetInstance()->RotateY(p_Camera->GetOrientation().y);
	Graphics::GetInstance()->RotateX(p_Camera->GetOrientation().x);
	Graphics::GetInstance()->Bind2DTexture(mHardGlow->GetTGATexture().texID);
	Graphics::GetInstance()->Color4(r, g, b, a);	// Set the color since the texture is a gray scale
	
	glBegin(GL_TRIANGLE_STRIP);										// Draw the Big Glow on a Triangle Strip
		glTexCoord2f(0.0f, 0.0f);					
		glVertex2f(q[0].x, q[0].y);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(q[1].x, q[1].y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(q[2].x, q[2].y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(q[3].x, q[3].y);
	glEnd();										
	Graphics::GetInstance()->ReleaseMatrix();
}

void LensFlare::RenderHalo(float r, float g, float b, float a, Vec3 p, float scale)
{
	Vec3 q[4];
	
	// Basically we are just going to make a 2D box
	// from four points we don't need a z coord because
	// we are rotating the camera by the inverse so the 
	// texture mapped quads will always face us.
	q[0].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[0].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.

	q[1].x = (p.x - scale);											// Set the x coordinate -scale units from the center point.
	q[1].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	q[2].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[2].y = (p.y - scale);											// Set the y coordinate -scale units from the center point.
		
	q[3].x = (p.x + scale);											// Set the x coordinate scale units from the center point.
	q[3].y = (p.y + scale);											// Set the y coordinate scale units from the center point.
		
	Graphics::GetInstance()->StoreMatrix();
	Graphics::GetInstance()->Translate(p.x, p.y, p.z);
	Graphics::GetInstance()->RotateY(p_Camera->GetOrientation().y);
	Graphics::GetInstance()->RotateX(p_Camera->GetOrientation().x);
	Graphics::GetInstance()->Bind2DTexture(mHalo->GetTGATexture().texID);
	Graphics::GetInstance()->Color4(r, g, b, a);	// Set the color since the texture is a gray scale
	
	glBegin(GL_TRIANGLE_STRIP);										// Draw the Big Glow on a Triangle Strip
		glTexCoord2f(0.0f, 0.0f);					
		glVertex2f(q[0].x, q[0].y);
		glTexCoord2f(0.0f, 1.0f);
		glVertex2f(q[1].x, q[1].y);
		glTexCoord2f(1.0f, 0.0f);
		glVertex2f(q[2].x, q[2].y);
		glTexCoord2f(1.0f, 1.0f);
		glVertex2f(q[3].x, q[3].y);
	glEnd();										
	Graphics::GetInstance()->ReleaseMatrix();
}


// Reference - http://nehe.gamedev.net/tutorial/3d_lens_flare_with_occlusion_testing/16007/
// Used for the lens flare effect
bool LensFlare::IsOccluded(Vec3 p)
{
	GLint viewport[4];                          // Space For Viewport Data
    GLdouble mvmatrix[16], projmatrix[16];                  // Space For Transform Matrix
    GLdouble winx, winy, winz;                      // Space For Returned Projected Coords
    GLdouble flareZ;                            // Here We Will Store The Transformed Flare Z
    GLfloat bufferZ;                            // Here We Will Store The Read Z From The Buffer

	// Now we will ask OGL to project some geometry for us using the gluProject function.
	// Practically we ask OGL to guess where a point in space will be projected in our current viewport,
	// using arbitrary viewport and transform matrices we pass to the function.
	// If we pass to the function the current matrices  (retrievede with the glGet funcs)
	// we will have the real position on screen where the dot will be drawn.
	// The interesting part is that we also get a Z value back, this means that 
	// reading the REAL buffer for Z values we can discover if the flare is in front or
	// if it's occluded by some objects.
 
    glGetIntegerv (GL_VIEWPORT, viewport);                  // Get Actual Viewport
    glGetDoublev (GL_MODELVIEW_MATRIX, mvmatrix);               // Get Actual Model View Matrix
    glGetDoublev (GL_PROJECTION_MATRIX, projmatrix);            // Get Actual Projection Matrix
 
    // This Asks OGL To Guess The 2D Position Of A 3D Point Inside The Viewport
    gluProject(p.x, p.y, p.z, mvmatrix, projmatrix, viewport, &winx, &winy, &winz);
    flareZ = winz;
 
    // We Read Back One Pixel From The Depth Buffer (Exactly Where Our Flare Should Be Drawn)
    glReadPixels(winx, winy,1,1,GL_DEPTH_COMPONENT, GL_FLOAT, &bufferZ);
 
    // If The Buffer Z Is Lower Than Our Flare Guessed Z Then Don't Draw
    // This Means There Is Something In Front Of Our Flare
    if (bufferZ < flareZ)
        return true;
    else
        return false;
}
