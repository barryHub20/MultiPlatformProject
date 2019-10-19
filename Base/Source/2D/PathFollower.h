#ifndef PATHFOLLOWER_H
#define PATHFOLLOWER_H

#include "Header.h"
#include "NavMesh.h"

/***************************************************************************************************************
Utility: tile for path follower component

Author: Tan Yie Cher
Date: 27/2/17
/***************************************************************************************************************/
struct NavMesh_Tile
{
	int x, y;
};

/***************************************************************************************************************
Component: Pathfollower utility class

Author: Tan Yie Cher
Date: 27/2/17


Must be called in this order:
Clear_Path();
Set_CurrentNavMesh(target_nmesh);
Set_Path(pathList);
/***************************************************************************************************************/
class PathFollower
{
	int currentTile;	//current tile
	int totalTiles;	//how many tiles in path
	vector<int> path;	//list of vec2s
	NavMesh* navmesh;

public:

	//CD--------------------------------//
	PathFollower();
	~PathFollower();

	//Set new path: Called by Pathfind--------------------------------//
	void Clear_Path();
	void Set_Path(vector<int>& pathList, Vector2 npc_pos);
	void Set_CurrentNavMesh(NavMesh* navmesh);
	//void Reset_CurrentTile();

	//Called by NPC-----------------------------------------//
	bool Reached_target();
	void Go_NextTile();
	Vector2 GetTargetPos();

	//debug purposes---------------------------------------------------//
	const vector<int>& Get_PathList();

	//get-----------------------------------------------------------//
	int Get_TotalTiles();
};

#endif