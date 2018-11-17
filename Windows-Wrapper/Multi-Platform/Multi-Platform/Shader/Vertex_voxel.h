#version 330 core

// Interpolated values from the vertex shaders
layout(location = 0) in vec3 position;
layout(location = 1) in int blockType;

//Pass to fragment----------------------------------------//
out vec3 vertexColor; // Specify a color output to the fragment shader
out vec2 TexCoord;

//Uniforms-----------------------------------------------//
uniform mat4 uM_Matrix;
uniform mat4 uMV_Matrix;
uniform mat4 uP_Matrix;

void main()
{
	//gl_Position = vec4(position, 1.0);
	gl_Position = uP_Matrix * uMV_Matrix * vec4(position, 1.0);
	vertexColor = color;
	TexCoord = texCoord;
}