/*
This class will allow the user to scroll a box along a bar to enable a setting

Author : Romesh Selvanathan

Date : 10/12
*/

#include "MenuScrollBar.h"
#include "PreLoader.h"
#include "Texture.h"

MenuScrollBar::MenuScrollBar(float posX, float posY, float width, float initialValue)
{
	m_fPosX = posX;
	m_fPosY = posY;
	m_bGlow = false;
	clicked = false;
	m_iValue = initialValue;

	m_fMousePosX = m_fMousePosY = 0.0f;

	// Create the image boxes (all have to be same size and positioned in the same place)
	c_Box_background.Init(width, 50, m_fPosX, m_fPosY);
	c_Box_background.SetTexture(PreLoader::GetInstance()->scroll_background->GetTGATexture().texID);

	c_Box_background_left.Init(100, 50, m_fPosX + 80, m_fPosY);
	c_Box_background_left.SetTexture(PreLoader::GetInstance()->scroll_background_left->GetTGATexture().texID);

	c_Box_background_right.Init(100, 50, m_fPosX + c_Box_background.GetWidth() - 190, m_fPosY);
	c_Box_background_right.SetTexture(PreLoader::GetInstance()->scroll_background_right->GetTGATexture().texID);

	// The orb texture

	// THe width of the orb
	float orb_width = 100;
	// Add half of the width to the x position, as x is usually read from the far left of the image, and we want it in the middle
	float temp_positionx = m_fPosX - (orb_width/2);
	/* Clamps this temporary x position between the 
		 -	X position of the Background bar + half of the width of the orb image (Image position is bottom left, so with by adding 
			half the width of the image, the Image position is moved to the center of the image rather than far left)
		 - THe full width of the background bar plus the full width of the Orb Image (This will allow the orb image to travel all the way 
		   to the end of the bar*/
	Clamp(temp_positionx, c_Box_background.GetXPosition() + (orb_width/2), c_Box_background.GetWidth() + orb_width);
	// Obtain the width of the total visible bar, which is what the orb can travel between, the two values are the values used above for
	// clamping
	float total_visible_width = (c_Box_background.GetWidth() + orb_width) -  (c_Box_background.GetXPosition() + (orb_width/2));
	// Finally obtain the final x position for the Orb images, where the initial value represents a percentage, which is used to 
	// obtain what part of the width the orb should be at. THen finally position the orb according to where the user wants it on the screen (temp_positionx)
	float finalPosX = (total_visible_width  * (initialValue/100)) + temp_positionx;

	c_Box.Init(orb_width, 50, finalPosX , m_fPosY);
	c_Box.SetTexture(PreLoader::GetInstance()->scroll_orb->GetTGATexture().texID);

	c_BoxGlow.Init(orb_width, 50, finalPosX, m_fPosY);
	c_BoxGlow.SetTexture(PreLoader::GetInstance()->scroll_orb->GetTGATexture().texID);

	c_BoxMask.Init(orb_width, 50, finalPosX, m_fPosY);
	c_BoxMask.SetTexture(PreLoader::GetInstance()->scroll_orb_mask->GetTGATexture().texID);

	// THe Text box texture

	c_Box_Text.Init(100, 50, m_fPosX + (c_Box_background.GetWidth()/2.0f - 50), m_fPosY - 100);
	c_Box_Text.SetTexture(PreLoader::GetInstance()->tickbox_unticked->GetTGATexture().texID);

	c_Box_Text_Mask.Init(100, 50, m_fPosX + (c_Box_background.GetWidth()/2.0f - 50), m_fPosY - 100);
	c_Box_Text_Mask.SetTexture(PreLoader::GetInstance()->tickbox_mask->GetTGATexture().texID);

	// Setup the Text
	p_ValueText = new GLText();
	p_ValueText->CreateText(-36, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
					CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
	p_ValueText->SetColor3f(1, 1, 1);
	p_ValueText->SetPosition(m_fPosX + (c_Box_background.GetWidth()/2.0f - 20), m_fPosY - 110);
}

void MenuScrollBar::SetValue(int value)
{
	m_iValue = value;

	// Add half of the width to the x position, as x is usually read from the far left of the image, and we want it in the middle
	float temp_positionx = m_fPosX - (c_Box.GetWidth()/2);
	/* Clamps this temporary x position between the 
		 -	X position of the Background bar + half of the width of the orb image (Image position is bottom left, so with by adding 
			half the width of the image, the Image position is moved to the center of the image rather than far left)
		 - THe full width of the background bar plus the full width of the Orb Image (This will allow the orb image to travel all the way 
		   to the end of the bar*/
	Clamp(temp_positionx, c_Box_background.GetXPosition() + (c_Box.GetWidth()/2), c_Box_background.GetWidth() + c_Box.GetWidth());
	// Obtain the width of the total visible bar, which is what the orb can travel between, the two values are the values used above for
	// clamping
	float total_visible_width = (c_Box_background.GetWidth() + c_Box.GetWidth()) -  (c_Box_background.GetXPosition() + (c_Box.GetWidth()/2));
	// Finally obtain the final x position for the Orb images, where the initial value represents a percentage, which is used to 
	// obtain what part of the width the orb should be at. THen finally position the orb according to where the user wants it on the screen (temp_positionx)
	float finalPosX = (total_visible_width  * static_cast<float>(m_iValue/100.0f)) + temp_positionx;

	// Finally update all three images with new position
	c_BoxMask.SetPosition(finalPosX, m_fPosY);
	c_Box.SetPosition(finalPosX, m_fPosY);
	c_BoxGlow.SetPosition(finalPosX, m_fPosY);
}

void MenuScrollBar::Update()
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
		m_bGlow = true;
	}
	else
	{
		m_bGlow = false;
	}

	// Test it with the bar rather than the orb in order to move the orb
	if(mouse_x >= c_Box_background.GetXPosition() && mouse_x <= c_Box_background.GetWidth() + c_Box_background.GetXPosition()
		&& mouse_y >= c_Box_background.GetYPosition() - c_Box_background.GetHeight() && mouse_y < c_Box_background.GetHeight()+ c_Box_background.GetYPosition())
	{
		// Nothing
	}
	else
	{
		// If the orb has been clicked then set it as not clicked anymore once the mouse moves outside the collision box
		if(clicked)
			clicked = false;
	}

	// If the orb has been clicked the move all the images for the orb by the mouse position + half the width of the image
	// That calculation makes sure whenever the click occurs the mouse is always in the middle of the orb image and not the
	// end. Clamp the value between the start of the background bar and the width of the background bar. Also set the Value
	// that is shown to the user as a integer depending on where the orb is relative to the bar.
	if(clicked)
	{
		// First work out the Temporary x position of the bar, which is equal to the mouse position + half of the width of the image
		float temp_positionx = m_fMousePosX - (c_Box.GetWidth()/2);
		/* Clamps this temporary x position between the 
		 -	X position of the Background bar + half of the width of the orb image (Image position is bottom left, so with by adding 
			half the width of the image, the Image position is moved to the center of the image rather than far left)
		 - THe full width of the background bar plus the full width of the Orb Image (This will allow the orb image to travel all the way 
		   to the end of the bar*/
		Clamp(temp_positionx, c_Box_background.GetXPosition() + (c_Box.GetWidth()/2), c_Box_background.GetWidth() + c_Box.GetWidth());
		// Obtain the width of the total visible bar, which is what the orb can travel between, the two values are the values used above for
		// clamping
		float total_visible_width = (c_Box_background.GetWidth() + c_Box.GetWidth()) -  (c_Box_background.GetXPosition() + (c_Box.GetWidth()/2));
		// Update the value to be printed where the formula is :
		/*
		Value = ( (CurrentPosition_of_Orb - Min_Positon_of_Orb) / total_visible_width_of_bar) * 100
		This will give a percentage value based on where the orb is relative to the bar
		*/
		m_iValue =  ((c_Box.GetXPosition()- (c_Box_background.GetXPosition()+ (c_Box.GetWidth()/2))) / total_visible_width) * 100; 
		// Finally update all three images with new position
		c_BoxMask.SetPosition(temp_positionx, m_fPosY);
		c_Box.SetPosition(temp_positionx, m_fPosY);
		c_BoxGlow.SetPosition(temp_positionx, m_fPosY);
	}
}

bool MenuScrollBar::IsClicked()
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

	// Test it with the bar rather than the orb in order to move the orb
	if(mouse_x >= c_Box_background.GetXPosition() && mouse_x <= c_Box_background.GetWidth() + c_Box_background.GetXPosition()
	&& mouse_y >= c_Box_background.GetYPosition() - c_Box_background.GetHeight() && mouse_y < c_Box_background.GetHeight()+ c_Box_background.GetYPosition()) 
	{	
		// If the orb has not been clicked yet then move it otherwise stop moving it once clicked a second time
		if(!clicked)
			clicked = true;
		else
			clicked = false;
	}

	if(mouse_x >= c_Box.GetXPosition() && mouse_x <= c_Box.GetWidth() + c_Box.GetXPosition()
		&& mouse_y >= c_Box.GetYPosition() - c_Box.GetHeight() && mouse_y < c_Box.GetHeight()+ c_Box.GetYPosition()) 
	{	
		return true;
	}
	else
		return false;
}

void MenuScrollBar::Render()
{
	// Render the different masks
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	c_Box_Text_Mask.Render();

	// Use Additive Blending to show the Glow Effect. - Where dest = GL_ONE
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	c_Box_Text.Render();

	c_Box_background_left.Render();
	c_Box_background_right.Render();
	c_Box_background.Render();

	// Render the Orb by first rendering the mask
	Graphics::GetInstance()->Blend_Masking();

	c_BoxMask.Render();

	// Use Additive Blending to show the Glow Effect. - Where dest = GL_ONE
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	c_Box.Render();
	if(m_bGlow)
		c_BoxGlow.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	p_ValueText->print("%i", m_iValue);
}