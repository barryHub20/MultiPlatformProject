#include "Scene_Collision.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Scene_Continuous::Scene_Continuous()
{
}

Scene_Continuous::~Scene_Continuous()
{
}

/*********************************************************************************************************
Init: ALWAYS CALLED LAST
/*********************************************************************************************************/
void Scene_Continuous::Init()
{
	Scene::Init();
	OBB_V_OBB_3D_Init();
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
void Scene_Continuous::Update()
{
	Scene::Update();

	OBB_V_OBB_3D_Update();

	//set basic shader so it starts first later-----------------------------------------------------------------------//
	CU::view.SetShader(BASIC_SHADER);
}

/*********************************************************************************************************
tmp draw
/*********************************************************************************************************/
void Scene_Continuous::Tmp_Draw()
{
	Scene::Tmp_Draw();
	OBB_V_OBB_3D_Draw();
}

void Scene_Continuous::Tmp_DrawOnScreen()
{
	CU::view.SetShader(TEXT_SHADER);
    CU::view.StartRendering2D_onScreen();
    ss.str("");
    ss << "Dist: " << gjk_simplex_3D.closestDist;
    CU::view.RenderText(ss.str(), Vector2(-200, 130), 0.9f, Color(22, 93, 186));

    //render text
    ss.str("");
    ss << "FPS: " << (int)CU::fps;
    CU::view.RenderText(ss.str(), Vector2(85, 130), 0.9f, Color(227, 157, 27));
}

/*********************************************************************************************************
Update press
/*********************************************************************************************************/
void Scene_Continuous::Update_KeyPressed()
{
	Scene::Update_KeyPressed();
}

void Scene_Continuous::Update_KeyReleased()
{
	Scene::Update_KeyReleased();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Scene_Continuous::Exit()
{
	Scene::Exit();
}

/*********************************************************************************************************
3D
/*********************************************************************************************************/
void Scene_Continuous::OBB_V_OBB_3D_Init()
{
	poly_3d_1.Init(Vector3(-55, 0, 0), Vector3(50, 100, 120), Color(255, 0, 0), Color(0, 150, 0));
	poly_3d_1_projected.Init(Vector3(-55, 0, 0), Vector3(50, 100, 120), Color(199, 40, 196), Color(196, 201, 34));
	poly_3d_2.Init(Vector3(70, 0, 0), Vector3(70, 50, 50), Color(255, 0, 0), Color(0, 150, 0));

	//project poly 1-------------------------------------------------------------//
	poly_3d_1.ProjectShape(35.f, 25.f, Vector3(70, 0, 0));
	poly_3d_2.ProjectShape_Transform(25.f, 0.f, Vector3(0, 0, 0));
}

void Scene_Continuous::OBB_V_OBB_3D_Update()
{
	float speed = 150.f * CU::dt;

	//camera---------------------------------------------------------------------//
	CU::view.camera.Update(CU::dt);
	CU::view.camera.UpdateInput(CU::dt * 2.f);
	
	//transformation-------------------------------------------------------------//
	//if (GetAsyncKeyState('E'))
	//{
	//	poly_3d_1.yawRot(speed * 0.2f);
	//}

	//poly_3d_1.yawRot(speed * 0.1f);
	//if (GetAsyncKeyState('R'))
	//{
	//	poly_3d_1.pitchRot(speed * 0.1f);
	//}
	poly_3d_2.yawRot(speed * 0.1f);
	poly_3d_2.pitchRot(speed * 0.05f);
	/*if (GetAsyncKeyState('Y'))
		poly_3d_2.yawRot(speed);
	if (GetAsyncKeyState('U'))
		poly_3d_2.pitchRot(speed);*/

	/*if (GetAsyncKeyState('U'))
		show_3d_shapes = true;
	if (GetAsyncKeyState('I'))
		show_3d_shapes = false;*/

	/*if (GetAsyncKeyState('T'))
		poly_3d_1.Forward(1.f);
	if (GetAsyncKeyState('G'))
		poly_3d_1.Forward(-1.f);
	if (GetAsyncKeyState('F'))
		poly_3d_1.Strafe(-1.f);
	if (GetAsyncKeyState('H'))
		poly_3d_1.Strafe(1.f);

	if (GetAsyncKeyState('U'))
		poly_3d_1.Soar(1.f);
	if (GetAsyncKeyState('J'))
		poly_3d_1.Soar(-1.f);*/

	//actual implementation------------------------------------------------------//
	poly_3d_1.RecalculatePoints();
	poly_3d_2.RecalculatePoints();
	
	gjk_simplex_3D.CollisionResponse(poly_3d_1, poly_3d_1_projected, poly_3d_2);
}

void Scene_Continuous::OBB_V_OBB_3D_Draw()
{
	//axes--------------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(0,0,0), Vector3(1700, 1700, 1700), CU::axes);
	CU::axes->Draw();

	poly_3d_1.Draw();
	poly_3d_1_projected.Draw();
	poly_3d_2.Draw();

	//gjk_simplex_3D.Draw();
}