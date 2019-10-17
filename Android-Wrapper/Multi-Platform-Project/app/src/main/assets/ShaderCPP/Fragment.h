//From vertex------------------------------------------------//
precision mediump float;

varying vec3 vertexColor; // The input variable from the vertex shader (same name and same type)
varying vec2 TexCoord;

uniform int texture_active;
uniform sampler2D daTexture;

/********************************************************************************************************************************
Main function
********************************************************************************************************************************/
void main()
{
	if(texture_active != 0)
		gl_FragColor = texture2D(daTexture, TexCoord);
	else
		gl_FragColor = vec4(vertexColor, 1.0);
}