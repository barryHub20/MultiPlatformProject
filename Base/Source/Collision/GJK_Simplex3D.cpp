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
	infinite_loop = false;
	closestDist = new_ab_Dot = new_ac_Dot = new_bc_Dot = d00 = d01 = d11 = d20 = d21 = denom = 
		lambda1 = lambda2 = lambda3 = dotProd = largestDotProd = largestDotProd_index = a = f = u = v = t = t_tracker = 0.f;
	shapeA_MDpoint = shapeB_MDpoint = sA_pA = sB_pA = sA_pB = sB_pB = sA_pC = sB_pC
		= edge_type = vertType - 1;
	for (int i = 0; i < 8; ++i)
	{
		collect_Dist[i] = 0.f;
		collect_primitiveType[i] = '0';
		collect_edgeType[i] = collect_vertType[i] = -1;
		
		for (int k = 0; k < 6; ++k)
			collect_suppIdx[i][k] = -1.;
	}
	for (int i = 0; i < 6; ++i)
		lensqList[i] = 0.f;
}

GJK_Simplex_3D::~GJK_Simplex_3D()
{
}

/*********************************************************************************************************
reset data
/*********************************************************************************************************/
void GJK_Simplex_3D::Reset()
{
	closestDist = -1.f;
	lambda1 = lambda2 = lambda3 = 0.f;
}

/*********************************************************************************************************
Get support vertice
/*********************************************************************************************************/
int GJK_Simplex_3D::GetSupportingVertice(CD_Polygon_3D& poly, Vector3 dir)
{
	dotProd = (poly.pointList[0] - poly.shapePos).Dot(dir);
	largestDotProd = dotProd;
	largestDotProd_index = 0;

	//get vertice with largest dot product
	for (int i = 1; i < 8; ++i)
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
	v0 = b - a;
	v1 = c - a;
	v2 = p - a;
	d00 = v0.Dot(v0);
	d01 = v0.Dot(v1);
	d11 = v1.Dot(v1);
	d20 = v2.Dot(v0);
	d21 = v2.Dot(v1);
	denom = d00 * d11 - d01 * d01;
	retVal.y = (d11 * d20 - d01 * d21) / denom;
	retVal.z = (d00 * d21 - d01 * d20) / denom;
	retVal.x = 1.0f - retVal.y - retVal.z;
}

/*********************************************************************************************************
Project origin to AB and get the contact point
Vector3 vertices[0]: the 3 points of the triangle
(doesn't have to be in any order)
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::closestPointToOrigin(int v0_idx, int v1_idx, int v2_idx)
{
	edge1 = vertices[v1_idx] - vertices[v0_idx];
	edge2 = vertices[v2_idx] - vertices[v0_idx];

	Vector3 rayOrigin;	//origin 0,0,0
	Vector3 rayVector = edge2.Cross(edge1);	//compute normal
	
	//point towards origin---------------------------------------------//
	if (rayVector.Dot(-vertices[v0_idx]) < 0.f)
		rayVector *= -1.f;

	h = rayVector.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -Math::EPSILON && a < Math::EPSILON)
		return NonIntersection(v0_idx, v1_idx, v2_idx);

	f = 1.f / a;
	s = rayOrigin - vertices[v0_idx];
	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
		return NonIntersection(v0_idx, v1_idx, v2_idx);

	q = s.Cross(edge1);
	v = f * rayVector.Dot(q);

	if (v < 0.f || v + u > 1.f)
		return NonIntersection(v0_idx, v1_idx, v2_idx);

	float t = f * edge2.Dot(q);

	triangle_intersectPt = rayOrigin + rayVector * t;
	final_primitiveType = 'T';
	return triangle_intersectPt;
}

/*********************************************************************************************************
check the 2D and 1D projections
/*********************************************************************************************************/
Vector3 GJK_Simplex_3D::NonIntersection(int v0_idx, int v1_idx, int v2_idx)
{
	//AB, AC, BC edges projection check
	//A(0) -> C(2)

	edges[0] = vertices[v1_idx] - vertices[v0_idx];	//AB
	edges[1] = vertices[v2_idx] - vertices[v0_idx];	//AC
	edges[2] = vertices[v2_idx] - vertices[v1_idx];	//BC

	Vector3 AO;
	float t = 0.f;

	//AB-------------------------------------------------------------------------//
	AO = -vertices[v0_idx];
	t = (AO.Dot(edges[0])) / edges[0].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[0] = edges[0] * t + vertices[v0_idx];
		lensqList[0] = edges_intersectPt[0].LengthSquared();
	}
	else
		lensqList[0] = 1000000.f;

	//AC-------------------------------------------------------------------------//
	AO = -vertices[v0_idx];
	t = (AO.Dot(edges[1])) / edges[1].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[1] = edges[1] * t + vertices[v0_idx];
		lensqList[1] = edges_intersectPt[1].LengthSquared();
	}
	else
		lensqList[1] = 1000000.f;

	//BC-------------------------------------------------------------------------//
	AO = -vertices[v1_idx];
	t = (AO.Dot(edges[2])) / edges[2].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[2] = edges[2] * t + vertices[v1_idx];
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
		//Barycentric coords
		Vector3 lambda;
		BarycentricCoord(closestPoint, vertices[0], vertices[1], vertices[2], lambda);

		shapeA_closestPt = A.pointList[sA_pA] * lambda.x + A.pointList[sA_pB] * lambda.y + A.pointList[sA_pC] * lambda.z;
		shapeB_closestPt = B.pointList[sB_pA] * lambda.x + B.pointList[sB_pB] * lambda.y + B.pointList[sB_pC] * lambda.z;
	}
	else if (final_primitiveType == 'E')
	{
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
void GJK_Simplex_3D::TransformShape(CD_Polygon_3D& shape, float yaw, float pitch, Vector3 vel, float t)
{
	shape.Translate(vel * t);
	shape.yawRot(yaw * t);
	shape.pitchRot(pitch * t);
	shape.RecalculatePoints();
}

void GJK_Simplex_3D::SetShape_ToAbsoute(CD_Polygon_3D& projected, CD_Polygon_3D& main)
{
	projected.CopyFrom(main);
	projected.Translate(main.proj_Vel_tr);
	projected.yawRot(main.proj_Yaw_tr);
	projected.pitchRot(main.proj_Pitch_tr);
	projected.RecalculatePoints();
}

/*********************************************************************************************************
calculate normals of the tetrathron
/*********************************************************************************************************/
void GJK_Simplex_3D::calculateNormals()
{
	//normals of all 3 triangles-----------------------------------//
	Vector3 new_a = vertices[3] - vertices[0];
	Vector3 new_b = vertices[3] - vertices[1];
	Vector3 new_c = vertices[3] - vertices[2];

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

	rootDir = -vertices[3];

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
	dir = -closestPointToOrigin(0, 1, 2);

	//loop-------------------------------------------------------------//
	vertices[3] = GetNewLast(dir, A, B);	//C
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
	int vertReplaced = -1;	//replaced by vertices[3]. 0: A, 1: B, 2: C

	int infinite_counter = 0;
	while (true)
	{
		//terminating condition--------------------------------------------//
		if (vertices[3].Same(vertices[0]) || vertices[3].Same(vertices[1])
			|| vertices[3].Same(vertices[2]))
		{
			closestPoint = closestPointToOrigin(0, 1, 2);
			break;
		}

		//test out the following triangles---------------------------------//
		Vector3 new_ab = closestPointToOrigin(3, 0, 1);
		Vector3 new_ac = closestPointToOrigin(3, 0, 2);
		Vector3 new_bc = closestPointToOrigin(3, 1, 2);

		//the magnitude
		float new_ab_mag = new_ab.LengthSquared();
		float new_ac_mag = new_ac.LengthSquared();
		float new_bc_mag = new_bc.LengthSquared();

		//shortest magnitude found-----------------------------------------//
		if (new_ab_mag < new_ac_mag && new_ab_mag < new_bc_mag)	//AB shortest
		{
			dir = -new_ab;
			vertices[2] = vertices[3];	//remove C
			vertReplaced = 2;
		}
		else if (new_ac_mag < new_ab_mag && new_ac_mag < new_bc_mag)	//AC shortest
		{
			dir = -new_ac;
			vertices[1] = vertices[3];	//remove B
			vertReplaced = 1;
		}
		else if (new_bc_mag < new_ab_mag && new_bc_mag < new_ac_mag)	//BC shortest
		{
			dir = -new_bc;
			vertices[0] = vertices[3];	//remove A
			vertReplaced = 0;
		}
		else if (new_ab_mag == new_ac_mag && new_ab_mag == new_bc_mag && new_ac_mag == new_bc_mag)	//all same
		{
			calculateNormals();

			if (new_ab_Dot > new_ac_Dot && new_ab_Dot > new_bc_Dot)	//AB closest
			{
				vertices[2] = vertices[3];	//C is the furthest
				vertReplaced = 2;
			}
			else if (new_ac_Dot > new_ab_Dot && new_ac_Dot > new_bc_Dot)	//AC closest
			{
				vertices[1] = vertices[3];	//B is the furthest
				vertReplaced = 1;
			}
			else	//BC closest
			{
				vertices[0] = vertices[3];	//A is the furthest
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
					vertices[2] = vertices[3];	//C is the furthest
					vertReplaced = 2;
				}
				else //BC closer
				{
					vertices[0] = vertices[3];	//A is the furthest
					vertReplaced = 0;
				}
				dir = -new_ab;	//-new_bc is the same
			}
			//AB and AC equal and shortest---------------------------------//
			else if (new_ab_mag == new_ac_mag)
			{
				if (new_ab_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//AB closer
				{
					vertices[2] = vertices[3];	//C is the furthest
					vertReplaced = 2;
				}
				else //AC closer
				{
					vertices[1] = vertices[3];	//B is the furthest
					vertReplaced = 1;
				}
				dir = -new_ab;	//-new_ac is the same
			}
			//BC and AC equal and shortest---------------------------------//
			else
			{
				if (new_bc_normal.Dot(rootDir) > new_ac_normal.Dot(rootDir))	//BC closer
				{
					vertices[0] = vertices[3];	//A is the furthest
					vertReplaced = 0;
				}
				else //AC closer
				{
					vertices[1] = vertices[3];	//B is the furthest
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
		vertices[3] = GetNewLast(dir, A, B);
		infinite_counter++;

		//collect info-----------------------------------------------------//
		if (infinite_counter >= 10)
		{
			int i = infinite_counter - 10;
			collect_Pt[i] = closestPointToOrigin(0, 1, 2);
			collect_Dist[i] = collect_Pt[i].LengthSquared();
			collect_verts[i][0] = vertices[0];
			collect_verts[i][1] = vertices[1];
			collect_verts[i][2] = vertices[2];
			collect_primitiveType[i] = final_primitiveType;
			collect_edgeType[i] = edge_type;
			collect_vertType[i] = vertType;

			//support func vert index
			//0: sA_pA -> 5: sB_pC (follow the order in header file)
			collect_suppIdx[i][0] = sA_pA;
			collect_suppIdx[i][1] = sB_pA;
			collect_suppIdx[i][2] = sA_pB;
			collect_suppIdx[i][3] = sB_pB;
			collect_suppIdx[i][4] = sA_pC;
			collect_suppIdx[i][5] = sB_pC;

			//rare case the algo will oscillate around closest solution----//
			if (infinite_counter >= 17)
			{
				//collect the most most optimal solution
				int inf_cd_idx = 0;
				float inf_closestDist = collect_Dist[0];
				for (int i = 1; i < 8; ++i)
				{
					if (inf_closestDist > collect_Dist[i])
					{
						inf_cd_idx = i;
						inf_closestDist = collect_Dist[i];
					}
				}

				//set the info for the closest dist------------------------//
				closestPoint = collect_Pt[inf_cd_idx];
				vertices[0] = collect_verts[inf_cd_idx][0];
				vertices[1] = collect_verts[inf_cd_idx][1];
				vertices[2] = collect_verts[inf_cd_idx][2];
				final_primitiveType = collect_primitiveType[inf_cd_idx];
				edge_type = collect_edgeType[inf_cd_idx];
				vertType = collect_vertType[inf_cd_idx];
				sA_pA = collect_suppIdx[inf_cd_idx][0];
				sB_pA = collect_suppIdx[inf_cd_idx][1];
				sA_pB = collect_suppIdx[inf_cd_idx][2];
				sB_pB = collect_suppIdx[inf_cd_idx][3];
				sA_pC = collect_suppIdx[inf_cd_idx][4];
				sB_pC = collect_suppIdx[inf_cd_idx][5];

				break;
			}
		}
	}

	//closest point and dist (NORMALIZED)------------------------------//
	closestDist = closestPoint.Normalize_andLength();

	//compute closest points-------------------------------------------//
	computeClosestPoints(A, B);
}

/*********************************************************************************************************
collision check
/*********************************************************************************************************/
bool GJK_Simplex_3D::CollisionResponse(CD_Polygon_3D& A, CD_Polygon_3D& Aresponse, CD_Polygon_3D& B)
{
	/*SetShape_ToAbsoute(Aresponse, A);
	return true;*/

	//set up----------------------------------------------------------------------//
	const float ANGULAR_TOLERANCE = 0.1f;
	noCollision = false;
	t = t_tracker = 0.f;

	//A_Response copied from A----------------------------------------------------//
	Aresponse.CopyFrom(A);
	GetClosestPoints(A, B);	//shortest dist.

	//loop------------------------------------------------------------------------//
	Vector3 ray = B.proj_Vel_tr - A.proj_Vel_tr;	//relative vel
	while (!noCollision && closestDist >= ANGULAR_TOLERANCE)
	{
		float rayN = ray.Dot(closestPoint);	//normalized, reuse dir variable
		float dRel = rayN + A.av + B.av;	//REMOVED A.r

		t = abs(closestDist / dRel);	//tutorial version resets the whole matrix
		t_tracker += t;
		
		if (t_tracker < 0 || t_tracker > 1)
		{
			//cout << "ASD" << endl;
			noCollision = true;
			SetShape_ToAbsoute(Aresponse, A);
			return false;
		}

		TransformShape(Aresponse, A.proj_Yaw_tr, A.proj_Pitch_tr, A.proj_Vel_tr, t);
		GetClosestPoints(Aresponse, B);	//shortest dist.
	}

	//loop------------------------------------------------------------------------//
	return false;
}

/*********************************************************************************************************
draw
/*********************************************************************************************************/
void GJK_Simplex_3D::Draw()
{
	//point 1: blue
	CU::view.Pre_DrawMesh(shapeA_closestPt, Vector3(5, 5, 5), CU::sphere_blue);
	CU::sphere_blue->Draw();

	//point 2: green
	CU::view.Pre_DrawMesh(shapeB_closestPt, Vector3(5, 5, 5), CU::sphere_green);
	CU::sphere_green->Draw();

	////draw dir
	Vector3 closestPt_dir = shapeB_closestPt - shapeA_closestPt;
	float len = closestPt_dir.Length();
	closestPt_dir.Normalize();
	float ray_yaw = Math::RadianToDegree(atan2(-closestPt_dir.z, closestPt_dir.x));
	float ray_pitch = -Math::RadianToDegree(asin(closestPt_dir.y));

	CU::view.Pre_DrawMesh(shapeA_closestPt, ray_yaw, ray_pitch, Vector3(0, 1, 0), Vector3(0, 0, -1),
			Vector3(len, len, len), CU::line_purple);
	glLineWidth(2.f);
	CU::line_purple->Draw();
	glLineWidth(1.f);
}
