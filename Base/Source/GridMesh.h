#ifndef GRID_MESH_H
#define GRID_MESH_H

#include "Mesh.h"

/***************************************************************************************************************
Debug grid mesh: change the colro of selected tiles

Author: Tan Yie Cher
Date: 26/2/17

Tile type:
Off: green
On: yellow and more opaque
/***************************************************************************************************************/
class Debug_GridMesh : public Mesh
{
protected:

	//Data-------------------------------------//
	int totalX, totalY;
	vector<Vertex> vertexList;

	//utilities------------------------------------//
	void Bind_VertexBuffer();

public:

	//CD--------------------------------//
	Debug_GridMesh();
	~Debug_GridMesh();

	//Core------------------------------//
	void Init_Debug_GridMap(int totalX, int totalY);

	//user------------------------------------------//
	void SetAllTiles_Green();
	void SetAllTiles_Blue();
	void SetAllTiles_Red();
	void SetTile_Green(int x, int y);
	void SetTile_Blue(int x, int y);
	void SetTile_Red(int x, int y);
	void SetTile_Green(int index);
	void SetTile_Blue(int index);
	void SetTile_Red(int index);

	void UpdateMesh();	//call in each frame if there's an edit

	//define yourself-----------------------------------------------------------//
	void BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode) override;
};

#endif