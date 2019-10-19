#include "PathFollower.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
PathFollower::PathFollower()
{
	navmesh = NULL;
}

PathFollower::~PathFollower()
{
}

/*********************************************************************************************************
Set path
/*********************************************************************************************************/
void PathFollower::Clear_Path()
{
	currentTile = totalTiles = 0;
}

void PathFollower::Set_Path(vector<int>& pathList, Vector2 npc_pos)
{
	totalTiles = pathList.size();
	
	//create new pos---------------------------//
	if (totalTiles > path.size())
		path.resize(totalTiles);
	
	for (int i = 0; i < totalTiles; ++i)
		path[i] = pathList[i];

	//check if NPC at tile 0--------------------------------//
	if (totalTiles > 1)
	{
		currentTile = 1;
	}
}

void PathFollower::Set_CurrentNavMesh(NavMesh* navmesh)
{
	this->navmesh = navmesh;
}

//void PathFollower::Reset_CurrentTile()
//{
//	currentTile = 0;
//}

/*********************************************************************************************************
NPC Reached target pos?
/*********************************************************************************************************/
bool PathFollower::Reached_target()
{
	return currentTile >= totalTiles - 1;
}

/*********************************************************************************************************
Get target pos, as long as reached
/*********************************************************************************************************/
Vector2 PathFollower::GetTargetPos()
{
	return navmesh->Get_TargetPos(path[currentTile]);
}

void PathFollower::Go_NextTile()
{
	//go to next tile down the list till the root
	if (currentTile < totalTiles - 1)
	{
		++currentTile;
	}
}

/*********************************************************************************************************
debug
/*********************************************************************************************************/
const vector<int>& PathFollower::Get_PathList()
{
	return path;
}

/*********************************************************************************************************
return total tiles
/*********************************************************************************************************/
int PathFollower::Get_TotalTiles()
{
	return totalTiles;
}