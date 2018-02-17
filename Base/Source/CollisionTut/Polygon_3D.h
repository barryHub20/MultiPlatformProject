#ifndef Polygon3D_H
#define Polygon3D_H
#include "Transform.h"
#include "Mesh.h"

/***************************************************************************************************************
Collision Tutorial Discrete (CTD)

By default all polygon 3D starts with y=0

Euler to quarternion rotation in world space, not OBJECT SPACE
yaw (rot Y axis)
pitch (rot Z axis)

Author: Tan Yie Cher
Date: 14/10/17
/***************************************************************************************************************/
class CD_Polygon_3D
{

	static void DirRecalculate(float rot_w, float rot_x, float rot_y, float rot_z, Vector3 shapePos, Vector3 dir_prime, Vector3& right, Vector3& up);
	static void ToQuaternion(float yaw, float pitch, float roll, Vector3 shapePos, float& rot_w, float& rot_x, float& rot_y, float& rot_z,
		Vector3& dir_prime, Vector3& right, Vector3& up);

public:
	
	//main vars---------------------------------------------------------//
	Vector3 shapePos, shapeScale, dir_prime, right, up;
	const static Vector3 original_dir;
	float yaw, pitch, roll;
	Vector3 pointList[8];
	Vector3 pointNormalList[8];

	//quaternion--------------------------------------------------------//
	static float w, x, y, z;
	float rot_w, rot_x, rot_y, rot_z;

	//render purposes---------------------------------------------------//
	Mesh* mesh;

	//projection (user input)-------------------------------------------//
	float proj_Yaw_tr;	//tr: this round only, not acculmulative
	float proj_Pitch_tr;
	Vector3 proj_Vel_tr;

	//projection (program input)----------------------------------------//
	float calulatedMag;	//quaternion.w
	float av;	//angular velocity
	float r;	//radius

	//CD==============================================================================//
	CD_Polygon_3D();
	~CD_Polygon_3D();

	//Core=============================================================================//
	void Init(Vector3 shapePos, Vector3 shapeScale, Color color, Color color2);
	void CopyFrom(CD_Polygon_3D& copyFromMe);
	void Draw();

	//Transform=======================================================================//
	void ProjectShape(float proj_Yaw, float proj_Pitch, Vector3 vel);
	void ProjectShape_Transform(float proj_Yaw, float proj_Pitch, Vector3 vel);

	void RecalculatePoints();
	
	void yawRot(float angle);
	void pitchRot(float angle);
	void rollRot(float angle);

	void Forward(float dir);
	void Strafe(float dir);
	void Soar(float dir);
	void Translate(Vector3 vel);
};

#endif