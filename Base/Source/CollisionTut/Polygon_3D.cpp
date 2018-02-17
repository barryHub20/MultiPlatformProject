#include "Polygon_3D.h"
#include "CoreUtilities.h"
float CD_Polygon_3D::w = 0.f, CD_Polygon_3D::x = 0.f, CD_Polygon_3D::y = 1.f, CD_Polygon_3D::z = 0.f;
const Vector3 CD_Polygon_3D::original_dir(1, 0, 0);

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
void CD_Polygon_3D::Init(Vector3 shapePos, Vector3 shapeScale, Color color, Color color2)
{
	//var------------------------------------------------------------------------//
	this->shapePos = shapePos;
	this->shapeScale = shapeScale;
	this->dir_prime = original_dir;
	this->up.Set(0, 1, 0);
	this->right = this->original_dir.Cross(up);
	this->yaw = this->pitch = this->roll = 0.f;
	rot_w = rot_x = rot_z = 0.f;
	rot_y = 1.f;

	//render---------------------------------------------------------------------//
	mesh = new Mesh();
	mesh->Init_Cuboid(color, color2, shapeScale);
}

/*********************************************************************************************************
Copy from all data except mesh and projection data
/*********************************************************************************************************/
void CD_Polygon_3D::CopyFrom(CD_Polygon_3D& copyFromMe)
{
	//var------------------------------------------------------------------------//
	this->shapePos = copyFromMe.shapePos;
	this->shapeScale = copyFromMe.shapeScale;
	this->dir_prime = copyFromMe.dir_prime;
	this->up = copyFromMe.up;
	this->right = copyFromMe.right;
	this->yaw = copyFromMe.yaw;
	this->pitch = copyFromMe.pitch;
	this->roll = copyFromMe.roll;
	this->w = copyFromMe.w;
	this->x = copyFromMe.x;
	this->z = copyFromMe.z;
	this->rot_w = copyFromMe.rot_w;
	this->rot_x = copyFromMe.rot_x;
	this->rot_z = copyFromMe.rot_z;
	this->y = copyFromMe.y;
	this->rot_y = copyFromMe.rot_y;

	for (int i = 0; i < 8; ++i)
	{
		pointList[i] = copyFromMe.pointList[i];
		pointNormalList[i] = copyFromMe.pointNormalList[i];
	}
}

/*********************************************************************************************************
project shape: set the projection data here (not transformation data)
Translational and rotational data for this frame
/*********************************************************************************************************/
void CD_Polygon_3D::ProjectShape(float proj_Yaw, float proj_Pitch, Vector3 vel)
{
	//store values
	this->proj_Yaw_tr = proj_Yaw;
	this->proj_Pitch_tr = proj_Pitch;
	this->proj_Vel_tr = vel;

	//get accumulated yaw + pitch
	proj_Yaw = yaw + proj_Yaw;
	proj_Pitch = pitch + proj_Pitch;

	//get the new quarternion values
	float new_rot_w, new_rot_x, new_rot_y, new_rot_z;
	Vector3 new_dir_prime, new_right, new_up;

	ToQuaternion(proj_Yaw, proj_Pitch, roll, shapePos + vel, new_rot_w, new_rot_x, new_rot_y, new_rot_z, new_dir_prime, new_right, new_up);

	//calculate the magnitude(angle) difference
	calulatedMag = new_rot_w - rot_w;

	//angular velocity
	Vector3 halfScale = shapeScale * 0.5f;
	r = sqrt(halfScale.x * halfScale.x + halfScale.y * halfScale.y + halfScale.z * halfScale.z);
	av = Math::DegreeToRadian(calulatedMag) * r;
}

/*********************************************************************************************************
project + move shape: set the projection + transformation data here 
(recalculatePoints() called here)
/*********************************************************************************************************/
void CD_Polygon_3D::ProjectShape_Transform(float proj_Yaw, float proj_Pitch, Vector3 vel)
{
	ProjectShape(proj_Yaw, proj_Pitch, vel);
	Translate(vel);
	yawRot(proj_Yaw);
	pitchRot(proj_Pitch);
}

void CD_Polygon_3D::Draw()
{
	//cuboid-----------------------------------------------------------------------------//
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	CU::view.Pre_DrawMesh(shapePos, rot_w, Vector3(rot_x, rot_y, rot_z), Vector3(1,1,1), mesh);
	mesh->Draw();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//draw using yaw/pitch rot-----------------------------------------------------------//
	/*CU::view.Pre_DrawMesh(shapePos, yaw, pitch, Vector3(0, 1, 0), Vector3(0, 0, 1), Vector3(1, 1, 1), mesh2);
	mesh2->Draw();*/

	//vetices----------------------------------------------------------------------------//
	//for (int i = 0; i < 8; ++i)
	//{
	//	CU::view.Pre_DrawMesh(pointList[i], Vector3(10, 10, 10), CU::sphere_blue);
	//	CU::sphere_blue->Draw();
	//}

	////target (dir)-----------------------------------------------------------------------//
	/*CU::view.Pre_DrawMesh(shapePos + (dir_prime * shapeScale.x * 0.55f), Vector3(15, 15, 15), CU::sphere_blue);
	CU::sphere_blue->Draw();*/

	////right------------------------------------------------------------------------------//
	/*CU::view.Pre_DrawMesh(shapePos + (right * shapeScale.z * 0.55f), Vector3(15, 15, 15), CU::sphere_dark_green);
	CU::sphere_dark_green->Draw();*/

	////up---------------------------------------------------------------------------------//
	//CU::view.Pre_DrawMesh(shapePos + (up * shapeScale.y * 0.55f), Vector3(15, 15, 15), CU::sphere_yellow);
	//CU::sphere_yellow->Draw();
}

/*********************************************************************************************************
STATIC
/*********************************************************************************************************/
void CD_Polygon_3D::DirRecalculate(float rot_w, float rot_x, float rot_y, float rot_z, Vector3 shapePos, Vector3 dir_prime, Vector3& right, Vector3& up)
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

/*********************************************************************************************************
rotation
/*********************************************************************************************************/
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

void CD_Polygon_3D::Translate(Vector3 vel)
{
	shapePos += vel;
}

/*********************************************************************************************************
TRS
/*********************************************************************************************************/
void CD_Polygon_3D::RecalculatePoints()
{
	ToQuaternion(yaw, pitch, roll, shapePos, rot_w, rot_x, rot_y, rot_z, dir_prime, right, up);
	
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

/*********************************************************************************************************
STATIC
convert euler space to quarternion space
Get:
-quarternion values
-dir vector
-right vector
-up vector
/*********************************************************************************************************/
void CD_Polygon_3D::ToQuaternion(float yaw, float pitch, float roll, Vector3 shapePos, float& rot_w, float& rot_x, float& rot_y, float& rot_z,
	Vector3& dir_prime, Vector3& right, Vector3& up)
{
	yaw = Math::DegreeToRadian(yaw);
	pitch = Math::DegreeToRadian(pitch);
	roll = Math::DegreeToRadian(roll);

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

	// Extract the scalar part of the quaternion
	Vector3 u(x, y, z);

	// Extract the magnitude part of the quaternion
	float s = w;

	//get direction vector
	dir_prime = 2.0f * u.Dot(original_dir) * u
		+ (s*s - u.Dot(u)) * original_dir
		+ 2.0f * s * u.Cross(original_dir);

	//get the right and up
	DirRecalculate(rot_w, rot_x, rot_y, rot_z, shapePos, dir_prime, right, up);
}