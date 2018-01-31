#include "GJK_Simplex2D.h"
#include "CoreUtilities.h"
const float ANGULAR_TOLERANCE3 = 0.1f;
Vector3 ao, ab, ac, abPerb, acPerb, contactPt, p1, p2;

/*********************************************************************************************************
GJK_Simplex2D
/*********************************************************************************************************/
GJK_Simplex_2D::GJK_Simplex_2D() {
}
GJK_Simplex_2D::~GJK_Simplex_2D() {
}

/*********************************************************************************************************
get closest point to origin along point A and B
type:
0: point A is the dir
1: neither point A or B is the dir
2: point B is the dir
/*********************************************************************************************************/
Vector3 GJK_Simplex_2D::closestPointToOrigin(Vector3 A, Vector3 B, int& type)
{
	ab = B - A;
	ao = -A;
	float t = (ao.Dot(ab)) / ab.LengthSquared();

	//get the contact point
	if (t > 1.f)
	{
		contactPt = B;
		type = 2;
	}
	else if (t < 0.f)
	{
		contactPt = A;
		type = 0;
	}
	else
	{
		contactPt = ab * t + A;
		type = 1;
	}
		
	return contactPt;
}

/*********************************************************************************************************
get supporting vertice for a extreme direction (on the edge)
/*********************************************************************************************************/
float dotProd, largestDotProd, largestDotProd_index;
int GJK_Simplex_2D::GetSupportingVertice(CD_Polygon& poly, Vector3 dir)
{
	dotProd = -10.f;
	largestDotProd = -10.f;
	largestDotProd_index = -1;

	//get vertice with largest dot product
	for (int i = 0; i < 4; ++i)
	{
		dotProd = (poly.faceList[i].pt0 - poly.shapePos).Dot(dir);

		if (dotProd > largestDotProd)
		{
			largestDotProd = dotProd;
			largestDotProd_index = i;
		}
	}

	return largestDotProd_index;
}

/*********************************************************************************************************
reset data
/*********************************************************************************************************/
void GJK_Simplex_2D::Reset()
{
	total_vertices = 0;
	closestDist = -1.f;
	Asa = Asb = Bsa = Bsb = 0;
	doesNotContain = false;	//unverified
}

/*********************************************************************************************************
return true if contains origin
sa: index of the vertex of polygon A
sb: index of the vertex of polygon B
/*********************************************************************************************************/
Vector3 extremeA, extremeB, last;
Vector3 GJK_Simplex_2D::GetNewLast(Vector3 dir, CD_Polygon& A, CD_Polygon& B, int& sa, int& sb)
{
	//get the extreme of A
	sa = GetSupportingVertice(A, dir);
	extremeA = A.faceList[sa].pt0;

	//get the extreme of B
	sb = GetSupportingVertice(B, dir * -1.f);
	extremeB = B.faceList[sb].pt0;

	//get the last
	last = extremeA - extremeB;
	return last;	//last -> second last
}

Vector3 GJK_Simplex_2D::tripleProduct(Vector3 A, Vector3 B, Vector3 C)
{
	return (B * C.Dot(A)) - (A * C.Dot(B));
	//return (A.Cross(B)).Cross(C);
}

/*********************************************************************************************************
check if simplex contains origin and if not, get new dir.
and discard the extra corrosponding pt
return false if simplex does not contain origin

A = (very last)
B = (second last)
C = (third last)
/*********************************************************************************************************/
bool GJK_Simplex_2D::containsOrigin(CD_Polygon& A, CD_Polygon& B)
{
	if (vertices[0].Dot(dir) <= 0.f)
	{
		doesNotContain = true;
		return true;
	}

	int sa = 0, sb = 0;	//NOT USED HERE
	ao = vertices[0] * -1.f;
	ab = vertices[1] - vertices[0];
	
	//triangle test--------------------------------------------------------//
	if (total_vertices == 3)
	{
		ac = vertices[2] - vertices[0];

		//check if origin in R4 (AB region)--------------------------------//
		abPerb = tripleProduct(ac, ab, ab);
		acPerb = tripleProduct(ab, ac, ac);

		if (abPerb.Dot(ao) > 0.f)	//in
		{
			//remove point C, AB perb is the new dir
			vertices[2] = vertices[1];
			vertices[1] = vertices[0];

			//Add new point
			vertices[0] = GetNewLast(abPerb, A, B, sa, sb);
			dir = abPerb;
		}
		else
		{
			//check if origin in R3 (AC region)----------------------------//
			if (acPerb.Dot(ao) > 0.f)	//in
			{
				//remove point B, AC perb is the new dir
				vertices[1] = vertices[0];

				//Add new point
				vertices[0] = GetNewLast(acPerb, A, B, sa, sb);
				dir = acPerb;
			}

			//simplex contains origin!-------------------------------------//
			else
				return true;
		}
	}

	//line segment---------------------------------------------------------//
	else
	{
		//Add new point
		dir = tripleProduct(ab, ao, ab);
		vertices[2] = vertices[1];
		vertices[1] = vertices[0];
		vertices[0] = GetNewLast(dir, A, B, sa, sb);
		total_vertices = 3;
	}

	return false;
}

/*********************************************************************************************************
StartSimplexCheck
return false if first check origin is already outside the MD
/*********************************************************************************************************/
bool GJK_Simplex_2D::StartSimplexCheck(CD_Polygon& A, CD_Polygon& B)
{
	Reset();

	int sa = 0, sb = 0;	//NOT USED HERE

	//start dir is shape1 -> shape2----------------------------------------//
	dir = B.shapePos - A.shapePos;
	
	//get the very first point---------------------------------------------//
	vertices[1] = GetNewLast(dir, A, B, sa, sb);

	//get the second point-------------------------------------------------//
	dir *= -1.f;
	vertices[0] = GetNewLast(dir, A, B, sa, sb);
	total_vertices = 2;

	return true;
}

/*********************************************************************************************************
step through simplex creation
return true if origin contained
Reminder:
pt A: vertice[0] last point added
pt B: vertice[1] 2nd last point added
/*********************************************************************************************************/
bool GJK_Simplex_2D::StepThrough(CD_Polygon& A, CD_Polygon& B)
{
	return containsOrigin(A, B);
}

bool GJK_Simplex_2D::CollisionCheck(CD_Polygon& A, CD_Polygon& B)
{
	StartSimplexCheck(A, B);

	while (true)
	{
		if (containsOrigin(A, B))
		{
			if (doesNotContain)
				return false;
			return true;
		}
	}
	return false;	//ERROR!
}

/*********************************************************************************************************
get closest points
/*********************************************************************************************************/
void GJK_Simplex_2D::GetClosestPoints(CD_Polygon& A, CD_Polygon& B)
{
	Reset();
	int type = -1;
	int tmp_sa = 0, tmp_sb = 0;

	//set the initial two points first-------------------------------------//
	dir = B.shapePos - A.shapePos;

	//get the very first point---------------------------------------------//
	vertices[1] = GetNewLast(dir, A, B, Bsa, Bsb);

	//get the second point-------------------------------------------------//
	vertices[0] = GetNewLast(dir * -1.f, A, B, Asa, Asb);
	
	//get the closest point to origin along A and B------------------------//
	dir = closestPointToOrigin(vertices[0], vertices[1], type);
	
	Vector3 c;
	while (true)
	{
		dir *= -1.f;	//negate so pointing at Origin
		
		if (dir.LengthSquared() <= 0.f)	//touching
		{
			closestDist = dir.Length();
			//computeClosestPoints(A, B);
			return;
		}

		//obtain new MD point along dir------------------------------------//
		c = GetNewLast(dir, A, B, tmp_sa, tmp_sb);	//C

		//USE TOLERANCE VALUE FOR CURVED SHAPES
		if (c.Same(vertices[0]) || c.Same(vertices[1]))
		{
			closestDist = dir.Length();
			//computeClosestPoints(A, B);
			return;
		}
		
		//get the point closer to origin (A or B), C is already the closest//
		p1 = closestPointToOrigin(c, vertices[0], type);	//CA
		p2 = closestPointToOrigin(c, vertices[1], type);	//CB

		//get the new closest edge-----------------------------------------//
		if (p1.LengthSquared() < p2.LengthSquared())	//CA
		{
			//B replaced with A, A replaced with C
			vertices[1] = vertices[0];
			vertices[0] = c;
			dir = p1;

			//polygon points
			Bsa = Asa;
			Bsb = Asb;
			Asa = tmp_sa;
			Asb = tmp_sb;
		}
		else
		{
			//B remains, A replaced with C
			vertices[0] = c;
			dir = p2;

			//polygon points
			Asa = tmp_sa;
			Asb = tmp_sb;
		}
	}
}

/*********************************************************************************************************
compute closest points between two polygons
Called when closest dist. found
/*********************************************************************************************************/
void GJK_Simplex_2D::computeClosestPoints(CD_Polygon& A, CD_Polygon& B)
{
	/*cout << "closestDist: " << closestDist << endl;*/
	Vector3 L = vertices[1] - vertices[0];
	float LdotL = L.Dot(L);
	float LdotA = -L.Dot(vertices[0]);
	float lambda2 = LdotA / LdotL;
	float lambda1 = 1.f - lambda2;
	
	if (L.IsZero())
	{
		poly_A_closest_pt = A.faceList[Asa].pt0;
		poly_B_closest_pt = B.faceList[Asb].pt0;
	}
	else if (lambda1 < 0.f)
	{
		poly_A_closest_pt = A.faceList[Bsa].pt0;
		poly_B_closest_pt = B.faceList[Bsb].pt0;
	}
	else if (lambda2 < 0.f)
	{
		poly_A_closest_pt = A.faceList[Asa].pt0;
		poly_B_closest_pt = B.faceList[Asb].pt0;
	}
	else
	{
		poly_A_closest_pt = lambda1 * A.faceList[Asa].pt0 + lambda2 * A.faceList[Bsa].pt0;
		poly_B_closest_pt = lambda1 * B.faceList[Asb].pt0 + lambda2 * B.faceList[Bsb].pt0;
	}
}

/*********************************************************************************************************
Create closest contact with origin
/*********************************************************************************************************/
void GJK_Simplex_2D::TransformShape(CD_Polygon& shape, Vector3 vel, float rotAngle, float t)
{
	shape.Translate(vel.x * t, vel.y * t, vel.z * t);
	shape.Rotate(0, 0, rotAngle * t);
	shape.RecalculatePoints();
}
void GJK_Simplex_2D::SetShape_ToAbsoute(CD_Polygon& projected, CD_Polygon& main)
{
	projected.CopyFrom(main);
	projected.Translate(main.projVel.x, main.projVel.y, main.projVel.z);
	projected.Rotate(0, 0, main.projRot);
	projected.RecalculatePoints();
}

/*********************************************************************************************************
Angular continuous collision response
/*********************************************************************************************************/
bool GJK_Simplex_2D::CollisionResponse(CD_Polygon& A, CD_Polygon& Aresponse, CD_Polygon& B)
{
	//set up----------------------------------------------------------------------//
	noCollision = false;
	t = t_tracker = 0.f;

	//A_Response copied from A
	Aresponse.CopyFrom(A);
	GetClosestPoints(A, B);
	
	//loop------------------------------------------------------------------------//
	Vector3 ray = A.projVel;	//relative vel
	while (!noCollision && closestDist >= ANGULAR_TOLERANCE3)
	{
		dir.Normalize();	//dir must be normalized!

		float rayN = ray.Dot(dir);
		float dRel = rayN + A.av + B.av;	//REMOVED A.r

		t = abs(closestDist / dRel);	//tutorial version resets the whole matrix
		t_tracker += t;

		if (t_tracker < 0 || t_tracker > 1)
		{
			noCollision = true;
			SetShape_ToAbsoute(Aresponse, A);
			return false;
		}

		TransformShape(Aresponse, A.projVel, A.projRot, t);
		GetClosestPoints(Aresponse, B);
	}

	return true;
}

/*********************************************************************************************************
draw
/*********************************************************************************************************/
void GJK_Simplex_2D::Draw()
{
	//closest points
	//CU::view.Pre_DrawMesh(poly_A_closest_pt, Vector3(10, 10, 10), CU::sphere_dark_green);
	//CU::sphere_dark_green->Draw();
	//CU::view.Pre_DrawMesh(poly_B_closest_pt, Vector3(10, 10, 10), CU::sphere_dark_green);
	//CU::sphere_dark_green->Draw();
}
