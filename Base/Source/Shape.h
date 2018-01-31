#ifndef SHAPE_H
#define SHAPE_H
#include "Shape_Utilities.h"
#include "Mesh.h"

/***************************************************************************************************************
Collidable shape, no nodes YET

Author: Tan Yie Cher
Date: 16/7/17

2D shape collider.

3--------2
|		 |
0--------1

Point 0 and 1 will be checked, 2 and 3 is simply the opposite of 1 and 2

Issues:
-When pushing a shape, it translate instead of rotate
/***************************************************************************************************************/
class Shape : public Transform
{
private:

	Point pointList[4];
	Face faceList[2];
	bool collided;
	Mesh* mesh, *collided_mesh;
	bool collidedFace[4];	//face whose normal is used

	//utilities=======================================================================//
	void RecalculateFaces(float angle);
	void RecalculatePoints();
	static void GetProjection(Shape& projectMe, Vector3 normal, float& min, float& max);
	static float Get_SqDist(Vector3 thisPos, Vector3 checkPos, float bounce, Vector3 normal);

	//temp utilities=================================================================//
	void DrawFace(Vector3 pos, float angle, float len, bool collided);
public:

	//CD=======================================================================//
	Shape();
	~Shape();

	//Core=======================================================================//
	void Init(Vector3 shapePos, Vector3 shapeScale);
	void PreUpdate();
	bool CollisionCheck(Shape& check, Vector3& vel);
	void Draw();

	//Overwrites=======================================================================//
	//transformation----------------------------------//
	virtual void Rotate(float x, float y, float z);

	//Parent call-------------------------------------------------//
	virtual void Rotate_ByParent(float x, float y, float z);
	virtual void CalculateTRS_byParent(Mtx44& parentFinal);

	//get/set=======================================================================//
	bool isCollided();

private:

	//SAT=======================================================================//
	int ProjectShapeOntoTHIS(Shape& check, float& lowest_bounce, bool thisShape);
	Vector3 response(float lowest_bounce, Vector3& normal);
};

#endif