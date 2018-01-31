#ifndef NPC_H
#define NPC_H
#include "PathFollower.h"
#include "SpriteRenderer.h"
#include "Transform.h"

/***************************************************************************************************************
Entity: Basic NPC zombie class

Author: Tan Yie Cher
Date: 3/3/17

Follow a set of path.

Temporary: get transform and path_follower
/***************************************************************************************************************/
class NPC
{
public:

	enum STATE
	{
		IDLE,
		FOLLOW,
		ATTACK
	};

protected:
	
	//data------------------------------------------------------//
	STATE state;
	PathFollower path_follower;
	SpriteRenderer sprite_renderer;
	Transform transform;
	Vector2 scale;
	float speed;

public:

	//CD--------------------------------//
	NPC();
	~NPC();

	//Core------------------------------//
	void Init(TILEMAP_TYPE sprite_type, Vector3 position, Vector2 scale, float speed);
	void Update();
	void TmpDraw();

	void PathFind();

	//get-----------------------------------------------//
	Transform* Get_Transform();
	PathFollower* Get_PathFollower();
	Vector3 GetPos();
};

#endif