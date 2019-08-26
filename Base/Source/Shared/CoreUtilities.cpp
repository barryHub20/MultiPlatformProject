#include "CoreUtilities.h"

View CU::view;
AssetManager CU::assetMan;

#if CURR_PLAT_PC
Input_PC CU::input;
#endif
#if CURR_PLAT_ANDROID
Input_Android CU::input;
#endif

bool CU::fuck_flag = false;
double CU::dt = 0.0;
float CU::fps = 0.f;
string CU::assets_path;
bool CU::initialized_resources = false;
//pthread_mutex_t CU::mutex_;

Mesh* CU::circle_green;
Mesh* CU::circle_gray;
Mesh* CU::circle_red;
Mesh* CU::circle_blue;
Mesh* CU::circle_darkGreen;
Mesh* CU::circle_lightblue;
Mesh* CU::quad;
Mesh* CU::quad2;
Mesh* CU::quad_red;
Mesh* CU::quad_blue;
Mesh* CU::quad_green;
Mesh* CU::quad_yellow;
Mesh* CU::quad_purple;
Mesh* CU::quad_black;
Mesh* CU::quad_grid;
Mesh* CU::line_blue;
Mesh* CU::line_grey;
Mesh* CU::line_yellow;
Mesh* CU::line_purple;
Mesh* CU::line_red;
Mesh* CU::line_light_blue;
Mesh* CU::line_orange;
Mesh* CU::line_green;
Mesh* CU::line_dark_green;
Mesh* CU::sphere_blue;
Mesh* CU::sphere_green;
Mesh* CU::sphere_dark_green;
Mesh* CU::sphere_red;
Mesh* CU::sphere_yellow;
Mesh* CU::axes;
GLuint CU::textureList[TOTAL_TEXTURES];
Mtx44 CU::mtx[5];

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Init()
{
    initialized_resources = true;

#if CURR_PLAT_PC
	assets_path = "../../../../../Base/Assets/";
#endif
#if CURR_PLAT_ANDROID
	//assets_paths[ANDROID_PLAT] = "/sdcard/";	//external storage
	assets_path = "";    //cannot start with slash
#endif

	//gen texture-------------------------------------------------------------------------//
	textureList[TEX_INNER_DPAD] = TextureLoader::GenTexture(assets_path + "textures/d_pad_inner.png");
	textureList[TEX_INNER_DPAD_WASD] = TextureLoader::GenTexture(assets_path + "textures/d_pad_inner_wasd.png");
	textureList[TEX_OUTER_DPAD] = TextureLoader::GenTexture(assets_path + "textures/d_pad_outer.png");

	//tilemap-----------------------------------------------------------------------------//
	
	//shared meshes-----------------------------------------------------------------------//
	circle_green = new Mesh();
	circle_gray = new Mesh();
	circle_red = new Mesh();
	circle_blue = new Mesh();
	circle_darkGreen = new Mesh();
	circle_lightblue = new Mesh();
	quad = new Mesh();
	quad2 = new Mesh();
	quad_red = new Mesh();
	quad_blue = new Mesh();
	quad_green = new Mesh();
	quad_yellow = new Mesh();
	quad_purple = new Mesh();
	quad_black = new Mesh();
	quad_grid = new Mesh();
	line_blue = new Mesh();
	line_yellow = new Mesh();
	line_red = new Mesh();
	line_grey = new Mesh();
	line_purple = new Mesh();
	line_light_blue = new Mesh();
	line_orange = new Mesh();
	line_green = new Mesh();
	line_dark_green = new Mesh();
	sphere_blue = new Mesh();
	sphere_green = new Mesh();
	sphere_dark_green = new Mesh();
	sphere_red = new Mesh();
	sphere_yellow = new Mesh();
	axes = new Mesh();

	circle_green->Init_Circle(Color(0, 255, 0), 0.5f);
	circle_gray->Init_Circle(Color(162, 163, 160), 0.5f);
	circle_red->Init_Circle(Color(255, 0, 0), 0.5f);
	circle_blue->Init_Circle(Color(113, 212, 245), 0.5f);
	circle_lightblue->Init_Circle(Color(110, 200, 230), 0.5f);
	circle_darkGreen->Init_Circle(Color(0, 100, 0), 0.5f);
	quad->Init_Quad(false);
	quad2->Init_Quad(false);
	quad_red->Init_Quad(true, Vector3(1, 1, 1), Color(255, 0, 0));
	quad_blue->Init_Quad(true, Vector3(1, 1, 1), Color(0, 0, 255), Color(100, 100, 0));
	quad_green->Init_Quad(true, Vector3(1, 1, 1), Color(0, 255, 0));
	quad_yellow->Init_Quad(true, Vector3(1, 1, 1), Color(236, 242, 44));
	quad_purple->Init_Quad(true, Vector3(1, 1, 1), Color(255, 0, 255), Color(0, 100, 100));
	quad_black->Init_Quad(true, Vector3(1, 1, 1), Color(0, 0, 0));
	quad_grid->Init_GridMap(8, 8);
	line_blue->Init_Line(Color(0, 0, 255));
	line_yellow->Init_Line(Color(232, 136, 9));
	line_red->Init_Line(Color(255, 0, 0));
	line_grey->Init_Line(Color(138, 138, 138));
	line_purple->Init_Line(Color(159, 61, 186));
	line_light_blue->Init_Line(Color(110, 200, 230));
	line_green->Init_Line(Color(0, 255, 0));
	line_dark_green->Init_Line(Color(0, 100, 0));
	line_orange->Init_Line(Color(164, 186, 50));
	sphere_blue->Init_Sphere(Color(0, 0, 255), 36, 36, 0.5f);
	sphere_green->Init_Sphere(Color(0, 255, 0), 36, 36, 0.5f);
	sphere_dark_green->Init_Sphere(Color(0, 100, 0), 36, 36, 0.5f);
	sphere_red->Init_Sphere(Color(255, 0, 0), 36, 36, 0.5f);
	sphere_yellow->Init_Sphere(Color(255, 255, 0), 36, 36, 0.5f);
	axes->Init_Axes();

	//pthread_mutex_init(&mutex_, NULL);

	view.Init();
	input.Init();
}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::PreUpdate()
{
	//input must update first---------------------------------------------//
	input.Update();
}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Update_Stage1()
{

}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Update_Stage2()
{

}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Update_Stage3()
{

}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Update_Stage4()
{

}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::DrawInWorld()
{
}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::DrawOnScreen()
{
	//draw input last
	//input.Draw();
}

/*********************************************************************************************************
n: normal of the plane (pass in as -normal)
p0: pt0
l0: origin of ray
l: ray normal
t: store the pen dist along the ray
/*********************************************************************************************************/
bool CU::intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t)
{
	float denom = n.Dot(l);

	if (denom > 1e-6)
	{
		Vector3 p0l0 = p0 - l0;
		t = p0l0.Dot(n) / denom;
		return (t >= 0);
	}

	return false;
}

bool CU::lineIntersection(Vector3 rayOrigin, Vector3 rayDir, float rayLimit, Vector3 linePos, 
	Vector3 linePos_1, Vector3 lineNormal, float lineLen, float& rayLength)
{
	Vector3 intersectPos;
	float t = -1.f;
	bool intersectedPlane = intersectPlane(-lineNormal, linePos, rayOrigin, rayDir, t);

	if (intersectedPlane)
	{
		if (t > rayLimit)
			return false;

		intersectPos = rayOrigin + rayDir * t;

		//out of range of the plane width
		float face_lensq = (linePos - linePos_1).LengthSquared();
		float pt0_to_proj = (intersectPos - linePos).LengthSquared();
		float pt1_to_proj = (intersectPos - linePos_1).LengthSquared();
		intersectedPlane = pt1_to_proj < face_lensq && pt0_to_proj < face_lensq;
	}

	if (intersectedPlane)
		rayLength = t;

	return intersectedPlane;
}

/*********************************************************************************************************
PC ver. reset
/*********************************************************************************************************/
void CU::Exit()
{
	input.Exit();
	view.Exit();

	for (int i = 0; i < TOTAL_TEXTURES; ++i)
		glDeleteTextures(1, &textureList[i]);
	
	//pthread_mutex_destroy(&mutex_);
	delete circle_green;
	delete circle_gray;
	delete circle_red;
	delete circle_blue;
	delete circle_lightblue;
	delete circle_darkGreen;
	delete line_grey;
	delete quad;
	delete quad2;
	delete quad_red;
	delete quad_blue;
	delete quad_green;
	delete quad_yellow;
	delete quad_purple;
	delete quad_black;
	delete quad_grid;
	delete line_blue;
	delete line_yellow;
	delete line_purple;
	delete line_light_blue;
	delete line_orange;
	delete line_green;
	delete line_dark_green;
	delete line_red;
	delete sphere_blue;
	delete sphere_green;
	delete sphere_dark_green;
	delete sphere_red;
	delete sphere_yellow;
	delete axes;
}