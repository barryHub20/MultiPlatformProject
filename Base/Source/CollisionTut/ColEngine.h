#ifndef ColEngine_H
#define ColEngine_H
#include "MD_Shape.h"

/***************************************************************************************************************
Collision methods

Accompanying documentation here:
https://docs.google.com/document/d/1RZ4P26gKQ6gD8MRgaE_5IVlkIg7qwuYFJ6Z1TjLcRv8

Author: Tan Yie Cher
Date: 18/9/17
/***************************************************************************************************************/
class ColEngine
{
	void OffsetAngle(Transform& thisShape, Vector3& N);
	void OffsetAngle(Vector3& N, float angle);
	MD_Shape MD_Shape;

public:

	ColEngine();
	~ColEngine();

	//OBB V OBB=======================================================================//
	bool start_OBB_V_OBB2(CD_Polygon& poly1, CD_Polygon& poly2, CD_Polygon& poly1_projected);
	
	//debugging purposes
	void Draw();
};

#endif