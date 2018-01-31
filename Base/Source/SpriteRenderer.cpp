#include "SpriteRenderer.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
SpriteRenderer::SpriteRenderer()
{
}

SpriteRenderer::~SpriteRenderer()
{
}

/*********************************************************************************************************
Set
/*********************************************************************************************************/
void SpriteRenderer::Set(TILEMAP_TYPE tilemap_type, double duration)
{
	this->tilemap_type = tilemap_type;
	this->duration = duration;
	this->min = 0;
	this->max = CU::tileMapList[tilemap_type].Get_TotalX() - 1;

	currIndex = -1;	//nothing yet
	timer = 0.f;
}

/*********************************************************************************************************
Set current set
/*********************************************************************************************************/
void SpriteRenderer::Current_Set(TileMap::SET_TYPE set_type)
{
	current_set_skip = CU::tileMapList[tilemap_type].setList_index[set_type] * CU::tileMapList[tilemap_type].Get_TotalX();

	if (currIndex == -1)	//nothing yet, set to first tile start
		currIndex = CU::tileMapList[tilemap_type].Get_StartFrame();
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
void SpriteRenderer::Update()
{
	//loop through from start to end
	timer += CU::dt;

	//loops through the indexes
	if (currIndex != -1 && timer >= duration)
	{
		timer = 0.0;
		currIndex >= max ? currIndex = min : currIndex++;
	}
}

/*********************************************************************************************************
Draw
/*********************************************************************************************************/
int curr_index2;
void SpriteRenderer::Draw()
{
	curr_index2 = current_set_skip + currIndex;

	CU::tileMapList[tilemap_type].GetMesh()->Draw_GridMap(curr_index2 * 6, (curr_index2 + 1) * 6);
}

/*********************************************************************************************************
Get mesh
/*********************************************************************************************************/
Mesh* SpriteRenderer::GetMesh()
{
	return CU::tileMapList[tilemap_type].GetMesh();
}