#include "Shape.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
2D shape
/*********************************************************************************************************/
Shape::Shape()
{
}

Shape::~Shape()
{
	delete mesh;
	delete collided_mesh;
}

/*********************************************************************************************************
Init: a quad + mesh
/*********************************************************************************************************/
void Shape::Init(Vector3 shapePos, Vector3 shapeScale)
{
	//points--------------------------------------------------------//
	Vector3 pointPos = -shapeScale * 0.5f;
	pointList[0].Set(pointPos, shapePos);
	pointPos.x += shapeScale.x;
	pointList[1].Set(pointPos, shapePos);
	pointPos.y += shapeScale.y;
	pointList[2].Set(pointPos, shapePos);
	pointPos.x -= shapeScale.x;
	pointList[3].Set(pointPos, shapePos);

	//face----------------------------------------------------------//
	faceList[0].Set(0, 1, pointList);
	faceList[1].Set(1, 2, pointList);

	//custom mesh----------------------------------------------------//
	mesh = new Mesh();
	mesh->Init_Quad(false, shapeScale, Color(247, 163, 45));
	collided_mesh = new Mesh();
	collided_mesh->Init_Quad(false, shapeScale, Color(255, 0, 0));
	
	//TRS----------------------------------------------------------//
	Translate(shapePos.x, shapePos.y, shapePos.z);
	CalculateTRS();
}

/*********************************************************************************************************
Call before transformations
/*********************************************************************************************************/
void Shape::PreUpdate()
{
	collided = false;
	for (int i = 0; i < 4; ++i)
		collidedFace[i] = false;
}

/*********************************************************************************************************
Draw: TMP
/*********************************************************************************************************/
void Shape::DrawFace(Vector3 pos, float angle, float len, bool collided)
{
	if (!collided)
		CU::view.Draw_Line(pos, angle, len, 3.f, CU::line_blue);
	else
		CU::view.Draw_Line(pos, angle, len, 3.f, CU::line_red);
}

void Shape::Draw()
{
	if (!collided) 
	{
		CU::view.Pre_DrawMesh(Get_FinalTRS(), Vector3(1, 1, 1), mesh);
		mesh->Draw();
	}
	else
	{
		CU::view.Pre_DrawMesh(Get_FinalTRS(), Vector3(1, 1, 1), collided_mesh);
		collided_mesh->Draw();
	}

	////face 1-2 -------------------------------//
	//DrawFace(pointList[0].pos, faceList[0].angle, faceList[0].len, collidedFace[0]);
	//DrawFace(pointList[1].pos, faceList[1].angle, faceList[1].len, collidedFace[1]);

	////face 3-4 -------------------------------//
	//DrawFace(pointList[2].pos, faceList[0].angle + 180.f, faceList[0].len, collidedFace[2]);
	//DrawFace(pointList[3].pos, faceList[1].angle + 180.f, faceList[1].len, collidedFace[3]);
}

/*********************************************************************************************************
recalculate faces when a rotation occurs
/*********************************************************************************************************/
void Shape::RecalculateFaces(float angle)
{
	for (int i = 0; i < 2; ++i)
		faceList[i].Rotate(angle);
}

/*********************************************************************************************************
recalculate points after transformation stage: called by cal_TRS()
/*********************************************************************************************************/
void Shape::RecalculatePoints()
{
	//All points pos calculated with updated TRS------------------------------------------//
	for (int i = 0; i < 4; ++i)
	{
		CU::mtx[2].SetToTranslation(pointList[i].offset.x, pointList[i].offset.y, 0.f);

		//final transformation matrix-----------------------------//
		CU::mtx[3] = finalTRS * CU::mtx[2];

		pointList[i].pos.Set(1, 1, 0);
		pointList[i].pos = CU::mtx[3] * pointList[i].pos;
	}

	position.Set(1, 1, 0);
	position = finalTRS * position;
}

/*********************************************************************************************************
rotate
/*********************************************************************************************************/
void Shape::Rotate(float x, float y, float z)
{
	Transform::Rotate(0, 0, z);

	//rotate and change pos------------------------//
	RecalculateFaces(z);
}

/*********************************************************************************************************
called by Transform::rotate()
/*********************************************************************************************************/
void Shape::Rotate_ByParent(float x, float y, float z)
{
	Transform::Rotate_ByParent(0, 0, z);

	//rotate and change pos------------------------//
	RecalculateFaces(z);

	//children must update points despite not updating TRS
	for (int i = 0; i < childList.size(); ++i)
		static_cast<Shape*>(childList[i])->RecalculateFaces(z);
}

/*********************************************************************************************************
called by Transform::CalculateTRS()
/*********************************************************************************************************/
void Shape::CalculateTRS_byParent(Mtx44 & parentFinal)
{
	Transform::CalculateTRS_byParent(parentFinal);
	RecalculatePoints();
}

bool Shape::isCollided() { return collided; }

/*********************************************************************************************************
get projection min/max of shape onto normal
get the min and max points index of shape onto normal  
/*********************************************************************************************************/
float curr_dot_product;
void Shape::GetProjection(Shape& projectMe, Vector3 normal, float& min, float& max)
{
	//pre-empt dot product test
	min = projectMe.pointList[0].pos.Dot(normal);
	max = min;

	//loop through all points of projectMe
	for (int i = 1; i < 4; ++i)
	{
		//get the dot product
		curr_dot_product = projectMe.pointList[i].pos.Dot(normal);

		//check if is min/max
		if (curr_dot_product < min)
			min = curr_dot_product;
		else if (curr_dot_product > max)
			max = curr_dot_product;
	}
}

/*********************************************************************************************************
get the square dist btw this and check with given bounce + normal
/*********************************************************************************************************/
float Shape::Get_SqDist(Vector3 thisPos, Vector3 checkPos, float bounce, Vector3 normal)
{
	thisPos += normal * bounce;
	return (thisPos - checkPos).LengthSquared();
}

/*********************************************************************************************************
projections of CHECK and THIS onto every normal of THIS
param lowest_bounce: the lowest bounce value

this_intersected: the index of the point of THIS shape intersected
check_intersected: the index of the point of CHECK shape intersected

returns: index of face with lowest bounce
/*********************************************************************************************************/
float current_bounce, lowest_bounce, this_min, this_max, check_min, check_max, ulti_min, ulti_max;
float proj_magnitude;
float min_max_len;	//max - min
int lowest_bounce_face;

int Shape::ProjectShapeOntoTHIS(Shape & check, float& lowest_bounce, bool thisShape)
{
	current_bounce = lowest_bounce = 10000000.f;
	lowest_bounce_face = -1;	//index of lowest bounce's face

	//loop through all faces of THIS
	for (int i = 0; i < 2; ++i)
	{
		//project THIS onto the face and get min/max + corr. point index
		Shape::GetProjection(*this, faceList[i].normal, this_min, this_max);

		//project CHECK onto the face and get min/max + corr. point index
		Shape::GetProjection(check, faceList[i].normal, check_min, check_max);

		//if total intersect
		if ((this_min > check_min && this_max < check_max) || (check_min > this_min && check_max < this_max))
		{
			ulti_min = abs(this_min - check_min);
			ulti_max = abs(this_max - check_max);
		}
		else {
			//get the ultimate min/max
			ulti_min = this_min < check_min ? this_min : check_min;
			ulti_max = this_max > check_max ? this_max : check_max;
		}

		//get the magnitude of both projections
		proj_magnitude = (this_max - this_min) + (check_max - check_min);

		//get the total projection length
		min_max_len = ulti_max - ulti_min;

		//return false if bounce > magnitude
		if (min_max_len > proj_magnitude)
			return -1;

		//get the bounce value (intersect)
		current_bounce = proj_magnitude - min_max_len;

		//check for lowest bounce, if yes, replace
		if (current_bounce < lowest_bounce)
		{
			//replace
			lowest_bounce = current_bounce;
			lowest_bounce_face = i;
		}
	}

	return lowest_bounce_face;
}

/*********************************************************************************************************
2D shape
/*********************************************************************************************************/
int this_lb_index, check_lb_index;	//index of face with lowest bounce for THIS/CHECK shape
float this_lb_val, check_lb_val;	//bounce value of ~
float ulti_lb_val;
Vector3 ulti_lb_normal;
int collidedFace_index = -1;
bool thisShape_face = false;	//if the normal of a face from this shape is used

bool Shape::CollisionCheck(Shape & check, Vector3& vel)
{
	////responded before in this frame
	//if (collided)
	//	return false;
	//reset for this check--------------------------------------------------------------//
	thisShape_face = false;
	collidedFace_index = -1;

	//do SAT check for both THIS and CHECK-------------------------------------------------//
	this_lb_index = ProjectShapeOntoTHIS(check, this_lb_val, true);

	if (this_lb_index == -1) return false;

	check_lb_index = check.ProjectShapeOntoTHIS(*this, check_lb_val, false);

	if (check_lb_index == -1) return false;

	collided = true;	//positive collision
	check.collided = true;

	//get the lowest bounce and corrosponding normal---------------------------------------//
	//if both same, take either one (THIS)
	if (this_lb_val < check_lb_val)
	{
		ulti_lb_val = this_lb_val;
		thisShape_face = true;
		ulti_lb_normal = faceList[this_lb_index].normal;
		collidedFace_index = this_lb_index;	//collided face index
	}
	else
	{
		ulti_lb_val = check_lb_val;
		ulti_lb_normal = check.faceList[check_lb_index].normal;
		collidedFace_index = check_lb_index;	//collided face index
	}

	//do distance check for * 1 and  * -1-------------------------------------------------//
	//if * 1 > * -1
	if (Get_SqDist(position, check.position, ulti_lb_val, ulti_lb_normal) >
		Get_SqDist(position, check.position, ulti_lb_val, ulti_lb_normal * -1.f))
	{
		vel = response(ulti_lb_val, ulti_lb_normal);
	}
	else
	{
		ulti_lb_normal *= -1.f;
		vel = response(ulti_lb_val, ulti_lb_normal);

		//index 2-3 is correct
		collidedFace_index += 2;
	}

	//set the collided face flag-----------------------------------------//
	if (thisShape_face)
		collidedFace[collidedFace_index] = true;
	else
		check.collidedFace[collidedFace_index] = true;

	return true;
}

/*********************************************************************************************************
called by Transform::CalculateTRS()
/*********************************************************************************************************/
Vector3 offset_vel;
Transform* ulti_parent;
Vector3 Shape::response(float lowest_bounce, Vector3 & normal)
{
	//use grandparent's angle, if not dir will be wrong------//
	ulti_parent = Get_TopParent();
	float original_angle = ulti_parent->rotation.z;

	//do rotation repel (NORMAL MODIFIED!)---------------------------------------------//
	float angle = Vector3::getAngleFromDir(normal.x, normal.y);
	angle -= original_angle;
	normal.x = cos(Math::DegreeToRadian(angle));
	normal.y = sin(Math::DegreeToRadian(angle));

	//do translation repel---------------------------------------------//
	offset_vel = normal * lowest_bounce * 1.05f;

	//grandparent do TRS cal------------------------------------------//
	ulti_parent->Translate(offset_vel.x, offset_vel.y, offset_vel.z);
	ulti_parent->CalculateTRS();

	return offset_vel;
}
