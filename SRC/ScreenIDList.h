/*
This class contains all the IDs for screen states. Used for message dispatching

Author : Romesh Selvanathan

Date : 3/05/12
*/

#ifndef __SCREENIDLIST_H__
#define __SCREENIDLIST_H__

// Define the NULL
#define NULL 0

class ScreenIDList
{
public:
	static ScreenIDList* GetInstance()
	{
		if(mScreenIDList == NULL)
			mScreenIDList = new ScreenIDList();
		return mScreenIDList;
	}

	int keybind_options;
	int mouse_options;
	int options;
	int message_box_rebind;
	int message_box;
	int gameplay;
	int help;
	int highscore;
	int intro;
	int main_menu;
	int pause;
	int loading;

private:
	ScreenIDList() {}

	static ScreenIDList* mScreenIDList;
};

#endif