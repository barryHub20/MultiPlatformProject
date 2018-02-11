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
Get the barycentric coordinate (u, v, w) with respect to point P
in Triangle (a, b, c)
https://gamedev.stackexchange.com/questions/23743/whats-the-most-efficient-way-to-find-barycentric-coordinates
/*********************************************************************************************************/
void GJK_Simplex_3D::BarycentricCoord(Vector3 p, Vector3 a, Vector3 b, Vector3 c, Vector3& retVal)
{
	Vector3 v0 = b - a;
	Vector3 v1 = c - a;
	Vector3 v2 = p - a;
	float d00 = v0.Dot(v0);
	float d01 = v0.Dot(v1);
	float d11 = v1.Dot(v1);
	float d20 = v2.Dot(v0);
	float d21 = v2.Dot(v1);
	float denom = d00 * d11 - d01 * d01;
	retVal.y = (d11 * d20 - d01 * d21) / denom;
	retVal.z = (d00 * d21 - d01 * d20) / denom;
	retVal.x = 1.0f - retVal.y - retVal.z;
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
	final_primitiveType = 'T';
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
	{
		final_primitiveType = 'E';
		triangle_intersectPt = edges_intersectPt[smallestLensq_Index];
		edge_type = smallestLensq_Index;
	}
	else
	{
		triangle_intersectPt = vertices[smallestLensq_Index - 3];
		final_primitiveType = 'V';
		vertType = smallestLensq_Index - 3;
	}
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
(lensq or dot can be used)
/*********************************************************************************************************/
void GJK_Simplex_3D::computeClosestPoints(CD_Polygon_3D& A, CD_Polygon_3D& B)
{
	if (final_primitiveType == 'T')
	{
		//cout << "T" << endl;
		//Barycentric coords
		Vector3 lambda;
		BarycentricCoord(closestPoint, vertices[0], vertices[1], vertices[2], lambda);

		shapeA_closestPt = A.pointList[sA_pA] * lambda.x + A.pointList[sA_pB] * lambda.y + A.pointList[sA_pC] * lambda.z;
		shapeB_closestPt = B.pointList[sB_pA] * lambda.x + B.pointList[sB_pB] * lambda.y + B.pointList[sB_pC] * lambda.z;
	}
	else if (final_primitiveType == 'E')
	{
		//cout << "E" << endl;
		Vector3 edge;
		float p_lensq, edge_lensq;

		//linear coefficient
		if (edge_type == 0)	//AB (startVert: A, endVert: B)
		{
			edge = vertices[1] - vertices[0];
			p_lensq = -edge.Dot(vertices[0]);
			edge_lensq = edge.Dot(edge);
			
			lambda2 = p_lensq / edge_lensq;
			lambda1 = 1.f - lambda2;

			//lamba1 = startVert, lambda2 = endVert
			shapeA_closestPt = A.pointList[sA_pA] * lambda1 + A.pointList[sA_pB] * lambda2;
			shapeB_closestPt = B.pointList[sB_pA] * lambda1 + B.pointList[sB_pB] * lambda2;
		}
		else if (edge_type == 1)	//AC (startVert: A, endVert: C)
		{
			edge = vertices[2] - vertices[0];
			p_lensq = -edge.Dot(vertices[0]);
			edge_lensq = edge.Dot(edge);

			lambda2 = p_lensq / edge_lensq;
			lambda1 = 1.f - lambda2;

			//lamba1 = startVert, lambda2 = endVert
			shapeA_closestPt = A.pointList[sA_pA] * lambda1 + A.pointList[sA_pC] * lambda2;
			shapeB_closestPt = B.pointList[sB_pA] * lambda1 + B.pointList[sB_pC] * lambda2;
		}
		else //BC (startVert: B, endVert: C)
		{
			edge = vertices[2] - vertices[1];
			p_lensq = -edge.Dot(vertices[1]);
			edge_lensq = edge.Dot(edge);

			lambda2 = p_lensq / edge_lensq;
			lambda1 = 1.f - lambda2;

			//lamba1 = startVert, lambda2 = endVert
			shapeA_closestPt = A.pointList[sA_pB] * lambda1 + A.pointList[sA_pC] * lambda2;
			shapeB_closestPt = B.pointList[sB_pB] * lambda1 + B.pointList[sB_pC] * lambda2;
		}
	}
	else
	{
		//cout << "V" << endl;
		//none
		if (vertType == 0)
		{
			shapeA_closestPt = A.pointList[sA_pA];
			shapeB_closestPt = B.pointList[sB_pA];
		}
		else if (vertType == 1)
		{
			shapeA_closestPt = A.pointList[sA_pB];
			shapeB_closestPt = B.pointList[sB_pB];
		}
		else
		{
			shapeA_closestPt = A.pointList[sA_pC];
			shapeB_closestPt = B.pointList[sB_pC];
		}
	}
}

/*********************************************************************************************************
Create closest contact with origin
/*********************************************************************************************************/
void GJK_Simplex_3D::TransformShape(CD_Polygon_3D& shape, Vector3 vel, float rotAngle, float t)
{
}
void GJK_Simplex_3D::SetShape_ToAbsoute(CD_Polygon_3D& projected, CD_Polygon_3D& main)
{
}

/*********************************************************************************************************
calculate normals of the tetrathron
/*********************************************************************************************************/
void GJK_Simplex_3D::calculateNormals()
{
	//normals of all 3 triangles-----------------------------------//
	Vector3 new_a = newPoint - vertices[0];
	Vector3 new_b = newPoint - vertices[1];
	Vector3 new_c = newPoint - vertices[2];

	//newB and newC are the same
	new_ab_normal = new_a.Cross(new_b);

	if (new_ab_normal.IsZero())	//if is zero, normal same as triangle normal
	{
		edge1 = vertices[1] - vertices[0];
		edge2 = vertices[2] - vertices[0];
		new_ab_normal = edge2.Cross(edge1);
	}

	if (new_ab_normal.Dot(-vertices[0]) < 0.f) new_ab_normal *= -1.f;
	new_ab_normal.Normalize();

	new_ac_normal = new_a.Cross(new_c);

	if (new_ac_normal.IsZero())	//if is zero, normal same as triangle normal
	{
		edge1 = vertices[1] - vertices[0];
		edge2 = vertices[2] - vertices[0];
		new_ac_normal = edge2.Cross(edge1);
	}

	if (new_ac_normal.Dot(-vertices[0]) < 0.f) new_ac_normal *= -1.f;
	new_ac_normal.Normalize();

	new_bc_normal = new_b.Cross(new_c);

	if (new_bc_normal.IsZero())	//if is zero, normal same as triangle normal
	{
		edge1 = vertices[1] - vertices[0];
		edge2 = vertices[2] - vertices[0];
		new_bc_normal = edge2.Cross(edge1);
	}

	if (new_bc_normal.Dot(-vertices[0]) < 0.f) new_bc_normal *= -1.f;
	new_bc_normal.Normalize();

	rootDir = -newPoint;

	new_ab_Dot = new_ab_normal.Dot(rootDir);
	new_ac_Dot = new_ac_normal.Dot(rootDir);
	new_bc_Dot = new_bc_normal.Dot(rootDir);
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
	sA_pA = shapeA_MDpoint;
	sB_pA = shapeB_MDpoint;
	vertices[1] = GetNewLast(-dir, A, B);	//B
	sA_pB = shapeA_MDpoint;
	sB_pB = shapeB_MDpoint;

	dir = closestPointToOrigin_AB(vertices[0], vertices[1]);
	if (dir.Dot(-vertices[0]) < 0.f)	dir *= -1.f;

	vertices[2] = GetNewLast(dir, A, B);	//C
	sA_pC = shapeA_MDpoint;
	sB_pC = shapeB_MDpoint;
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
	GetClosestPoints_Start(A, B);

	infinite_loop = false;
	int vertReplaced = -1;	//replaced by newPoint. 0: A, 1: B, 2: C

	int infinite_counter = 0;
	while (true)
	{
		//terminating condition--------------------------------------------//
		if (newPoint.Same(vertices[0]) || newPoint.Same(vertices[1])
			|| newPoint.Same(vertices[2]))
		{
			closestPoint = closestPointToOrigin(vertices[0], vertices[1], vertices[2]);
			closestDist = closestPoint.Length();

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

		//shortest magnitude found-----------------------------------------//
		if (new_ab_mag < new_ac_mag && new_ab_mag < new_bc_mag)	//AB shortest
		{
			dir = -new_ab;
			vertices[2] = newPoint;	//remove C
			vertReplaced = 2;
		}
		else if (new_ac_mag < new_ab_mag && new_ac_mag < new_bc_mag)	//AC shortest
		{
			dir = -new_ac;
			vertices[1] = newPoint;	//remove B
			vertReplaced = 1;
		}
		else if (new_bc_mag < new_ab_mag && new_bc_mag < new_ac_mag)	//BC shortest
		{
			dir = -new_bc;
			vertices[0] = newPoint;	//remove A
			vertReplaced = 0;
		}
		else if (new_ab_mag == new_ac_mag && new_ab_mag == new_bc_mag && new_ac_mag == new_bc_mag)	//all same
		{
			calculateNormals();

			if (new_ab_Dot > new_ac_Dot && new_ab_Dot > new_bc_Dot)	//AB closest
			{
				//cout << "1" << endl;
				vertices[2] = newPoint;	//C is the furthest
				vertReplaced = 2;
			}
			else if (new_ac_Dot > new_ab_Dot && new_ac_Dot > new_bc_Dot)	//AC closest
			{
				//cout << "2" << endl;
				vertices[1] = newPoint;	//B is the furthest
				vertReplaced = 1;
			}
			else	//BC closest
			{
				//cout << "3" << endl;
				vertices[0] = newPoint;	//A is the furthest
				vertReplaced = 0;
			}

			dir = -new_ab;	//-new_bc & -new_ac are the same
		}
		else //2 of them are the same, and also the shortest (proven in the first 3 checks)
		{
			calculateNormals();

			//AB and BC equal and shortest---------------------------------//
			if (new_ab_mag == new_bc_mag)
			{
				if (new_ab_normal.Dot(rootDir) > new_bc_normal.Dot(rootDir))	//AB closer
				{
					//cout << "4" << endl;
					vertices[2] = newPoint;	//C is the furthest
					vertReplaced = 2;
				}
				else //BC closer
				{
					//cout << "5" << endl;
					vertices[0] = newPoint;	//A is the furthest
					vertReplaced = 0;
				}
				dir = -new_ab;	//-new_bc is the same
			}
			//AB and AC equal and shortest---------------------------------//
			else if (new_ab_mag == new_ac_mag)
			{
				if (new_ab_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//AB closer
				{
					//cout << "6" << endl;
					vertices[2] = newPoint;	//C is the furthest
					vertReplaced = 2;
				}
				else //AC closer
				{
					//cout << "7" << endl;
					vertices[1] = newPoint;	//B is the furthest
					vertReplaced = 1;
				}
				dir = -new_ab;	//-new_ac is the same
			}
			//BC and AC equal and shortest---------------------------------//
			else
			{
				if (new_bc_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//BC closer
				{
					//cout << "8" << endl;
					vertices[0] = newPoint;	//A is the furthest
					vertReplaced = 0;
				}
				else //AC closer
				{
					//cout << "9" << endl;
					vertices[1] = newPoint;	//B is the furthest
					vertReplaced = 1;
				}
				dir = -new_bc;	//-new_ac is the same
			}
		}

		//get the shape points that form new MD point----------------------//
		if (vertReplaced == 0)	//A
		{
			sA_pA = shapeA_MDpoint;
			sB_pA = shapeB_MDpoint;
		}
		else if (vertReplaced == 1)	//B
		{
			sA_pB = shapeA_MDpoint;
			sB_pB = shapeB_MDpoint;
		}
		else //C
		{
			sA_pC = shapeA_MDpoint;
			sB_pC = shapeB_MDpoint;
		}

		//loop-------------------------------------------------------------//
		newPoint = GetNewLast(dir, A, B);
		infinite_counter++;

		//rare case the algo will oscillate around closest solution--------// 
		if (infinite_counter > 10)
		{
			//take a small sampling of the past few results (7)
			if (infinite_counter < 15)
			{
				lastFew_pt[infinite_counter - 11] = closestPointToOrigin(vertices[0], vertices[1], vertices[2]);
				lastFew_dist[infinite_counter - 11] = lastFew_pt[infinite_counter - 11].LengthSquared();
			}
			else
			{
				//take the shortest dist. from this sampling
				float shortestDist = lastFew_dist[0];
				for (int i = 1; i < 4; ++i)
				{
					if (shortestDist > lastFew_dist[i])
						shortestDist = lastFew_dist[i];
				}
				closestPoint = lastFew_pt[infinite_counter - 11];
				closestDist = sqrt(shortestDist);
				infinite_loop = true;

				if (simplexMesh)
					delete simplexMesh;
				simplexMesh = new Mesh();
				simplexMesh->Init_Triangle(vertices, Color(109, 242, 236), Color(71, 191, 185));

				break;
			}
		}
	}

	//compute closest points-------------------------------------------//
	computeClosestPoints(A, B);
}

/*********************************************************************************************************
collision check
/*********************************************************************************************************/
bool GJK_Simplex_3D::CollisionResponse(CD_Polygon_3D& A, CD_Polygon_3D& Aresponse, CD_Polygon_3D& B)
{
	//set up----------------------------------------------------------------------//
	const float ANGULAR_TOLERANCE = 0.1f;
	noCollision = false;
	t = t_tracker = 0.f;

	//A_Response copied from A----------------------------------------------------//
	Aresponse.CopyFrom(A);
	GetClosestPoints(A, B);

	//loop------------------------------------------------------------------------//
	return false;
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

	////point 0: red
	//CU::view.Pre_DrawMesh(vertices[0], Vector3(10, 1, 10), CU::sphere_red);
	//CU::sphere_red->Draw();

	////point 1: blue
	//CU::view.Pre_DrawMesh(shapeA_closestPt, Vector3(10, 10, 10), CU::sphere_blue);
	//CU::sphere_blue->Draw();

	////point 2: green
	//CU::view.Pre_DrawMesh(shapeB_closestPt, Vector3(10, 10, 10), CU::sphere_green);
	//CU::sphere_green->Draw();

	////last_newPoint: yellow
	//CU::view.Pre_DrawMesh(edges_intersectPt[0], Vector3(10, 10, 10), CU::sphere_yellow);
	//CU::sphere_yellow->Draw();

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
		CU::view.Pre_DrawMesh(MD_points[i], Vector3(5, 5, 5), CU::sphere_dark_green);
		CU::sphere_dark_green->Draw();
	}
}
