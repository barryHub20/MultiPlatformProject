#ifndef GJK_SIMPLEX_H
#define GJK_SIMPLEX_H
#include "Polygon.h"

/***************************************************************************************************************
GJK Simplex 2D
Collision training

Author: Tan Yie Cher
Date: 23/12/17
/***************************************************************************************************************/
class GJK_Simplex_2D
{
	//Simplex data---------------------------------------------------------//
	int total_vertices;
	Vector3 dir;
	bool doesNotContain;
	Vector3 vertices[3];

	//closest points-------------------------------------------------------//
	float closestDist;
	int Asa, Asb, Bsa, Bsb;	//polygon points that form the MD point
	Vector3 poly_A_closest_pt, poly_B_closest_pt;

	//collision response---------------------------------------------------//
	bool noCollision;
	float t, t_tracker;	//t_tracker: tracks the value of t over all iterations

	//utilities=======================================================================//
	//shared
	int GetSupportingVertice(CD_Polygon& poly, Vector3 dir);
	Vector3 GetNewLast(Vector3 dir, CD_Polygon& A, CD_Polygon& B, int& sa, int& sb);
	void Reset();

	//angular response
	void SetShape_ToAbsoute(CD_Polygon& projected, CD_Polygon& main);
	void TransformShape(CD_Polygon& shape, Vector3 vel, float rotAngle, float t);
	void computeClosestPoints(CD_Polygon& A, CD_Polygon& B);
	Vector3 closestPointToOrigin(Vector3 A, Vector3 B, int& type);

	//discrete
	Vector3 tripleProduct(Vector3 A, Vector3 B, Vector3 C);
	bool containsOrigin(CD_Polygon& A, CD_Polygon& B);

public:

	//CD==============================================================================//
	GJK_Simplex_2D();
	~GJK_Simplex_2D();

	//core============================================================================//
	bool StartSimplexCheck(CD_Polygon& A, CD_Polygon& B);

	//discrete collision
	bool StepThrough(CD_Polygon& A, CD_Polygon& B);
	bool CollisionCheck(CD_Polygon& A, CD_Polygon& B);

	//continuous collision
	void GetClosestPoints(CD_Polygon& A, CD_Polygon& B);
	bool CollisionResponse(CD_Polygon& A, CD_Polygon& Aresponse, CD_Polygon& B);

	void Draw();
};

#endif