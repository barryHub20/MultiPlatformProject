#ifndef CAMERA_H
#define CAMERA_H

#include "Vector3.h"

/*********************************************************************************************************
Camera class
Date: c.2016

Accessed: 27/9/17
/*********************************************************************************************************/
class Camera
{
public:
	Vector3 position;
	Vector3 target, targetOffset;
	Vector3 up;

	Camera();
	~Camera();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Translate(float x, float y, float z);

	virtual void Update(double dt);	//constant update
	virtual void UpdateInput(double dt);	//update if inputs available
};

#endif