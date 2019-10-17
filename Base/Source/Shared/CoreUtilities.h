#ifndef COREUTIL_H
#define COREUTIL_H

#include "View.h"
#include "TextureLoader.h"
#include "AssetManager.h"

#if CURR_PLAT_PC
#include "Input_PC.h"
#endif
#if CURR_PLAT_ANDROID
#include "Input_Android.h"
#endif

/*********************************************************************************************************
Core utility class. Contains:
-View

For utility classes that are conviniently made for both platforms

Author: Tan Yie Cher
Date: 26/12/2016
*********************************************************************************************************/
class CU
{
public:
    static bool fuck_flag;

	/******************** uni-platform utilities **********************/
	static View view;
	static AssetManager assetMan;

	/******************** platform-dependent utilities **********************/
#if CURR_PLAT_PC
	static Input_PC input;
#endif
#if CURR_PLAT_ANDROID
	static Input_Android input;
#endif
#if CURR_PLAT_ANDROID
    static pthread_mutex_t mutex_;
#endif
	static double dt;
	static float fps;

    //flags-----------------------------------------------//
    static bool initialized_resources;

	static string assets_path;

	//shared resources----------------------------------//
	//ONE EACH OF EACH TYPE
	static Mesh* circle_green;
	static Mesh* circle_darkGreen;
	static Mesh* circle_gray;
	static Mesh* circle_red;
	static Mesh* circle_blue;
	static Mesh* circle_lightblue;
	static Mesh* quad;
	static Mesh* quad2;
	static Mesh* quad_red;
	static Mesh* quad_blue;
	static Mesh* quad_green;
	static Mesh* quad_yellow;
	static Mesh* quad_purple;
	static Mesh* quad_black;
	static Mesh* quad_grid;
	static Mesh* line_blue;
	static Mesh* line_grey;
	static Mesh* line_purple;
	static Mesh* line_yellow;
	static Mesh* line_orange;
	static Mesh* line_light_blue;
	static Mesh* line_red;
	static Mesh* line_green;
	static Mesh* line_dark_green;
	static Mesh* sphere_blue;
	static Mesh* sphere_green;
	static Mesh* sphere_dark_green;
	static Mesh* sphere_red;
	static Mesh* sphere_yellow;
	static Mesh* axes;
	static GLuint textureList[TOTAL_TEXTURES];

	static Mtx44 mtx[5];

	/******************** wrapper functions **********************/
	static void Init();

	/******************** Update function: Call all AFTER corrosponding scene functions **********************/
	static void PreUpdate();

	static void Update_Stage1();
	static void Update_Stage2();
	static void Update_Stage3();
	static void Update_Stage4();

	/******************** Render **********************/
	static void DrawInWorld();
	static void DrawOnScreen();

	/******************** Utilities **********************/
	//plane/line intersection
	static bool intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t);
	static bool lineIntersection(Vector3 rayOrigin, Vector3 rayDir, float rayLimit, Vector3 linePos, Vector3 linePos_1,
		Vector3 lineNormal, float lineLen, float& rayLength);

	/******************** Exit **********************/
	static void Exit();
};

#endif