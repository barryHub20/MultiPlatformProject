#ifndef MESH_H
#define MESH_H

#include "Header.h"
#include "Vertex.h"

/***************************************************************************************************************
New and improved mesh

Author: Tan Yie Cher
Date: 27/12/2016 (Revised: jan 2017)

Codes recycled from prev. version: July 2016

Note:
-IMPORTANT: render mesh of this type without binding to any other external VAO, ie. text VAO!!!
-PC VERSION: if a code is under PC VERSION, remove when in ANDROID VERSION
/***************************************************************************************************************/
class Mesh
{
protected:

    static int currentMesh;	//Keeps track of last rendered mesh ID
    static int totalMesh;
    int meshID;

    //Data-------------------------------------//
    GLuint VBO;
    GLuint VAO;	//PC VERSION
    GLuint IBO;
    GLuint total_indices;
    GLuint mode;
	GLuint texture_ID;

	//utilities--------------------------------//
	void Internal_BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode, GLuint usage);

public:

    //CD--------------------------------//
    Mesh();
    ~Mesh();

    //Core------------------------------//
    void Init_Quad(bool start0);
	void Init_Quad(bool start0, Vector3 scale, Color color);
	void Init_Quad(bool start0, Vector3 scale, Color color, Color color2);
	void Init_RATriangle(Vector3 scale, Color color, Color color2);
	void Init_GridMap(int totalX, int totalY);
	void Init_Circle(Color color, float radius);
	void Init_Ring(Color color);
	void Init_Triangle(Vector3 points[3], Color color, Color color2);
	void Init_Line(Color color);
	void Init_Axes();
	void Init_Sphere(Color color, unsigned numStack, unsigned numSlice, float radius);
	void Init_Cuboid(Color color, Color color2, Vector3 scale);
	void Init_ExternalFile();
	void Init_Points_Convex(Color color, vector<Vector3>& pointList);

	void AssignTexture(GLuint texture_ID);

	//define yourself-----------------------------------------------------------//
	virtual void BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode);
	void Draw();
	void Draw_GridMap(unsigned offset, unsigned count);

    //Static------------------------------//
    static void ResetCurrentMesh();

	GLint Get_TextureID();
};

#endif