/*
This class sets up a basic OpenGL window

Author : Romesh Selvanathan

Referenced from previous project : Escape - Advanced Games Technology
*/

#ifndef __GAMEWINDOW_H__
#define __GAMEWINDOW_H__

#include "GameUtil.h"

class GameDisplay;
class Window;

class GameWindow
{
public:
	static GameWindow* GetInstance()
	{
		if(!mInstance)
			mInstance = new GameWindow();
		return mInstance;
	}
	~GameWindow();

	//void Init(HWND hwnd, HINSTANCE hInstance);
	bool Init(HINSTANCE hInstance, bool fullscreenFlag);

	void InitOpenGL(int width, int height);
	bool SetupPixelFormat();
	void KillGLWindow();
	void ChangeToFullscreen(int width, int height, int pixels);
	GLvoid ReSizeGLScene(GLsizei width, GLsizei height);

	GameDisplay* GetDisplay(){return mDisplay;}
	const HDC GetHDC() const {return hDC;}
	const HWND GetHWND() const {return hWnd;}
	const HINSTANCE GetHInstance() const {return hInstance;}

	void SetWindowCaption(const std::string& str)
	{
		SetWindowText(hWnd, str.c_str());
	}

private:
	GameWindow(){};
	static GameWindow* mInstance;

	SmartPtr<GameDisplay> mDisplay;
	SmartPtr<Window> mWindow;

	bool fullscreen;

	RECT		globalRect;
	HDC			hDC;			// Private GDI Device Context
	HGLRC		hRC;			// Permanent Rendering Context
	HWND		hWnd;			// Holds Our Window Handle
	HINSTANCE	hInstance;		// Holds The Instance Of The Application
};

#endif