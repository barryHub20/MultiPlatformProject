#ifndef SP_RENDERER_H
#define SP_RENDERER_H

#include "GridMesh.h"
#include "TileMap.h"

/***************************************************************************************************************
Sprite renderer: Renderer component

Author: Tan Yie Cher
Date: 25/2/17

Call draw AFTER calling CU::view.Pre_DrawMesh(...)
Min and Max is INCLUSIVE
/***************************************************************************************************************/
class SpriteRenderer
{
protected:

	//Data-------------------------------------//
	int currIndex;
	int min, max;
	TILEMAP_TYPE tilemap_type;
	int current_set_skip;	//skip how many tiles for this layer
	double duration, timer;	//duration per tile

public:

	//CD--------------------------------//
	SpriteRenderer();
	~SpriteRenderer();

	//Core------------------------------//
	void Set(TILEMAP_TYPE tilemap_type, double duration);
	void Current_Set(TileMap::SET_TYPE set_type);	//set the current tilemap set
	void Update();
	void Draw();

	//Get------------------------------//
	Mesh* GetMesh();
};

#endif