#include "NPC.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
c/d
/*********************************************************************************************************/
NPC::NPC()
{
}

NPC::~NPC()
{
}

/*********************************************************************************************************
init
/*********************************************************************************************************/
void NPC::Init(TILEMAP_TYPE sprite_type, Vector3 position, Vector2 scale, float speed)
{
	sprite_renderer.Set(sprite_type, 0.08);
	sprite_renderer.Current_Set(TileMap::SET_TYPE::WALK_N);

	//starts from center, but sprite starts from bottom left
	transform.Translate(position.x, position.y, position.z);
	transform.Rotate(0, 0, 0);
	transform.CalculateTRS();

	this->scale.Set(scale.x, scale.y);

	this->speed = speed;

	state = IDLE;
}

/*********************************************************************************************************
update
/*********************************************************************************************************/
Vector2 NPC_target_pos;
Vector2 NPC_pos;
Vector2 NPC_target_dir;
float NPC_speed;
Vector2 NPC_dist;
void NPC::Update()
{
	NPC_speed = speed * CU::dt;

	//tmp component update------------------------------//
	sprite_renderer.Update();

	if (state == FOLLOW)
	{
		//if not reached target--------------------------------//
		if (!path_follower.Reached_target())
		{
			//get target pos---------------------------------//
			NPC_target_pos = path_follower.GetTargetPos();
			
			NPC_pos.Set(transform.position.x, transform.position.y);

			//go to target--------------------------------------//
			if (!NPC_target_pos.Same(NPC_pos))
			{
				NPC_dist = NPC_target_pos - NPC_pos;
				NPC_target_dir = NPC_dist.Normalized() * NPC_speed;

				//check if reached
				if (NPC_dist.LengthSquared() <= NPC_speed * NPC_speed)
					transform.SetTranslate(NPC_target_pos.x, NPC_target_pos.y, transform.position.z);
				else
					transform.Translate(NPC_target_dir.x, NPC_target_dir.y, 0.f);

				transform.CalculateTRS();
			}
			else
				path_follower.Go_NextTile();
		}
		else
		{
			//reached target
			state = IDLE;
		}
	}
	else if (state == ATTACK)
	{

	}
}

/*********************************************************************************************************
draw
/*********************************************************************************************************/
void NPC::TmpDraw()
{
	CU::view.Pre_DrawMesh(transform.Get_FinalTRS(), Vector3(scale.x, scale.y, 1.f), sprite_renderer.GetMesh());
	sprite_renderer.Draw();
}

/*********************************************************************************************************
path find
/*********************************************************************************************************/
void NPC::PathFind()
{
	NPC_pos.Set(transform.position.x, transform.position.y);
	if (CU::pathfind.AssignPath(path_follower, NPC_pos))
	{
		state = FOLLOW;
	}
}

/*********************************************************************************************************
Get
/*********************************************************************************************************/
Transform* NPC::Get_Transform()
{
	return &transform;
}

PathFollower* NPC::Get_PathFollower()
{
	return &path_follower;
}

Vector3 NPC::GetPos()
{
	return transform.GetPosition();
}