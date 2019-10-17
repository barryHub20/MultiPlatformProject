// Interpolated values from the vertex shaders
attribute vec4 position;

//Pass to fragment----------------------------------------//
varying vec2 TexCoord;

//Uniforms-----------------------------------------------//
uniform mat4 uMV_Matrix;
uniform mat4 uP_Matrix;

void main()
{
	//gl_Position = vec4(position, 1.0);
	gl_Position = uP_Matrix * uMV_Matrix * vec4(position.x, position.y, 0.0, 1.0);
	TexCoord = vec2(position.z, position.w);
}