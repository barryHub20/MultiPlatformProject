#include "Pathfinding.h"
#define STARTING_GRID_PARENT -5

/*********************************************************************************************************
broadphrase

map arrangement:
[6,7,8,9,10]
[0,1,2,3,4,5]
/*********************************************************************************************************/
void Pathfinding::BroadPhrase_Search(Vector2 startPos, NavMesh * navmesh)
{
	target_nmesh = navmesh;
	total_grids = target_nmesh->total_FreeTile;

	//do some resetting-------------------------------------------------------------------------------//
	for (int i = 0; i < total_grids; ++i)
		tileParent_List[i] = -1;

	current_total_grids = 0;

	//check start pos X and Y location-----------------------------------------------------------------//
	tileIndex_List[0] = target_nmesh->Get_PosIndex(startPos);
	tileParent_List[tileIndex_List[0]] = STARTING_GRID_PARENT;


	//start broadphrase check---------------------------------------------//
	for(int i = 0; i < total_grids; ++i)	//while not processed all free grids
	{
		/***************************** Reverse *****************************/
		//i instead of current_total_grids
		if ((target_nmesh->gridmap_tile[tileIndex_List[i]].xPos +
			target_nmesh->gridmap_tile[tileIndex_List[i]].yPos) % 2 != 0)
		{
			AddToFrontier(PF_Tile::AJACENT::RIGHT, i);
			AddToFrontier(PF_Tile::AJACENT::DOWN, i);
			AddToFrontier(PF_Tile::AJACENT::LEFT, i);
			AddToFrontier(PF_Tile::AJACENT::UP, i);
		}
		else
		{
			AddToFrontier(PF_Tile::AJACENT::UP, i);
			AddToFrontier(PF_Tile::AJACENT::LEFT, i);
			AddToFrontier(PF_Tile::AJACENT::DOWN, i);
			AddToFrontier(PF_Tile::AJACENT::RIGHT, i);
		}
	}
}

/*********************************************************************************************************
Add a tile to frontier provided:
-not out of bounds
-added before
/*********************************************************************************************************/
int tile_index = -1;
void Pathfinding::AddToFrontier(PF_Tile::AJACENT neighbour_type, int curr_index)
{
	tile_index = target_nmesh->gridmap_tile[tileIndex_List[curr_index]].ajacent[neighbour_type];

	if (tile_index == -1)
		return;

	//if is a wall-------------------------//
	if (target_nmesh->gridmap_type[tile_index] == NavMesh::TILETYPE::WALL)
		return;
	
	//if not added to a list yet
	if (tileParent_List[tile_index] == -1)
	{
		tileParent_List[tile_index] = tileIndex_List[curr_index];
		tileIndex_List[++current_total_grids] = tile_index;	//ADD to frontier
	}
}

/*********************************************************************************************************
Assign from target point to start point
index 0: NPC pos
index max: target pos
/*********************************************************************************************************/
bool Pathfinding::AssignPath(PathFollower & pf, Vector2& NPC_pos)
{
	//if NPC is intersecting a wall/out of bounds, IGONORE
	if (target_nmesh->Is_Pos_OutOfBounds(NPC_pos) || target_nmesh->Is_Pos_InWall(NPC_pos))
		return false;

	vector<int> pathList;
	int npc_index = target_nmesh->Get_PosIndex(NPC_pos);
	int total_xGrids = target_nmesh->Get_TotalX_grids();

	//parent of npc_index
	int cu = npc_index;
	int parent = tileParent_List[npc_index];
	pathList.push_back(cu);

	while (parent != STARTING_GRID_PARENT)
	{
		//check if following tile is (1) valid, (2) diagonal and (3) not ajacent to a wall-------------------//
		if (tileParent_List[parent] != -1 &&
			abs(cu - tileParent_List[parent]) == target_nmesh->Get_TotalX_grids() + 1)
		{
			cu = tileParent_List[parent];
		}
		else
			cu = parent;
		
		pathList.push_back(cu);
		parent = tileParent_List[cu];
	}

	//startIndex
	pf.Clear_Path();
	pf.Set_CurrentNavMesh(target_nmesh);
	pf.Set_Path(pathList, NPC_pos);

	return true;
}