#include "CollisionTut\ColEngine.h"

ColEngine::ColEngine()
{
}
ColEngine::~ColEngine()
{
}

void ColEngine::OffsetAngle(Transform& thisShape, Vector3& N)
{
	////angle offset
	//cout << "Before: " << N << endl;
	//Transform* ulti_parent = thisShape.Get_TopParent();
	//float original_angle = ulti_parent->rotation.z;
	//
	//float angle = Vector3::getAngleFromDir(N.x, N.y);
	//angle -= original_angle;

	//N.x = cos(Math::DegreeToRadian(angle));
	//N.y = sin(Math::DegreeToRadian(angle));
	//cout << "After: " << N << endl;

	Vector3 normal = N.Normalized();
	float len = N.Length();

	Transform* ulti_parent = thisShape.Get_TopParent();
	float original_angle = ulti_parent->rotation.z;
	float angle = Vector3::getAngleFromDir(normal.x, normal.y);
	angle -= original_angle;

	normal.x = cos(Math::DegreeToRadian(angle));
	normal.y = sin(Math::DegreeToRadian(angle));

	N = normal * len;
}
void ColEngine::OffsetAngle(Vector3& N, float passangle)
{
	//angle offset
	float original_angle = passangle;
	float angle = Vector3::getAngleFromDir(N.x, N.y);
	angle -= original_angle;
	N.x = cos(Math::DegreeToRadian(angle));
	N.y = sin(Math::DegreeToRadian(angle));
}

/*********************************************************************************************************
CTD OBB
Accompanying doc section 3.1.4
TL;DR: simplify to point V Polygon
/*********************************************************************************************************/
bool ColEngine::start_OBB_V_OBB2(CD_Polygon& poly1, CD_Polygon& poly2, CD_Polygon& poly1_projected)
{
	MD_Shape.CollisionResponse(poly1, poly2, poly1_projected);
	return false;
}

/*********************************************************************************************************
origin onto normal
/*********************************************************************************************************/
void ColEngine::Draw()
{
	//MD_Shape.Draw();
}