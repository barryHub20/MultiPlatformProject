#ifndef Polygon3D_H
#define Polygon3D_H
#include "Transform.h"
#include "Mesh.h"

/***************************************************************************************************************
Collision Tutorial Discrete (CTD)

By default all polygon 3D starts with y=0

Author: Tan Yie Cher
Date: 14/10/17
/***************************************************************************************************************/
class CD_Polygon_3D
{

	void DirRecalculate();
public:
	
	//main vars---------------------------------------------------------//
	Vector3 shapePos, shapeScale, original_dir, dir_prime, right, up;
	float yaw, pitch, roll;
	float w, x, y, z;	//quaternion
	float rot_w, rot_x, rot_y, rot_z;
	Vector3 pointList[8];
	Vector3 pointNormalList[8];

	//render purposes---------------------------------------------------//
	Mesh* mesh;

	//CD==============================================================================//
	CD_Polygon_3D();
	~CD_Polygon_3D();

	//Core=============================================================================//
	void Init(Vector3 shapePos, Vector3 shapeScale, Vector3 dir, bool projectorShape, Color color);
	void CopyFrom(CD_Polygon_3D& copyFromMe);
	void Draw();

	//Transform=======================================================================//
	void RecalculatePoints();
	void ToQuaternion();
	
	void yawRot(float angle);
	void pitchRot(float angle);
	void rollRot(float angle);

	void Forward(float dir);
	void Strafe(float dir);
};

#endif