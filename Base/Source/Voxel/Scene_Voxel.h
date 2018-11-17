#ifndef Scene_Voxel_H
#define Scene_Voxel_H

#include "Scene.h"

/***************************************************************************************************************
Voxel engine

Author: Tan Yie Cher
Date: 6/3/2018

Tutorial: 
https://sites.google.com/site/letsmakeavoxelengine/home
/***************************************************************************************************************/
class Scene_Voxel : public Scene
{

public:

	//CD----------------------------------------//
	Scene_Voxel();
	~Scene_Voxel();

	//Core--------------------------------------//
	virtual void Init();
	virtual void Update();
	virtual void Tmp_Draw();
	virtual void Tmp_DrawOnScreen();
	virtual void Exit();

	//key pressed-------------------------------//
	virtual void Update_KeyPressed();
	virtual void Update_KeyReleased();
};

#endif