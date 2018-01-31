#include "Polygon_3D.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
CTD polygon
/*********************************************************************************************************/
CD_Polygon_3D::CD_Polygon_3D()
{
}

CD_Polygon_3D::~CD_Polygon_3D()
{
	delete mesh;
}

/*********************************************************************************************************
Core
/*********************************************************************************************************/
void CD_Polygon_3D::Init(Vector3 shapePos, Vector3 shapeScale, Vector3 dir, bool projectorShape, Color color)
{
	//var------------------------------------------------------------------------//
	this->shapePos = shapePos;
	this->shapeScale = shapeScale;
	this->original_dir = this->dir_prime = dir;
	this->up.Set(0, 1, 0);
	this->right = this->original_dir.Cross(up);
	this->yaw = this->pitch = this->roll = 0.f;
	w = x = z = rot_w = rot_x = rot_z = 0.f;
	y = rot_y = 1.f;

	//render---------------------------------------------------------------------//
	mesh = new Mesh();
	mesh->Init_Cuboid(color, color, shapeScale);
}

void CD_Polygon_3D::CopyFrom(CD_Polygon_3D& copyFromMe)
{

}

void CD_Polygon_3D::Draw()
{
	//cuboid-----------------------------------------------------------------------------//
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	CU::view.Pre_DrawMesh(shapePos, rot_w, Vector3(rot_x, rot_y, rot_z), Vector3(1,1,1), mesh);
	mesh->Draw();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//for (int i = 0; i < 8; ++i)
	//{
	//	CU::view.Pre_DrawMesh(pointList[i], Vector3(10, 10, 10), CU::sphere_blue);
	//	CU::sphere_blue->Draw();
	//}
}

/*********************************************************************************************************
rotation
/*********************************************************************************************************/
void CD_Polygon_3D::DirRecalculate()
{
	CU::mtx[0].SetToRotation(rot_w, rot_x, rot_y, rot_z);
	CU::mtx[1].SetToTranslation(shapePos.x, shapePos.y, shapePos.z);
	CU::mtx[2].SetToTranslation(0, 0, 1000000000.f);	//value bigger = more accuracy
	CU::mtx[3] = CU::mtx[1] * CU::mtx[0] * CU::mtx[2];
	right.Set(1, 1, 1);
	right = CU::mtx[3] * right;

	right.Normalize();
	up = right.Cross(dir_prime);
}

void CD_Polygon_3D::yawRot(float angle)
{
	yaw += angle;
}

void CD_Polygon_3D::pitchRot(float angle)
{
	pitch += angle;
}

void CD_Polygon_3D::rollRot(float angle)
{
	roll += angle;
}

/*********************************************************************************************************
translation
/*********************************************************************************************************/
void CD_Polygon_3D::Forward(float dir)
{
	float speed = 150.f;
	shapePos += dir_prime * dir * speed * CU::dt;
}

void CD_Polygon_3D::Strafe(float dir)
{
	float speed = 150.f;
	shapePos += right * dir * speed * CU::dt;
}

/*********************************************************************************************************
TRS
/*********************************************************************************************************/
void CD_Polygon_3D::RecalculatePoints()
{
	ToQuaternion();

	//recalculate points-----------------------------------------------------------------//
	Vector3 pt0 = shapePos + right * shapeScale.z * 0.5f;

	pt0 += (-dir_prime * shapeScale.x * 0.5f) + (-up * shapeScale.y * 0.5f);
	pointList[0] = pt0;

	pt0 += dir_prime * shapeScale.x;
	pointList[1] = pt0;

	pt0 += -right * shapeScale.z;
	pointList[2] = pt0;

	pt0 += -dir_prime * shapeScale.x;
	pointList[3] = pt0;

	pt0 = pointList[0];
	pt0 += up * shapeScale.y;
	pointList[4] = pt0;

	pt0 += dir_prime * shapeScale.x;
	pointList[5] = pt0;

	pt0 += -right * shapeScale.z;
	pointList[6] = pt0;

	pt0 += -dir_prime * shapeScale.x;
	pointList[7] = pt0;

	//point normal list------------------------------------------------------------------//
	for(int i = 0; i < 8; ++i)
		pointNormalList[i] = pointList[i] - shapePos;
}

void CD_Polygon_3D::ToQuaternion()
{
	float yaw = Math::DegreeToRadian(this->yaw);
	float pitch = Math::DegreeToRadian(this->pitch);
	float roll = Math::DegreeToRadian(this->roll);

	float cy = cos(yaw * 0.5);
	float sy = sin(yaw * 0.5);
	float cr = cos(roll * 0.5);
	float sr = sin(roll * 0.5);
	float cp = cos(pitch * 0.5);
	float sp = sin(pitch * 0.5);

	//z and y swapped
	w = cy * cr * cp + sy * sr * sp;
	x = cy * sr * cp - sy * cr * sp;
	z = cy * cr * sp + sy * sr * cp;
	y = sy * cr * cp - cy * sr * sp;

	//convert to degree
	rot_w = 2 * acos(w);
	float sqrt_w = sqrt(1.f - w*w);

	if (sqrt_w == 0.f)
	{
		rot_x = rot_z = 0.f;
		rot_y = 1.f;
	}
	else
	{
		rot_x = x / sqrt_w;
		rot_y = y / sqrt_w;
		rot_z = z / sqrt_w;
	}

	rot_w = Math::RadianToDegree(rot_w);

	// Extract the vector part of the quaternion
	Vector3 u(x, y, z);

	// Extract the scalar part of the quaternion
	float s = w;

	//get direction vector
	dir_prime = 2.0f * u.Dot(original_dir) * u
		+ (s*s - u.Dot(u)) * original_dir
		+ 2.0f * s * u.Cross(original_dir);

	//get the right and up
	DirRecalculate();
}