#include "GridMesh.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Debug_GridMesh::Debug_GridMesh()
{
}

Debug_GridMesh::~Debug_GridMesh()
{
}

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
void Debug_GridMesh::Init_Debug_GridMap(int totalX, int totalY)
{
	this->totalX = totalX;
	this->totalY = totalY;

	vector<GLuint> indicesList;
	Vertex vertex;

	float xScale = 1.f / (float)totalX;
	float yScale = 1.f / (float)totalY;
	float xStart = 0.0f;
	float yStart = 0.0f;

	//Y over X--------------------------------------------------//
	int counter = 0;
	for (int y = 0; y < totalY; ++y)
	{
		for (int x = 0; x < totalX; ++x)
		{
			int currIndex = (y * totalY) + x;
			
			//rectangle mesh-------------------------------------------------------------------------//
			vertex.position.Set(xStart + (x + 1) * xScale, yStart + (y + 1) * yScale, 0.f);
			vertex.color.Set(0, 0, 255);
			vertex.tex.Set((x + 1) * xScale, (y + 1) * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(xStart + x * xScale, yStart + (y + 1) * yScale, 0.f);
			vertex.color.Set(255, 0, 255);
			vertex.tex.Set(x * xScale, (y + 1) * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(xStart + x * xScale, yStart + y * yScale, 0.f);
			vertex.color.Set(255, 0, 255);
			vertex.tex.Set(x * xScale, y * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(xStart + (x + 1) * xScale, yStart + y * yScale, 0.f);
			vertex.color.Set(255, 0, 255);
			vertex.tex.Set((x + 1) * xScale, y * yScale);
			vertexList.push_back(vertex);

			//indices---------------------------------------------//
			indicesList.push_back(counter + 0);
			indicesList.push_back(counter + 1);
			indicesList.push_back(counter + 3);
			indicesList.push_back(counter + 3);
			indicesList.push_back(counter + 1);
			indicesList.push_back(counter + 2);
			counter += 4;
		}
	}

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);;
}

/*********************************************************************************************************
Bind vertex buffer only
/*********************************************************************************************************/
void Debug_GridMesh::Bind_VertexBuffer()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertexList.size() * sizeof(Vertex), &vertexList[0]);
}

/*********************************************************************************************************
Tile operations
/*********************************************************************************************************/
void Debug_GridMesh::SetAllTiles_Green()
{
	for (int y = 0; y < totalY; ++y)
	{
		for (int x = 0; x < totalX; ++x)
			SetTile_Green(x, y);
	}
}

void Debug_GridMesh::SetAllTiles_Blue()
{
	for (int y = 0; y < totalY; ++y)
	{
		for (int x = 0; x < totalX; ++x)
			SetTile_Blue(x, y);
	}
}

void Debug_GridMesh::SetAllTiles_Red()
{
	for (int y = 0; y < totalY; ++y)
	{
		for (int x = 0; x < totalX; ++x)
			SetTile_Red(x, y);
	}
}

void Debug_GridMesh::SetTile_Green(int x, int y)
{
	//convert X and Y to single index
	SetTile_Green(totalX * y + x);
}

void Debug_GridMesh::SetTile_Blue(int x, int y)
{
	//convert X and Y to single index
	SetTile_Blue(totalX * y + x);
}

void Debug_GridMesh::SetTile_Red(int x, int y)
{
	//convert X and Y to single index
	SetTile_Red(totalX * y + x);
}

void Debug_GridMesh::SetTile_Green(int index)
{
	//mul. by 4 (4 vertices)
	index *= 4;

	//collapse the tile
	vertexList[index + 0].color.Set(0, 255, 0);
	vertexList[index + 1].color.Set(255, 255, 255);
	vertexList[index + 2].color.Set(0, 255, 0);
	vertexList[index + 3].color.Set(255, 255, 255);
}

void Debug_GridMesh::SetTile_Blue(int index)
{
	//mul. by 4 (4 vertices)
	index *= 4;

	//collapse the tile
	vertexList[index + 0].color.Set(0, 0, 255);
	vertexList[index + 1].color.Set(0, 255, 255);
	vertexList[index + 2].color.Set(0, 255, 255);
	vertexList[index + 3].color.Set(0, 255, 255);
}

void Debug_GridMesh::SetTile_Red(int index)
{
	//mul. by 4 (4 vertices)
	index *= 4;

	//collapse the tile
	vertexList[index + 0].color.Set(255, 0, 0);
	vertexList[index + 1].color.Set(255, 255, 0);
	vertexList[index + 2].color.Set(255, 255, 0);
	vertexList[index + 3].color.Set(255, 255, 0);
}

void Debug_GridMesh::UpdateMesh()
{
	Bind_VertexBuffer();
}

/*********************************************************************************************************
Bind mesh
/*********************************************************************************************************/
void Debug_GridMesh::BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode)
{
	Internal_BindMesh(vertexList, indiceList, drawmode, GL_DYNAMIC_DRAW);
}
