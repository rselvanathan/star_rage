#include "GameUtil.h"
#include "MenuEntry.h"
#include "GL2DBox.h"
#include "InputManager.h"

MenuEntry::MenuEntry(char* name, unsigned int texture, float positionx, float positiony)
{
	// Setup both the boxes
	box.Init(100, 50, positionx, positiony);
	box.SetTexture(texture);
	boxGlow.Init(100, 50, positionx, positiony);
	boxGlow.SetTexture(texture);
	m_glow = false;

	m_name = name;

	m_fMousePosX = m_fMousePosY = 0.0f;
}

MenuEntry::~MenuEntry()
{
}

void MenuEntry::Update()
{
	// First Obtain the screen resolution, by obtaining the desktop screen size
	RECT Desktop;
	HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &Desktop);
	// Update the box by checking where mouse x and  y positions are 
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
	if(mouse_x >= box.GetXPosition() && mouse_x <= box.GetWidth() + box.GetXPosition()
		&& mouse_y >= box.GetYPosition() - box.GetHeight() && mouse_y < box.GetHeight()+ box.GetYPosition())
		m_glow = true;
	else m_glow = false;
}

const bool MenuEntry::IsClicked()
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
	bool clicked = false;
	if(mouse_x >= box.GetXPosition() && mouse_x <= box.GetWidth() + box.GetXPosition()
		&& mouse_y >= box.GetYPosition() - box.GetHeight() && mouse_y < box.GetHeight()+ box.GetYPosition())
		clicked = true;

	return clicked;
}

void MenuEntry::Render()
{
	// Use Additive Blending to show the Glow Effect. - Where dest = GL_ONE
	Graphics::GetInstance()->DisableDepthTest();
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	box.Render();
	if(m_glow)
		boxGlow.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();
}