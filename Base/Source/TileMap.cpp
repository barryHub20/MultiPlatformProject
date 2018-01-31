#include "TileMap.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
TileMap::TileMap()
{
}

TileMap::~TileMap()
{
	delete mesh;
}

/*********************************************************************************************************
Set
/*********************************************************************************************************/
void TileMap::Init(int total_tiles_layer, int total_layers, int start_frame, GLuint texture_ID)
{
	this->total_tiles_layer = total_tiles_layer;	//total tiles per layer
	this->total_layers = total_layers;
	this->start_frame = start_frame;

	mesh = new Mesh();
	mesh->Init_GridMap(total_tiles_layer, total_layers);
	mesh->AssignTexture(texture_ID);
}

void TileMap::AddSet(int layer_index, SET_TYPE set_type)
{
	setList_index[(int)set_type] = layer_index;
}

/*********************************************************************************************************
Get
/*********************************************************************************************************/
int TileMap::Get_TotalX() { return total_tiles_layer; }
int TileMap::Get_TotalY() { return total_layers; }
int TileMap::Get_StartFrame() { return start_frame; }
Mesh* TileMap::GetMesh() { return mesh; }