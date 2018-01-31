#ifndef SCENE_TRANSFORMATION_H
#define SCENE_TRANSFORMATION_H

#include "Scene.h"

/***************************************************************************************************************
Scene transforamtion

Author: Tan Yie Cher
Date: 26/2/2017

Used to test transformation: parent and child relations
/***************************************************************************************************************/
class Scene_Transformation : public Scene
{
	//test objects---------------------------------------//
	Transform transform, transform2, transform3;
	SpriteRenderer zombie_Renderer;


public:

	//CD--------------------------------//
	Scene_Transformation();
	~Scene_Transformation();

	//Core------------------------------//
	virtual void Init();
	virtual void Update();
	virtual void Tmp_Draw();
	virtual void Exit();

	//key pressed------------------//
	virtual void Update_KeyPressed();
	virtual void Update_KeyReleased();
};

#endif