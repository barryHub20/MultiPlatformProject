#include "Shape_Utilities.h"
#include "CoreUtilities.h"

/********************************************************************************
Constructor/dextructor for point
********************************************************************************/
Point::Point() {}
Point::Point(const Point& copy)
{
	this->pos = copy.pos;
	this->offset = copy.offset;
}
Point::~Point() {}

//pos: pos relative to shape position
//shapePos: this shape pos
void Point::Set(Vector3 pos, Vector3 shapePos)
{
	this->pos = pos;
	this->offset = this->pos;
	this->pos += shapePos;
}

/********************************************************************************
Constructor/dextructor
********************************************************************************/
Face::Face() {}
Face::Face(const Face& copy)
{
	this->start = copy.start;
	this->dir = copy.dir;
	this->len = copy.len;
	this->normal = copy.normal;
	this->angle = copy.angle;
}
Face::~Face() {}

/********************************************************************************
Face init
********************************************************************************/
void Face::Set(int startPoint_index, int endPoint_index, Point* pList)
{
	//Indexes--------------------------------------------------------//
	this->start = startPoint_index;
	this->end = endPoint_index;

	this->dir = (pList[end].pos - pList[start].pos).Normalized();
	this->len = (pList[end].pos - pList[start].pos).Length();
	this->normal = this->dir.Cross(Vector3(0, 0, 1));
	this->angle = Vector3::getAngleFromDir(dir.x, dir.y);
}

void Face::Set2(int startPoint_index, int endPoint_index, vector<Point>& pList)
{
	//Indexes--------------------------------------------------------//
	this->start = startPoint_index;
	this->end = endPoint_index;

	this->dir = (pList[end].pos - pList[start].pos).Normalized();
	this->len = (pList[end].pos - pList[start].pos).Length();
	this->normal = this->dir.Cross(Vector3(0, 0, 1));
	this->angle = Vector3::getAngleFromDir(dir.x, dir.y);
}

/********************************************************************************
rotate by angle
angle, normal and direction changes
********************************************************************************/
void Face::Rotate(float angle)
{
	//angle-------------------------------------------------------//
	this->angle += angle;

	if (this->angle > 360.f)
		this->angle -= 360.f;
	else if (this->angle < 0.f)
		this->angle += 360.f;

	//dir-------------------------------------------------------//
	this->dir.x = cos(Math::DegreeToRadian(this->angle));
	this->dir.y = sin(Math::DegreeToRadian(this->angle));

	this->normal = this->dir.Cross(Vector3(0, 0, 1));
}


/********************************************************************************
plane intersection
********************************************************************************/
bool Face::intersectPlane(const Vector3 &n, const Vector3 &p0, const Vector3 &l0, const Vector3 &l, float &t)
{
	float denom = n.Dot(l);

	if (denom > 1e-6)
	{
		Vector3 p0l0 = p0 - l0;
		t = p0l0.Dot(n) / denom;
		return (t >= 0);
	}

	return false;
}

/********************************************************************************
test line intersection
********************************************************************************/
bool Face::LineIntersection(const Vector3& lineOrigin, float lineAngle, vector<Point>& pList, Vector3& intersectedPos)
{
	//offset everything to (0,0)
	Vector3 offset = lineOrigin;

	Vector3 rayNormal;

	rayNormal.x = cos(Math::DegreeToRadian(lineAngle));
	rayNormal.y = sin(Math::DegreeToRadian(lineAngle));
	float t = 0.f;


	//plane pos
	Vector3 planePos = pList[start].pos + (dir * len * 0.5f);

	bool intersectedPlane = intersectPlane(-normal, planePos, lineOrigin, rayNormal, t);

	if (intersectedPlane)
	{
		intersectedPos = lineOrigin + rayNormal * t;

		//check if intersected pos is "out of range"
		float lensq = (intersectedPos - planePos).LengthSquared();
		float halfLenPlane = len * 0.5f;

		//out of range of the plane width
		if (lensq > halfLenPlane * halfLenPlane)
			intersectedPlane = false;

		//add with offset
		intersectedPos -= offset;
	}

	return intersectedPlane;
}

/********************************************************************************
Face draw
********************************************************************************/
void Face::Draw(vector<Point>& pointList, bool collided)
{
	////Draw the point---------------------------------//
	//CU::view.Pre_DrawMesh(pointList[start].pos, Vector3(5,5,1), CU::quad);
	//CU::quad->Draw();

	//Draw the dir---------------------------------//
	if(!collided)
		CU::view.Draw_Line(pointList[start].pos, this->angle, this->len, 3.f, CU::line_blue);
	else
		CU::view.Draw_Line(pointList[start].pos, this->angle, this->len, 3.f, CU::line_red);

	////Draw the normal---------------------------------//
	//float normalAngle = Vector3::getAngleFromDir(normal.x, normal.y);
	//shareVec = pointList[start].pos;
	//shareVec.x += len * 0.5f * dir.x;
	//shareVec.y += len * 0.5f * dir.y;
	//CU::shared.DrawLine(CU::shared.line_2, shareVec, normalAngle, 50.f, 1.f);
}