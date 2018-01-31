#ifndef POLYGON_H
#define POLYGON_H
#include "Transform.h"
#include "Mesh.h"
#define Polygon_TOTAL_PTS 4

/***************************************************************************************************************
Minkowki face
Do face calculation
Renders the face
CopyFrom: copies everything except mesh
pt: middle point of the face
/***************************************************************************************************************/
struct MD_Face
{
	//local data
	Vector3 pos, pt0, pt1, normal;
	Vector3 dirAlongFace;
	float faceLen, angle;

	//core------------------------------------------------------//
	MD_Face();
	MD_Face(MD_Face& copy);
	~MD_Face();

	void SetFace(float faceLen);
	void CopyFrom(MD_Face& copyFromMe);
	void RecalFace(Vector3 new_mfp, Vector3 dirAlongFace);
	void RecalFace_pt0(Vector3 new_mfp0, Vector3 dirAlongFace);
	void RecalFace_pt1(Vector3 new_mfp1, Vector3 dirAlongFace);
	void Draw(Mesh* lineMesh, float thickness);
};

/***************************************************************************************************************
Collision Tutorial Discrete (CTD)
Polygon:
Add points to shape, relative to shape pos
Call AddPoints() after Init and before more transformation functions
Call calculateFaces() after you are done
Call ProjectShape(..) for projection
Call SetAsProjected(..) AFTER CalculateTRS()

NOTE:
-Remove CalculateTRS() overwrite as this is meant to be used a as a component
-Can only be convex and no 2 faces have the same normal
-Can only start on shapeAngle = 0

CopyFrom: copies everything except mesh and pointOffset_List

Author: Tan Yie Cher
Date: 18/9/17
/***************************************************************************************************************/
class CD_Polygon
{
public:

	//main vars---------------------------------------------------------//
	Vector3 shapePos, shapeScale, shapeDir;
	Vector3 pointOffsetList[Polygon_TOTAL_PTS];
	MD_Face faceList[Polygon_TOTAL_PTS];

	//render purposes---------------------------------------------------//
	float shapeAngle;
	Mesh* mesh, *lineMesh;

	//projection--------------------------------------------------------//
	bool projectorShape;	//is this shape meant for projection?
	Vector3 projDir, projVel;
	float projMag;
	float projRot;
	float av;	//angular velocity
	float r;	//radius

	//CD==============================================================================//
	CD_Polygon();
	~CD_Polygon();

	//Transform=======================================================================//
	void ProjectShape(Vector3 projVel, float projRot);
	void ProjectObstacle(Vector3 projVel, float projRot);

	void Translate(float x, float y, float z);
	void Rotate(float x, float y, float z);
	void SetTranslate(float x, float y, float z);
	void SetRotate(float x, float y, float z);
	void SetTransformation(CD_Polygon& copyMe);

	void RecalculatePoints();

	//Core=============================================================================//
	void Init(Vector3 shapePos, Vector3 shapeScale, bool projectorShape, Color color, Mesh* lineMesh);
	void CopyFrom(CD_Polygon& copyFromMe);
	void Draw(float z);
};

#endif