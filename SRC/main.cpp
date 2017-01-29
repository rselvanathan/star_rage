#include "GameUtil.h"
#include "GameSession.h"

bool fullscreen = TRUE;	// Fullscreen Flag Set To Fullscreen Mode By Default

int WINAPI WinMain(	HINSTANCE	hInstance,			// Instance
					HINSTANCE	hPrevInstance,		// Previous Instance
					LPSTR		lpCmdLine,			// Command Line Parameters
					int			nCmdShow)			// Window Show State
{
	//// Ask The User Which Screen Mode They Prefer
	//if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	//{
	//	fullscreen=FALSE;							// Windowed Mode
	//}

	fullscreen = FALSE; // For the purpose of the demo, switch the question off and always windowed

	if(!GameSession::GetInstance()->Init(hInstance, fullscreen)){
		MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;	
	 }

	return GameSession::GetInstance()->MainLoop();							// Exit The Program
}
