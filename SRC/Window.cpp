/*
This class sets up the window itself with the appropriate properties and, listens to the Windows Call back messages

Author : Romesh Selvanathan
*/

#include "Window.h"
#include "GameWindow.h"
#include "InputManager.h"
#include "GameUtil.h"

Window::Window(GameWindow* mGame)
{
	game = mGame;
}

Window::~Window()
{}

HWND Window::CreateWindowGL(const char* title, int width, int height, int bits, bool fullscreenflag, HINSTANCE hInstance)
{
	HWND		hwnd;
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values
	WindowRect.left=(long)0;			// Set Left Value To 0
	WindowRect.right=(long)width;		// Set Right Value To Requested Width
	WindowRect.top=(long)0;				// Set Top Value To 0
	WindowRect.bottom=(long)height;		// Set Bottom Value To Requested Height

	hinstance			= hInstance;				// Grab An Instance For Our Window
	memset(&wc, 0, sizeof(WNDCLASS));							// Init the size of the class(so that the other variables of wc are set to 0/NULL)
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// Redraw On Size, And Own DC For Window.
	wc.lpfnWndProc		= (WNDPROC)  WndProc;		// WndProc Handles Messages
	wc.hInstance		= hinstance;							// Set The Instance
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// Load The Default Icon
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// Load The Arrow Pointer
	wc.lpszClassName	= "Star Rage";								// Set The Class Name

	if (!RegisterClass(&wc))									// Attempt To Register The Window Class
		throw MyException("Failed To Register The Window Class.");

	if (fullscreenflag)												// Attempt Fullscreen Mode?
	{
		dwExStyle=WS_EX_APPWINDOW;								// Window Extended Style
		dwStyle=WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;	// Windows Style
		game->ChangeToFullscreen(width, height, bits);
		ShowCursor(FALSE);										// Hide Mouse Pointer
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// Window Extended Style
		dwStyle=WS_OVERLAPPED | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;// Windows Style
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// Adjust Window To True Requested Size

	// Create The Window
	if (!(hwnd=CreateWindowEx(	dwExStyle,							// Extended Style For The Window
								"Star Rage",								// Class Name
								title,								// Window Title
								dwStyle, 							// Defined Window Style
								0, 0,								// Window Position
								WindowRect.right-WindowRect.left,	// Calculate Window Width
								WindowRect.bottom-WindowRect.top,	// Calculate Window Height
								NULL,								// No Parent Window
								NULL,								// No Menu
								hinstance,							// Instance
								NULL)))								// Dont Pass Anything To WM_CREATE
	{	throw MyException("Window Creation Error."); }

	ShowWindow(hwnd,SW_SHOW);						// Show The Window
	SetForegroundWindow(hwnd);						// Slightly Higher Priority
	SetFocus(hwnd);									// Sets Keyboard Focus To The Window

	return hwnd;									// Success
}


LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		//case WM_KEYDOWN:
		//	switch(wParam) {								// Check if we hit a key
		//	case VK_ESCAPE:								// If we hit the escape key
		//		PostQuitMessage(0);						// Send a QUIT message to the window
		//		break;
		//	}
		//	break;
		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_SIZE:								// Resize The OpenGL Window
		{
			GameWindow::GetInstance()->ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width, HiWord=Height
			return 0;								// Jump Back
		}

		// By Default give message to InputHandler
		default:
			InputManager::Instance()->HandleMessage(uMsg, wParam, lParam);
			break;
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

