#include "Scene_SAT_Collision.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Scene_SAT_Collision::Scene_SAT_Collision()
{
}

Scene_SAT_Collision::~Scene_SAT_Collision()
{
}

/*********************************************************************************************************
Init: ALWAYS CALLED LAST
/*********************************************************************************************************/
void Scene_SAT_Collision::Init()
{
	Scene::Init();

	//transform comp---------------------------------------------------------------------------//
	float posX = -200.f;
	float posY = 0.f;
	float width = 100.f;

	//player parent-----------------------------------------//
	player_parent.Translate(posX, posY, 0);
	player_parent.Rotate(0, 0, 0);
	player_parent.CalculateTRS();

	//player--------------------------------------------------//
	player.Init(Vector3(posX, posY, 0), Vector3(100, 50, 1));

	//player 2----------------------------------------------//
	player2.Init(Vector3(posX + 100.f, posY, 0), Vector3(100, 10, 1));

	//obstacle parent-----------------------------------------//
	posX = 50.f;
	posY = -50.f;
	obstacle_parent.Translate(posX, posY, 0);
	obstacle_parent.Rotate(0, 0, 0);
	obstacle_parent.CalculateTRS();

	//obstacle------------------------------------------------//
	obstacle.Init(Vector3(posX - 50, posY, 0), Vector3(120, 50, 1));

	//obstacle 2--------------------------------------------------//
	obstacle2.Init(Vector3(posX + 100.f, posY + 50.f, 0), Vector3(180, 50, 1));

	//Add child--------------------------------------------------//
	player_parent.AddChild(player);
	player.AddChild(player2);
	obstacle_parent.AddChild(obstacle);
	obstacle_parent.AddChild(obstacle2);

	//DPad modification-----------------------------------------------//
	CU::input.dpad_L.SetTexture(TEX_INNER_DPAD_WASD, TEX_INNER_DPAD, TEX_OUTER_DPAD);

	FP_multiplier = 3.f;	//3 times collision check
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
void Scene_SAT_Collision::Update()
{
	Scene::Update();

	//FP reset-----------------------------------------------------------------//
	FP_reset();

	//pre-update must call-------------------------------------------------//
	player.PreUpdate();
	player2.PreUpdate();
	obstacle.PreUpdate();
	obstacle2.PreUpdate();

	//do some transforming-------------------------------------------------//
	Vector2 dir;
	float speed = 140.f * CU::dt;
	float rotate_speed = 100.f * CU::dt;

	/*if (GetAsyncKeyState('A'))
		TranslatePlayer(0, speed, 0);
	if (GetAsyncKeyState('D'))
		TranslatePlayer(0, -speed, 0);
	if (GetAsyncKeyState('S'))
		TranslatePlayer(-speed, 0, 0);
	if (GetAsyncKeyState('W'))
		TranslatePlayer(speed, 0, 0);

	if (GetAsyncKeyState(VK_LEFT))
		RotatePlayer(0.f, 0.f, rotate_speed);
	if (GetAsyncKeyState(VK_RIGHT))
		RotatePlayer(0.f, 0.f, -rotate_speed);*/

	DPAD_controls();

	//obstacle rotate------------------------------------------------------//
	RotateObstacle2(0, 0, 50 * CU::dt);

	//check collision---------------------------------------------------------//
	FP_CollisionCheck();

	//set basic shader so it starts first later-----------------------------------------------------------------------//
	CU::view.SetShader(BASIC_SHADER);
}

/*********************************************************************************************************
D-PAD controls
/*********************************************************************************************************/
void Scene_SAT_Collision::DPAD_controls()
{
	//Vector2 dir;
	//float speed = 250.f * CU::dt;
	//float rotate_speed = 200.f * CU::dt;

	////rotation with D-PAD-----------------------------------------------------------------------------------//
	//float threshold = rotate_speed * 2.f;
	//float positive_dir_angle = 0.f;
	//float negative_dir_angle = 0.f;
	//if (CU::input.DPad_Right(dir))
	//{
	//	//calculate angles----------------------------------------------------------//
	//	float curr_angle = player_parent.GetRotation(false).z;
	//	float pad_angle = Vector2::getAngleFromDir(dir);
	//	if (pad_angle < 0.f)
	//		pad_angle += 360.f;
	//	if (curr_angle < 0.f)
	//		curr_angle += 360.f;
	//	else if (curr_angle > 360.f)
	//		curr_angle -= 360.f;
	//	
	//	//if dpad has higher angle value-------------------------------------------//
	//	if (pad_angle > curr_angle)
	//	{
	//		positive_dir_angle = pad_angle - curr_angle;
	//		negative_dir_angle = curr_angle - (pad_angle - 360.f);
	//	}

	//	//if dpad has lower angle value-------------------------------------------//
	//	else if (pad_angle <= curr_angle)
	//	{
	//		positive_dir_angle = (360.f - curr_angle) + pad_angle;
	//		negative_dir_angle = curr_angle - pad_angle;
	//	}

	//	//no rotation needed if reached threshold range-----------------------------------//
	//	if (positive_dir_angle > threshold && negative_dir_angle > threshold)
	//	{
	//		//rotate according to direction------------------------------------------------//
	//		if (positive_dir_angle < negative_dir_angle)	//add angle
	//			player_parent.Rotate(0.f, 0.f, rotate_speed);
	//		else
	//			player_parent.Rotate(0.f, 0.f, -rotate_speed);
	//	}
	//}

	////direction with D-PAD-----------------------------------------------------------------------------------//
	//if (CU::input.DPad_Left(dir))
	//{
	//	//see the quadrant current angle is in
	//	float pad_angle = Vector2::getAngleFromDir(dir);
	//	if (pad_angle >= 360.f)pad_angle -= 360.f;
	//	else if (pad_angle < 0.f)pad_angle += 360.f;
	//	if ((pad_angle >= 315.f && pad_angle <= 360.f) || (pad_angle >= 0.f && pad_angle < 45.f))
	//	{
	//		CU::input.dpad_L.SetRotate(315.f);
	//		player_parent.Translate(0, -speed, 0);
	//	}
	//	else if (pad_angle >= 45.f && pad_angle < 135.f)	//W
	//	{
	//		CU::input.dpad_L.SetRotate(45.f);
	//		player_parent.Translate(speed, 0, 0);
	//	}
	//	else if (pad_angle >= 135.f && pad_angle < 225.f)
	//	{
	//		CU::input.dpad_L.SetRotate(135.f);
	//		player_parent.Translate(0, speed, 0);
	//	}
	//	else if (pad_angle >= 225.f && pad_angle < 315.f)	//S
	//	{
	//		CU::input.dpad_L.SetRotate(225.f);
	//		player_parent.Translate(-speed, 0, 0);
	//	}
	//}
}

/*********************************************************************************************************
framerate proof
/*********************************************************************************************************/
void Scene_SAT_Collision::FP_reset()
{
	player_vel.SetZero();
	player_angle.SetZero();
	obstacle2_angle.SetZero();
}

void Scene_SAT_Collision::TranslatePlayer(float x, float y, float z)
{
	player_vel += Vector3(x, y, z);
	player_FP_transformed = true;
}
void Scene_SAT_Collision::RotatePlayer(float x, float y, float z)
{
	player_angle += Vector3(x, y, z);
	player_FP_transformed = true;
}
void Scene_SAT_Collision::RotateObstacle2(float x, float y, float z)
{
	obstacle2_angle += Vector3(x, y, z);
	obstacle2_FP_transformed = true;
}

void Scene_SAT_Collision::FP_CollisionCheck()
{
	//loop through
	Vector3 tmp_vel;
	float mul = 1.f / FP_multiplier;
	float ori_p2_x = player2.position.x;
	for (int i = 0; i < (int)FP_multiplier; ++i)
	{
		//trans + rotate player
		if (player_FP_transformed)
		{
			player_parent.Translate(player_vel.x * mul, player_vel.y * mul, player_vel.z * mul);
			player_parent.Rotate(player_angle.x * mul, player_angle.y * mul, player_angle.z * mul);
		}

		//trans + rotate obstacle 2
		if(obstacle2_FP_transformed)
			obstacle2.Rotate(obstacle2_angle.x * mul, obstacle2_angle.y * mul, obstacle2_angle.z * mul);

		if (i == 0)
		{
			//Calculate TRS for all objects
			TRS_calculations();
		}

		//collision check
		player.CollisionCheck(obstacle, tmp_vel);
		player2.CollisionCheck(obstacle, tmp_vel);
		player.CollisionCheck(obstacle2, tmp_vel);
		player2.CollisionCheck(obstacle2, tmp_vel);
	}
}

void Scene_SAT_Collision::TRS_calculations()
{
	player_parent.CalculateTRS();
	obstacle_parent.CalculateTRS();
	player.CalculateTRS();
	player2.CalculateTRS();
	obstacle.CalculateTRS();
	obstacle2.CalculateTRS();
}

/*********************************************************************************************************
tmp draw
/*********************************************************************************************************/
void Scene_SAT_Collision::Tmp_Draw()
{
	Scene::Tmp_Draw();

	//axes--------------------------------------------------------------//
	//CU::view.Pre_DrawMesh(Vector3(0,0,0), Vector3(700, 700, 1), CU::axes);
	//CU::axes->Draw();

	//put draw here for now until we add Render component
	//transforms draw---------------------------------------------------------//
	player.Draw();
	player2.Draw();
	obstacle.Draw();
	obstacle2.Draw();
}

void Scene_SAT_Collision::Tmp_DrawOnScreen()
{
	CU::view.SetShader(TEXT_SHADER);
	CU::view.RenderText("SAT COLLISION", Vector2(-160, 140), 0.9f, Color(5, 242, 230));

	//render text
	ss.str("");
	ss << "FPS: " << (int)CU::fps;
	CU::view.RenderText(ss.str(), Vector2(110, 150), 0.8f, Color(227, 157, 27));
}

/*********************************************************************************************************
Update press
/*********************************************************************************************************/
void Scene_SAT_Collision::Update_KeyPressed()
{
	Scene::Update_KeyPressed();
}

/*********************************************************************************************************
Update release
/*********************************************************************************************************/
void Scene_SAT_Collision::Update_KeyReleased()
{
	Scene::Update_KeyReleased();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Scene_SAT_Collision::Exit()
{
	Scene::Exit();
}