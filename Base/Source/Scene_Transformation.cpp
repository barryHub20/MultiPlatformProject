#include "Scene_Transformation.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Scene_Transformation::Scene_Transformation()
{
}

Scene_Transformation::~Scene_Transformation()
{
}

/*********************************************************************************************************
Init: ALWAYS CALLED LAST
/*********************************************************************************************************/
void Scene_Transformation::Init()
{
	Scene::Init();

	//transform comp---------------------------------------------------------------------------//
	transform.Translate(-400, -300, 0);
	transform.Rotate(0, 0, 0);
	transform.CalculateTRS();

	//child transform comp---------------------------------------------------------//
	transform2.Translate(-400, -400, 0);
	transform2.CalculateTRS();

	//child child transform comp---------------------------------------------------------//
	transform3.Translate(-300, -400, 0);
	transform3.CalculateTRS();

	//add child--------------------------------------------------------------------------//
	transform.AddChild(transform2);
	transform2.AddChild(transform3);

	transform.Translate(300, 300, 0);

	//renderers------------------------------------------------------------------------//
	zombie_Renderer.Set(TM_ZOMBIE, 0.08);
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
void Scene_Transformation::Update() 
{
	Scene::Update();

	//do some transforming-------------------------------------------------//
	float speed = 250.f * CU::dt;

	//if (tmpIsKeyPressed2(VK_UP))
	//	transform.Translate(0, speed, 0);
	//if (tmpIsKeyPressed2(VK_DOWN))
	//	transform.Translate(0, -speed, 0);
	//if (tmpIsKeyPressed2(VK_LEFT))
	//	transform.Translate(-speed, 0, 0);
	//if (tmpIsKeyPressed2(VK_RIGHT))
	//	transform.Translate(speed, 0, 0);

	////Rotate transform------------------------------------------------------//
	//if (tmpIsKeyPressed2('R'))
	//	transform.Rotate(0, 0, 0.5f);

	////Add transform2 to 1------------------------------------------------------//
	//if (tmpIsKeyPressed2('P'))
	//	transform.AddChild(transform2);
	////Add transform3 to 2------------------------------------------------------//
	//if (tmpIsKeyPressed2('Q'))
	//	transform2.AddChild(transform3);

	Vector2 dir;
	if (CU::input.DPad_Left(dir))
	{
		transform.SetRotate(0.f, 0.f, Vector2::getAngleFromDir(dir));
		transform.Translate(speed, 0, 0);
	}
	if (CU::input.DPad_Right(dir))
	{
		transform2.SetRotate(0.f, 0.f, Vector2::getAngleFromDir(dir));
	}

	//set basic shader so it starts first later-----------------------------------------------------------------------//
	CU::view.SetShader(BASIC_SHADER);

	//tmp. update components-------------------------------------------------------------------//
	zombie_Renderer.Update();
}

/*********************************************************************************************************
tmp draw
/*********************************************************************************************************/
void Scene_Transformation::Tmp_Draw()
{
	Scene::Tmp_Draw();

	//called once before draw
	//Calculate TRS------------------------------------------------------//
	transform.CalculateTRS();
	transform2.CalculateTRS();
	transform3.CalculateTRS();

	//put draw here for now until we add Render component
	//transforms draw---------------------------------------------------------//
	CU::quad->AssignTexture(CU::textureList[TEX_STAR]);

	CU::view.Pre_DrawMesh(transform.Get_FinalTRS(), Vector3(100, 100, 1), CU::quad);
	CU::quad->Draw();
	CU::view.Pre_DrawMesh(transform2.Get_FinalTRS(), Vector3(50, 50, 1), CU::quad);
	CU::quad->Draw();
	CU::view.Pre_DrawMesh(transform3.Get_FinalTRS(), Vector3(50, 50, 1), CU::quad);
	CU::quad->Draw();

	//draw zombies-----------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(0, -100, 0), Vector3(100, 100, 1), zombie_Renderer.GetMesh());
	zombie_Renderer.Draw();

	CU::view.SetShader(TEXT_SHADER);
	CU::view.RenderText("TRANSFORM", Vector2(-60, 140), 0.9f, Color(5, 242, 230));

	//render text
	ss.str("");
	ss << "FPS: " << (int)CU::fps;
	CU::view.RenderText(ss.str(), Vector2(110, 150), 0.8f, Color(227, 157, 27));
}

/*********************************************************************************************************
Update press
/*********************************************************************************************************/
void Scene_Transformation::Update_KeyPressed()
{
	Scene::Update_KeyPressed();
}

/*********************************************************************************************************
Update release
/*********************************************************************************************************/
void Scene_Transformation::Update_KeyReleased()
{
	Scene::Update_KeyReleased();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Scene_Transformation::Exit()
{
	Scene::Exit();
}