#ifndef VIEW_H
#define VIEW_H
#include "Header.h"
#include "shader.hpp"
#include "Mesh.h"
#include "FPS_Camera.h"
#include "Transform.h"
#include <map>
#include "TextRenderer.h"

/*********************************************************************************************************
New and improved view.

Author: Tan Yie Cher
Date: 25/12/2016

Notes:
1) View is just a abstract class, View_PC and View_Android must inherit all functions!!
2) ADD LIGHTS BEFORE CALLING VIEW INIT() !!!! If not uniforms not registered

Improvements:
-in RenderMesh(), we use if else to determine uniforms to pass in depending on shaders used, eliminate if else for each render call
-Solve jerkiness
-RenderText(), VAO pass in values at Init(), not Render()
/*********************************************************************************************************/
enum SHADER_TYPE
{
	BASIC_SHADER,
	TEXT_SHADER,
	TOTAL_SHADERS
};



/*********************************************************************************************************
Abstract class, please inherit
Date: 25/12/2016
/*********************************************************************************************************/
class View
{
public:

	//Shader---------------------------------------------//
	static GLint shaderProgramList[TOTAL_SHADERS];

	//test camera
	FPS_Cam camera;

	//basics shader
	GLint uM_Matrix, uMV_Matrix, uP_Matrix;

	//text shader
	FreeType_Text textRenderer;

	//Text shaders, DOES NOT SHARE SAME ATTRIBUTES!!!!
	GLuint text_VBO;	//text data aka mesh
	GLint text_uMV_Matrix, text_uP_Matrix, text_uColor, text_uText;
	

	//current shader
	SHADER_TYPE current_shader;

	/********************** Transformation **********************/
	//Transform utility var----------------------------//
	Mtx44 mvMatrix;   //model view matrix
	MS modelStack;	//model matrix
	MS viewStack;	//view matrix
	MS projectionStack;	//perspective matrix


	/********************** Core functions *****************************/
	//init-----------------------------------------------//
	void Init_Opengl();
	void Init_Shaders();
	void Init_Text();

	//exit----------------------------------------------//
	void Exit_Text();

public:

	/********************** Core functions *****************************/
	void Init();
	void Reset();	//changing scene etc. needs to reset lights etc.
	void Exit();

	/********************** Camera *****************************/

	/********************** Core Render functions *****************************/
	void ClearScreen();	//more like Pre-Render
	void StartRendering2D(Camera& camera);
	void StartRendering2D_onScreen();
	void StartRendering3D(Camera& camera);
	void PostRender();
	virtual void SetShader(SHADER_TYPE shaderType);

	/********************** Transformation *****************************/
	void PushMatrix();
	void PopMatrix();
	void SetIdentity();
	void LoadMatrix(const Mtx44& mat);

	/********************** Render *****************************/
	void Pre_DrawMesh(const Mtx44& loadMat, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, float rotation, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, float angle, Vector3 rotAxis, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, Vector3 eulerAngles, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, float yaw, float pitch, Vector3 yawAxis, Vector3 pitchAxis, Vector3 scale, Mesh* mesh);
	void Pre_DrawMesh(Vector3 pos, Mtx44& rotMtx, Vector3 scale, Mesh* mesh);

	//utility renders
	void Draw_Line(Vector3 origin, float angle, float length, float thickness, Mesh* mesh);

	void RenderText(string text, Vector2 pos, float yScale, Color color);

	void Translate(float x, float y, float z);
	void Rotate(float angle, float x, float y, float z);
	void Scale(float x, float y, float z);
};

#endif