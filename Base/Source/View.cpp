#include "View.h"
#include "Controller.h"
GLint View::shaderProgramList[TOTAL_SHADERS];

//screen dimensions----------------------------------//
//very small: 600, 450
//small screen size: 800, 600
//bigger byt still small: 900, 675
//big screen size: 1000, 750
//Extra big: 1150, 862.5
float Screen::SCREEN_WIDTH = 800;	//PC only
float Screen::SCREEN_HEIGHT = 600;	//PC only
unsigned short Screen::CAMERA_WIDTH = 800.f;	//camera view size X 
unsigned short Screen::CAMERA_HEIGHT = 600.f;	//camera view size Y
float Screen::CAM_X_TO_Y_RATIO = 4.f / 3.f;
float Screen::zoom_percent = 1.f;

/********************************************************************************
Init
********************************************************************************/
void View::Init()
{
	Init_Shaders();
	Init_Opengl();

	//freetype library
	Init_Text();

	//uniforms (BASIC SHADER)-----------------------------------------------------------------------//
	uM_Matrix = glGetUniformLocation(shaderProgramList[BASIC_SHADER], "uM_Matrix");
	uMV_Matrix = glGetUniformLocation(shaderProgramList[BASIC_SHADER], "uMV_Matrix");
	uP_Matrix = glGetUniformLocation(shaderProgramList[BASIC_SHADER], "uP_Matrix");

	//uniforms (TEXT SHADER)-----------------------------------------------------------------------//
	text_uMV_Matrix = glGetUniformLocation(shaderProgramList[TEXT_SHADER], "uMV_Matrix");
	text_uP_Matrix = glGetUniformLocation(shaderProgramList[TEXT_SHADER], "uP_Matrix");
	text_uColor = glGetUniformLocation(shaderProgramList[TEXT_SHADER], "textColor");
	text_uText = glGetUniformLocation(shaderProgramList[TEXT_SHADER], "text");

	//test camera--------------------------------------------------------------------//
	//camera.Init(Vector3(0, 0, 1), Vector3(0, 0, 0), Vector3(0, 1, 0));	//2D
	//camera.Init(Vector3(0, 0, 1000), Vector3(0, 0, 500), Vector3(0, 1, 0));	//3D
	//camera.Init(Vector3(-47.6996f, 238.696f, 200.899f), Vector3(-47.485f, 238.004f, 200.21f), Vector3(0.205f, 0.722f, -0.660f));	//3D stuck to pos
	camera.Init(Vector3(-84.7776, 238.696, -291.117), Vector3(-84.5819, 238.1, -290.338), Vector3(0.145163, 0.803268, 0.577657));	//3D stuck to pos
}

/********************************************************************************
Init opengl settings
********************************************************************************/
void View::Init_Opengl()
{
	// Blue background
	glClearColor(1, 0, 0, 0.0f);

	// Enable depth test
	//glEnable(GL_DEPTH_TEST);

	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	//glEnable(GL_CULL_FACE);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glFrontFace(GL_CCW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/*********************************************************************************************************
Shaders
/*********************************************************************************************************/
void View::Init_Shaders()
{
#if CURR_PLAT_PC
		//Basics shader-------------------------------------------------------------------------//
		shaderProgramList[BASIC_SHADER] = LoadShaders("Shader//Vertex.h", "Shader//Fragment.h");
		shaderProgramList[TEXT_SHADER] = LoadShaders("Shader//Vertex_text.h", "Shader//Fragment_text.h");
#endif
#if CURR_PLAT_ANDROID
		//Load shader "/ is auto binded to asset folder)
		shaderProgramList[BASIC_SHADER] = LoadShaders("ShaderCPP/Vertex.h", "ShaderCPP/Fragment.h");
		shaderProgramList[TEXT_SHADER] = LoadShaders("ShaderCPP/Vertex_text.h", "ShaderCPP/Fragment_text.h");
#endif
}

/********************************************************************************
Init freetype glyph library
********************************************************************************/
void View::Init_Text()
{
	textRenderer.Init();

	//Gen attributes and buffer object----------------------------------------------//
	glGenBuffers(1, &text_VBO);	//buffer data

	//Buffer-----------------------------------------------//
	//the 2D quad requires 6 vertices of 4 floats each,
	//6 vertices per quad
	//4 floats as per shader, 2 for pos 2 for texcoord
	glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void View::Exit_Text()
{
	textRenderer.Exit();

	//text shader VAO and VBO-------------//
	glDeleteBuffers(1, &text_VBO);
}

/********************************************************************************
Reset
********************************************************************************/
void View::Reset()
{

}

/********************************************************************************
Clear screen
********************************************************************************/
void View::ClearScreen()
{
	Mesh::ResetCurrentMesh();

	//clear screen after each render
	if (CU::fuck_flag)
		glClearColor(1, 0, 0, 1.f);
	else
		glClearColor(1, 1, 1, 1.f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	//start rendering 2D--------------------------------------//
	StartRendering3D(camera);
}

/********************************************************************************
Clear screenvoid StartRendering2D(Camera& camera)
********************************************************************************/
Mtx44 perspective;
void View::StartRendering2D(Camera& camera)
{
	float offsetX = -Screen::CAMERA_WIDTH*0.5f + camera.position.x;
	float offsetY = -Screen::CAMERA_HEIGHT*0.5f + camera.position.y;

	perspective.SetToOrtho(offsetX, Screen::CAMERA_WIDTH + offsetX, offsetY, Screen::CAMERA_HEIGHT + offsetY, -100.f, 100.f);

	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void View::StartRendering2D_onScreen()
{
	float offsetX = -Screen::CAMERA_WIDTH*0.5f;
	float offsetY = -Screen::CAMERA_HEIGHT*0.5f;

	perspective.SetToOrtho(offsetX, Screen::CAMERA_WIDTH + offsetX, offsetY, Screen::CAMERA_HEIGHT + offsetY, -100.f, 100.f);

	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(0, 0, 1,
		0, 0, 0,
		0, 1, 0);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void View::StartRendering3D(Camera& camera)
{
	perspective.SetToPerspective(45.f, Screen::CAM_X_TO_Y_RATIO, 0.1f, 5000.f);	//3D

	projectionStack.LoadMatrix(perspective);

	// Set up the view
	viewStack.LoadIdentity();
	viewStack.LookAt(camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z);

	// Model matrix : an identity matrix (model will be at the origin)
	modelStack.LoadIdentity();
}

void View::SetShader(SHADER_TYPE shaderType)
{
	glUseProgram(shaderProgramList[shaderType]);
	glBindVertexArray(0);	//reset the VAO
	current_shader = shaderType;

	Mesh::ResetCurrentMesh();
}

/********************************************************************************
Transformation
********************************************************************************/
void View::PushMatrix()
{
	modelStack.PushMatrix();
}

void View::PopMatrix()
{
	modelStack.PopMatrix();
}

void View::SetIdentity()
{
	modelStack.LoadIdentity();
}

void View::LoadMatrix(const Mtx44& mat)
{
	modelStack.LoadMatrix(mat);
}

void View::Translate(float x, float y, float z)
{
	modelStack.Translate(x, y, z);
}

void View::Rotate(float angle, float x, float y, float z)
{
	modelStack.Rotate(angle, x, y, z);
}

void View::Scale(float x, float y, float z)
{
	modelStack.Scale(x, y, z);
}

/********************************************************************************
Draw mesh
********************************************************************************/
void View::Pre_DrawMesh(const Mtx44& loadMat, Vector3 scale, Mesh* mesh)
{
	LoadMatrix(loadMat);
	Scale(scale.x, scale.y, scale.z);	//scale the renderer comp

	//Get MV matrix---------------------------------------------------------//
	mvMatrix = viewStack.Top() * modelStack.Top();

	//Pass in uniforms------------------------------------------------------//
	glUniformMatrix4fv(uM_Matrix, 1, GL_FALSE, &modelStack.Top().a[0]);
	glUniformMatrix4fv(uMV_Matrix, 1, GL_FALSE, &mvMatrix.a[0]);
	glUniformMatrix4fv(uP_Matrix, 1, GL_FALSE, &projectionStack.Top().a[0]);

	//bind texture-------------------------------------------------------------------//
	if (mesh->Get_TextureID() != 0)
	{
		glUniform1i(glGetUniformLocation(View::shaderProgramList[current_shader], "texture_active"), 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->Get_TextureID());
		glUniform1i(glGetUniformLocation(View::shaderProgramList[current_shader], "daTexture"), 0);
	}
	else
	{
		glUniform1i(glGetUniformLocation(View::shaderProgramList[current_shader], "texture_active"), 0);
	}
}

void View::Pre_DrawMesh(Vector3 pos, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

void View::Pre_DrawMesh(Vector3 pos, float rotation, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	modelStack.Rotate(rotation, 0, 0, 1);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

void View::Pre_DrawMesh(Vector3 pos, float angle, Vector3 rotAxis, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	modelStack.Rotate(angle, rotAxis.x, rotAxis.y, rotAxis.z);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

//DONT CHANGE THE ORDER
void View::Pre_DrawMesh(Vector3 pos, Vector3 euler, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	modelStack.Rotate(euler.y, 0, 1, 0);
	modelStack.Rotate(euler.x, 1, 0, 0);
	modelStack.Rotate(euler.z, 0, 0, 1);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

void View::Pre_DrawMesh(Vector3 pos, float yaw, float pitch, 
	Vector3 yawAxis, Vector3 pitchAxis, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	modelStack.Rotate(yaw, yawAxis.x, yawAxis.y, yawAxis.z);
	modelStack.Rotate(pitch, pitchAxis.x, pitchAxis.y, pitchAxis.z);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

void View::Pre_DrawMesh(Vector3 pos, Mtx44& rotMtx, Vector3 scale, Mesh* mesh)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos);
	modelStack.MultMatrix(rotMtx);
	Pre_DrawMesh(modelStack.Top(), scale, mesh);
}

/*****************************************************************************************************************************
Utility render
*****************************************************************************************************************************/
void View::Draw_Line(Vector3 origin, float angle, float length, float thickness, Mesh* mesh)
{
	glLineWidth(thickness);
	CU::view.SetIdentity();
	CU::view.Translate(origin.x, origin.y, 0.f);
	CU::view.Rotate(angle, 0, 0, 1);

	Pre_DrawMesh(modelStack.Top(), Vector3(length, 1.f, 1.f), mesh);
	mesh->Draw();
	glLineWidth(1.f);
}

/*****************************************************************************************************************************
Text render
Directly renders in projection space
*****************************************************************************************************************************/
void View::RenderText(string text, Vector2 pos, float yScale, Color color)
{
	modelStack.LoadIdentity();
	modelStack.Translate(pos.x, pos.y, 1.f);
	modelStack.Scale(yScale, yScale, 1.f);

	//Get MV matrix---------------------------------------------------------//
	mvMatrix = viewStack.Top() * modelStack.Top();

	//Uniforms--------------------------------------------------------//
	glUniformMatrix4fv(text_uMV_Matrix, 1, GL_FALSE, &mvMatrix.a[0]);
	glUniformMatrix4fv(text_uP_Matrix, 1, GL_FALSE, &projectionStack.Top().a[0]);
	glUniform3fv(text_uColor, 1, &color.r);

	//Bind Texture and VAO-------------------------------------------------------//
	glActiveTexture(GL_TEXTURE0);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		TextCharacter ch = textRenderer.characters[*c];

		/*GLfloat xpos = 0.f;
		GLfloat ypos = 0.f;
		GLfloat w = 100.f;
		GLfloat h = 100.f;*/
		GLfloat xpos = pos.x + ch.bearing.x * yScale;
		GLfloat ypos = pos.y - (ch.size.y - ch.bearing.y) * yScale;
		GLfloat w = ch.size.x * yScale;
		GLfloat h = ch.size.y * yScale;

		// Update VBO for each character
        		GLfloat vertices[6][4] = {
                            { xpos,     ypos + h,   0.0, 0.0 },
                            { xpos,     ypos,       0.0, 1.0 },
                            { xpos + w, ypos,       1.0, 1.0 },

                            { xpos,     ypos + h,   0.0, 0.0 },
                            { xpos + w, ypos,       1.0, 1.0 },
                            { xpos + w, ypos + h,   1.0, 0.0 }
                        };

		//Pass in texture-----------------------------------------------------//
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		//glBindTexture(GL_TEXTURE_2D, CU::textureList[0]); works in Android??
		//glUniform1i(text_uText, 0);

		// Update content of VBO memory-----------------------------------------------------//
		glBindBuffer(GL_ARRAY_BUFFER, text_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);	//update buffer with new data
		//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertices, GL_DYNAMIC_DRAW);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);

		//ANDROID VERSION-------------------------------------------------------------------------//
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
		glEnableVertexAttribArray(0);

		//Draw--------------------------------------------------------------------------------------//
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//unbind-----------------------------------------------------------------------------------//
		glDisableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		pos.x += (ch.advance >> 6) * yScale;
	}

	//Unbind texture----------------------------------------------------//
	glBindTexture(GL_TEXTURE_2D, 0);
}

/********************************************************************************
Core
********************************************************************************/
void View::PostRender()
{
	Mesh::ResetCurrentMesh();
	modelStack.Clear();
	projectionStack.Clear();
	viewStack.Clear();
}

void View::Exit()
{
	Exit_Text();

	for (int i = 0; i < TOTAL_SHADERS; ++i)
	{
		glDeleteProgram(shaderProgramList[i]);
		shaderProgramList[i] = 0;
	}
}

