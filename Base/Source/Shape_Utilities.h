#ifndef SHAPE_UTIL_H
#define SHAPE_UTIL_H
#include "Transform.h"

/***************************************************************************************************************
Point class

Author: Tan Yie Cher
Date: 16/7/2016
/***************************************************************************************************************/
class Point
{
public:

	Vector3 pos;
	Vector3 offset;	//offset from shape

	Point();
	Point(const Point& copy);
	~Point();

	void Set(Vector3 pos, Vector3 shapePos);
};

/***************************************************************************************************************
face: contains necessary info for a face in a shape

Author: Tan Yie Cher
Date: 16/7/2016
/***************************************************************************************************************/
class Face
{
	static Vector3 shareVec;

public:

	Face();
	Face(const Face& copy);
	~Face();

	unsigned start, end;	//index to start and end point handle
	float len;	//length from start to end
	float angle;	//angle of dir.
	Vector3 dir;	//dir
	Vector3 normal;	//normal, points outwards

	/**************************************** core functions ******************************************/
	void Set(int startPoint_index, int endPoint_index, Point* pointList);
	void Set2(int startPoint_index, int endPoint_index, vector<Point>& pointList);
	void Rotate(float angle);
	void Draw(vector<Point>& pointList, bool collided);

	/**************************************** Line intersection ******************************************/
	static bool intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t);
	bool LineIntersection(const Vector3& lineOrigin, float lineAngle, vector<Point>& pList, Vector3& intersectedPos);
};

#endif