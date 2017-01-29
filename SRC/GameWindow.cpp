/*
This class sets up a basic OpenGL window

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#include "GameWindow.h"
#include "GameUtil.h"

#include "InputManager.h"
#include "Window.h"

GameWindow* GameWindow::mInstance;

/** Destructor */
GameWindow::~GameWindow(){
	SAFE_DELETE(mDisplay);
	SAFE_DELETE(mWindow);
}

/* Initialize the GameWindow */ 
//void GameWindow::Init(HWND hwnd, HINSTANCE hInstance)
bool GameWindow::Init(HINSTANCE hinstance, bool fullscreenFlag)
{
	//mDisplay = new GameDisplay(800, 600);
	mWindow = new Window(this);
	fullscreen = fullscreenFlag;
	hInstance = hinstance;

	try	{
		if(fullscreen){
			hWnd = mWindow->CreateWindowGL("Star Rage",WINDOW_WIDTH,WINDOW_HEIGHT,32,fullscreen,hInstance);
		}
		else {
			hWnd = mWindow->CreateWindowGL("Star Rage",WINDOW_WIDTH,WINDOW_HEIGHT,32,fullscreen,hInstance);
		}

		GetClientRect(hWnd, &globalRect);
		InitOpenGL(globalRect.right, globalRect.bottom);
	} catch (MyException &e) {
		// if error show a messagebox with the error then kill an close the program
		MessageBox(NULL,e.GetReason(),"ERROR",MB_OK|MB_ICONEXCLAMATION);
		KillGLWindow();								// Reset The Display
		PostQuitMessage(0);
	}

	//glEnable(GL_TEXTURE_2D);
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Red Background
	glClearDepth(1.0f);									// Depth Buffer Setup
	glEnable(GL_DEPTH_TEST);							// Enables Depth Testing
	glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);				// Really Nice Point Smoothing	*/

	return true;
}


void GameWindow::InitOpenGL(int width, int height)
{
	if (!(hDC=GetDC(hWnd)))							// Did We Get A Device Context?
		throw MyException("Can't Create A GL Device Context.");
	if(!SetupPixelFormat())
		PostQuitMessage(0);
	if (!(hRC=wglCreateContext(hDC)))				// Are We Able To Get A Rendering Context?
		throw MyException("Can't Create A GL Rendering Context.");
	if(!wglMakeCurrent(hDC,hRC))					// Try To Activate The Rendering Context
		throw MyException("Can't Activate The GL Rendering Context.");

	ReSizeGLScene(width, height);
}

bool GameWindow::SetupPixelFormat()
{
	GLuint		PixelFormat;			// Holds The Results After Searching For A Match
	PIXELFORMATDESCRIPTOR pfd;			// pfd Tells Windows How We Want Things To Be

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);			// Set the size of the structure
    pfd.nVersion = 1;									// Always set this to 1
														// Pass in the appropriate OpenGL flags
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER; 
    pfd.dwLayerMask = PFD_MAIN_PLANE;					// We want the standard mask (this is ignored anyway)
    pfd.iPixelType = PFD_TYPE_RGBA;						// We want RGB and Alpha pixel type
    pfd.cColorBits = 32;						// Here we use our #define for the color bits
    pfd.cDepthBits = 32;						// Depthbits is ignored for RGBA, but we do it anyway
    pfd.cAccumBits = 0;						// Number of bits for the accumulation buffer
    pfd.cStencilBits = 0;						// We desire no stencil bits

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Did Windows Find A Matching Pixel Format?
		throw MyException("Can't Find A Suitable PixelFormat.");
	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// Are We Able To Set The Pixel Format?
		throw MyException("Can't Set The PixelFormat.");

	return true;
}

void GameWindow::ChangeToFullscreen(int width, int height, int pixels)
{
	DEVMODE dmSettings;									// Device Mode variable

	memset(&dmSettings,0,sizeof(dmSettings));			// Makes Sure Memory's Cleared
	dmSettings.dmSize=sizeof(dmSettings);				// Size Of The Devmode Structure
	dmSettings.dmPelsWidth	= width;					// Selected Screen Width
	dmSettings.dmPelsHeight	= height;					// Selected Screen Height
	dmSettings.dmBitsPerPel = pixels;					// Selected Screen Pixels
	dmSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

	// Try To Set Selected Mode.  NOTE: CDS_FULLSCREEN Gets Rid Of Start Bar.
	if (ChangeDisplaySettings(&dmSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		throw MyException("The Requested Fullscreen Mode Is Not Supported By\nYour Video Card.");
}

void GameWindow::ReSizeGLScene(GLsizei width, GLsizei height)
{
	if (height==0)										// Prevent A Divide By Zero By
	{
		height=1;										// Making Height Equal One
	}

	glViewport(0,0,width,height);						// Reset The Current Viewport

	glMatrixMode(GL_PROJECTION);						// Select The Projection Matrix
	glLoadIdentity();									// Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(55.0f,(GLfloat)width/(GLfloat)height,0.1f,7000.0f);

	glMatrixMode(GL_MODELVIEW);							// Select The Modelview Matrix
	glLoadIdentity();									// Reset The Modelview Matrix
}

void GameWindow::KillGLWindow()
{
	if(fullscreen)	{									// Are We In Fullscreen Mode?
		ChangeDisplaySettings(NULL,0);					// If So Switch Back To The Desktop
		ShowCursor(TRUE);								// Show Mouse Pointer
	} 
	if (hRC)											// Do We Have A Rendering Context?
	{
		if (!wglMakeCurrent(NULL,NULL))					// Are We Able To Release The DC And RC Contexts?
			MessageBox(NULL,"Release Of DC And RC Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		if (!wglDeleteContext(hRC))						// Are We Able To Delete The RC?
			MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);

		hRC=NULL;										// Set RC To NULL
	}
	if (hDC && !ReleaseDC(hWnd,hDC))					// Are We Able To Release The DC
	{
		MessageBox(NULL,"Release Device Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// Set DC To NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// Are We Able To Destroy The Window?
	{
		MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// Set hWnd To NULL
	}

	if (!UnregisterClass("Star Rage",hInstance))			// Are We Able To Unregister Class
	{
		MessageBox(NULL,"Could Not Unregister Class.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// Set hInstance To NULL
	}
}