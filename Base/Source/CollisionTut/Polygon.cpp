#include "Polygon.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
MD face
/*********************************************************************************************************/
MD_Face::MD_Face() {}
MD_Face::MD_Face(MD_Face& copy) {
	CopyFrom(copy);
}
MD_Face::~MD_Face() {}

/*********************************************************************************************************
Init time: Set this face
/*********************************************************************************************************/
void MD_Face::SetFace(float faceLen)
{
	this->faceLen = faceLen;
}

void MD_Face::CopyFrom(MD_Face& copy)
{
	this->pos = copy.pos;
	this->pt0 = copy.pt0;
	this->pt1 = copy.pt1;
	this->faceLen = copy.faceLen;
	this->dirAlongFace = copy.dirAlongFace;
	this->normal = copy.normal;
	this->angle = copy.angle;
}

/*********************************************************************************************************
Update time: recalculate face
/*********************************************************************************************************/
void MD_Face::RecalFace(Vector3 new_mfp, Vector3 dirAlongFace)
{
	this->pos = new_mfp;
	this->dirAlongFace = dirAlongFace;
	this->pt0 = this->pos - (dirAlongFace * faceLen* 0.5f);
	this->pt1 = this->pos + (dirAlongFace * faceLen* 0.5f);
	this->normal = this->dirAlongFace.Cross(Vector3(0, 0, -1));
	this->angle = Vector3::getAngleFromDir(this->normal.x, this->normal.y);
}
void MD_Face::RecalFace_pt0(Vector3 new_mfp0, Vector3 dirAlongFace)
{
	this->pt0 = new_mfp0;
	this->dirAlongFace = dirAlongFace;
	this->pos = this->pt0 + (dirAlongFace * faceLen * 0.5f);
	this->pt1 = this->pt0 + (dirAlongFace * faceLen);
	this->normal = this->dirAlongFace.Cross(Vector3(0, 0, -1));
	this->angle = Vector3::getAngleFromDir(this->normal.x, this->normal.y);
}
void MD_Face::RecalFace_pt1(Vector3 new_mfp1, Vector3 dirAlongFace)
{
	this->pt1 = new_mfp1;
	this->dirAlongFace = dirAlongFace;
	this->pos = this->pt1 - (dirAlongFace * faceLen * 0.5f);
	this->pt0 = this->pt1 - (dirAlongFace * faceLen);
	this->normal = this->dirAlongFace.Cross(Vector3(0, 0, -1));
	this->angle = Vector3::getAngleFromDir(this->normal.x, this->normal.y);
}

/*********************************************************************************************************
Draw MD face
/*********************************************************************************************************/
void MD_Face::Draw(Mesh* lineMesh, float thickness)
{
	//draw the face
	CU::view.Draw_Line(pt0, angle - 90.f, faceLen, thickness, lineMesh);
	lineMesh->Draw();

	//face normal
	/*CU::view.Draw_Line(pos, angle, 40.f, 1.f, CU::line_yellow);
	CU::line_yellow->Draw();*/
}

/*********************************************************************************************************
CTD polygon
/*********************************************************************************************************/
CD_Polygon::CD_Polygon()
{
}

CD_Polygon::~CD_Polygon()
{
	delete mesh;
}

/*********************************************************************************************************
Init
/*********************************************************************************************************/
void CD_Polygon::Init(Vector3 shapePos, Vector3 shapeScale, bool projectorShape, Color color, Mesh* lineMesh)
{
	this->shapePos = shapePos;
	this->shapeScale = shapeScale;
	this->shapeAngle = 0.f;
	this->projMag = 0.f;
	this->shapeDir.Set(1, 0, 0);
	this->projectorShape = projectorShape;

	//add points
	Vector3 halfScale = shapeScale * 0.5f;
	pointOffsetList[0] = Vector3(-halfScale.x, 0, 0);
	pointOffsetList[1] = Vector3(0, halfScale.y, 0);
	pointOffsetList[2] = Vector3(halfScale.x, 0, 0);
	pointOffsetList[3] = Vector3(0, -halfScale.y, 0);

	//create face
	faceList[0].SetFace(shapeScale.y);
	faceList[0].RecalFace(pointOffsetList[0] + shapePos, Vector3(0, 1, 0));
	faceList[1].SetFace(shapeScale.x);
	faceList[1].RecalFace(pointOffsetList[1] + shapePos, Vector3(1, 0, 0));
	faceList[2].SetFace(shapeScale.y);
	faceList[2].RecalFace(pointOffsetList[2] + shapePos, Vector3(0, -1, 0));
	faceList[3].SetFace(shapeScale.x);
	faceList[3].RecalFace(pointOffsetList[3] + shapePos, Vector3(-1, 0, 0));

	//create mesh
	mesh = new Mesh();
	mesh->Init_Quad(false, shapeScale, color);
	this->lineMesh = lineMesh;

	//do one in init
	RecalculatePoints();
}

void CD_Polygon::CopyFrom(CD_Polygon& copyFromMe)
{
	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)
		this->pointOffsetList[i] = copyFromMe.pointOffsetList[i];

	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)
	{
		this->faceList[i].CopyFrom(copyFromMe.faceList[i]);
	}
	this->shapePos = copyFromMe.shapePos;
	this->shapeScale = copyFromMe.shapeScale;
	this->shapeAngle = copyFromMe.shapeAngle;
	this->shapeDir = shapeDir;
}

/*********************************************************************************************************
project shape
/*********************************************************************************************************/
void CD_Polygon::ProjectShape(Vector3 projVel, float projRot)
{
	projDir.x = cos(Math::DegreeToRadian(shapeAngle));
	projDir.y = sin(Math::DegreeToRadian(shapeAngle));
	projMag = 0.f;
	this->projVel = projVel;

	if (!projVel.IsZero())
		projMag = projVel.Length();

	this->projRot = projRot;

	//angular velocity
	Vector3 halfScale = shapeScale * 0.5f;
	r = sqrt(halfScale.x * halfScale.x + halfScale.y * halfScale.y);
	av = Math::DegreeToRadian(projRot) * r;
}

/*********************************************************************************************************
project + move shape
/*********************************************************************************************************/
void CD_Polygon::ProjectObstacle(Vector3 projVel, float projRot)
{
	ProjectShape(projVel, projRot);
	Translate(projVel.x, projVel.y, projVel.z);
	Rotate(0, 0, projRot);
}

/*********************************************************************************************************
transform overwrites
/*********************************************************************************************************/
void CD_Polygon::Translate(float x, float y, float z)
{
	shapePos += Vector3(x, y, z);
}

void CD_Polygon::Rotate(float x, float y, float z)
{
	shapeAngle += z;
}

void CD_Polygon::SetTranslate(float x, float y, float z)
{
	shapePos = Vector3(x, y, z);
}

void CD_Polygon::SetRotate(float x, float y, float z)
{
	shapeAngle = z;
}
void CD_Polygon::SetTransformation(CD_Polygon& copyMe)
{
	SetTranslate(copyMe.shapePos.x, copyMe.shapePos.y, copyMe.shapePos.z);
	SetRotate(0, 0, copyMe.shapeAngle);
}

/*********************************************************************************************************
transform overwrites
/*********************************************************************************************************/
Vector3 newNormalList[4];
void CD_Polygon::RecalculatePoints()
{
	CU::mtx[1].SetToRotation(shapeAngle, 0, 0, 1);
	CU::mtx[4].SetToTranslation(shapePos.x, shapePos.y, 0.f);

	//angle to dir-----------------------------------------------------------------------//
	shapeDir.x = cos(Math::DegreeToRadian(shapeAngle));
	shapeDir.y = sin(Math::DegreeToRadian(shapeAngle));

	//new face dir-----------------------------------------------------------------------//
	newNormalList[0].Set(-shapeDir.x, -shapeDir.y, 0);	//dir (-1, 0)
	newNormalList[1].Set(-shapeDir.y, shapeDir.x, 0);	//dir (0, 1)
	newNormalList[2] = shapeDir;	//dir (1, 0)
	newNormalList[3].Set(shapeDir.y, -shapeDir.x, 0);	//dir (0, -1)

	//All points pos calculated with updated TRS------------------------------------------//
	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)
	{
		CU::mtx[2].SetToTranslation(pointOffsetList[i].x, pointOffsetList[i].y, 0.f);

		//final transformation matrix-----------------------------//
		CU::mtx[3] = CU::mtx[4] * CU::mtx[1] * CU::mtx[2];

		Vector3 newPos(1, 1, 0);
		newPos = CU::mtx[3] * newPos;
		faceList[i].RecalFace(newPos, newNormalList[i].Cross(Vector3(0, 0, 1)));
	}
}

/*********************************************************************************************************
Draw
/*********************************************************************************************************/
void CD_Polygon::Draw(float z)
{
	//draw mesh-------------------------------------------------------------------//
	CU::view.Pre_DrawMesh(shapePos, shapeAngle, Vector3(1, 1, 1), mesh);
	mesh->Draw();

	//draw faces------------------------------------------------------------------//
	//for better render accuracy
	/*if (!projectorShape)
	{
		for (int i = 0; i < Polygon_TOTAL_PTS; ++i)
			faceList[i].Draw(lineMesh, 1.f);
	}
	else
	{
		for (int i = 0; i < Polygon_TOTAL_PTS; ++i)
			faceList[i].Draw(lineMesh, 0.5f);
	}*/
}