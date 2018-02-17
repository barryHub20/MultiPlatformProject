#include "Mesh.h"
int Mesh::currentMesh = 0;
int Mesh::totalMesh = 0;


/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Mesh::Mesh()
{
	meshID = totalMesh++;
	texture_ID = 0;

	//buffers---------------------//
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &IBO);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
}

/*********************************************************************************************************
init this mesh as a quad
/*********************************************************************************************************/
void Mesh::Init_Quad(bool start0)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	//rectangle mesh-------------------------------------------------------------------------//
	if (start0) {
		vertex.position.Set(0.5f, 0.5f, 0.f);
		vertex.color.Set(255, 0, 255);
		vertex.tex.Set(1.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-0.5f, 0.5f, 0.f);
		vertex.color.Set(255, 0, 255);
		vertex.tex.Set(0.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-0.5f, -0.5f, 0.f);
		vertex.color.Set(0, 0, 255);
		vertex.tex.Set(0.f, 0.f);
		vertexList.push_back(vertex);

		vertex.position.Set(0.5f, -0.5f, 0.f);
		vertex.color.Set(0, 0, 255);
		vertex.tex.Set(1.f, 0.f);
		vertexList.push_back(vertex);
	}
	else
	{
		vertex.position.Set(0.5f, 0.5f, 0.f);
		vertex.color.Set(255, 0, 255);
		vertex.tex.Set(1.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-0.5f, 0.5f, 0.f);
		vertex.color.Set(255, 0, 255);
		vertex.tex.Set(0.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-0.5f, -0.5f, 0.f);
		vertex.color.Set(0, 0, 255);
		vertex.tex.Set(0.f, 0.f);
		vertexList.push_back(vertex);

		vertex.position.Set(0.5f, -0.5f, 0.f);
		vertex.color.Set(0, 0, 255);
		vertex.tex.Set(1.f, 0.f);
		vertexList.push_back(vertex);
	}

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);
	indicesList.push_back(3);
	indicesList.push_back(3);
	indicesList.push_back(1);
	indicesList.push_back(2);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

void Mesh::Init_Quad(bool start0, Vector3 scale, Color color)
{
	Init_Quad(start0, scale, color, color);
}

void Mesh::Init_Quad(bool start0, Vector3 scale, Color color, Color color2)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	if (!start0)
	{
		vertex.position.Set(scale.x * 0.5f, scale.y * 0.5f, 0.f);
		vertex.color = color;
		vertex.tex.Set(1.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-scale.x * 0.5f, scale.y * 0.5f, 0.f);
		vertex.color = color;
		vertex.tex.Set(0.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(-scale.x * 0.5f, -scale.y * 0.5f, 0.f);
		vertex.color = color2;
		vertex.tex.Set(0.f, 0.f);
		vertexList.push_back(vertex);

		vertex.position.Set(scale.x * 0.5f, -scale.y * 0.5f, 0.f);
		vertex.color = color2;
		vertex.tex.Set(1.f, 0.f);
		vertexList.push_back(vertex);
	}
	else
	{
		vertex.position.Set(scale.x, scale.y, 0.f);
		vertex.color = color;
		vertex.tex.Set(1.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(0.f, scale.y, 0.f);
		vertex.color = color;
		vertex.tex.Set(0.f, 1.f);
		vertexList.push_back(vertex);

		vertex.position.Set(0.f, 0.f, 0.f);
		vertex.color = color2;
		vertex.tex.Set(0.f, 0.f);
		vertexList.push_back(vertex);

		vertex.position.Set(scale.x, 0.f, 0.f);
		vertex.color = color2;
		vertex.tex.Set(1.f, 0.f);
		vertexList.push_back(vertex);
	}

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);
	indicesList.push_back(3);
	indicesList.push_back(3);
	indicesList.push_back(1);
	indicesList.push_back(2);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
Right angled triangle
/*********************************************************************************************************/
void Mesh::Init_RATriangle(Vector3 scale, Color color, Color color2)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	vertex.position.Set(0.f, scale.y, 0.f);
	vertex.color = color;
	vertex.tex.Set(0.f, 1.f);
	vertexList.push_back(vertex);

	vertex.position.Set(0.f, 0.f, 0.f);
	vertex.color = color2;
	vertex.tex.Set(0.f, 0.f);
	vertexList.push_back(vertex);

	vertex.position.Set(scale.x, scale.y, 0.f);
	vertex.color = color;
	vertex.tex.Set(1.f, 1.f);
	vertexList.push_back(vertex);

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);
	indicesList.push_back(2);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
draw gridmap
X -> Y
X -> Y

L -> R
Downwards

Starts from -0.5, -0.5
/*********************************************************************************************************/
void Mesh::Init_GridMap(int totalX, int totalY)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	float xScale = 1.f / (float)totalX;
	float yScale = 1.f / (float)totalY;
	float xStart = -0.5f;
	float yStart = -0.5f;

	//Y over X--------------------------------------------------//
	int counter = 0;
	for (int y = totalY - 1; y >= 0; --y)
	{
		for (int x = 0; x < totalX; ++x)
		{
			int currIndex = (y * totalY) + x;

			//rectangle mesh-------------------------------------------------------------------------//
			vertex.position.Set(0.5f, 0.5f, 0.f);
			vertex.color.Set(255, 0, 255);
			vertex.tex.Set((x + 1) * xScale, (y + 1) * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(-0.5f, 0.5f, 0.f);
			vertex.color.Set(255, 0, 255);
			vertex.tex.Set(x * xScale, (y + 1) * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(-0.5f, -0.5f, 0.f);
			vertex.color.Set(0, 0, 255);
			vertex.tex.Set(x * xScale, y * yScale);
			vertexList.push_back(vertex);

			vertex.position.Set(0.5f, -0.5f, 0.f);
			vertex.color.Set(0, 0, 255);
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
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
init with point lists
Only convex shape
MUST BE IN ANTI CLOCKWISE ORDER
/*********************************************************************************************************/
void Mesh::Init_Points_Convex(Color color, vector<Vector3>& pointList)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	//rectangle mesh-------------------------------------------------------------------------//
	for (int i = 0; i < pointList.size(); ++i)
	{
		vertex.position.Set(pointList[i].x, pointList[i].y, pointList[i].z);
		vertex.color = color;
		vertex.tex.Set(0.f, 1.f);
		vertexList.push_back(vertex);
	}

	for (int i = 0; i < pointList.size() - 2; ++i)
	{
		indicesList.push_back(0);
		indicesList.push_back(i + 1);
		indicesList.push_back(i + 2);
	}

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
obj/FBX
/*********************************************************************************************************/
void Mesh::Init_ExternalFile()
{

}

/*********************************************************************************************************
init this mesh as a circle
/*********************************************************************************************************/
void Mesh::Init_Circle(Color color, float radius)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	float total_slices = 18.f;
	float angle_per_slice = 360.f / total_slices;

	//origin (0)---------------------------------------------------------------------//
	vertex.position.Set(0.0f, 0.0f, 0.f);
	vertex.color = color;
	vertexList.push_back(vertex);

	//points-------------------------------------------------------------------------//
	float curr_angle = 0.f;
	for (int i = 0; i <= (int)total_slices; ++i)
	{
		float Y = sin(Math::DegreeToRadian(curr_angle));
		float X = cos(Math::DegreeToRadian(curr_angle));

		vertex.position.Set(radius * Y, radius * X, 0.f);
		vertex.color = color;
		vertexList.push_back(vertex);

		curr_angle += angle_per_slice;
	}

	//indices-------------------------------------------------------------------------//
	for (GLuint i = 0; i <= (GLuint)total_slices; ++i)
	{
		//indices---------------------------------------------------//
		indicesList.push_back(i + 1);
		indicesList.push_back(0);
		indicesList.push_back(i + 2);
	}



	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
init this mesh as a circle
/*********************************************************************************************************/
void Mesh::Init_Ring(Color color)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	float total_slices = 36;
	float radius = 0.5f;
	float inner_radius = 0.3f;
	float angle_per_slice = 360.f / total_slices;

	//points-------------------------------------------------------------------------//
	float curr_angle = 0.f;
	for (int i = 0; i <= total_slices; ++i)
	{
		float Y = sin(Math::DegreeToRadian(curr_angle));
		float X = cos(Math::DegreeToRadian(curr_angle));

		//outer-----------------------------------------------//
		vertex.position.Set(radius * X, radius * Y, 0.f);
		vertex.color = color;
		vertexList.push_back(vertex);

		//inner-----------------------------------------------//
		vertex.position.Set(inner_radius * X, inner_radius * Y, 0.f);
		vertex.color = color;
		vertexList.push_back(vertex);

		curr_angle += angle_per_slice;
	}

	//indices-------------------------------------------------------------------------//
	for (int i = 0; i <= total_slices; ++i)
	{
		//indices---------------------------------------------------//
		indicesList.push_back(2 * i + 0);
		indicesList.push_back(2 * i + 1);
	}

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLE_STRIP);
}

/*********************************************************************************************************
init this mesh as a triangle
/*********************************************************************************************************/
void Mesh::Init_Triangle(Vector3 points[3], Color color, Color color2)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	vertex.position.Set(points[0].x, points[0].y, points[0].z);
	vertex.color = color;
	vertex.tex.Set(0.f, 1.f);
	vertexList.push_back(vertex);

	vertex.position.Set(points[1].x, points[1].y, points[1].z);
	vertex.color = color2;
	vertex.tex.Set(0.f, 0.f);
	vertexList.push_back(vertex);

	vertex.position.Set(points[2].x, points[2].y, points[2].z);
	vertex.color = color;
	vertex.tex.Set(1.f, 1.f);
	vertexList.push_back(vertex);

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);
	indicesList.push_back(2);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
line: starting point is origin
/*********************************************************************************************************/
void Mesh::Init_Line(Color color)
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	//rectangle mesh-------------------------------------------------------------------------//
	vertex.position.Set(0.0f, 0.0f, 0.f);
	vertex.color = color;
	vertex.tex.Set(1.f, 1.f);
	vertexList.push_back(vertex);

	vertex.position.Set(1.0f, 0.0f, 0.f);
	vertex.color = color;
	vertex.tex.Set(0.f, 1.f);
	vertexList.push_back(vertex);

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_LINES);
}

void Mesh::Init_Axes()
{
	vector<Vertex> vertexList;
	vector<GLuint> indicesList;
	Vertex vertex;

	//red: X-------------------------------------------------------------------------//
	vertex.position.Set(-1.0f, 0.0f, 0.f);
	vertex.color.Set(255, 0, 0);
	vertexList.push_back(vertex);

	vertex.position.Set(1.0f, 0.0f, 0.f);
	vertex.color.Set(255, 0, 0);
	vertexList.push_back(vertex);

	//green: Y-------------------------------------------------------------------------//
	vertex.position.Set(0.0f, -1.0f, 0.f);
	vertex.color.Set(0, 255, 0);
	vertexList.push_back(vertex);

	vertex.position.Set(0.0f, 1.0f, 0.f);
	vertex.color.Set(0, 255, 0);
	vertexList.push_back(vertex);

	//blue: Z-------------------------------------------------------------------------//
	vertex.position.Set(0.0f, 0.0f, -1.f);
	vertex.color.Set(0, 0, 255);
	vertexList.push_back(vertex);

	vertex.position.Set(0.0f, 0.0f, 1.f);
	vertex.color.Set(0, 0, 255);
	vertexList.push_back(vertex);

	//indices--------------------------------------------------------------------------------//
	indicesList.push_back(0);
	indicesList.push_back(1);
	indicesList.push_back(2);
	indicesList.push_back(3);
	indicesList.push_back(4);
	indicesList.push_back(5);

	//Opengl ver. specific---------------------------------------------------------------------//
	BindMesh(vertexList, indicesList, GL_LINES);
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

void Mesh::Init_Sphere(Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertexList;
	std::vector<GLuint> indicesList;

	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for (unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for (unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.position.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			//v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertexList.push_back(v);
		}
	}
	for (unsigned stack = 0; stack < numStack; ++stack)
	{
		for (unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			indicesList.push_back((numSlice + 1) * stack + slice + 0);
			indicesList.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	BindMesh(vertexList, indicesList, GL_TRIANGLE_STRIP);
}

void Mesh::Init_Cuboid(Color color, Color color2, Vector3 scale)
{
	Vertex v;
	std::vector<Vertex> vertexList;

	v.position.Set(-scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f);	//bottom 0
	v.color = color;
	vertexList.push_back(v);

	v.position.Set(scale.x * 0.5f, -scale.y * 0.5f, -scale.z * 0.5f);	//bottom 3
	v.color = color2;
	vertexList.push_back(v);

	v.position.Set(scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f);	//top 3
	v.color = color;
	vertexList.push_back(v);

	v.position.Set(-scale.x * 0.5f, scale.y * 0.5f, -scale.z * 0.5f);	//top 0
	v.color = color2;
	vertexList.push_back(v);

	v.position.Set(-scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f);	//bottom 1
	v.color = color;
	vertexList.push_back(v);

	v.position.Set(scale.x * 0.5f, -scale.y * 0.5f, scale.z * 0.5f);	//bottom 2
	v.color = color2;
	vertexList.push_back(v);

	v.position.Set(scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);	//top 2
	v.color = color;
	vertexList.push_back(v);

	v.position.Set(-scale.x * 0.5f, scale.y * 0.5f, scale.z * 0.5f);	//top 1
	v.color = color2;
	vertexList.push_back(v);

	//new cube
	std::vector<GLuint> indicesList;

	indicesList.push_back(7);
	indicesList.push_back(4);
	indicesList.push_back(6);

	indicesList.push_back(5);
	indicesList.push_back(6);
	indicesList.push_back(4);

	indicesList.push_back(6);
	indicesList.push_back(5);
	indicesList.push_back(2);

	indicesList.push_back(1);
	indicesList.push_back(2);
	indicesList.push_back(5);

	indicesList.push_back(3);
	indicesList.push_back(7);
	indicesList.push_back(2);

	indicesList.push_back(6);
	indicesList.push_back(2);
	indicesList.push_back(7);

	indicesList.push_back(2);
	indicesList.push_back(1);
	indicesList.push_back(3);

	indicesList.push_back(0);
	indicesList.push_back(3);
	indicesList.push_back(1);

	indicesList.push_back(3);
	indicesList.push_back(0);
	indicesList.push_back(7);

	indicesList.push_back(4);
	indicesList.push_back(7);
	indicesList.push_back(0);

	indicesList.push_back(1);
	indicesList.push_back(5);
	indicesList.push_back(0);

	indicesList.push_back(4);
	indicesList.push_back(0);
	indicesList.push_back(5);

	//set the number of vertices
	BindMesh(vertexList, indicesList, GL_TRIANGLES);
}

/*********************************************************************************************************
bind mesh
/*********************************************************************************************************/
void Mesh::Internal_BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode, GLuint usage)
{
	total_indices = indiceList.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);	//bind VBO
	glBufferData(GL_ARRAY_BUFFER, vertexList.size() * sizeof(Vertex), &vertexList[0], usage);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, total_indices * sizeof(GLuint), &indiceList[0], usage);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mode = drawmode;
}

void Mesh::BindMesh(vector<Vertex>& vertexList, vector<GLuint>& indiceList, GLuint drawmode)
{
	Internal_BindMesh(vertexList, indiceList, drawmode, GL_STATIC_DRAW);
}

/*********************************************************************************************************
Assign a texture ID to it
/*********************************************************************************************************/
void Mesh::AssignTexture(GLuint texture_ID)
{
	this->texture_ID = texture_ID;
}

/*********************************************************************************************************
init
/*********************************************************************************************************/
void Mesh::Draw()
{
	//if VAO not binded before
	if (meshID != currentMesh)
	{
		currentMesh = meshID;
		//glBindVertexArray(VAO);	//PC VERSION
	}

	//ANDROID VERSION: Draw---------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//ANDROID VERSION-------------------------------------------------------------------------//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	glEnableVertexAttribArray(2);

	//Draw--------------------------------------------------------------------------------------//
	glDrawElements(mode, total_indices, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*********************************************************************************************************
draw: grid based
offset: offset by how many vertices
count: draw how many vertices
/*********************************************************************************************************/
void Mesh::Draw_GridMap(unsigned offset, unsigned count)
{
	//if VAO not binded before
	if (meshID != currentMesh)
	{
		currentMesh = meshID;
		//glBindVertexArray(VAO);	//PC VERSION
	}

	//ANDROID VERSION: Draw---------------------------------------------------------------//
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

	//ANDROID VERSION-------------------------------------------------------------------------//
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Vector3)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(Vector3)));
	glEnableVertexAttribArray(2);

	//Draw--------------------------------------------------------------------------------------//
	glDrawElements(mode, count, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

/*********************************************************************************************************
Reset current mesh: Call at start of every frame
/*********************************************************************************************************/
void Mesh::ResetCurrentMesh()
{
	currentMesh = -1;
}

GLint Mesh::Get_TextureID()
{
	return texture_ID;
}