#include "FPS_Camera.h"
#include "CoreUtilities.h"
#include "Input.h"

/*******************************************************************************/
/*
	Do: Why mouse pitch and yaw must invert dt?
*/
/*******************************************************************************/
Mtx44 rotation22;
Vector3 trololo;
Vector3 MoveDirection;

/**************************** Constructor/destructor ****************************/
FPS_Cam::FPS_Cam()
{
}

FPS_Cam::~FPS_Cam()
{
}

/**************************** Init ****************************/
void FPS_Cam::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	Vector3 trololo = target;
	this->position  = pos;
	this->target = trololo = target;

	trololo.y = position.y;
	MoveDirection = (target - position).Normalized();
	Vector3 right = MoveDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(MoveDirection).Normalized();
	speed = 200.f;
	mouse_speed = 230.f;
	pitchAngle = 0.f;
	FaceDirection = (target - position).Normalized();
}

/**************************** Update ****************************/
void FPS_Cam::Update(double dt)	//constant update
{
	/* Mouse */
	LookAround(CU::input.getPitch(), CU::input.getYaw(), dt);
} 

void FPS_Cam::UpdateInput(double dt)	//update if inputs available
{
	/* Strafe */
	if (CU::input.isKeyPressed(InputState::A) && !CU::input.isKeyPressed(InputState::D))
	{
		Strafe(-dt);
	}

	if (!CU::input.isKeyPressed(InputState::A) && CU::input.isKeyPressed(InputState::D))
	{
		Strafe(dt);
	}

	/* walk */
	if (CU::input.isKeyPressed(InputState::W) && !CU::input.isKeyPressed(InputState::S))
	{
		Walk(dt);
	}

	if (!CU::input.isKeyPressed(InputState::W) && CU::input.isKeyPressed(InputState::S))
	{
		Walk(-dt);
	}

	/* Fly */
	if (CU::input.isKeyPressed(InputState::K))
	{
		Fly(dt);
	}
	else if (CU::input.isKeyPressed(InputState::L))
	{
		Fly(-dt);
	}
}

/**************************** Utilities ****************************/
void FPS_Cam::Strafe(double dt)
{
	Vector3 right = FaceDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = right.Cross(FaceDirection).Normalized();

	// Update the camera and target position
	position += right * speed * dt;
	target += right * speed * dt;
}

void FPS_Cam::Walk(double dt)
{
	trololo = target;
	trololo.y = position.y;
	MoveDirection = (trololo - position).Normalized();
	// Update the camera and target position
	position += MoveDirection * speed * dt;
	target += MoveDirection * speed * dt;
}

void FPS_Cam::LookAround(float mouse_pitchValue, float mouse_yawValue, double dt)
{
	if( mouse_pitchValue != 0.f )	
		LookPitch(mouse_pitchValue, -dt);

	if( mouse_yawValue != 0.f )	
		LookYaw(mouse_yawValue, -dt);
}

void FPS_Cam::LookPitch(float pitchValue, double dt)
{
	float mspeed = mouse_speed * pitchValue * dt;

	FaceDirection = (target - position).Normalized();
	Vector3 right = FaceDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(FaceDirection).Normalized();
	rotation22.SetToRotation(mspeed, right.x, right.y, right.z);
	FaceDirection = rotation22 * FaceDirection;
	target = position + FaceDirection;

	up = rotation22 * up;
}

void FPS_Cam::LookYaw(float yawValue, double dt)
{
	float mspeed = mouse_speed * yawValue;

	FaceDirection = (target - position).Normalized();
	Vector3 right = FaceDirection.Cross(up);
	right.y = 0;
	right.Normalize();
	up = right.Cross(FaceDirection).Normalized();
	rotation22.SetToRotation(mspeed * dt, 0, 1, 0);
	FaceDirection = rotation22 * FaceDirection;
	target = position + FaceDirection;

	up = rotation22 * up;
}

void FPS_Cam::Fly(double dt)
{
	position.y += speed * dt;
	target.y += speed * dt;
}