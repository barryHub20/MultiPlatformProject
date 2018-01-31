#include "Camera.h"
#include "Mtx44.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = pos;
	this->target = this->targetOffset = target;
	this->up = up;
}

void Camera::Translate(float x, float y, float z)
{
	position += Vector3(x, y, z);
	target = position + targetOffset;
}