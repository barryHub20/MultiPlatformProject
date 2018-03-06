#ifndef SCENE_H
#define SCENE_H

#include "Header.h"
#include "Transform.h"

/***************************************************************************************************************
Scene framework class

Author: Tan Yie Cher
Date: 20/1/2017

Temporary: show a parent/child/relationships

Note:
-Update_KeyPress, KeyHeld and KeyRelease() are callbacks on Android
/***************************************************************************************************************/
class Scene
{
protected:

	//shared-----------------------------------------------------------//
	static ostringstream ss;

public:

	//CD--------------------------------//
	Scene();
	virtual ~Scene();

	//Core------------------------------//
	virtual void Init();
	virtual void Update();
	virtual void Tmp_Draw();
	virtual void Tmp_DrawOnScreen();
	virtual void Exit();

	//key pressed------------------//
	virtual void Update_KeyPressed();
	virtual void Update_KeyReleased();
};

#endif