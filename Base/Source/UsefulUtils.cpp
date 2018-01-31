#include "UsefulUtils.h"
#include <iostream>
using namespace std;

/*********************************************************************************************************
Update
/*********************************************************************************************************/
bool Utils::PointInAABB(Vector2 point, Vector2 bottomLeft, Vector2 topRight)
{
	return point <= topRight && point > bottomLeft;
}

bool Utils::pointInCircle(Vector2 point, Vector2 circlePos, float radius)
{
	return (point - circlePos).LengthSquared() <= radius * radius;
}