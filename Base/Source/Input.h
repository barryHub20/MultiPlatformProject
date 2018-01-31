#ifndef INPUT_H
#define INPUT_H
#include "Header.h"

//Keycodes------------------------------------------------//
namespace InputState
{
	enum keycodes
	{
		L_MOUSE_BTN,
		R_MOUSE_BTN,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,

		//android
		TAP,

		//D-pad
		L_DPAD,
		R_DPAD,
		U_DPAD,
		D_DPAD,

		TOTAL_KEYS
	};

	enum State
	{
		PRESSED,
		HELD,
		RELEASED
	};
}

/***************************************************************************************************************
D-Pad class

Author: Tan Yie Cher
Date: 2/2/2017

cursor_ID: the input_ID assigned by native activity (Android), always 0 for PC

A circular controller that returns a direction vec based on input trigger.
Trigger condition: cursor must be inside the D-Pad for it to be triggered
Release condition: the trigger key being released anywhere
/***************************************************************************************************************/
struct DPad_Btn
{
	bool active;	//touched?
	float radius;
	float angle;
	TEXTURE_TYPE inner_texture_pressed, inner_texture, outer_texture;

	Vector2 pos;
	Vector2 innerPad_pos;
	Vector2 direction;
	int cursor_ID;
	InputState::keycodes triggerKey;

	//Init-----------------------------------------------------//
	void Init(Vector2 pos, float radius, InputState::keycodes triggerKey);
	void SetTexture(TEXTURE_TYPE inner_texture_pressed, TEXTURE_TYPE inner_texture, TEXTURE_TYPE outer_texture);	//for external use

	//Updates (called by cursor click/tap functions)---------------------------------------------------//
	void OnTrigger(InputState::keycodes triggerKey, int cursor_ID, Vector2 cursorPos);	//is figer/cursor on D_Pad?
	void Update(int cursor_ID, Vector2 cursorPos);
	void OnRelease(int cursor_ID);

	//rotation
	void SetRotate(float angle);

	//Draw-----------------------------------------------------//
	void Draw();
	void DrxawQuadrant(int quadrant);	//WASD indicator

	//get-----------------------------------------------------//
	bool isActive();
	Vector2 GetDirection();
};

/***************************************************************************************************************
Input framework class

Author: Tan Yie Cher
Date: 20/1/2017

Key press: 1st frame on held
Key held: subsequent frames
Key released: 1st frame on release

aKeyDown/Up called when a key is pressed

Note:
-Call init before GLFW init
-Define for PC and Android
-Get cursor pos: PC is real-time cursor pos (only one)
-cursor callbacks: cursor input (click/tap) 
	PC trigger: any key down
	Android trigger: tap
-DPad functions: 

Future:
-create Input UI object, so don't need to have in-built temporary D-pad
-List of cursor tracker (for android and other multi-touch screens)
/***************************************************************************************************************/
class Input
{
protected:

	//array of the keys
	bool keymapList[InputState::keycodes::TOTAL_KEYS];

	//flags
	bool dpad_enabled;
	InputState::keycodes dpad_key;
    Vector2 tapPoint;

public:

	DPad_Btn dpad_R, dpad_L;	//R = movement

	//CD--------------------------------//
	Input()
	{
		for (int i = 0; i < InputState::keycodes::TOTAL_KEYS; ++i)
			keymapList[i] = false;
	}
	~Input(){}

	//core--------------------------------//
	virtual void Init();
	virtual void Update();
	void Draw();	//temporary
	virtual void Exit();

	//cursor callbacks-----------------------------//
	void cursor_KeyDown(InputState::keycodes keycode, int input_ID, Vector2 cursorPosint, int input_index);
	void cursor_KeyUp(InputState::keycodes keycode, int input_ID, int input_index);
	void cursor_KeyHeld(int input_ID, Vector2 cursorPos, int input_index);

	//check key pressed------------------//
	bool isKeyPressed(InputState::keycodes keycode);

	//useful utilities------------------//
	void SetCalledKey(InputState::keycodes pressKey, bool state);
	void Enable_DPad(InputState::keycodes pressKey);	//can be clicked/tapped

	//dpad--------------------------------------------//
	bool DPad_Left(Vector2& dir);
	bool DPad_Right(Vector2& dir);
};

#endif