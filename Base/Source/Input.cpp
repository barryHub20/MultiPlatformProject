#include "Input.h"
#include "CoreUtilities.h"
#include "UsefulUtils.h"

/*********************************************************************************************************
D-Pad functions
/*********************************************************************************************************/
void DPad_Btn::Init(Vector2 pos, float radius, InputState::keycodes triggerKey)
{
	this->pos = this->innerPad_pos = pos;
	this->radius = radius;
	this->triggerKey = triggerKey;
	this->cursor_ID = -1;
	this->direction.Set(1, 0);	//default
	active = false;
	this->inner_texture_pressed = TEX_INNER_DPAD;
	this->inner_texture = TEX_INNER_DPAD;
	this->outer_texture = TEX_OUTER_DPAD;
}

void DPad_Btn::SetTexture(TEXTURE_TYPE inner_texture_pressed, TEXTURE_TYPE inner_texture, TEXTURE_TYPE outer_texture)
{
	this->inner_texture_pressed = inner_texture_pressed;
	this->inner_texture = inner_texture;
	this->outer_texture = outer_texture;
}

//Updates-----------------------------------------------------//
void DPad_Btn::OnTrigger(InputState::keycodes triggerKey, int cursor_ID, Vector2 cursorPos)
{
	if (this->triggerKey == triggerKey)
	{
		//check eligible cursor trackers-----------------------------------//
		if (Utils::pointInCircle(cursorPos, pos, radius))
		{
			//get dir-----------------------------------//
			this->cursor_ID = cursor_ID;
			active = true;

			if (!cursorPos.Same(cursorPos))
				direction = (cursorPos - pos).Normalized();
			else
				direction.Set(1, 0);
		}
	}
}

void DPad_Btn::Update(int cursor_ID, Vector2 cursorPos)
{
	if (this->cursor_ID == cursor_ID && active)
	{
		if (!pos.Same(cursorPos))
			direction = (cursorPos - pos).Normalized();
		else
			direction.Set(1, 0);

		//inner pad will gravitate towards cursor----------------------------//
		//if cursor point in pad, inner-pad pos = cursor pos
		//else cursor point outside pad, reach edge of pad
		if (Utils::pointInCircle(cursorPos, pos, radius * 0.3f))
		{
			innerPad_pos = cursorPos;
			//innerPad_pos = pos + direction * radius * 0.3f;
		}
		else
		{
			innerPad_pos = pos + direction * radius * 0.3f;
		}
	}
}

void DPad_Btn::OnRelease(int cursor_ID)
{
	if (this->cursor_ID == cursor_ID)
	{
		active = false;
		this->cursor_ID = -1;
		innerPad_pos = pos;
	}
}

//when u want to rotate the D Pad for left right up down etc.
void DPad_Btn::SetRotate(float angle)
{
	this->angle = angle;
}

//Draw-----------------------------------------------------//
void DPad_Btn::Draw()
{
	float diameter = radius * 2.f;

	//draw a circular shape, one outer ring one inner ring
	CU::quad->AssignTexture(CU::textureList[outer_texture]);
	CU::view.Pre_DrawMesh(Vector3(pos.x, pos.y, 1.f), Vector3(diameter, diameter), CU::quad);
	CU::quad->Draw();

	if (active)	//pressed
	{
		CU::quad->AssignTexture(CU::textureList[inner_texture_pressed]);
		CU::view.Pre_DrawMesh(Vector3(innerPad_pos.x, innerPad_pos.y, 1.5f), angle, Vector3(diameter * 0.7f, diameter * 0.7f), CU::quad);
		CU::quad->Draw();
	}
	else
	{
		CU::quad->AssignTexture(CU::textureList[inner_texture]);
		CU::view.Pre_DrawMesh(Vector3(innerPad_pos.x, innerPad_pos.y, 1.5f), Vector3(diameter * 0.7f, diameter * 0.7f), CU::quad);
		CU::quad->Draw();
	}
}

void DPad_Btn::DrxawQuadrant(int quadrant)
{
}

//get-----------------------------------------------------//
bool DPad_Btn::isActive(){ return active; }
Vector2 DPad_Btn::GetDirection(){ return direction; }

/*********************************************************************************************************
init
/*********************************************************************************************************/
void Input::Init()
{
}

/*********************************************************************************************************
key down/up callbacks 
On android: ID assigned by native activity and keycode will ALWAYS be tap
PC trigger: any key down
Android trigger: tap
keycode:
PC: anything
Android: TAP only
/*********************************************************************************************************/
void Input::cursor_KeyDown(InputState::keycodes keycode, int input_ID, Vector2 cursorPos, int input_index)
{
	//if keycode matches and collides with D-Pad, cursor ID assigned to this D-pad
	dpad_L.OnTrigger(keycode, input_ID, cursorPos);
	dpad_R.OnTrigger(keycode, input_ID, cursorPos);
}

void Input::cursor_KeyUp(InputState::keycodes keycode, int input_ID, int input_index)
{
	dpad_L.OnRelease(input_ID);
	dpad_R.OnRelease(input_ID);
}

void Input::cursor_KeyHeld(int input_ID, Vector2 cursorPos, int input_index)
{
	dpad_L.Update(input_ID, cursorPos);
	dpad_R.Update(input_ID, cursorPos);
    tapPoint = cursorPos;
}

/*********************************************************************************************************
is key pressed
/*********************************************************************************************************/
bool Input::isKeyPressed(InputState::keycodes keycode)
{
	return keymapList[keycode];
}

bool Input::DPad_Left(Vector2& dir)
{
	if (dpad_L.active)
	{
		dir = dpad_L.direction;
		return true;
	}

	return false;
}

bool Input::DPad_Right(Vector2& dir)
{
	if (dpad_R.active)
	{
		dir = dpad_R.direction;
		return true;
	}

	return false;
}

/*********************************************************************************************************
update
/*********************************************************************************************************/
void Input::Update()
{
}

/*********************************************************************************************************
draw
/*********************************************************************************************************/
void Input::Draw()
{
	dpad_L.Draw();
	dpad_R.Draw();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Input::Exit()
{

}

/*********************************************************************************************************
Useful utilities
/*********************************************************************************************************/
void Input::Enable_DPad(InputState::keycodes pressKey)
{
	dpad_key = pressKey;
	dpad_enabled = true;

	//init D-pad shapes
	dpad_L.Init(Vector2(-250, -210), 80.f, pressKey);
	dpad_R.Init(Vector2(250, -210), 80.f, pressKey);
}

void Input::SetCalledKey(InputState::keycodes pressKey, bool state)
{
	keymapList[pressKey] = state;
}