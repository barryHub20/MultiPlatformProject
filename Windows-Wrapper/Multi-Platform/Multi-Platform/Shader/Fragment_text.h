#version 330 core

//From vertex------------------------------------------------//
in vec2 TexCoords; 

//To geometry----------------------------------------------//
out vec4 color;

uniform sampler2D text;	//bitmap of glyph
uniform vec3 textColor;

void main()
{
	//texture data stored in ALPHA component, sample r component of texture as sampled alpha value
	//so if alpha is 0, color * vec4(1, 1, 1, 0);
	//if alpha is 1, color * vec4(1, 1, 1, 1);
	vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
	color = vec4(textColor, 1.0) * sampled;
	//color = vec4(1.0, 1.0, 0.0, 1.0);
}