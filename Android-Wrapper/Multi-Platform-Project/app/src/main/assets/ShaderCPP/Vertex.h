// Interpolated values from the vertex shaders
attribute vec3 position;
attribute vec3 color;
attribute vec2 texCoord;

//Pass to fragment----------------------------------------//
varying vec3 vertexColor; // Specify a color output to the fragment shader
varying vec2 TexCoord;

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