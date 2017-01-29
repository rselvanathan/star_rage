#include "KeybindEntry.h"
#include "InputManager.h"
#include "GLText.h"

//----------------------------------------------------------------------------------------------------------------------
// Constructor for Action
//----------------------------------------------------------------------------------------------------------------------
KeybindEntry::KeybindEntry(std::string& action_name, std::string& button_name, unsigned int texture, unsigned int mask_texture, float posX, float posY, Action action)
{
	// Set the names
	m_sAction = action_name;
	m_sButton = button_name;

	// Set the action
	c_Action = action;
	m_bIsAction = true;
	// Set the state to a insane value
	c_State = static_cast<Button_State>(9999);

	m_fPosX = posX;
	m_fPosY = posY;
	m_bGlow = false;

	m_fMousePosX = m_fMousePosY = 0.0f;

	// Create the image boxes (all have to be same size and positioned in the same place)
	c_Box.Init(900, 300, m_fPosX, m_fPosY);
	c_Box.SetTexture(texture);

	c_BoxGlow.Init(900, 300, m_fPosX, m_fPosY);
	c_BoxGlow.SetTexture(texture);

	c_BoxMask.Init(900, 300, m_fPosX, m_fPosY);
	c_BoxMask.SetTexture(mask_texture);

	// Create a GL Text object
	p_Text = new GLText();
	p_Text->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
}

//----------------------------------------------------------------------------------------------------------------------
// Constructor for State
//----------------------------------------------------------------------------------------------------------------------
KeybindEntry::KeybindEntry(std::string& state_name, std::string& button_name, unsigned int texture, unsigned int mask_texture, float posX, float posY, Button_State state)
{
	// Set the names
	m_sAction = state_name;
	m_sButton = button_name;

	// Set the action to a insane value
	c_Action = static_cast<Action>(9999);;
	m_bIsAction = false;
	// Set the state 
	c_State = state;

	m_fPosX = posX;
	m_fPosY = posY;
	m_bGlow = false;

	m_fMousePosX = m_fMousePosY = 0.0f;

	// Create the image boxes (all have to be same size and positioned in the same place)
	c_Box.Init(900, 300, m_fPosX, m_fPosY);
	c_Box.SetTexture(texture);

	c_BoxGlow.Init(900, 300, m_fPosX, m_fPosY);
	c_BoxGlow.SetTexture(texture);

	c_BoxMask.Init(900, 300, m_fPosX, m_fPosY);
	c_BoxMask.SetTexture(mask_texture);

	// Create a GL Text object
	p_Text = new GLText();
	p_Text->CreateText(-24, 0, 0, 0, FW_BOLD, false, false, false, ANSI_CHARSET, OUT_TT_PRECIS,
				CLIP_DEFAULT_PRECIS, FF_DONTCARE|DEFAULT_PITCH, "Courier New");
}

//----------------------------------------------------------------------------------------------------------------------
// Destructor
//----------------------------------------------------------------------------------------------------------------------
KeybindEntry::~KeybindEntry()
{
	SAFE_DELETE(p_Text);
}

void KeybindEntry::Update()
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
	if(mouse_x >= c_Box.GetXPosition() + 102 && mouse_x <= c_Box.GetWidth() + (c_Box.GetXPosition() - 102) 
		&& mouse_y >= c_Box.GetYPosition() - 25 && mouse_y <  c_Box.GetYPosition() + 74)
		m_bGlow = true;
	else m_bGlow = false;
}

const bool KeybindEntry::IsClicked()
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
	if(mouse_x >= c_Box.GetXPosition() + 102 && mouse_x <= c_Box.GetWidth() + (c_Box.GetXPosition() - 102) 
		&& mouse_y >= c_Box.GetYPosition() - 25 && mouse_y <  c_Box.GetYPosition() + 74)
		return true;
	else return false;
}

void KeybindEntry::Render()
{
	// Render the different masks
	Graphics::GetInstance()->Blend_Masking();
	Graphics::GetInstance()->DisableDepthTest();

	c_BoxMask.Render();

	// Use Additive Blending to show the Glow Effect. - Where dest = GL_ONE
	Graphics::GetInstance()->AdditiveBlend_Src_Alpha();

	c_Box.Render();
	if(m_bGlow)
		c_BoxGlow.Render();

	Graphics::GetInstance()->AllowDepthTest();
	Graphics::GetInstance()->DisableBlend();

	p_Text->SetColor3f(1, 1, 1);
	p_Text->SetPosition(m_fPosX + 50, m_fPosY + 10);
	p_Text->print("%20s        - %18s",m_sAction.c_str(), m_sButton.c_str());
}