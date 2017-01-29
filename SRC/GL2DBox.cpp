/*
This class simply creates a opengl 2D box in orthogonal view to be used in things like the HUD

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "GL2DBox.h"
#include "GameUtil.h"
#include "Graphics.h"

#include "Matrix4.h"

/* Constructor */
GL2DBox::GL2DBox()
{
	/* Initialise the values */
	x = y = width = height = pitch = roll = yaw = 0.0f;
	scale = r = g = b = a = 1.0f;
	texture = NULL;
	RollAxis = Vec3(0, 0, 0);
	m = Matrix4::IDENTITY;
}

/* Destructor */
GL2DBox::~GL2DBox()
{}

/* Initialise the width, height, xPosition and YPosition of the box */
void GL2DBox::Init(float Width, float Height, float X, float Y)
{
	width = Width;
	height = Height;
	x = X;
	y = Y;
}
/* Set the Position of this box */
void GL2DBox::SetPosition(float X, float Y)
{x = X; y = Y;}

/* Set the Color of this box, including the alpha channel */
void GL2DBox::SetColor(float _r, float _g, float _b, float _a)
{r = _r; g = _g; b = _b; a = _a;}

/* Set the scale of this box */
void GL2DBox::SetScale(float Scale)
{scale = Scale;}

/* Set The texture to be used by this box */ 
void GL2DBox::SetTexture(unsigned int Texture)
{texture = Texture;}

/* Set the width of this box */
void GL2DBox::SetWidth(float Width)
{width = Width;}

/* Set the pitch rotation value */ 
void GL2DBox::SetPitch(float Pitch)
{pitch = Pitch;}

/* Set the Yaw rotation value */ 
void GL2DBox::SetYaw(float Yaw)
{yaw = Yaw;}

/* Set the Roll rotation value */ 
void GL2DBox::SetRoll(float Roll)
{roll = Roll;}

/* Render the box */
void GL2DBox::Render()
{
	// Disable Lighting and GL2DTexturing (for now)
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);

	// Set Up Orthographic View
	Graphics::GetInstance()->SetOrthoView(0, WINDOW_WIDTH, 0, WINDOW_HEIGHT);
	// Store current Matrix
	Graphics::GetInstance()->StoreMatrix();

	// If a texture is specifed enable 2D texture 
	if(texture != NULL)
		glEnable(GL_TEXTURE_2D);

	// Translate, Color and scale the 2D box
	glLoadName(v);
	Graphics::GetInstance()->Translate(x, y, 0);
	Graphics::GetInstance()->RotateX(pitch);
	Graphics::GetInstance()->RotateY(yaw);
	Graphics::GetInstance()->RotateZ(roll);
	Graphics::GetInstance()->Color4(r, g, b, a);
	Graphics::GetInstance()->Scale(scale);

	if(m != Matrix4::IDENTITY)
	{
		m[3][0] = 0.0f;
		m[3][1] = 0.0f;
		m[3][2] = 0.0f;
			// Multiply the matrix with the current matrix in the stack to apply the rotations
		glMultMatrixf(&m[0][0]);
	}
	// If a texture is specified Bind the texture to this box
	if(texture !=NULL)
		Graphics::GetInstance()->Bind2DTexture(texture);

	// Begin Drawing the 2D box
	if(texture !=NULL)
		Graphics::GetInstance()->Draw2DQuad(0, width, -height, height, 0, 1, 0, 1);
	else
		Graphics::GetInstance()->DrawQuad(width, height, 0);

	// If a texture is specified disable the texture
	if(texture !=NULL)
		glDisable(GL_TEXTURE_2D);

	//Release Current Matrix
	Graphics::GetInstance()->ReleaseMatrix();
	Graphics::GetInstance()->SetPerspective();
	// Reset back to Perspective

	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
}
