#ifndef TEXTURE_PC_H
#define TEXTURE_PC_H

#include "libpng_demo.h"

/***************************************************************************************************************
Texture loader with libpng library
Tutorial: http://www.learnopengles.com/loading-a-png-into-memory-and-displaying-it-as-a-texture-with-opengl-es-2-using-almost-the-same-code-on-ios-android-and-emscripten/

Author: Tan Yie Cher
Date: 22/1/17

Note:
-Can be used for: window/Android
/***************************************************************************************************************/
class TextureLoader
{
protected:

	//load Opengl texture
	static GLuint Load_Textures(GLsizei width, GLsizei height, GLenum type, GLvoid* pixels);

public:

	//Core------------------------------//
	static GLuint GenTexture(string address);

	static void Exit();

};

#endif