#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
using namespace std;

#include <stdlib.h>
#include <string.h>

#include "shader.hpp"

GLuint LoadShaders(const char * vertex_file_path, const char * fragment_file_path){

	GLuint ProgramID;

	GLuint vert_shader, frag_shader;


	// Create shader program
	ProgramID = glCreateProgram();
	LOGI("Created Shader %d", ProgramID);

	// Create and compile vertex shader
	if (!ndk_helper::shader::CompileShader(&vert_shader, GL_VERTEX_SHADER, vertex_file_path))
	{
		LOGI("Failed to compile vertex shader");
		glDeleteProgram(ProgramID);
		return false;
	}

	// Create and compile fragment shader
	if (!ndk_helper::shader::CompileShader(&frag_shader, GL_FRAGMENT_SHADER, fragment_file_path))
	{
		LOGI("Failed to compile fragment shader");
		glDeleteProgram(ProgramID);
		return false;
	}

	// Attach vertex shader to program
	glAttachShader(ProgramID, vert_shader);

	// Attach fragment shader to program
	glAttachShader(ProgramID, frag_shader);

	// Link program
	if (!ndk_helper::shader::LinkProgram(ProgramID))
	{
		LOGI("Failed to link program: %d", ProgramID);

		if (vert_shader)
		{
			glDeleteShader(vert_shader);
			vert_shader = 0;
		}
		if (frag_shader)
		{
			glDeleteShader(frag_shader);
			frag_shader = 0;
		}
		/*if( ProgramID )
		{
		glDeleteProgram( ProgramID );
		}*/
		__android_log_print(ANDROID_LOG_INFO, "TestApp", "FAIL SHADERS");
		return 0;
	}
	__android_log_print(ANDROID_LOG_INFO, "TestApp", "PASS SHADERS");
	return ProgramID;
}


