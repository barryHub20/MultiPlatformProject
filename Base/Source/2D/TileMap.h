#ifndef TILEMAP_H
#define TILEMAP_H

#include "Mesh.h"

/***************************************************************************************************************
Tile map

Author: Tan Yie Cher
Date: 25/2/17

Requirements:
-spritesheet has the same amount of frames per action!!!
-Each action starts from the same frame

Min/max are inclusive

For example:
min: 0
max: 7
for all layers
/***************************************************************************************************************/
class TileMap
{
public:

	//might not reflect actual order
	enum SET_TYPE
	{
		WALK_N,
		WALK_NE,
		WALK_E,
		WALK_SE,
		WALK_S,
		WALK_SW,
		WALK_W,
		WALK_NW,
		TOTAL_SETS
	};

	//public access-------------------------------//
	int setList_index[TOTAL_SETS];

protected:

	//Data-------------------------------------//
	int total_tiles_layer;
	int total_layers;
	int start_frame;
	Mesh* mesh;

public:

	//CD--------------------------------//
	TileMap();
	~TileMap();

	//Core------------------------------//
	void Init(int total_tiles_layer, int total_layers, int start_frame, GLuint texture_ID);
	void AddSet(int layer_index, SET_TYPE set_type);

	//Get--------------------------------//
	int Get_TotalX();
	int Get_TotalY();
	int Get_StartFrame();
	Mesh* GetMesh();
};

#endif