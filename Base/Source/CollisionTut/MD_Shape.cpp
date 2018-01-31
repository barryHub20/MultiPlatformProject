#include "MD_Shape.h"
#include "CoreUtilities.h"
const float ANGULAR_TOLERANCE2 = 0.1f;

/*********************************************************************************************************
MD shape
/*********************************************************************************************************/
MD_Shape::MD_Shape() {
	Reset();
}
MD_Shape::~MD_Shape() {}

/*********************************************************************************************************
Reset MD shape
/*********************************************************************************************************/
void MD_Shape::Reset()
{
	faceCount = 0;
	shapeChange_break = nonBridged_faceCount = -1;
}

/*********************************************************************************************************
Create closest contact with origin
/*********************************************************************************************************/
void MD_Shape::CreateContact()
{
	//resets--------------------------------------------------------------------//
	int closest_face_index = -1;
	int closest_pt_index = -1;
	float closest_face_dist = -1000000000.f;
	float closest_pt_dist = 100000000.f;
	contactNormal.Set(0, 0, 0);
	Vector3 interset_pos_face, intersect_pos_pt;

	//face vars-----------------------------------------------------------------//
	Vector3 projection, pt0, pt1;

	//shared vars---------------------------------------------------------------//
	Vector3 origin;

	//loop through faces--------------------------------------------------------//
	for (int i = 0; i < faceCount; ++i)
	{
		float faceDist = faceList[i].face.pt0.Dot(faceList[i].face.normal);

		if (faceDist >= 0.f)	//facing away
			continue;

		//the vector from origin to face
		projection = origin + (faceDist * faceList[i].face.normal);
		pt0 = faceList[i].face.pt0;
		pt1 = faceList[i].face.pt1;

		//check if projection inside the given points
		float lenSq = faceList[i].face.faceLen * faceList[i].face.faceLen;
		float pt0_to_proj = (projection - pt0).LengthSquared();
		float pt1_to_proj = (projection - pt1).LengthSquared();
		bool projection_intersects = pt1_to_proj <= lenSq && pt0_to_proj <= lenSq;

		//get closest face
		if (projection_intersects && faceDist > closest_face_dist)
		{
			closest_face_index = i;
			closest_face_dist = faceDist;
			interset_pos_face = projection;
		}
	}
	closest_face_dist = -closest_face_dist;	//make it positive

	//loop through points-------------------------------------------------------//
	for (int i = 0; i < faceCount; ++i)
	{
		float ptDist = faceList[i].face.pt0.LengthSquared();

		if (closest_pt_dist > ptDist)
		{
			closest_pt_index = i;
			closest_pt_dist = ptDist;
			intersect_pos_pt = faceList[i].face.pt0;
		}
	}

	//get the closest dist------------------------------------------------------//
	if (closest_face_index != -1 && (closest_face_dist * closest_face_dist) < closest_pt_dist)
	{
		contactDist = closest_face_dist;
		contactNormal = interset_pos_face;
	}
	else
	{
		contactDist = faceList[closest_pt_index].face.pt0.Length();
		contactNormal = intersect_pos_pt;
	}
	
	//normal
	if (!contactNormal.IsZero())
		contactNormal = contactNormal.Normalized();
}

/*********************************************************************************************************
Create closest contact with origin
/*********************************************************************************************************/
void MD_Shape::TransformShape(CD_Polygon& shape, Vector3 vel, float rotAngle, float t)
{
	shape.Translate(vel.x * t, vel.y * t, vel.z * t);
	shape.Rotate(0, 0, rotAngle * t);
	shape.RecalculatePoints();
}
void MD_Shape::SetShape_ToAbsoute(CD_Polygon& projected, CD_Polygon& main)
{
	projected.CopyFrom(main);
	projected.Translate(main.projVel.x, main.projVel.y, main.projVel.z);
	projected.Rotate(0, 0, main.projRot);
	projected.RecalculatePoints();
}

/*********************************************************************************************************
Add new face
/*********************************************************************************************************/
void MD_Shape::AddNewFace(Vector3 pos, Vector3 dirAlongFace, float faceLen, int supportVert)
{
	faceList[faceCount].face.SetFace(faceLen);
	faceList[faceCount].face.RecalFace_pt0(pos, dirAlongFace);
	faceList[faceCount++].supportVert = supportVert;
}

/*********************************************************************************************************
Bridge points
/*********************************************************************************************************/
void MD_Shape::BridgePoints(CD_Polygon& A, CD_Polygon& B)
{
	//B-A------------------------------------------------------------------------//
	for (int i = 0; i < shapeChange_break; ++i)
	{
		int i1 = i;
		int i2 = (i + 1 == shapeChange_break) ? 0 : i + 1;
		AddBridgePoint(i1, i2, i1, A, B, false);
	}

	//A-B------------------------------------------------------------------------//
	for (int i = shapeChange_break; i < nonBridged_faceCount; ++i)
	{
		int i1 = i;
		int i2 = (i + 1 == nonBridged_faceCount) ? shapeChange_break : i + 1;
		AddBridgePoint(i1, i2, i1 - shapeChange_break, B, A, true);
	}
}

/*********************************************************************************************************
Add bridge point
i1, i2: pt0 and pt1 index
faceIndex: index of the face to use
poly: if B-A, pass in A
isAB: is A-B
/*********************************************************************************************************/
void MD_Shape::AddBridgePoint(int i1, int i2, int faceIndex, CD_Polygon& poly, CD_Polygon& other, bool isAB)
{
	Vector3 pt0 = faceList[i1].face.pt1;
	Vector3 dirAlongFace;
	float faceLen = other.faceList[faceList[i1].supportVert].faceLen;

	if (!isAB)
		dirAlongFace = other.faceList[faceList[i1].supportVert].dirAlongFace;
	else
		dirAlongFace = -other.faceList[faceList[i1].supportVert].dirAlongFace;

	AddNewFace(pt0, dirAlongFace, faceLen, -1);
}

/*********************************************************************************************************
Create MD shape
/*********************************************************************************************************/
void MD_Shape::Create_MDShape(CD_Polygon& A, CD_Polygon& B)
{
	//reset------------------------------------------------------------------------//
	Reset();

	MD_origin = B.shapePos - A.shapePos;

	//B-A--------------------------------------------------------------------------//
	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)	//total faces in A
	{
		//Get the opp. supporting  vertice
		int supvert_index = GetSupportingVertice(A.faceList[i], B);

		//form a new face
		AddNewFace(B.faceList[supvert_index].pt0 - A.faceList[i].pt0, -A.faceList[i].dirAlongFace, A.faceList[i].faceLen, supvert_index);
	}
	shapeChange_break = faceCount;	//A-B from now on

	//A-B--------------------------------------------------------------------------//
	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)	//total faces in B
	{
		//Get the opp. supporting  vertice
		int supvert_index = GetSupportingVertice(B.faceList[i], A);

		//form a new face
		AddNewFace(B.faceList[i].pt0 - A.faceList[supvert_index].pt0, B.faceList[i].dirAlongFace, B.faceList[i].faceLen, supvert_index);
	}
	nonBridged_faceCount = faceCount;

	//bridge points----------------------------------------------------------------//
	BridgePoints(A, B);
}

/*********************************************************************************************************
Collision response
/*********************************************************************************************************/
void MD_Shape::CollisionResponse(CD_Polygon& A, CD_Polygon& B, CD_Polygon& Aresponse)
{
	//set up----------------------------------------------------------------------//
	Create_MDShape(A, B);

	//A_Response copied from A
	Aresponse.CopyFrom(A);
	noCollision = false;
	t = t_tracker = 0.f;

	//create initial contact
	CreateContact();
	
	//loop------------------------------------------------------------------------//
	while (!noCollision && contactDist >= ANGULAR_TOLERANCE2)
	{
		Vector3 ray = A.projVel;	//relative vel
		float rayN = ray.Dot(contactNormal);
		float dRel = rayN + A.av + B.av;	//REMOVED A.r

		t = abs(contactDist / dRel);	//tutorial version resets the whole matrix
		t_tracker += t;

		if (t_tracker < 0 || t_tracker > 1)
		{
			noCollision = true;
			SetShape_ToAbsoute(Aresponse, A);
			return;
		}

		TransformShape(Aresponse, A.projVel, A.projRot, t);
		Create_MDShape(Aresponse, B);

		CreateContact();
	}
}

/*********************************************************************************************************
Draw
/*********************************************************************************************************/
void MD_Shape::Draw()
{
	//MD-----------------------------------------------------------------------------------//
	for (int i = 0; i < shapeChange_break; ++i)
	{
		faceList[i].face.Draw(CU::line_purple, 1.f);
	}
	for (int i = shapeChange_break; i < nonBridged_faceCount; ++i)
	{
		faceList[i].face.Draw(CU::line_red, 1.f);
	}

	for (int i = nonBridged_faceCount; i < nonBridged_faceCount + shapeChange_break; ++i)
	{
		faceList[i].face.Draw(CU::line_purple, 1.f);
	}
	for (int i = nonBridged_faceCount + shapeChange_break; i < faceCount; ++i)
	{
		faceList[i].face.Draw(CU::line_red, 1.f);
	}

	//closest dist-------------------------------------------------------------------------//
	float contactNormal_angle = Vector3::getAngleFromDir(contactNormal.x, contactNormal.y);
	CU::view.Draw_Line(Vector3(0, 0, 0), contactNormal_angle, contactDist, 0.5f, CU::line_green);
	CU::line_green->Draw();
}

/*********************************************************************************************************
Get vertices most 'opposite' given normal
Returns:
the index of the face from CHECK
/*********************************************************************************************************/
int MD_Shape::GetSupportingVertice(MD_Face& face, CD_Polygon& check)
{
	float lowest_dotProduct = 10000.f;
	int index = -1;

	//get most opposite dot product first
	for (int i = 0; i < Polygon_TOTAL_PTS; ++i)	//total faces
	{
		//get face normal
		float dot_val = face.normal.Dot(check.faceList[i].normal);

		if (lowest_dotProduct > dot_val)
		{
			lowest_dotProduct = dot_val;
			index = i;
		}
	}
	return index;
}