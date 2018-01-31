#version 330 core

// Combined pos and texture coord data into one vec4
//layout(location = 0) in vec4 vertex;
//Interpolated values from the vertex shaders
layout(location = 0) in vec4 vertex;

//Pass to fragment----------------------------------------//
out vec2 TexCoords; 

//Uniforms-----------------------------------------------//
uniform mat4 uMV_Matrix;
uniform mat4 uP_Matrix;

void main()
{
	gl_Position = uP_Matrix * uMV_Matrix * vec4(vertex.xy, 0.0, 1.0);
	TexCoords = vertex.zw;
}