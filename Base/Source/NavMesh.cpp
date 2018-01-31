#include "NavMesh.h"
#include "CoreUtilities.h"

PF_Tile::PF_Tile()
{
}

PF_Tile::~PF_Tile()
{
}

/*********************************************************************************************************
path finding tile
/*********************************************************************************************************/
void PF_Tile::SetTile(int left, int right, int up, int down)
{
	ajacent[LEFT] = left;
	ajacent[RIGHT] = right;
	ajacent[UP] = up;
	ajacent[DOWN] = down;
}

void PF_Tile::SetPos(int x, int y)
{
	xPos = x;
	yPos = y;
}

void PF_Tile::ResetTile()
{
	ajacent[0] = ajacent[1] = ajacent[2] = ajacent[3] = -1;
}

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
NavMesh::NavMesh()
{
	gridmap_type = NULL;
	gridmap_pos = NULL;
	debug_GridMesh = NULL;
	gridmap_tile = NULL;
}

NavMesh::~NavMesh()
{
	delete debug_GridMesh;
	delete[] gridmap_type;
	delete[] gridmap_pos;
	delete[] gridmap_tile;
}

/*********************************************************************************************************
Set/reset the navigation space
gridmap_pos: Y over X
[6,7,8,9,10]
[0,1,2,3,4,5]
/*********************************************************************************************************/
void NavMesh::SetNav_Space(int xGrids, int yGrids, Vector2 bottomLeft_pos, float tileScale)
{
	//Set values------------------------------------------------//
	this->bottomLeft_pos = bottomLeft_pos;
	this->tileScale = tileScale;

	this->xGrids = xGrids;
	this->yGrids = yGrids;

	xScale = tileScale * xGrids;
	yScale = tileScale * yGrids;

	total_FreeTile = xGrids * yGrids;

	//delete prev. data------------------------------------------------//
	if (gridmap_type)
		delete[] gridmap_type;
	if (gridmap_pos)
		delete[] gridmap_pos;
	if (debug_GridMesh)
		delete debug_GridMesh;
	if (gridmap_tile)
		delete[] gridmap_tile;

	debug_GridMesh = new Debug_GridMesh;

	//create-------------------------------------------------//
	gridmap_type = new TILETYPE[xGrids * yGrids];
	gridmap_pos = new Vector2[xGrids * yGrids];
	gridmap_tile = new PF_Tile[xGrids * yGrids];

	//set tile type-------------------------------------------------------//
	for (int i = 0; i < total_FreeTile; ++i)
	{
		gridmap_type[i] = NOTHING;
	}

	//set the pos------------------------------------//
	int counter = 0;
	for (int y = 0; y < yGrids; ++y)
	{
		for (int x = 0; x < xGrids; ++x)
			gridmap_pos[counter++] = Vector2(tileScale * x, tileScale * y);
	}

	//set ajacent tiles--------------------------------------------------//
	gridmap_tile[0].SetTile(-1, 1, xGrids, -1);
	gridmap_tile[0].SetPos(0, 0);

	for (int i = 1; i < total_FreeTile; ++i)
	{
		//check if is on extreme--------------------------//
		bool extremeLeft = i % xGrids == 0;
		bool extremeRight = i % xGrids == xGrids - 1;
		bool extremeBottom = i < xGrids;
		bool extremeTop = i >= xGrids * (yGrids - 1);

		//assign index and ajacent indexes----------------------------------//
		gridmap_tile[i].ResetTile();

		if (!extremeLeft)
			gridmap_tile[i].ajacent[PF_Tile::AJACENT::LEFT] = i - 1;
		if (!extremeRight)
			gridmap_tile[i].ajacent[PF_Tile::AJACENT::RIGHT] = i + 1;
		if (!extremeBottom)
			gridmap_tile[i].ajacent[PF_Tile::AJACENT::DOWN] = i - xGrids;
		if (!extremeTop)
			gridmap_tile[i].ajacent[PF_Tile::AJACENT::UP] = i + xGrids;

		gridmap_tile[i].SetPos(i % xGrids, i / xGrids);
	}

	/*int i = 39;
	cout << gridmap_tile[i].xPos << ' ' << gridmap_tile[i].yPos << endl;
	cout << gridmap_tile[i].ajacent[PF_Tile::AJACENT::LEFT] << ' ' << gridmap_tile[i].ajacent[PF_Tile::AJACENT::RIGHT] << endl;
	cout << gridmap_tile[i].ajacent[PF_Tile::AJACENT::UP] << ' ' << gridmap_tile[i].ajacent[PF_Tile::AJACENT::DOWN] << endl;
*/

	//set the debug mesh------------------------------------//
	debug_GridMesh->Init_Debug_GridMap(xGrids, yGrids);
}

void NavMesh::Add_Wall(int x, int y)
{
	gridmap_type[(y * xGrids) + x] = WALL;
	//total_FreeTile--;
}

void NavMesh::Remove_Wall(int x, int y)
{
	gridmap_type[(y * xGrids) + x] = NOTHING;
	//total_FreeTile++;
}

void NavMesh::SetGrid_AllOff()
{
	for (int i = 0; i < xGrids * yGrids; ++i)
	{
		gridmap_type[i] = NOTHING;
	}
}

/*********************************************************************************************************
Set a grid on/off
/*********************************************************************************************************/
void NavMesh::SetGrid_state(int index, TILETYPE type)
{
	gridmap_type[index] = type;
}

/*********************************************************************************************************
Get target pos
/*********************************************************************************************************/
Vector2 NavMesh::Get_TargetPos(int index)
{
	return bottomLeft_pos + gridmap_pos[index];
}

/*********************************************************************************************************
index of grid pos is closest to
/*********************************************************************************************************/
int NavMesh::Get_PosIndex(Vector2 pos)
{
	pos -= bottomLeft_pos;	//set to origin space

	int xIndex = pos.x / tileScale;
	int yIndex = pos.y / tileScale;
	
	return (yIndex * xGrids) + xIndex;
}

/*********************************************************************************************************
index of grid pos is closest to
/*********************************************************************************************************/
void NavMesh::TmpDraw()
{
	CU::view.Pre_DrawMesh(Vector3(bottomLeft_pos.x, bottomLeft_pos.y, -2.f), Vector3(xScale, yScale, 1), debug_GridMesh);
	debug_GridMesh->Draw();
}

/*********************************************************************************************************
for debugging purposes

How to use:
Call:
1) Reset_GridMesh()
2) LightUp_Path(...) as many times
3) Update_GridMesh() to update mesh to buffer
/*********************************************************************************************************/
void NavMesh::LightUp_Path(const vector<int>& pathList, int total_tiles)
{
	int xIndex, yIndex;

	for (int i = 0; i < total_tiles; ++i)
	{
		xIndex = pathList[i] % xGrids;
		yIndex = pathList[i] / xGrids;	//Y inverted due to mesh
		debug_GridMesh->SetTile_Green(xIndex, yIndex);
	}
}

/*********************************************************************************************************
grid mesh reset
NO MESH UPDATE
/*********************************************************************************************************/
void NavMesh::Reset_GridMesh()
{
	debug_GridMesh->SetAllTiles_Blue();

	//set walls----------------------------------------//
	for (int i = 0; i < xGrids * yGrids; ++i)
	{
		if (gridmap_type[i] == WALL)
			debug_GridMesh->SetTile_Red(i);
	}
}

void NavMesh::Update_GridMesh()
{
	debug_GridMesh->UpdateMesh();
}

/*********************************************************************************************************
Essential utilities
/*********************************************************************************************************/
Vector2 asd;
bool NavMesh::Is_Pos_OutOfBounds(Vector2 pos)
{
	asd = pos - bottomLeft_pos;
	return asd.x < 0.f || asd.y < 0.f || asd.x >= xScale || asd.y >= yScale;
}

bool NavMesh::Is_Pos_InWall(Vector2 pos)
{
	return gridmap_type[Get_PosIndex(pos)] == WALL;
}

/*********************************************************************************************************
Get
/*********************************************************************************************************/
int NavMesh::Get_TotalX_grids()
{
	return xGrids;
}

int NavMesh::Get_TotalY_grids()
{
	return yGrids;
}
