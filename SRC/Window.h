/*
This class sets up the window itself with the appropriate properties and, listens to the Windows Call back messages

Author : Romesh Selvanathan
*/
#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "GameUtil.h"

class GameWindow;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// Declaration For WndProc

class Window
{
public:
	Window(GameWindow* mGame);
	~Window();

	HWND CreateWindowGL(const char* title, int width, int height, int bits, bool fullscreenflag, HINSTANCE hInstance);

private:
	HWND		hwnd;
	WNDCLASS	wc;						// Windows Class Structure
	DWORD		dwExStyle;				// Window Extended Style
	DWORD		dwStyle;				// Window Style
	RECT		WindowRect;				// Grabs Rectangle Upper Left / Lower Right Values

	SmartPtr<GameWindow> game;

	HINSTANCE hinstance;
};

#endif