/*
This class simply creates a opengl 2D box in orthogonal view to be used in things like the HUD

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#ifndef __GL2DBOX_H__
#define __GL2DBOX_H__

#include "Matrix4.h"
#include "DirectXVector.h"

class GL2DBox
{
public:
	/* Constructor */
	GL2DBox();

	/* Destructor */
	~GL2DBox();

	/* Initialise the width height and length of the box */
	void Init(float Width, float Height, float X, float Y);
	/* Set The texture to be used by this box */ 
	void SetTexture(unsigned int Texture);
	/* Set the Position of this box */
	void SetPosition(float X, float Y);
	/* Set the Color of this box, including the alpha channel */
	void SetColor(float _r, float _g, float _b, float _a);
	/* Set the scale of this box */
	void SetScale(float Scale);
	/* Set the pitch rotation value */ 
	void SetPitch(float Pitch);
	/* Set the yaw rotation value */ 
	void SetYaw(float Yaw);
	/* Set the roll rotation value */ 
	void SetRoll(float Roll);
	void SetRoll(float Roll, Vec3 RollAxiss)
	{roll = Roll; RollAxis = RollAxiss;}
	/* Set the width and height of the ball */
	void SetWidth(float Width);
	void SetMatrix(Matrix4 ma) {m = ma;}
	void SetV(int f){v = f;}

	/* Get Methods */
	float GetWidth() {return width;}
	float GetHeight() {return height;}
	float GetXPosition() {return x;}
	float GetYPosition() {return y;}
	/* Render the box */
	void Render();
private:
	float width, height;			// width and height of the box
	float x, y;						// x and y position of the box
	float r, g, b, a;				// the 3 color components as well as the alpha value
	float scale;					// scale of the box
	float pitch, yaw, roll;			// Rotation values
	Vec3 RollAxis;
	Matrix4 m;
	int v;
	unsigned int texture;			// the texture assigned to the box
};


#endif