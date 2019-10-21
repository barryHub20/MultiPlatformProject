#ifndef SCENE_PATHFINDING_H
#define SCENE_PATHFINDING_H

#include "Scene.h"
const static int TOTAL_NPC = 20;

/***************************************************************************************************************
Scene pathfinding

Author: Tan Yie Cher
Date: 26/2/2017

Used to test pathfinding
/***************************************************************************************************************/
class Scene_Pathfinding : public Scene
{
	//test objects---------------------------------------//
	SpriteRenderer testSprite;

	NPC npc_list[TOTAL_NPC];
	Vector2 test_playerPos;
	bool changed_playerPos;

	//nav mesh---------------------------//
	NavMesh navmesh;

	//internal funcs----------------------------------------//
	void Calculate_Paths();

public:

	//CD--------------------------------//
	Scene_Pathfinding();
	~Scene_Pathfinding();

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