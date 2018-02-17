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

	//Simplex data---------------------------------------------------------//
	Vector3 dir;
	Vector3 vertices[4];	//newPoint: index 3
	bool infinite_loop;
	float closestDist;
	Vector3 closestPoint;
	Vector3 rootDir;
	Vector3 new_ab_normal, new_ac_normal, new_bc_normal;
	float new_ab_Dot, new_ac_Dot, new_bc_Dot;

	//data collection------------------------------------------------------//
	Vector3 collect_Pt[8];
	float collect_Dist[8];
	Vector3 collect_verts[8][3];
	char collect_primitiveType[8];
	int collect_edgeType[8];
	int collect_vertType[8];
	int collect_suppIdx[8][6];

	//closest points-------------------------------------------------------//
	Vector3 v0, v1, v2;
	float d00, d01, d11, d20, d21, denom;
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
	//min dist utilities---------------------------------------------------//
	int GetSupportingVertice(CD_Polygon_3D& poly, Vector3 dir);
	int GetMin(float a, float b, float c);
	Vector3 GetNewLast(Vector3 dir, CD_Polygon_3D& A, CD_Polygon_3D& B);
	Vector3 closestPointToOrigin(int v0_idx, int v1_idx, int v2_idx);
	Vector3 NonIntersection(int v0_idx, int v1_idx, int v2_idx);
	void calculateNormals();
	static Vector3 closestPointToOrigin_AB(Vector3 A, Vector3 B);

	//closest points utilities---------------------------------------------//
	void BarycentricCoord(Vector3 p, Vector3 a, Vector3 b, Vector3 c, Vector3& retVal);

	//angular response-----------------------------------------------------//
	void TransformShape(CD_Polygon_3D& shape, float yaw, float pitch, Vector3 vel, float t);
	void SetShape_ToAbsoute(CD_Polygon_3D& projected, CD_Polygon_3D& main);

	//core============================================================================//
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