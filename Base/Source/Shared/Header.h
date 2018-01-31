//PC memory checker
#ifndef HEADER_H
#define HEADER_H

/*************************************************************
All includes here

Author: Tan Yie Cher
Date: 23/6/2016
/*************************************************************/
#include "Mtx44.h"
#include "Vector3.h"
#include "Vector2.h"
#include "MatrixStack.h"
#include "Vector2.h"
//#include "gtc/quaternion.hpp"
#include <iostream>
#include <vector>

//include Opengl--------------------------------------//
#if CURR_PLAT_PC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "timer.h"
#include <cstdlib>
#endif

//include Opengl es--------------------------------------//
#if CURR_PLAT_ANDROID
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <android/log.h>
#include <JNIHelper.h>
#include <shader.h> //no choice
#include <GLContext.h>
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include "MyMath.h"

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>
#include "shader.hpp"
#include <map>
#include <sstream>

using namespace std;

//screen dimensions----------------------------------//
//small screen size: 800, 600
//big screen size: 1000, 750
//Extra big: 1150, 862.5
class Screen
{
	static float zoom_percent;
public:
	static float SCREEN_WIDTH;
	static float SCREEN_HEIGHT;

	// Viewing dimensions (2D/3D) ------------------------/
	static float CAM_X_TO_Y_RATIO;
	static unsigned short CAMERA_WIDTH;	//camera view size X 
	static unsigned short CAMERA_HEIGHT;	//camera view size Y
};

//textures------------------------------------------------//
enum TEXTURE_TYPE
{
	TEX_STAR,
	TEX_ZOMBIE,
	TEX_MALE_HERO,
	TEX_FEMALE,
	TEX_INNER_DPAD,
	TEX_INNER_DPAD_WASD,
	TEX_OUTER_DPAD,
	TOTAL_TEXTURES
};

//tilemap------------------------------------------------//
enum TILEMAP_TYPE
{
	TM_ZOMBIE,
	TM_HERO_MALE,
	TM_FEMALE,
	TOTAL_TILEMAPS
};

#endif