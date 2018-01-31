#ifndef SCENE_SAT_COL_H
#define SCENE_SAT_COL_H

#include "Scene.h"
#include "Shape.h"

/***************************************************************************************************************
Scene SAT collision

Author: Tan Yie Cher
Date: 11/6/2017

SAT collison with single nodes. SAT technology taken from 2016 project.
/***************************************************************************************************************/
class Scene_SAT_Collision : public Scene
{
	//test objects---------------------------------------//
	Transform player_parent;
	Transform obstacle_parent;
	Shape player;
	Shape player2;
	Shape obstacle;
	Shape obstacle2;

	//Framerate proof collision
	float FP_multiplier;
	bool player_FP_transformed;
	bool obstacle2_FP_transformed;
	Vector3 player_vel;
	Vector3 player_angle;
	Vector3 obstacle2_angle;

	//FP transformation
	void FP_reset();
	void TranslatePlayer(float x, float y, float z);
	void RotatePlayer(float x, float y, float z);
	void RotateObstacle2(float x, float y, float z);
	void FP_CollisionCheck();
	void TRS_calculations();

	//mobile
	void DPAD_controls();

public:

	//CD--------------------------------//
	Scene_SAT_Collision();
	~Scene_SAT_Collision();

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