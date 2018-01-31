#ifndef MD_SHAPE_H
#define MD_SHAPE_H
#include "Polygon.h"
#define MD_TOTAL_POINTS 52

struct MD_FaceProp
{
	MD_Face face;
	int supportVert;
};

/***************************************************************************************************************
Minkowki shape

Usage:
Call Create_MDShape() for a B-A MD shape
Call Draw()

Follow this order:
B-A points
A-B points
B-A bridge
A-B bridge

Author: Tan Yie Cher
Date: 18/9/17
/***************************************************************************************************************/
class MD_Shape
{
private:
	
	//MD-------------------------------------------------------------------//
	MD_FaceProp faceList[MD_TOTAL_POINTS];
	Vector3 MD_origin;
	
	//indexes--------------------------------------------------------------//
	int shapeChange_break;	//the index where A-B occurs (can be used as before A-B flag)
	int nonBridged_faceCount;	//non-bridging faces
	int faceCount;

	//contact--------------------------------------------------------------//
	Vector3 contactNormal;
	float contactDist;

	//angular collision----------------------------------------------------//
	bool noCollision;
	float t, t_tracker;	//t_tracker: tracks the value of t over all iterations

	//utilities=======================================================================//
	void Reset();
	int GetSupportingVertice(MD_Face& face, CD_Polygon& check);
	void CreateContact();

	void TransformShape(CD_Polygon& shape, Vector3 vel, float rotAngle, float t);
	void SetShape_ToAbsoute(CD_Polygon& projected, CD_Polygon& main);

	//collision check=================================================================//
	void AddNewFace(Vector3 pos, Vector3 dirAlongFace, float faceLen, int supportVert);
	void BridgePoints(CD_Polygon& A, CD_Polygon& B);
	void AddBridgePoint(int i1, int i2, int faceIndex, CD_Polygon& poly, CD_Polygon& other, bool isAB);
	void Create_MDShape(CD_Polygon& A, CD_Polygon& B);

public:

	//CD==============================================================================//
	MD_Shape();
	~MD_Shape();

	//core============================================================================//
	void CollisionResponse(CD_Polygon& A, CD_Polygon& B, CD_Polygon& Aresponse);
	void Draw();
};

#endif