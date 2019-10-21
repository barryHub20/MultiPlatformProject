#include "Scene_Pathfinding.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Scene_Pathfinding::Scene_Pathfinding()
{
}

Scene_Pathfinding::~Scene_Pathfinding()
{
}

/*********************************************************************************************************
Init: ALWAYS CALLED LAST
/*********************************************************************************************************/
void Scene_Pathfinding::Init()
{
	Scene::Init();

	//test debug grid mesh-------------------------------------------------------------//
	navmesh.SetNav_Space(20, 20, Vector2(-200, -200), 20.f);
	int wallY = 3;
	int wallX = 6;
	int wallX_2 = 16;
	int wallY_2 = 0;

	for(int i = 0; i < 15; ++i)
		navmesh.Add_Wall(wallX, wallY + i);

	navmesh.Remove_Wall(wallX, wallY + 7);
	
	for (int i = 0; i < 19; ++i)
		navmesh.Add_Wall(wallX_2, wallY_2 + i);

	//navmesh.Remove_Wall(wallX_2, wallY_2 + 3);

	navmesh.Reset_GridMesh();

	//renderers------------------------------------------------------------------------//
	float speed = 7.f;
	int wall = -2;
	float NPC_z = -1.f;
	float NPC_scale = 40.f;
	TILEMAP_TYPE NPC_tilemap_type = TM_FEMALE;

	for (int i = 0; i < 5; ++i)
		npc_list[i].Init(NPC_tilemap_type, Vector3(wall++ * 70, -199, NPC_z += 0.01f), Vector2(NPC_scale, NPC_scale), speed *= 2.f);

	speed = 8.f;
	wall = -2;
	for (int i = 5; i < 10; ++i)
		npc_list[i].Init(NPC_tilemap_type, Vector3(199, wall++ * 70, NPC_z += 0.01f), Vector2(NPC_scale, NPC_scale), speed *= 2.f);

	speed = 5.f;
	wall = -2;
	for (int i = 10; i < 15; ++i)
		npc_list[i].Init(NPC_tilemap_type, Vector3(wall++ * 70, 199, NPC_z += 0.01f), Vector2(NPC_scale, NPC_scale), speed *= 2.f);

	speed = 10.f;
	wall = -2;
	for (int i = 15; i < 20; ++i)
		npc_list[i].Init(NPC_tilemap_type, Vector3(-199, wall++ * 70, NPC_z += 0.01f), Vector2(NPC_scale, NPC_scale), speed *= 2.f);

	//test player--------------------------------------------------------//
	test_playerPos.Set(-179, -199);
	changed_playerPos = false;

	//initial pathfind-------------------------------------------------------//
	Calculate_Paths();

	//test sprite--------------------------------------------------//
	testSprite.Set(TM_FEMALE, 0.2);
	testSprite.Current_Set(TileMap::SET_TYPE::WALK_S);
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
double changeDir_Timer = 0.0;
int dir_index = 1;
void Scene_Pathfinding::Update()
{
	Scene::Update();

	//update------------------------------------------------------------------//
	Vector2 dir;
	float speed = 150.f * CU::dt;

	if (CU::input.DPad_Right(dir))
	{
		test_playerPos.x += dir.x * speed;
		test_playerPos.y += dir.y * speed;

		if( !(navmesh.Is_Pos_InWall(test_playerPos) || navmesh.Is_Pos_OutOfBounds(test_playerPos)))
			changed_playerPos = true;

		/*if (changeDir_Timer >= 0.5)
		{
			changeDir_Timer = 0.0;
			testSprite.Current_Set((TileMap::SET_TYPE)dir_index++);

			if (dir_index >= TileMap::SET_TYPE::TOTAL_SETS)
				dir_index = 0;
		}*/
	}
	//changeDir_Timer += CU::dt;

	//start pathfind-------------------------------------------------------------------//
	if (changed_playerPos)
	{
		changed_playerPos = false;
		Calculate_Paths();
	}

	//set basic shader so it starts first later-----------------------------------------------------------------------//
	CU::view.SetShader(BASIC_SHADER);

	//tmp. update components-------------------------------------------------------------------//
	for (int i = 0; i < TOTAL_NPC; ++i)
		npc_list[i].Update();

	//test sprite update-----------------------------------------------------------//
	testSprite.Update();
}

/*********************************************************************************************************
calculate paths for NPCs
/*********************************************************************************************************/
void Scene_Pathfinding::Calculate_Paths()
{
	CU::pathfind.BroadPhrase_Search(test_playerPos, &navmesh);

	//debug mesh reset-----------------------------//
	navmesh.Reset_GridMesh();

	//path finding------------------------------------------------------------------------------------//
	for (int i = 0; i < TOTAL_NPC; ++i)
	{
		npc_list[i].PathFind();
		navmesh.LightUp_Path(npc_list[i].Get_PathFollower()->Get_PathList(), npc_list[i].Get_PathFollower()->Get_TotalTiles());
	}

	//update the mesh-------------------------------------------------------//
	navmesh.Update_GridMesh();
}

/*********************************************************************************************************
tmp draw
/*********************************************************************************************************/
void Scene_Pathfinding::Tmp_Draw()
{
	Scene::Tmp_Draw();

	//nav mesh------------------------------------------------------------------------------------//
	navmesh.TmpDraw();

	//test sprite-----------------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(280, 100, -1.f), Vector3(200, 200, 1.f), testSprite.GetMesh());
	testSprite.Draw();

	//draw intersected----------------------------------------------------------------------------//
	for (int i = 0; i < TOTAL_NPC; ++i)
		npc_list[i].TmpDraw();

	//player--------------------------------------------------------------------------------------//
	CU::quad->AssignTexture(CU::textureList[TEX_TRAIN]);
	CU::view.Pre_DrawMesh(Vector3(test_playerPos.x, test_playerPos.y, 1.f), Vector3(25.f, 25.f, 1.f), CU::quad);
	CU::quad->Draw();
}

void Scene_Pathfinding::Tmp_DrawOnScreen()
{
	CU::view.SetShader(TEXT_SHADER);
	CU::view.RenderText("PATHFIND", Vector2(-160, 140), 0.9f, Color(5, 242, 230));

	//render text
	ss.str("");
	ss << "FPS: " << (int)CU::fps;
	CU::view.RenderText(ss.str(), Vector2(110, 150), 0.8f, Color(227, 157, 27));
}

/*********************************************************************************************************
Update press
/*********************************************************************************************************/
void Scene_Pathfinding::Update_KeyPressed()
{
	Scene::Update_KeyPressed();
}

/*********************************************************************************************************
Update release
/*********************************************************************************************************/
void Scene_Pathfinding::Update_KeyReleased()
{
	Scene::Update_KeyReleased();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Scene_Pathfinding::Exit()
{
	Scene::Exit();
}