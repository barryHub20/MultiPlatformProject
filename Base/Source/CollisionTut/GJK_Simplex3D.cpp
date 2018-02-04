#include "GJK_Simplex3D.h"
#include "CoreUtilities.h"
Vector3 GJK_Simplex_3D::ao;
Vector3 GJK_Simplex_3D::ab;
Vector3 GJK_Simplex_3D::contactPt;

/*********************************************************************************************************
GJK_Simplex3D
/*********************************************************************************************************/
GJK_Simplex_3D::GJK_Simplex_3D()
{
	dotProd = largestDotProd = largestDotProd_index = 0.f;
	simplexMesh = NULL;
}

GJK_Simplex_3D::~GJK_Simplex_3D()
{
	if(simplexMesh)
		delete simplexMesh;
}

/*********************************************************************************************************
reset data
/*********************************************************************************************************/
void GJK_Simplex_3D::Reset()
{
	total_vertices = 0;
	closestDist = -1.f;
	doesNotContain = false;	//unverified
}

/*********************************************************************************************************
Get support vertice
/*********************************************************************************************************/
int GJK_Simplex_3D::GetSupportingVertice(CD_Polygon_3D& poly, Vector3 dir)
{
	dotProd = -10.f;
	largestDotProd = -10.f;
	largestDotProd_index = -1;

	//get vertice with largest dot product
	for (int i = 0; i < 8; ++i)
	{
		dotProd = (poly.pointList[i] - poly.shapePos).Dot(dir);

		if (dotProd > largestDotProd)
		{
			largestDotProd = dotProd;
			largestDotProd_index = i;
		}
	}
	return largestDotProd_index;
}

/*********************************************************************************************************
Get new MD point furthest in given direction
sa: index of the vertex of polygon A
sb: index of the vertex of polygon B
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::GetNewLast(Vector3 dir, CD_Polygon_3D& A, CD_Polygon_3D& B)
{
	//get the extreme of A
	extremeA = A.pointList[GetSupportingVertice(A, dir)];

	//get the extreme of B
	extremeB = B.pointList[GetSupportingVertice(B, dir * -1.f)];

	//get the last
	last = extremeA - extremeB;

	return last;	//last -> second last
}

/*********************************************************************************************************
Project origin to AB and get the contact point
Vector3 vertices[0]: the 3 points of the triangle
(doesn't have to be in any order)
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::closestPointToOrigin(Vector3 vert0, Vector3 vert1, Vector3 vert2)
{
	Vector3 vertices[3];
	vertices[0] = vert0;
	vertices[1] = vert1;
	vertices[2] = vert2;

	edge1 = vertices[1] - vertices[0];
	edge2 = vertices[2] - vertices[0];

	Vector3 rayOrigin;	//origin 0,0,0
	Vector3 rayVector = edge2.Cross(edge1);	//compute normal
	
	//point towards origin---------------------------------------------//
	if (rayVector.Dot(-vertices[0]) < 0.f)
		rayVector *= -1.f;

	h = rayVector.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -Math::EPSILON && a < Math::EPSILON)
		return NonIntersection(vertices[0], vertices[1], vertices[2]);

	f = 1.f / a;
	s = rayOrigin - vertices[0];
	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
		return NonIntersection(vertices[0], vertices[1], vertices[2]);

	q = s.Cross(edge1);
	v = f * rayVector.Dot(q);

	if (v < 0.f || v + u > 1.f)
		return NonIntersection(vertices[0], vertices[1], vertices[2]);

	float t = f * edge2.Dot(q);

	triangle_intersectPt = rayOrigin + rayVector * t;
	return triangle_intersectPt;
}

/*********************************************************************************************************
check the 2D and 1D projections
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::NonIntersection(Vector3 vert0, Vector3 vert1, Vector3 vert2)
{
	Vector3 vertices[3];
	vertices[0] = vert0;
	vertices[1] = vert1;
	vertices[2] = vert2;

	//AB, AC, BC edges projection check
	//A(0) -> C(2)

	edges[0] = vertices[1] - vertices[0];	//AB
	edges[1] = vertices[2] - vertices[0];	//AC
	edges[2] = vertices[2] - vertices[1];	//BC

	Vector3 AO;
	float t = 0.f;

	//AB-------------------------------------------------------------------------//
	AO = -vertices[0];
	t = (AO.Dot(edges[0])) / edges[0].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[0] = edges[0] * t + vertices[0];
		lensqList[0] = edges_intersectPt[0].LengthSquared();
	}
	else
		lensqList[0] = 1000000.f;

	//AC-------------------------------------------------------------------------//
	AO = -vertices[0];
	t = (AO.Dot(edges[1])) / edges[1].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[1] = edges[1] * t + vertices[0];
		lensqList[1] = edges_intersectPt[1].LengthSquared();
	}
	else
		lensqList[1] = 1000000.f;

	//BC-------------------------------------------------------------------------//
	AO = -vertices[1];
	t = (AO.Dot(edges[2])) / edges[2].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[2] = edges[2] * t + vertices[1];
		lensqList[2] = edges_intersectPt[2].LengthSquared();
	}
	else
		lensqList[2] = 1000000.f;

	//closest vertex check-------------------------------------------------------//
	for (int i = 0; i < 3; ++i)
		lensqList[i + 3] = vertices[i].LengthSquared();

	//sort-----------------------------------------------------------------------//
	float smallestLensq = lensqList[0];
	int smallestLensq_Index = 0;

	for (int i = 1; i < 6; ++i)
	{
		if (lensqList[i] < smallestLensq)
		{
			smallestLensq = lensqList[i];
			smallestLensq_Index = i;
		}
	}

	//get closest point----------------------------------------------------------//
	if (smallestLensq_Index < 3)
		triangle_intersectPt = edges_intersectPt[smallestLensq_Index];
	else
		triangle_intersectPt = vertices[smallestLensq_Index - 3];

	return triangle_intersectPt;
}

/*********************************************************************************************************
projection of origin onto AB
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::closestPointToOrigin_AB(Vector3 A, Vector3 B)
{
	ab = B - A;
	ao = -A;
	float t = (ao.Dot(ab)) / ab.LengthSquared();

	//get the contact point
	if (t > 1.f)
		contactPt = B;
	else if (t < 0.f)
		contactPt = A;
	else
		contactPt = ab * t + A;

	return contactPt;
}

Vector3 GJK_Simplex_3D::closestPointToOrigin_AB(Vector3 origin, Vector3 A, Vector3 B)
{
	ab = B - A;
	ao = origin - A;
	float t = (ao.Dot(ab)) / ab.LengthSquared();

	//get the contact point
	if (t > 1.f)
		contactPt = B;
	else if (t < 0.f)
		contactPt = A;
	else
		contactPt = ab * t + A;

	return contactPt;
}

int GJK_Simplex_3D::GetMin(float a, float b, float c)
{
	if (a < b && a < c)
		return 0;
	if (b < a && b < c)
		return 1;
	return 2;
}

/*********************************************************************************************************
compute the closest points between 2 3D polygons
/*********************************************************************************************************/
void GJK_Simplex_3D::computeClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B)
{
}

/*********************************************************************************************************
start the GJK search
Get the initial 3 points
/*********************************************************************************************************/
void GJK_Simplex_3D::GetClosestPoints_Start(CD_Polygon_3D& A, CD_Polygon_3D& B)
{
	Reset();

	//create all MD points out-----------------------------------------//
	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
			MD_points[j + i * 8] = A.pointList[i] - B.pointList[j];
	}

	//get random dir---------------------------------------------------//
	dir.Set(0, 1, 0);

	vertices[0] = GetNewLast(dir, A, B);	//A
	vertices[1] = GetNewLast(-dir, A, B);	//B

	dir = closestPointToOrigin_AB(vertices[0], vertices[1]);
	if (dir.Dot(-vertices[0]) < 0.f)	dir *= -1.f;

	vertices[2] = GetNewLast(dir, A, B);	//C
	dir = -closestPointToOrigin(vertices[0], vertices[1], vertices[2]);

	//loop-------------------------------------------------------------//
	newPoint = GetNewLast(dir, A, B);	//C
}

/*********************************************************************************************************
caller function
A/0: first added
C/2: last added
Check if intersection FIRST

To do:
Remove face origin check if found redundant
/*********************************************************************************************************/
void GJK_Simplex_3D::GetClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B)
{
	//if (finishedGJK)
	//	return;
	infinite_loop = false;

	int infinite_counter = 0;
	while (true)
	{

		//terminating condition--------------------------------------------//
		if (newPoint.Same(vertices[0]) || newPoint.Same(vertices[1])
			|| newPoint.Same(vertices[2]))
		{
			closestDist = closestPointToOrigin(vertices[0], vertices[1], vertices[2]).Length();

			if (simplexMesh)
				delete simplexMesh;
			simplexMesh = new Mesh();
			simplexMesh->Init_Triangle(vertices, Color(109, 242, 236), Color(71, 191, 185));

			return;
		}

		//test out the following triangles---------------------------------//
		Vector3 new_ab = closestPointToOrigin(newPoint, vertices[0], vertices[1]);
		Vector3 new_ac = closestPointToOrigin(newPoint, vertices[0], vertices[2]);
		Vector3 new_bc = closestPointToOrigin(newPoint, vertices[1], vertices[2]);

		//the magnitude
		float new_ab_mag = new_ab.LengthSquared();
		float new_ac_mag = new_ac.LengthSquared();
		float new_bc_mag = new_bc.LengthSquared();

		//no similar magnitude---------------------------------------------//
		//if (new_ab_mag != new_ac_mag && new_ab_mag != new_bc_mag && new_ac_mag != new_bc_mag)
		if (!new_ab.Same(new_ac) && !new_ab.Same(new_bc) && !new_ac.Same(new_bc))
		{
			//get the new dir
			if (new_ab_mag < new_ac_mag && new_ab_mag < new_bc_mag)
			{
				dir = -new_ab;
				vertices[2] = newPoint;	//remove C
			}
			else if (new_ac_mag < new_ab_mag && new_ac_mag < new_bc_mag)
			{
				dir = -new_ac;
				vertices[1] = newPoint;	//remove B
			}
			else if (new_bc_mag < new_ab_mag && new_bc_mag < new_ac_mag)
			{
				dir = -new_bc;
				vertices[0] = newPoint;	//remove A
			}
		}
		else
		{
			//the root dir-------------------------------------------------//
			Vector3 rootDir = -newPoint;

			//normals of all 3 triangles-----------------------------------//
			Vector3 new_a = newPoint - vertices[0];
			Vector3 new_b = newPoint - vertices[1];
			Vector3 new_c = newPoint - vertices[2];

			Vector3 new_ab_normal = new_a.Cross(new_b);
			if (new_ab_normal.Dot(-vertices[0]) < 0.f) new_ab_normal *= -1.f;
			new_ab_normal.Normalize();

			Vector3 new_ac_normal = new_a.Cross(new_c);
			if (new_ac_normal.Dot(-vertices[0]) < 0.f) new_ac_normal *= -1.f;
			new_ac_normal.Normalize();

			Vector3 new_bc_normal = new_b.Cross(new_c);
			if (new_bc_normal.Dot(-vertices[0]) < 0.f) new_bc_normal *= -1.f;
			new_bc_normal.Normalize();

			//biggest dot product means closest
			//(no such thing as negative dot product since normal will always
			//point towards origin)

			//All are the same dist----------------------------------------//
			if (new_ab.Same(new_ac) && new_ab.Same(new_bc) && new_ac.Same(new_bc))
			{
				float new_ab_Dot = new_ab_normal.Dot(rootDir);
				float new_ac_Dot = new_ac_normal.Dot(rootDir);
				float new_bc_Dot = new_bc_normal.Dot(rootDir);

				if (new_ab_Dot > new_ac_Dot && new_ab_Dot > new_bc_Dot)	//AB closest
					vertices[2] = newPoint;	//C is the furthest
				else if (new_ac_Dot > new_ab_Dot && new_ac_Dot > new_bc_Dot)	//AC closest
					vertices[1] = newPoint;	//B is the furthest
				else	//BC closest
					vertices[0] = newPoint;	//A is the furthest

				dir = -new_ab;	//-new_bc & -new_ac are the same
			}

			//AB and BC same dist------------------------------------------//
			else if (new_ab.Same(new_bc))
			{
				if (new_ab_normal.Dot(rootDir) > new_bc_normal.Dot(rootDir))	//AB closer
					vertices[2] = newPoint;	//C is the furthest
				else //BC closer
					vertices[0] = newPoint;	//A is the furthest

				dir = -new_ab;	//-new_bc is the same
			}

			//AB and AC same dist------------------------------------------//
			else if (new_ab.Same(new_ac))
			{
				if (new_ab_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//AB closer
					vertices[2] = newPoint;	//C is the furthest
				else //AC closer
					vertices[1] = newPoint;	//B is the furthest

				dir = -new_ab;	//-new_ac is the same
			}

			//BC and AC same dist------------------------------------------//
			else
			{
				if (new_bc_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//BC closer
					vertices[0] = newPoint;	//A is the furthest
				else //AC closer
					vertices[1] = newPoint;	//B is the furthest

				dir = -new_bc;	//-new_ac is the same
			}
		}

		//loop-------------------------------------------------------------//
		newPoint = GetNewLast(dir, A, B);

		infinite_counter++;

		//rare case the algo will oscillate around closest solution--------// 
		if (infinite_counter > 10)
		{
			//take a small sampling of the past few results (7)
			if (infinite_counter < 18)
			{
				lastFew_dist[infinite_counter - 11] = closestPointToOrigin(vertices[0], vertices[1], vertices[2]).LengthSquared();
			}
			else
			{
				//take the shortest dist. from this sampling
				float shortestDist = lastFew_dist[0];
				for (int i = 1; i < 7; ++i)
				{
					if (shortestDist > lastFew_dist[i])
						shortestDist = lastFew_dist[i];
				}
				closestDist = sqrt(shortestDist);
				infinite_loop = true;
				break;
			}
		}
	}

	//remove if looping----------------------------------------------------//
	if (simplexMesh)
		delete simplexMesh;
	simplexMesh = new Mesh();
	simplexMesh->Init_Triangle(vertices, Color(109, 242, 236), Color(71, 191, 185));
}

/*********************************************************************************************************
draw
/*********************************************************************************************************/
void GJK_Simplex_3D::Draw()
{
	if (simplexMesh)
	{
		CU::view.Pre_DrawMesh(Vector3(0, 0, 0), Vector3(1, 1, 1), simplexMesh);
		simplexMesh->Draw();
	}

	//point 0: red
	CU::view.Pre_DrawMesh(vertices[0], Vector3(10, 1, 10), CU::sphere_red);
	CU::sphere_red->Draw();

	//point 1: blue
	CU::view.Pre_DrawMesh(vertices[1], Vector3(10, 1, 10), CU::sphere_blue);
	CU::sphere_blue->Draw();

	//point 2: green
	CU::view.Pre_DrawMesh(vertices[2], Vector3(10, 1, 10), CU::sphere_green);
	CU::sphere_green->Draw();

	//last_newPoint: yellow
	CU::view.Pre_DrawMesh(newPoint, Vector3(10, 21, 5), CU::sphere_yellow);
	CU::sphere_yellow->Draw();

	////draw dir
	//float len = dir.Length();
	//dir.Normalize();
	//float ray_yaw = Math::RadianToDegree(atan2(-dir.z, dir.x));
	//float ray_pitch = -Math::RadianToDegree(asin(dir.y));
	//CU::view.Pre_DrawMesh(Vector3(0, 0, 0), ray_yaw, ray_pitch, Vector3(0, 1, 0), Vector3(0, 0, -1),
	//	Vector3(len, len, len), CU::line_purple);
	//CU::line_purple->Draw();

	//MD
	for (int i = 0; i < 64; ++i)
	{
		/*if (i == 32)
		{
			CU::view.Pre_DrawMesh(MD_points[i], Vector3(5, 5, 5), CU::sphere_red);
			CU::sphere_red->Draw();
		}
		else
		{*/
			CU::view.Pre_DrawMesh(MD_points[i], Vector3(5, 5, 5), CU::sphere_dark_green);
			CU::sphere_dark_green->Draw();
		//}
	}
}
