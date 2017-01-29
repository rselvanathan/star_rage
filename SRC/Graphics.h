/*
This is wrapper class around OpenGL which wraps the most frequently and most common Opengl API calls.
This means that if I do choose to switch to DirectX for example all I have to do is change these methods 
without changing the whole game by a Lot.

This class will be global singleton class as I do not want to allow object to contain this. Neither do I want
to create pointers of this class as it will cause more errors. Moreover it will also increase in memory
where most classes will use this to access the OpenGL commands. So keeping it a global singleton where only
one of this object ever exists and any class can access it is the best way to come around that problem.

Author : Romesh Selavanthan

Referenced from previous project : Escape - Advanced Games Technology
*/

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "GameUtil.h"

class Graphics
{
public:
	/* Initialise the Singleton */
	static Graphics* GetInstance(){
		if(mInstance == NULL)
			mInstance = new Graphics();
		return mInstance;
	}

	/* Clears The Screen (Depth Buffer and Color) */
	void ClearScreen();

	/* Set up a Orthographic View */
	void SetOrthoView(int left, int right, int top, int bottom);
	/* Set up a Perspective View */
	void SetPerspective();

	bool Project3DToScreen(Vec3& objectPosition, double* modelview, double* projection, int* viewport, double &winx, double &winy, double &winz);

	//-------------------------------Get Functions--------------------------------------------------
	void GetProjectionMatrix(double* projection) {glGetDoublev(GL_PROJECTION_MATRIX, projection);}
	void GetModelViewMatrix(double* modelview) {glGetDoublev(GL_MODELVIEW_MATRIX, modelview);}
	void GetViewport(int* viewport) {glGetIntegerv(GL_VIEWPORT, viewport);}
	//----------------------------------------------------------------------------------------------

	//-------------------------------Manipulating the Matrix Stack----------------------------------

	/* Store the Current Matrix in the stack */
	void StoreMatrix(){glPushMatrix();}
	/* Reset the Last matrix, by popping current matrix from the Stack */
	void ReleaseMatrix(){glPopMatrix();}
	/* Reset the matrix by Loading in the Identity Matrix */
	void ResetMatrix(){glLoadIdentity();}
	/* Multiply to the current Matrix Stack */
	void MultiplyToMatrix(Matrix4 matrix){glMultMatrixf(&matrix[0][0]);}
	/* Load the matrix on the the OpenGL Stack */ 
	void LoadMatrix(Matrix4 matrix) {glLoadMatrixf(&matrix[0][0]);}
	/* Set the current Matrix Mode to Modelview matrix mode */
	void CallModelViewMatrix() {glMatrixMode(GL_MODELVIEW);}
	/* Set the current Matrix Mode to Projection matrix mode */
	void CallProjectionMatrix() {glMatrixMode(GL_PROJECTION);}

	//---------------------------------------------------------------------------------------------

	//--------------------------------------Usual OpenGL Calls-------------------------------------

	/* Translate Call */
	void Translate(float x, float y, float z){glTranslatef(x, y, z);}
	/* Rotate in X axis */
	void RotateX(float angle){glRotatef(angle, 1, 0, 0);}
	/* Rotate in Y axis */
	void RotateY(float angle){glRotatef(angle, 0, 1, 0);}
	/* Rotate in Z axis */
	void RotateZ(float angle){glRotatef(angle, 0, 0, 1);}
	/* Set Color */
	void Color3(float r, float g, float b) {glColor3f(r, g, b);}
	/* Set Color including the alpha channel */
	void Color4(float r, float g, float b, float a){glColor4f(r, g, b, a);}
	/* Scale an object , if you wish in an uneven way */
	void Scale(float x, float y, float z){glScalef(x, y, z);}
	/* Scale an object where all sides are scaled in same way*/
	void Scale(float scale){glScalef(scale, scale, scale);}
	/* Bind A 2D Texture */
	void Bind2DTexture(unsigned int texture) {glBindTexture(GL_TEXTURE_2D, texture);}

	//---------------------------------------------------------------------------------------------

	//------------------------------------Drawing Calls-------------------------------------------

	/* Draw a simple Quad - only used by one class for now */
	void DrawQuad(float width, float height, float length);
	/* Draw a 2D Quad */
	void Draw2DQuad(float minX, float maxX, float minY, float maxY, 
		float texMinX, float texMaxX, float texMinY, float texMaxY);
	/* Begin Drawing */
	void StarDraw(GLenum type) {glBegin(type);}
	void Vertex3(float x, float y, float z) {glVertex3f(x, y, z);}
	void EndDraw() {glEnd();}

	//---------------------------------------------------------------------------------------------

	//-----------------------------------Blending-------------------------------------------
	void Blend_Masking();
	// Allow additive bledning using the Source Alpha
	void AdditiveBlend_Src_Alpha();
	// Allow additive bledning using the Destination Alpha
	void AdditiveBlend_Dest_Alpha();
	// Blending From Source Alpha to 1 minus Src Color
	void Blend_SRCWithOne_MinusSRCColor();
	// Blending From Source Alpha to 1 minus Src Alpha
	void Blend_SRCWithOne_MinusSRCAlpha();
	// Disable blending
	void DisableBlend();
	//--------------------------------------------------------------------------------------

	//----------------------------------Enabling and Disabling States------------------------

	// Enable the Depth Mask so the depth test can continue as normal
	void AllowDepthMask() {glDepthMask(GL_TRUE);}
	// Disable Depth Mask which will allow switch off dpeth test for a rendering call
	void DisableDepthMask() {glDepthMask(GL_FALSE);}

	void AllowDepthTest() {glEnable(GL_DEPTH_TEST);}
	void DisableDepthTest(){glDisable(GL_DEPTH_TEST);}

	// Enable Back Face Culling
	void AllowBackFaceCull() {glEnable(GL_CULL_FACE);}
	// Disable Back Face Culling
	void DisableBackFaceCull(){glDisable(GL_CULL_FACE);}

	// Enable Lighting as a whole
	void EnableLighting() {glEnable(GL_LIGHTING);}
	// Disable Lighting as a whole
	void DisableLighting() {glDisable(GL_LIGHTING);}

	// Enable Textures 
	void Enable2DTextures() {glEnable(GL_TEXTURE_2D);}
	// Disable Textures
	void Disable2DTextures() {glDisable(GL_TEXTURE_2D);}

	//--------------------------------------------------------------------------------------

private:
	Graphics(){}
	static Graphics* mInstance;
};

#endif