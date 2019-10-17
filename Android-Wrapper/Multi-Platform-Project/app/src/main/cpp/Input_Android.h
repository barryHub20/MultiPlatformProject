#ifndef INPUT_ANDROID_H
#define INPUT_ANDROID_H

#include "Input.h"

/*
struct TouchEvent
{
    int32_t pointer_ID;
    Vector2 pos;

    TouchEvent();
    ~TouchEvent();

    void Activate(int32_t pointer_ID, Vector2 pos);
    void Movement(int32_t pointer_ID, Vector2 pos);
    void Deactivate(int32_t pointer_ID);
};
*/

/***************************************************************************************************************
Input framework class for PC

Author: Tan Yie Cher
Date: 20/1/2017

Key press: 1st frame on held
Key held: subsequent frames
Key released: 1st frame on release

Note:
-Define for PC and Android
/***************************************************************************************************************/
class Input_Android : public Input
{
public:

	//CD--------------------------------//
	Input_Android();
	~Input_Android();

	//core------------------------------//
	void Init() override;

    //callbacks--------------------------//
    /*void pointer_Down(int32_t input_ID, Vector2 cursorPos);
    void pointer_Up(int32_t input_ID);
    void pointer_Held(int32_t input_ID, Vector2 cursorPos);*/
};
#endif