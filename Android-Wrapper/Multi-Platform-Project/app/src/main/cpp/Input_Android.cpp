#include "Input_Android.h"
#include "ControllerAndroid.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
Touch event
/*********************************************************************************************************/
/*TouchEvent::TouchEvent()
{
    pointer_ID = -1;
}

TouchEvent::~TouchEvent()
{

}

void TouchEvent::Activate(int32_t pointer_ID, Vector2 pos)
{
    //activated before
    if(this->pointer_ID != -1)
        return;

    this->pointer_ID = pointer_ID;
    this->pos = pos;
}

void TouchEvent::Movement(int32_t pointer_ID, Vector2 pos)
{
    if(this->pointer_ID == pointer_ID)
    {
        this->pos = pos;
    }
}

void TouchEvent::Deactivate(int32_t pointer_ID)
{
    this->pointer_ID = -1;
}*/

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Input_Android::Input_Android()
{
}

Input_Android::~Input_Android()
{
}

/*********************************************************************************************************
Init
param windows: the GLFW context
param
/*********************************************************************************************************/
void Input_Android::Init()
{
    Input::Init();
}

