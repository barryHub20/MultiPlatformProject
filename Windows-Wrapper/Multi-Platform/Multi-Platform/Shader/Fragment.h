#version 330 core

//From vertex------------------------------------------------//
in vec3 vertexColor; // The input variable from the vertex shader (same name and same type)
in vec2 TexCoord;
out vec4 color;

uniform int texture_active;
uniform sampler2D daTexture;

/********************************************************************************************************************************
Main function
********************************************************************************************************************************/
void main()
{
	if(texture_active != 0)
		color = texture(daTexture, TexCoord);
	else
		color = vec4(vertexColor, 1.0);
}