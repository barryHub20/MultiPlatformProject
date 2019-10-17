precision mediump float;

//From vertex------------------------------------------------//
varying vec2 TexCoord;

uniform sampler2D text;
uniform vec3 textColor;

/********************************************************************************************************************************
Main function
********************************************************************************************************************************/
void main()
{
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture2D(text, TexCoord).r);
	gl_FragColor = vec4(textColor, 1.0) * sampled;
	//gl_FragColor = vec4(textColor, 1.0);
}