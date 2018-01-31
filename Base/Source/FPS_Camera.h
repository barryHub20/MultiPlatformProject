#ifndef FPS_CAM_H
#define FPS_CAM_H
#include "Camera.h"

/**************************************************************************************/
/*
	Typical camera, WASD and fly. 
	Add: accelerate/decelerate when moving

	Author: Tan Yie Cher
	Creation date: c. 2015
	Updated: 25/6/2016
*/
/**************************************************************************************/
class FPS_Cam : public Camera
{
public:
	/**************************** Constructor/destructor ****************************/
	FPS_Cam();
	~FPS_Cam();

	/**************************** Init ****************************/
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);

	/**************************** Update ****************************/
	void Update(double dt);	//constant update
	void UpdateInput(double dt);	//update if inputs available

private:
	float speed;	//moving around
	float mouse_speed;	//mouse 
	Vector3 FaceDirection;
	float pitchAngle;

	/**************************** Utilities ****************************/
	void Strafe(double dt);
	void Walk(double dt);
	void LookAround(float mouse_pitchValue, float mouse_yawValue, double dt);
	void LookPitch(float pitchValue, double dt);
	void LookYaw(float yawValue, double dt);
	void Fly(double dt);
};


#endif