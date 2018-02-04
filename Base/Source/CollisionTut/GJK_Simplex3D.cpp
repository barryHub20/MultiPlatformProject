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
	int extremeA_idx = GetSupportingVertice(A, dir);

	//get the extreme of B
	int extremeB_idx = GetSupportingVertice(B, dir * -1.f);

	//store the points that form the MD point
	shapeA_MDpoint = extremeA_idx;
	shapeB_MDpoint = extremeB_idx;

	//get the last
	return A.pointList[extremeA_idx] - B.pointList[extremeB_idx];
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
primitive way of getting Barycentric coords.
https://www.gamedev.net/forums/topic/621445-barycentric-coordinates-c-code-check/
/*********************************************************************************************************/
//compute the area of a triangle using Heron's formula
float triarea(float a, float b, float c)
{
	float s = (a + b + c) / 2.0;
	float area = sqrt(fabs(s*(s - a)*(s - b)*(s - c)));
	return area;
}

// compute the distance between two 2d points
float dist(float x0, float y0, float z0, float x1, float y1, float z1)
{
	float a = x1 - x0;
	float b = y1 - y0;
	float c = z1 - z0;
	return sqrt(a*a + b*b + c*c);
}

// calculate barycentric coordinates
// triangle 1st vertex: x0,y0,z0
// triangle 2nd vertex: x1,y1,z1
//  triangle 3rd vertex: x2,y2,z2
// point inside triangle: vx, vy,vz
// *u,*v,*w are the coordinates returned

void barycent(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2,
	float vx, float vy, float vz,
	float *u, float *v, float *w)
{

	// compute the area of the big triangle
	float a = dist(x0, y0, z0, x1, y1, z1);
	float b = dist(x1, y1, z1, x2, y2, z2);
	float c = dist(x2, y2, z2, x0, y0, z0);
	float totalarea = triarea(a, b, c);

	// compute the distances from the outer vertices to the inner vertex
	float length0 = dist(x0, y0, z0, vx, vy, vz);
	float length1 = dist(x1, y1, z1, vx, vy, vz);
	float length2 = dist(x2, y2, z2, vx, vy, vz);

	// divide the area of each small triangle by the area of the big triangle
	*u = triarea(b, length1, length2) / totalarea;
	*v = triarea(c, length0, length2) / totalarea;
	*w = triarea(a, length0, length1) / totalarea;
}

/*********************************************************************************************************
compute the closest points between 2 3D polygons
/*********************************************************************************************************/
void GJK_Simplex_3D::computeClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B)
{
	//edge1 = (vertices[1] - vertices[0]).Normalized();
	//edge2 = (vertices[2] - vertices[0]).Normalized();

	//Vector3 rayOrigin;	//origin 0,0,0
	//Vector3 rayVector = edge2.Cross(edge1);	//compute normal

	////point towards origin---------------------------------------------//
	//if (rayVector.Dot(-vertices[0]) < 0.f)
	//	rayVector *= -1.f;

	//h = rayVector.Cross(edge2);
	//a = edge1.Dot(h);

	//if (a > -Math::EPSILON && a < Math::EPSILON)
	//{
	//	cout << "a" << endl;
	//	return;
	//}

	//f = 1.f / a;
	//s = (rayOrigin - vertices[0]).Normalized();
	//u = f * s.Dot(h);	//lambda 1

	//if (u < 0.f || u > 1.f)
	//{
	//	cout << "u" << endl;
	//	return;
	//}

	//q = s.Cross(edge1);
	//v = f * rayVector.Dot(q);	//lambda 2

	//if (v < 0.f || v + u > 1.f)
	//{
	//	cout << "u + v" << endl;
	//	return;
	//}
	//

	//closest points---------------------------------------------------//
	//float r1, r2, r3; // barycentric coordinates
	//barycent(vertices[0].x, vertices[0].y, vertices[0].z, vertices[1].x, 
	//	vertices[1].y, vertices[1].z, vertices[2].x, vertices[2].y, vertices[2].z, 
	//	triangle_intersectPt.x, triangle_intersectPt.y, triangle_intersectPt.z, &r1, &r2, &r3);

	//Vector3 shapeB_closest = (r1 * B.pointList[shapeB_MDpointB]) +
	//(r2 * B.pointList[shapeB_MDpointB]) + (r3 * B.pointList[shapeB_MDpointC]);
	//edges_intersectPt[0] = shapeB_closest;

	//cout << r1 + r2 + r3 << endl;
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
	shapeA_MDpointA = shapeA_MDpoint;
	shapeB_MDpointA = shapeB_MDpoint;
	vertices[1] = GetNewLast(-dir, A, B);	//B
	shapeA_MDpointB = shapeA_MDpoint;
	shapeB_MDpointB = shapeB_MDpoint;

	dir = closestPointToOrigin_AB(vertices[0], vertices[1]);
	if (dir.Dot(-vertices[0]) < 0.f)	dir *= -1.f;

	vertices[2] = GetNewLast(dir, A, B);	//C
	shapeA_MDpointC = shapeA_MDpoint;
	shapeB_MDpointC = shapeB_MDpoint;
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
	int vertReplaced = -1;	//replaced by newPoint. 0: A, 1: B, 2: C

	int infinite_counter = 0;
	while (true)
	{
		//terminating condition--------------------------------------------//
		if (newPoint.Same(vertices[0]) || newPoint.Same(vertices[1])
			|| newPoint.Same(vertices[2]))
		{
			closestDist = closestPointToOrigin(vertices[0], vertices[1], vertices[2]).Length();
			cout << closestDist << endl;
			if (simplexMesh)
				delete simplexMesh;
			simplexMesh = new Mesh();
			simplexMesh->Init_Triangle(vertices, Color(109, 242, 236), Color(71, 191, 185));

			break;
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
				vertReplaced = 2;
			}
			else if (new_ac_mag < new_ab_mag && new_ac_mag < new_bc_mag)
			{
				dir = -new_ac;
				vertices[1] = newPoint;	//remove B
				vertReplaced = 1;
			}
			else if (new_bc_mag < new_ab_mag && new_bc_mag < new_ac_mag)
			{
				dir = -new_bc;
				vertices[0] = newPoint;	//remove A
				vertReplaced = 0;
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

			//newB and newC are the same
			Vector3 new_ab_normal = new_a.Cross(new_b);

			if (new_ab_normal.IsZero())	//if is zero, normal same as triangle normal
			{
				edge1 = vertices[1] - vertices[0];
				edge2 = vertices[2] - vertices[0];
				new_ab_normal = edge2.Cross(edge1);
			}

			if (new_ab_normal.Dot(-vertices[0]) < 0.f) new_ab_normal *= -1.f;
			new_ab_normal.Normalize();

			Vector3 new_ac_normal = new_a.Cross(new_c);

			if (new_ac_normal.IsZero())	//if is zero, normal same as triangle normal
			{
				edge1 = vertices[1] - vertices[0];
				edge2 = vertices[2] - vertices[0];
				new_ac_normal = edge2.Cross(edge1);
			}

			if (new_ac_normal.Dot(-vertices[0]) < 0.f) new_ac_normal *= -1.f;
			new_ac_normal.Normalize();

			Vector3 new_bc_normal = new_b.Cross(new_c);

			if (new_bc_normal.IsZero())	//if is zero, normal same as triangle normal
			{
				edge1 = vertices[1] - vertices[0];
				edge2 = vertices[2] - vertices[0];
				new_bc_normal = edge2.Cross(edge1);
			}

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
				{
					vertices[2] = newPoint;	//C is the furthest
					vertReplaced = 2;
				}
				else if (new_ac_Dot > new_ab_Dot && new_ac_Dot > new_bc_Dot)	//AC closest
				{
					vertices[1] = newPoint;	//B is the furthest
					vertReplaced = 1;
				}
				else	//BC closest
				{
					vertices[0] = newPoint;	//A is the furthest
					vertReplaced = 0;
				}

				dir = -new_ab;	//-new_bc & -new_ac are the same
			}

			//AB and BC same dist------------------------------------------//
			else if (new_ab.Same(new_bc))
			{
				if (new_ab_normal.Dot(rootDir) > new_bc_normal.Dot(rootDir))	//AB closer
				{
					vertices[2] = newPoint;	//C is the furthest
					vertReplaced = 2;
				}
				else //BC closer
				{
					vertices[0] = newPoint;	//A is the furthest
					vertReplaced = 0;
				}

				dir = -new_ab;	//-new_bc is the same
			}

			//AB and AC same dist------------------------------------------//
			else if (new_ab.Same(new_ac))
			{
				if (new_ab_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//AB closer
				{
					vertices[2] = newPoint;	//C is the furthest
					vertReplaced = 2;
				}
				else //AC closer
				{
					vertices[1] = newPoint;	//B is the furthest
					vertReplaced = 1;
				}

				dir = -new_ab;	//-new_ac is the same
			}

			//BC and AC same dist------------------------------------------//
			else
			{
				if (new_bc_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//BC closer
				{
					vertices[0] = newPoint;	//A is the furthest
					vertReplaced = 0;
				}
				else //AC closer
				{
					vertices[1] = newPoint;	//B is the furthest
					vertReplaced = 1;
				}

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

	//get the shape points that form new MD point----------------------//
	if (vertReplaced == 0)	//A
	{
		shapeA_MDpointA = shapeA_MDpoint;
		shapeB_MDpointA = shapeB_MDpoint;
	}
	else if (vertReplaced == 1)	//B
	{
		shapeA_MDpointB = shapeA_MDpoint;
		shapeB_MDpointB = shapeB_MDpoint;
	}
	else //C
	{
		shapeA_MDpointC = shapeA_MDpoint;
		shapeB_MDpointC = shapeB_MDpoint;
	}

	computeClosestPoints(A, B);

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
	CU::view.Pre_DrawMesh(edges_intersectPt[0], Vector3(10, 10, 10), CU::sphere_yellow);
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
