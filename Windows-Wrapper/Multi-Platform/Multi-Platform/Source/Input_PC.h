#ifndef INPUT_PC_H
#define INPUT_PC_H

#include "Input.h"

//threading
//#define HAVE_STRUCT_TIMESPEC	//tell p-thread win32 headers we already defined timespec
//#include <pthread.h>

/***************************************************************************************************************
Input framework class for PC

Author: Tan Yie Cher
Date: 20/1/2017

Key press: 1st frame on held
Key held: subsequent frames
Key released: 1st frame on release

FPS cam functionalities: 28/9/17

Note:
-Define for PC and Android
/***************************************************************************************************************/
class Input_PC : public Input
{
	//Mouse variables--------------------------------------------------//
	double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	int m_window_deadzone;
	bool Mouse_DeadZone;	//update cursor with deadzone? if not, no mouse
	static double scrollxPos;
	static double scrollyPos;
	Vector3 cursorPos;
	float pitch, yaw;

	//core utils (thread-safe)-----------------------------------------//
	static void KeyDownAction(InputState::keycodes keycode);
	static void KeyUpAction(InputState::keycodes keycode);

	//utils------------------------------------------------------------//
	void KeyCallbackCheck(int key, int action, const int key_define, InputState::keycodes keycode);

	//for synchronization (this class potentially gets called by mutiple threads)
	//static pthread_mutex_t mutex_;

public:

	//CD--------------------------------//
	Input_PC(){}
	~Input_PC(){}

	//core------------------------------//
	void Init() override;
	void Update() override;
	void Exit() override;

	//mouse-----------------------------//
	void SetMouse(bool activate);
	void UpdateMouse();

	//callback----------------------------------------------------------//
	void PC_Key_Callback(GLFWwindow* windows, int key, int scancode, int action, int mods);
	void PC_Mouse_Callback(GLFWwindow* window, int button, int action, int mods);
	void PC_Cursor_Callback(GLFWwindow *, double x, double y);

	//get-----------------------------------------------------//
	Vector2 Get_CursorPos();	//always get
	float getYaw();
	float getPitch();
};

#endif