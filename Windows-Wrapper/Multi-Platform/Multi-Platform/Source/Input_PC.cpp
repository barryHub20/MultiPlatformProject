#include "Input_PC.h"
#include "ControllerPC.h"
pthread_mutex_t Input_PC::mutex_;

static void Key_Callback(GLFWwindow* windows, int key, int scancode, int action, int mods)
{
	CU::input.PC_Key_Callback(windows, key, scancode, action, mods);
}

static void Mouse_Callback(GLFWwindow* window, int button, int action, int mods)
{
	CU::input.PC_Mouse_Callback(window, button, action, mods);
}

static void Cursor_Callback(GLFWwindow* window, double x, double y)
{
	CU::input.PC_Cursor_Callback(window, x, y);
}

/*********************************************************************************************************
init
/*********************************************************************************************************/
void Input_PC::Init()
{
	Input::Init();

	//thread-safe
	pthread_mutex_init(&mutex_, NULL);

	//Mouse----------------------------------------------------------------------------------//
	mouse_last_x = mouse_last_y = mouse_current_x = mouse_current_y = mouse_diff_x = mouse_diff_y = 0.0;
	m_window_deadzone = 0;
	Mouse_DeadZone = false;
	pitch = yaw = 0.f;

	//set callbacks--------------------------------------------------------------------------//
	glfwSetKeyCallback(Controller_PC::Instance()->Get_windows(), Key_Callback);
	glfwSetMouseButtonCallback(Controller_PC::Instance()->Get_windows(), Mouse_Callback);
	glfwSetCursorPosCallback(Controller_PC::Instance()->Get_windows(), Cursor_Callback);

	//set to fps
	SetMouse(true);
}

/*********************************************************************************************************
constant update
/*********************************************************************************************************/
void Input_PC::Update()
{
	if (Mouse_DeadZone)
	{
		UpdateMouse();
	}
}

/*********************************************************************************************************
activate: activate cursor lock for FPS mode
/*********************************************************************************************************/
void Input_PC::SetMouse(bool activate)
{
	Mouse_DeadZone = activate;

	//Set input mode--------------------------------------------------------//
	if (activate)
		glfwSetInputMode(Controller_PC::Instance()->Get_windows(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	else
		glfwSetInputMode(Controller_PC::Instance()->Get_windows(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

/*********************************************************************************************************
update FPS mouse
/*********************************************************************************************************/
void Input_PC::UpdateMouse()
{
	GLFWwindow* window = Controller_PC::Instance()->Get_windows();

	glfwGetCursorPos(window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;
	//Calculate the yaw and pitch
	yaw = (float)mouse_diff_x * 0.0174555555555556f;// * 3.142f / 180.0f;
	pitch = mouse_diff_y * 0.0174555555555556f;// 3.142f / 180.0f );

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	int screen_width = (int)Screen::SCREEN_WIDTH;
	int screen_height = (int)Screen::SCREEN_HEIGHT;
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x >  -m_window_deadzone))
	{
		mouse_current_x = screen_width >> 1;
		glfwSetCursorPos(window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > Screen::SCREEN_HEIGHT - m_window_deadzone))
	{
		mouse_current_y = screen_height >> 1;
		glfwSetCursorPos(window, mouse_current_x, mouse_current_y);
	}
	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;
}

/*********************************************************************************************************
GLFW keyboard callback
GLFW_PRESSED, REPEAT, RELEASED
input_ID for cursor callbacks always 0, since there's only one mouse lol
Android has multi-touch
/*********************************************************************************************************/
void Input_PC::PC_Key_Callback(GLFWwindow* windows, int key, int scancode, int action, int mods)
{
	//keyboard--------------------------------------------------------------------------//
	KeyCallbackCheck(key, action, GLFW_KEY_W, InputState::W);
	KeyCallbackCheck(key, action, GLFW_KEY_S, InputState::S);
	KeyCallbackCheck(key, action, GLFW_KEY_A, InputState::A);
	KeyCallbackCheck(key, action, GLFW_KEY_D, InputState::D);
	KeyCallbackCheck(key, action, GLFW_KEY_K, InputState::K);
	KeyCallbackCheck(key, action, GLFW_KEY_L, InputState::L);
	KeyCallbackCheck(key, action, GLFW_KEY_E, InputState::E);

	//special---------------------------------------------------------//
	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		keymapList[InputState::keycodes::Q] = true;
}

//helper function
void Input_PC::KeyCallbackCheck(int key, int action, const int key_define, InputState::keycodes keycode)
{
	if (key == key_define && action == GLFW_PRESS)
		KeyDownAction(keycode);
	else if (key == key_define && action == GLFW_RELEASE)
		KeyUpAction(keycode);
}

/*********************************************************************************************************
GLFW mouse callback
GLFW_PRESSED, REPEAT, RELEASED
/*********************************************************************************************************/
void Input_PC::PC_Mouse_Callback(GLFWwindow* window, int button, int action, int mods)
{
	//Mouse--------------------------------------------------------------------------//
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		KeyDownAction(InputState::keycodes::L_MOUSE_BTN);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		KeyUpAction(InputState::keycodes::L_MOUSE_BTN);
	}
}

/*********************************************************************************************************
Key down action (thread-safe)
/*********************************************************************************************************/
void Input_PC::KeyDownAction(InputState::keycodes keycode)
{
	pthread_mutex_lock(&mutex_);

	CU::input.keymapList[keycode] = true;
	Controller_PC::Instance()->KeyPressed();
	CU::input.cursor_KeyDown(keycode, 0, Vector2(CU::input.cursorPos.x, CU::input.cursorPos.y), 0);

	pthread_mutex_unlock(&mutex_);
}

void Input_PC::KeyUpAction(InputState::keycodes keycode)
{
	pthread_mutex_lock(&mutex_);

	CU::input.keymapList[keycode] = false;
	Controller_PC::Instance()->KeyReleased();
	CU::input.cursor_KeyUp(keycode, 0, 0);

	pthread_mutex_unlock(&mutex_);
}

/*********************************************************************************************************
exit
/*********************************************************************************************************/
void Input_PC::Exit()
{
	pthread_mutex_destroy(&mutex_);
}

/*********************************************************************************************************
GLFW cursor pos callback
/*********************************************************************************************************/
void Input_PC::PC_Cursor_Callback(GLFWwindow *, double x, double y)
{
	//if mouse NOT activated, update cursor------------------------------------------------//
	if (!Mouse_DeadZone)
	{
		cursorPos.Set(x, y, 0);
		cursorPos.x = (cursorPos.x / Screen::SCREEN_WIDTH) * Screen::CAMERA_WIDTH;
		cursorPos.y = Screen::CAMERA_HEIGHT - ((cursorPos.y / Screen::SCREEN_HEIGHT) * Screen::CAMERA_HEIGHT);

		//tmp raycast to world
		cursorPos.x -= Screen::CAMERA_WIDTH * 0.5f;
		cursorPos.y -= Screen::CAMERA_HEIGHT * 0.5f;

		//cursor movement
		CU::input.cursor_KeyHeld(0, Vector2(cursorPos.x, cursorPos.y), 0);
	}
}

/*********************************************************************************************************
GLFW mouse callback
GLFW_PRESSED, REPEAT, RELEASED
/*********************************************************************************************************/
Vector2 Input_PC::Get_CursorPos()
{
	return Vector2(cursorPos.x, cursorPos.y);
}
float Input_PC::getYaw() { return yaw; }
float Input_PC::getPitch() { return pitch; }