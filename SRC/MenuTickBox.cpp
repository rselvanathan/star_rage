/*
This class will allow the user to Tick a box to enable a setting

Author : Romesh Selvanathan

Date : 10/12
*/

#include "MenuTickBox.h"
#include "PreLoader.h"
#include "Texture.h"

//----------------------------------------------------------------------------------------------------------------------
// Constructor
//----------------------------------------------------------------------------------------------------------------------
MenuTickBox::MenuTickBox(float posX, float posY)
{
	m_fPosX = posX;
	m_fPosY = posY;
	m_bGlow = false;
	m_bTicked = false;

	m_fMousePosX = m_fMousePosY = 0.0f;

	// Create the image boxes (all have to be same size and positioned in the same place)
	c_Box.Init(100, 50, m_fPosX, m_fPosY);
	c_Box.SetTexture(PreLoader::GetInstance()->tickbox_unticked->GetTGATexture().texID);

	c_BoxGlow.Init(100, 50, m_fPosX, m_fPosY);
	c_BoxGlow.SetTexture(PreLoader::GetInstance()->tickbox_unticked->GetTGATexture().texID);

	c_BoxMask.Init(100, 50, m_fPosX, m_fPosY);
	c_BoxMask.SetTexture(PreLoader::GetInstance()->tickbox_mask->GetTGATexture().texID);

	c_Box_ticked.Init(100, 50, m_fPosX, m_fPosY);
	c_Box_ticked.SetTexture(PreLoader::GetInstance()->tickbox_ticked->GetTGATexture().texID);

	c_Box_tickedGlow.Init(100, 50, m_fPosX, m_fPosY);
	c_Box_tickedGlow.SetTexture(PreLoader::GetInstance()->tickbox_ticked->GetTGATexture().texID);
}

void MenuTickBox::Update()
{
	// First Obtain the screen resolution, by obtaining the desktop screen size
	RECT Desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &Desktop);
	// Update the box by checking where mouse x and y positions are 
	// First obtain the ratio for mouse position conversion dependant upon the current screen resolution. If
	// the current screen size is larger than intended screen size then current/original otherwise, original/current. The original
	// means the original screen size this application was developed for and current being the current screen resolution and size.
	float yDiff;
	float xDiff;
	
	if((float)Desktop.bottom < WINDOW_HEIGHT)
		yDiff = ( 1024.0f/(float)Desktop.bottom);
	else
		yDiff = ( (float)Desktop.bottom/1024.0f);

	// for x difference the calculation is only applied when the current screen size if less than the intended, as otherwise the width will not changed
	// and affect the position itself whereas when the current screen size is less, the mouse position will have a different value
	if((float)Desktop.right < WINDOW_WIDTH)
		xDiff = ( WINDOW_WIDTH/(float)Desktop.right);
	else
		xDiff = 1.0f;

	float mouse_x, mouse_y;
	// Convert the mouse Y Position depending on the ratio of the current screen. This has to be done so that the position is
	// read almost equally across all different resolutions. There is some precision loss, however its quite minimal. 
	// Mouse Y has to be turned upside down as opengl and windows take the coordinates the other way around from
	// how we normally interpret them.
	// Then check if the mouse position is within the box and if it is, allow the glow effect from this entry
	mouse_x = m_fMousePosX * xDiff;
	mouse_y = WINDOW_HEIGHT - (m_fMousePosY * yDiff);
	// Values that are being added/subtracted for both x and y are worked out through trial and error
	// in order to get the area of collision perfect. They are done this way as the textures are all 1024 by 1024
	// and so they have a lot of empty space in them, making it difficult to determine where exactly the actual
	// pixels necessary are within the picture as a whole.
	if(mouse_x >= c_Box.GetXPosition() && mouse_x <= c_Box.GetWidth() + c_Box.GetXPosition()
		&& mouse_y >= c_Box.GetYPosition() - c_Box.GetHeight() && mouse_y < c_Box.GetHeight()+ c_Box.GetYPosition())
		m_bGlow = true;
	else m_bGlow = false;
}

bool MenuTickBox::IsClicked()
{
	// First Obtain the screen resolution, by obtaining the desktop screen size
	RECT Desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &Desktop);
	// Update the box by checking where mouse x and y positions are 
	// First obtain the ratio for mouse position conversion dependant upon the current screen resolution. If
	// the current screen size is larger than intended screen size then current/original otherwise, original/current. The original
	// means the original screen size this application was developed for and current being the current screen resolution and size.
	float yDiff;
	float xDiff;
	
	if((float)Desktop.bottom < WINDOW_HEIGHT)
		yDiff = ( 1024.0f/(float)Desktop.bottom);
	else
		yDiff = ( (float)Desktop.bottom/1024.0f);

	// for x difference the calculation is only applied when the current screen size if less than the intended, as otherwise the width will not changed
	// and affect the position itself whereas when the current screen size is less, the mouse position will have a different value
	if((float)Desktop.right < WINDOW_WIDTH)
		xDiff = ( WINDOW_WIDTH/(float)Desktop.right);
	else
		xDiff = 1.0f;

	float mouse_x, mouse_y;
	// Convert the mouse Y Position depending on the ratio of the current screen. This has to be done so that the position is
	// read almost equally across all different resolutions. There is some precision loss, however its quite minimal. 
	// Mouse Y has to be turned upside down as opengl and windows take the coordinates the other way around from
	// how we normally interpret them.
	// Then check if the mouse position is within the box and if it is, allow the glow effect from this entry
	mouse_x = m_fMousePosX * xDiff;
	mouse_y = WINDOW_HEIGHT - (m_fMousePosY * yDiff);
	// Values that are being added/subtracted for both x and y are worked out through trial and error
	// in order to get the area of collision perfect. They are done this way as the textures are all 1024 by 1024
	// and so they have a lot of empty space in them, making it difficult to determine where exactly the actual
	// pixels necessary are within the picture as a whole.
	if(mouse_x >= c_Box.GetXPosition() && mouse_x <= c_Box.GetWidth() + c_Box.GetXPosition()
		&& mouse_y >= c_Box.GetYPosition() - c_Box.GetHeight() && mouse_y < c_Box.GetHeight()+ c_Box.GetYPosition())
	{
		TickBox();
		return true;
	}
	else
		return false;
}

void MenuTickBox::TickBox()
{
	if( m_bTicked)
		m_bTicked = false;
	else
		m_bTicked = true;
}

void MenuTickBox::Render()
{
	// Render the different masks
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	c_BoxMask.Render();

	// Use Additive Blending to show the Glow Effect. - Where dest = GL_ONE
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	if(!m_bTicked) {
		c_Box.Render();
		if(m_bGlow)
			c_BoxGlow.Render();
	} else {
		c_Box_ticked.Render();
		if(m_bGlow)
			c_Box_tickedGlow.Render();
	}

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();
}