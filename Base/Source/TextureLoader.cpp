#include "TextureLoader.h"
#include <iostream>
#include <fstream>
#include "CoreUtilities.h"

/*********************************************************************************************************
Load Opengl textures
/*********************************************************************************************************/
GLuint TextureLoader::Load_Textures(GLsizei width, GLsizei height, GLenum type, GLvoid* pixels)
{
	GLuint texture;

	//Gen texture---------------------//
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//texture wrapping--------------//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//texture filtering-------------------//
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);	//scale down/zoom out
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//scale up/zoom in

	//generate the texture--------------------//
	glTexImage2D(GL_TEXTURE_2D, 0, type, width, height, 0, type, GL_UNSIGNED_BYTE, pixels);
	glGenerateMipmap(GL_TEXTURE_2D);

	//mem. cleanup and unbind--------------------//
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

/*********************************************************************************************************
Load textures for PC version
GL_TEXTURE_MIN_FILTER: if in-app texture is smaller than original resolution
GL_TEXTURE_MAX_FILTER: if in-app texture is larger than original resolution
/*********************************************************************************************************/
GLuint TextureLoader::GenTexture(string address)
{
	//load textures
	FILE* fp = CU::assetMan.LoadFile(address.c_str());

	const RawImageData raw_image_data = read_png_file(fp, address.c_str());

	const GLuint texture_object_id = Load_Textures(
		raw_image_data.width, raw_image_data.height,
		raw_image_data.gl_color_format, raw_image_data.data);

	release_raw_image_data(&raw_image_data);

	return texture_object_id;
}

/*********************************************************************************************************
delete textures
/*********************************************************************************************************/
void TextureLoader::Exit()
{
}
