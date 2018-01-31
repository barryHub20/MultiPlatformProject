#ifndef USEFUL_UTILS_H
#define USEFUL_UTILS_H

#include "Vector3.h"
#include "Vector2.h"

/*********************************************************************************************************
useful utilities like:
-AABB detection
-Interpolation of float/vec3

Author: Tan Yie Cher
Date: 2/2/2017
*********************************************************************************************************/
namespace Utils
{
	bool PointInAABB(Vector2 point, Vector2 bottomLeft, Vector2 topRight);
	bool pointInCircle(Vector2 point, Vector2 circlePos, float radius);
}

#endif