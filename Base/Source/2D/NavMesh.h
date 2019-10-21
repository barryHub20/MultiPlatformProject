#ifndef NAVMESH_H
#define NAVMESH_H
#define MAX_GRIDS_NAVMESH 2000

#include "GridMesh.h"

/***************************************************************************************************************
Pathfinding tile

Author: Tan Yie Cher
Date: 28/2/17

Consists of:
-index
-ajacent neighbours indexes [4]
/***************************************************************************************************************/
class PF_Tile
{
public:
	enum AJACENT
	{
		LEFT,
		RIGHT,
		UP,
		DOWN
	};

	//data---------------------------------------------//
	int ajacent[4];
	int xPos, yPos;

	PF_Tile();
	~PF_Tile();

	//core---------------------------------------------//
	void SetTile(int left, int right, int up, int down);
	void SetPos(int x, int y);
	void ResetTile();
};


/***************************************************************************************************************
Component: Nav mesh, for all pathfinding entities
Grid map, not graph

Author: Tan Yie Cher
Date: 26/2/17

Always X over Y, [x][y]
No error checking available

stores a gridmap of types and pos

center_align_grids: starting grid start at center instead of bottom left
/***************************************************************************************************************/
class NavMesh
{
public:

	enum TILETYPE
	{
		NOTHING,
		WALL,
		TOTAL
	};

public:

	//tmp------------------------------------------------------//
	Debug_GridMesh* debug_GridMesh;

	//data-----------------------------------------------------//
	TILETYPE* gridmap_type;
	PF_Tile* gridmap_tile;	//ajacent neighbours
	Vector2* gridmap_pos;
	Vector2 bottomLeft_pos;
	float tileScale;
	int xGrids, yGrids;
	float xScale, yScale;

	//global data---------------------------------------------//
	int total_FreeTile;

public:

	//CD--------------------------------//
	NavMesh();
	~NavMesh();

	//Core------------------------------//
	void SetNav_Space(int xGrids, int yGrids, Vector2 bottomLeft_pos, float tileScale);
	void Add_Wall(int x, int y);
	void Remove_Wall(int x, int y);
	
	//modify grid-----------------------//
	void SetGrid_AllOff();
	void SetGrid_state(int index, TILETYPE type);

	//Called by PathFollower-----------------------//
	Vector2 Get_TargetPos(int index);

	//Called by PathFinding---------------------------//
	int Get_PosIndex(Vector2 pos);	//index of grid pos is closest to

	//Called by scene-------------------------------//
	void TmpDraw();

	//debug mesh------------------------------------------------//
	void Reset_GridMesh();
	void LightUp_Path(const vector<int>& pathList, int total_tiles);
	void Update_GridMesh();	

	//Essentials-----------------------------------------------------//
	bool Is_Pos_OutOfBounds(Vector2 pos);
	bool Is_Pos_InWall(Vector2 pos);

	//get-----------------------------------------------//
	int Get_TotalX_grids();
	int Get_TotalY_grids();
};

#endif