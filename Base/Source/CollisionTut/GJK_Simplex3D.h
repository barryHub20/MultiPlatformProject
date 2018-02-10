#ifndef GJK_SIMPLEX_3D_H
#define GJK_SIMPLEX_3D_H
#include "Polygon_3D.h"

/***************************************************************************************************************
GJK Simplex 3D
Collision training

Author: Tan Yie Cher
Date: 31/12/17
/***************************************************************************************************************/
class GJK_Simplex_3D
{
public:
	Vector3 MD_points[64];	//36

	//Simplex data---------------------------------------------------------//
	int total_vertices;
	Vector3 dir;
	bool doesNotContain;
	Vector3 vertices[3];
	Vector3 newPoint, prev_newPoint;
	Mesh* simplexMesh;
	float lastFew_dist[4];
	Vector3 lastFew_pt[4];
	bool infinite_loop;
	float closestDist;
	Vector3 closestPoint;

	//closest points-------------------------------------------------------//
	float lambda1, lambda2, lambda3;
	int shapeA_MDpoint, shapeB_MDpoint;
	int sA_pA, sB_pA;
	int sA_pB, sB_pB;
	int sA_pC, sB_pC;
	char final_primitiveType; //closest point primitive type
	int edge_type;	//0: AB, 1: AC, 2: BC
	int vertType;
	Vector3 shapeA_closestPt, shapeB_closestPt;

	//static data----------------------------------------------------------//
	static Vector3 ao, ab, contactPt;	//origin to AB projection
	Vector3 tmp_points[3];	//store new dirs from their respective edges
	float dotProd, largestDotProd, largestDotProd_index;	//for getting support vertice
	Vector3 extremeA, extremeB, last;	//for getting new last point

	//triangle projection--------------------------------------------------//
	Vector3 edge1, edge2, h, s, q, triangle_intersectPt;
	float a, f, u, v;

	//line and vertex projection-------------------------------------------//
	Vector3 edges[3];
	Vector3 edges_intersectPt[3];
	float lensqList[6];	//0 - 2: edge0 -> edge2, 3 - 5: A -> C

	//collision response---------------------------------------------------//
	bool noCollision;
	float t, t_tracker;	//t_tracker: tracks the value of t over all iterations

	//utilities=======================================================================//
	//get return value
	int GetSupportingVertice(CD_Polygon_3D& poly, Vector3 dir);
	int GetMin(float a, float b, float c);
	Vector3 GetNewLast(Vector3 dir, CD_Polygon_3D& A, CD_Polygon_3D& B);
	void BarycentricCoord(Vector3 p, Vector3 a, Vector3 b, Vector3 c, Vector3& retVal);

	//modifies class variables
	Vector3 closestPointToOrigin(Vector3 vert0, Vector3 vert1, Vector3 vert2);
	Vector3 NonIntersection(Vector3 vert0, Vector3 vert1, Vector3 vert2);
	static Vector3 closestPointToOrigin_AB(Vector3 A, Vector3 B);
	static Vector3 closestPointToOrigin_AB(Vector3 origin, Vector3 A, Vector3 B);

	//angular response
	void TransformShape(CD_Polygon_3D& shape, Vector3 vel, float rotAngle, float t);
	void SetShape_ToAbsoute(CD_Polygon_3D& projected, CD_Polygon_3D& main);

	//core
	void Reset();
	void computeClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B);

public:

	//CD==============================================================================//
	GJK_Simplex_3D();
	~GJK_Simplex_3D();

	//core============================================================================//
	//continuous collision
	void GetClosestPoints_Start(CD_Polygon_3D& A, CD_Polygon_3D& B);
	void GetClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B);
	bool CollisionResponse(CD_Polygon_3D& A, CD_Polygon_3D& Aresponse, CD_Polygon_3D& B);
	
	void Draw();
};

#endif