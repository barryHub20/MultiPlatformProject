#include "Scene_Collision.h"
#include "CoreUtilities.h"

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Scene_Continuous::Scene_Continuous()
{
}

Scene_Continuous::~Scene_Continuous()
{
	delete triangle_mesh;
}

/*********************************************************************************************************
Init: ALWAYS CALLED LAST
/*********************************************************************************************************/
void Scene_Continuous::Init()
{
	Scene::Init();
	OBB_V_OBB_3D_Init();
}

/*********************************************************************************************************
Update
/*********************************************************************************************************/
void Scene_Continuous::Update()
{
	Scene::Update();
	OBB_V_OBB_3D_Update();

	//set basic shader so it starts first later-----------------------------------------------------------------------//
	CU::view.SetShader(BASIC_SHADER);
}

/*********************************************************************************************************
tmp draw
/*********************************************************************************************************/
void Scene_Continuous::Tmp_Draw()
{
	Scene::Tmp_Draw();
	OBB_V_OBB_3D_Draw();
}

void Scene_Continuous::Tmp_DrawOnScreen()
{
	CU::view.SetShader(TEXT_SHADER);
	CU::view.RenderText("CONTINUOUS", Vector2(-190, 130), 1.f, Color(5, 242, 230));

	//render text
	ss.str("");
	ss << "FPS: " << (int)CU::fps;
	CU::view.RenderText(ss.str(), Vector2(90, 130), 1.f, Color(227, 157, 27));
}

/*********************************************************************************************************
Update press
/*********************************************************************************************************/
void Scene_Continuous::Update_KeyPressed()
{
	Scene::Update_KeyPressed();
}

void Scene_Continuous::Update_KeyReleased()
{
	Scene::Update_KeyReleased();
}

/*********************************************************************************************************
Exit
/*********************************************************************************************************/
void Scene_Continuous::Exit()
{
	Scene::Exit();
}

/*********************************************************************************************************
new and improved MD
/*********************************************************************************************************/
void Scene_Continuous::OBB_V_OBB_Init()
{
	Vector3 pos(100, -60, 0.f);
	Vector3 scale(170, 150, 1);

	//POLYGON MUST FOLLOW THE L -> R rule (0,1,2,3 order)
	new_poly1.Init(pos, scale, false, Color(242, 133, 242), CU::line_blue);
	new_poly1_projected.Init(pos, scale, false, Color(130, 237, 198), CU::line_blue);
	new_poly1_final.Init(pos, scale, false, Color(227, 232, 79), CU::line_purple);
	new_poly2.Init(Vector3(-100, -50, 0), Vector3(20, 150, 1), false, Color(0, 255, 0), CU::line_green);

	//project poly 1-------------------------------------------------------------//
	new_poly1.ProjectShape(Vector3(-50.f, -80.f, 0.f), 26.f);
	new_poly2.ProjectObstacle(Vector3(0.f, 0.f, 0.f), 100.f);
}

void Scene_Continuous::OBB_V_OBB_Update()
{
	//camera---------------------------------------------------------------------//
	CU::view.camera.Update(CU::dt);
	CU::view.camera.UpdateInput(CU::dt);

	float speed = 150.f * CU::dt;

	//D-Pad----------------------------------------------------------------------//
	Vector2 dir;

	if (CU::input.DPad_Right(dir))
	{
		CU::input.dpad_R.SetTexture(TEX_INNER_DPAD_WASD, TEX_INNER_DPAD, TEX_OUTER_DPAD);

		//see the quadrant current angle is in
		double pad_angle = Vector2::getAngleFromDir(dir);
		if (pad_angle >= 360.f)pad_angle -= 360.f;
		else if (pad_angle < 0.f)pad_angle += 360.f;

		if ((pad_angle >= 315.f && pad_angle <= 360.f) || (pad_angle >= 0.f && pad_angle < 45.f))	//D
		{
			CU::input.dpad_R.SetRotate(315.f);
			new_poly1.Translate(speed, 0, 0);
		}
		else if (pad_angle >= 45.f && pad_angle < 135.f)	//W
		{
			CU::input.dpad_R.SetRotate(45.f);
			new_poly1.Translate(0, speed, 0);
		}
		else if (pad_angle >= 135.f && pad_angle < 225.f)	//A
		{
			CU::input.dpad_R.SetRotate(135.f);
			new_poly1.Translate(-speed, 0, 0);
		}
		else if (pad_angle >= 225.f && pad_angle < 315.f)	//S
		{
			CU::input.dpad_R.SetRotate(225.f);
			new_poly1.Translate(0, -speed, 0);
		}
	}
	else
		CU::input.dpad_R.SetTexture(TEX_INNER_DPAD, TEX_INNER_DPAD, TEX_OUTER_DPAD);
	
	//controls PC----------------------------------------------------------------//
	/*if (GetAsyncKeyState('W'))
		new_poly1.Translate(0, speed, 0);
	if (GetAsyncKeyState('S'))
		new_poly1.Translate(0, -speed, 0);
	if (GetAsyncKeyState('A'))
		new_poly1.Translate(-speed, 0, 0);
	if (GetAsyncKeyState('D'))
		new_poly1.Translate(speed, 0, 0);*/

	//obstacle-------------------------------------------------------------------//
	new_poly2.Rotate(0, 0, speed * 0.5f);

	//the 2 polys
	new_poly1.RecalculatePoints();
	new_poly2.RecalculatePoints();

	//final projected shape
	new_poly1_final.CopyFrom(new_poly1);
	new_poly1_final.Translate(new_poly1.projVel.x, new_poly1.projVel.y, new_poly1.projVel.z);
	new_poly1_final.Rotate(0, 0, new_poly1.projRot);
	new_poly1_final.RecalculatePoints();

	//collision check
	CU::discrete_col.start_OBB_V_OBB2(new_poly1, new_poly2, new_poly1_projected);
}

void Scene_Continuous::OBB_V_OBB_Draw()
{
	new_poly2.Draw(0.f);
	new_poly1_projected.Draw(1.f);
	new_poly1.Draw(2.f);
	new_poly1_final.Draw(-1.f);
	//CU::discrete_col.Draw();

	//draw point at origin
	CU::view.Pre_DrawMesh(Vector3(0, 0, 0), Vector3(1, 1, 1), CU::circle_blue);
	CU::circle_blue->Draw();
}

/*********************************************************************************************************
3D
/*********************************************************************************************************/
void Scene_Continuous::OBB_V_OBB_3D_Init()
{
	show_mesh = false;
	poly_3d_1.Init(Vector3(-30, 0, 0), Vector3(50, 100, 120), Vector3(1, 0, 0), false, Color(0, 0, 255));
	poly_3d_2.Init(Vector3(70, 0, 0), Vector3(50, 50, 50), Vector3(1, 0, 0), false, Color(0, 0, 255));

	poly_3d_1.yawRot(205.f);
	poly_3d_1.pitchRot(12.0f);
	poly_3d_1.RecalculatePoints();

	poly_3d_1.RecalculatePoints();
	poly_3d_2.RecalculatePoints();

	//start the GJK iteration
	gjk_simplex_3D.GetClosestPoints_Start(poly_3d_1, poly_3d_2);
}

double stepThru_timer = 0.2;
bool show_3d_shapes = false;
void Scene_Continuous::OBB_V_OBB_3D_Update()
{
	float speed = 150.f * CU::dt;

	//camera---------------------------------------------------------------------//
	CU::view.camera.Update(CU::dt);
	CU::view.camera.UpdateInput(CU::dt * 2.f);

	//transformation-------------------------------------------------------------//
	bool key_pressed = false;
	if (GetAsyncKeyState('E'))
	{
		key_pressed = true;
		poly_3d_1.yawRot(speed * 0.1f);
	}

	if (GetAsyncKeyState('R'))
	{
		key_pressed = true;
		poly_3d_1.pitchRot(speed);
	}

	/*if (GetAsyncKeyState('Y'))
		poly_3d_2.yawRot(speed);
	if (GetAsyncKeyState('U'))
		poly_3d_2.pitchRot(speed);*/
	
	//step through---------------------------------------------------------------//
	//if (stepThru_timer <= 0.0)
	//{
	//	if (GetAsyncKeyState('Y'))
	//	{
	//		//ORDER IS IMPORTANT
	//		gjk_simplex_3D.GetClosestPoints(poly_3d_1, poly_3d_2);
	//		stepThru_timer = 0.2;
	//	}
	//}
	//else
	//	stepThru_timer -= CU::dt;

	if (GetAsyncKeyState('U'))
		show_3d_shapes = true;
	if (GetAsyncKeyState('I'))
		show_3d_shapes = false;

	/*if (GetAsyncKeyState('T'))
		poly_3d_1.Forward(1.f);
	if (GetAsyncKeyState('G'))
		poly_3d_1.Forward(-1.f);
	if (GetAsyncKeyState('F'))
		poly_3d_1.Strafe(-1.f);
	if (GetAsyncKeyState('H'))
		poly_3d_1.Strafe(1.f);*/

	//actual implementation------------------------------------------------------//
	poly_3d_1.RecalculatePoints();
	poly_3d_2.RecalculatePoints();
	gjk_simplex_3D.GetClosestPoints_Start(poly_3d_1, poly_3d_2);
	gjk_simplex_3D.GetClosestPoints(poly_3d_1, poly_3d_2);

	/*if (key_pressed)
	{
		if (!gjk_simplex_3D.infinite_loop)
			cout << "closestDist: " << gjk_simplex_3D.closestDist << endl;
		else
			cout << "closestDist (inf. loop): " << gjk_simplex_3D.closestDist << endl;
	}*/
}

void Scene_Continuous::OBB_V_OBB_3D_Draw()
{
	//axes--------------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(0,0,0), Vector3(1700, 1700, 1700), CU::axes);
	CU::axes->Draw();

	if (show_3d_shapes)
	{
		poly_3d_1.Draw();
		poly_3d_2.Draw();
	}

	//gjk_simplex_3D.Draw();
}

/*********************************************************************************************************
2D GJK
/*********************************************************************************************************/
void Scene_Continuous::GJK_2D_Init()
{
	Vector3 pos(-160, 100, 0);
	Vector3 scale(190, 100, 1);

	//the actual polygons
	gjk_2d_poly_1.Init(pos, scale, false, Color(242, 133, 242), CU::line_blue);
	gjk_2d_poly_2.Init(Vector3(-250, -40, 0), Vector3(175, 100, 1), false, Color(227, 232, 79), CU::line_orange);
	
	//visual aids
	gjk_2d_poly_1_projected.Init(pos, scale, true, Color(130, 237, 198), CU::line_blue);
	gjk_2d_poly_1_final.Init(pos, scale, false, Color(227, 170, 250), CU::line_purple);
	
	//some init time transformation
	gjk_2d_poly_1.Rotate(0, 0, 10);
	gjk_2d_poly_1.RecalculatePoints();


	//project poly 1
	gjk_2d_poly_1.ProjectShape(Vector3(-250.f, 120.f, 0.f), 20.f);
	gjk_2d_poly_2.ProjectObstacle(Vector3(0.f, 0.f, 0.f), 5.f);

	//flags
	gjk_simplex_check_started = false;
	gjk_stepthrough_timer = 0.2;
}

void Scene_Continuous::GJK_2D_Update()
{
	float speed = 150.f * CU::dt;

	//controls PC----------------------------------------------------------------//
	if (GetAsyncKeyState('W'))
		gjk_2d_poly_1.Translate(0, speed, 0);
	if (GetAsyncKeyState('S'))
		gjk_2d_poly_1.Translate(0, -speed, 0);
	if (GetAsyncKeyState('A'))
		gjk_2d_poly_1.Translate(-speed, 0, 0);
	if (GetAsyncKeyState('D'))
		gjk_2d_poly_1.Translate(speed, 0, 0);

	if (GetAsyncKeyState('T'))
		gjk_2d_poly_1.Rotate(0, 0, speed * 0.5f);
	if (GetAsyncKeyState('Y'))
		gjk_2d_poly_2.Rotate(0, 0, speed * 0.5f);

	//the 2 polys----------------------------------------------------------------//
	gjk_2d_poly_1.RecalculatePoints();
	gjk_2d_poly_2.RecalculatePoints();

	//final projected shape------------------------------------------------------//
	gjk_2d_poly_1_final.CopyFrom(gjk_2d_poly_1);
	gjk_2d_poly_1_final.Translate(gjk_2d_poly_1.projVel.x, gjk_2d_poly_1.projVel.y, gjk_2d_poly_1.projVel.z);
	gjk_2d_poly_1_final.Rotate(0, 0, gjk_2d_poly_1.projRot);
	gjk_2d_poly_1_final.RecalculatePoints();
	
	//gjk_simplex.GetClosestPoints(gjk_2d_poly_1, gjk_2d_poly_2);
	gjk_simplex.CollisionResponse(gjk_2d_poly_1, gjk_2d_poly_1_projected, gjk_2d_poly_2);
}

void Scene_Continuous::GJK_2D_Draw()
{
	gjk_2d_poly_1.Draw(-2.f);
	gjk_2d_poly_2.Draw(-1.f);
	gjk_2d_poly_1_projected.Draw(-1.f);
	gjk_2d_poly_1_final.Draw(0.f);

	gjk_simplex.Draw();

	//origin--------------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(0, 0, 0), Vector3(10, 10, 10), CU::circle_blue);
	CU::circle_blue->Draw();
}

/*********************************************************************************************************
3D triangle collision
/*********************************************************************************************************/
void Scene_Continuous::Triangle_3D_Init()
{
	triangle_pt[0].Set(-50, 0, 0);
	triangle_pt[1].Set(-80, 0, 100);
	triangle_pt[2].Set(-150, 100, 50);
	triangle_mesh = new Mesh();
	triangle_mesh->Init_Triangle(triangle_pt, Color(154, 163, 69), Color(129, 199, 54));
	
	Vector3 CA = triangle_pt[2] - triangle_pt[0];
	Vector3 BA = triangle_pt[1] - triangle_pt[0];

	triangle_normal = CA.Cross(BA).Normalized();
	triangle_up.Set(0, 1, 0);
	triangle_right = triangle_normal.Cross(triangle_up);
	triangle_up = triangle_normal.Cross(triangle_right);

	triangle_offset.Set(0, 0, 0);
}

void Scene_Continuous::Triangle_3D_Update()
{
	float speed = 150.f * CU::dt;

	//camera---------------------------------------------------------------------//
	CU::view.camera.Update(CU::dt);
	CU::view.camera.UpdateInput(CU::dt * 2.f);

	//triangle controls----------------------------------------------------------//
	Vector3 triangle_offset_;
	if (GetAsyncKeyState('G'))	//DOWN
		triangle_offset_ += Vector3(triangle_up.x * speed, triangle_up.y * speed, triangle_up.z * speed);
	if (GetAsyncKeyState('T'))	//UP
		triangle_offset_ += Vector3(triangle_up.x * -speed, triangle_up.y * -speed, triangle_up.z * -speed);
	if (GetAsyncKeyState('H'))	//RIGHT
		triangle_offset_ += Vector3(triangle_right.x * -speed, triangle_right.y * -speed, triangle_right.z * -speed);
	if (GetAsyncKeyState('F'))	//LEFT
		triangle_offset_ += Vector3(triangle_right.x * speed, triangle_right.y * speed, triangle_right.z * speed);

	if (GetAsyncKeyState('U'))	//FORWARD
		triangle_offset_ += Vector3(triangle_normal.x * speed, triangle_normal.y * speed, triangle_normal.z * speed);
	if (GetAsyncKeyState('J'))	//BACKWARD
		triangle_offset_ += Vector3(triangle_normal.x * -speed, triangle_normal.y * -speed, triangle_normal.z * -speed);

	triangle_offset += triangle_offset_;
	triangle_pt[0] += triangle_offset_;
	triangle_pt[1] += triangle_offset_;
	triangle_pt[2] += triangle_offset_;

	//closest point from origin--------------------------------------------------//
	triangleIntersection(Vector3(0, 0, 0), -triangle_normal);

	//triangle normal to angle (for render)--------------------------------------//
	Vector3 inv_triNormal = triangle_intersectPt.Normalized();
	ray_yaw = Math::RadianToDegree(atan2(-inv_triNormal.z, inv_triNormal.x));
	ray_pitch = -Math::RadianToDegree(asin(inv_triNormal.y));
}

bool Scene_Continuous::triangleIntersection(Vector3 rayOrigin, Vector3 rayVector)
{
	Vector3 edge1, edge2, h, s, q;
	float a, f, u, v;

	edge1 = triangle_pt[1] - triangle_pt[0];
	edge2 = triangle_pt[2] - triangle_pt[0];
	h = rayVector.Cross(edge2);
	a = edge1.Dot(h);

	if (a > -Math::EPSILON && a < Math::EPSILON)
	{
		NonIntersection();
		return false;
	}

	f = 1.f / a;
	s = rayOrigin - triangle_pt[0];
	u = f * s.Dot(h);

	if (u < 0.f || u > 1.f)
	{
		NonIntersection();
		return false;
	}

	q = s.Cross(edge1);
	v = f * rayVector.Dot(q);

	if (v < 0.f || v + u > 1.f)
	{
		NonIntersection();
		return false;
	}

	float t = f * edge2.Dot(q);
	
	triangle_intersectPt = rayOrigin + rayVector * t;
	return true;

	//if (t > 0.f)	//uncomment if inwards facing to be ignored
	//{
	//	triangle_intersectPt = rayOrigin + rayVector * t;
	//	return true;
	//}
	//else
	//{
	//	//line intersection
	//	NonIntersection();
	//	return false;
	//}

	//NonIntersection();
	//return false;
}

void Scene_Continuous::NonIntersection()
{
	//AB, AC, BC edges projection check
	//A(0) -> C(2)
	Vector3 edges[3];
	Vector3 edges_intersectPt[3];

	float lensqList[6];	//0 - 2: edge0 -> edge2, 3 - 5: A -> C
	edges[0] = triangle_pt[1] - triangle_pt[0];	//AB
	edges[1] = triangle_pt[2] - triangle_pt[0];	//AC
	edges[2] = triangle_pt[2] - triangle_pt[1];	//BC

	Vector3 AO;
	float t = 0.f;

	//AB-------------------------------------------------------------------------//
	AO = -triangle_pt[0];
	t = (AO.Dot(edges[0])) / edges[0].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[0] = edges[0] * t + triangle_pt[0];
		lensqList[0] = edges_intersectPt[0].LengthSquared();
	}
	else
		lensqList[0] = 100000000000.f;

	//AC-------------------------------------------------------------------------//
	AO = -triangle_pt[0];
	t = (AO.Dot(edges[1])) / edges[1].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[1] = edges[1] * t + triangle_pt[0];
		lensqList[1] = edges_intersectPt[1].LengthSquared();
	}
	else
		lensqList[1] = 100000000000.f;

	//BC-------------------------------------------------------------------------//
	AO = -triangle_pt[1];
	t = (AO.Dot(edges[2])) / edges[2].LengthSquared();

	if (t >= 0.f && t <= 1.f)
	{
		edges_intersectPt[2] = edges[2] * t + triangle_pt[1];
		lensqList[2] = edges_intersectPt[2].LengthSquared();
	}
	else
		lensqList[2] = 100000000000.f;

	//closest vertex check-------------------------------------------------------//
	for (int i = 0; i < 3; ++i)
		lensqList[i + 3] = triangle_pt[i].LengthSquared();

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
		triangle_intersectPt = triangle_pt[smallestLensq_Index - 3];
}

void Scene_Continuous::Triangle_3D_Draw()
{
	//axes--------------------------------------------------------------//
	CU::view.Pre_DrawMesh(Vector3(0, 0, 0), Vector3(1700, 1700, 1700), CU::axes);
	CU::axes->Draw();

	CU::view.Pre_DrawMesh(triangle_offset, Vector3(1, 1, 1), triangle_mesh);
	triangle_mesh->Draw();

	float len = triangle_intersectPt.Length();
	CU::view.Pre_DrawMesh(Vector3(0, 0, 0), ray_yaw, ray_pitch, Vector3(0, 1, 0), Vector3(0, 0, -1),
		Vector3(len, len, len), CU::line_yellow);
	CU::line_yellow->Draw();

	CU::view.Pre_DrawMesh(triangle_intersectPt, Vector3(5, 5, 5), CU::sphere_dark_green);
	CU::sphere_dark_green->Draw();
}