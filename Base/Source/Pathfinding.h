#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "PathFollower.h"


struct BP_Tile
{
	int parent;
	int currentIndex;
};

/***************************************************************************************************************
Pathfinding utility class

Author: Tan Yie Cher
Date: 26/2/17

How to use Broadphrase:
1) Call BroadPhrase_Search(..)
2) for an infinite number of PathFollowers, call AssignPath(..)

/***************************************************************************************************************/
class Pathfinding
{
	NavMesh* target_nmesh;

	//broadphrase---------------------------------//
	Vector2 startPos;
	
	//incremental order
	int current_total_grids;	//how many grids being processed (0 -> total free grids)
	int total_grids;
	int tileIndex_List[MAX_GRIDS_NAVMESH];	//index of tiles list (so that we can loop through the jumbled up indexes incrementally)

	//follow navmesh order
	int tileParent_List[MAX_GRIDS_NAVMESH];

	void AddToFrontier(PF_Tile::AJACENT neighbour_type, int curr_index);

public:

	//CD--------------------------------//
	Pathfinding();
	~Pathfinding();

	//Core------------------------------//
	void Init();

	//Called by user------------------------------------------//
	void BroadPhrase_Search(Vector2 startPos, NavMesh* navmesh);
	bool AssignPath(PathFollower& pf, Vector2& NPC_pos);
};

#endif