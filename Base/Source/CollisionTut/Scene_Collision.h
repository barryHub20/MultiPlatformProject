#ifndef Scene_Collision_H
#define Scene_Collision_H

#include "Scene.h"

//discrete collision types
#include "CollisionTut\Polygon.h"
#include "CollisionTut\Polygon_3D.h"
#include "CollisionTut\GJK_Simplex2D.h"
#include "CollisionTut\GJK_Simplex3D.h"

/***************************************************************************************************************
Discrete collision test

Author: Tan Yie Cher
Date: 14/8/2017

Colision tutorial leading to Minkowski Difference
1) Sphere V Sphere
2) AABB V Sphere
3) OBB V Sphere
4) OBB V OBB

Run the corrosponding update and run function

Accompanying documentation here:
https://docs.google.com/document/d/1RZ4P26gKQ6gD8MRgaE_5IVlkIg7qwuYFJ6Z1TjLcRv8/edit#

Minkowski Difference start Date: 18/9/17
/***************************************************************************************************************/
class Scene_Continuous : public Scene
{
	//OBB---------------------------------------//
	CD_Polygon new_poly1, new_poly1_projected, new_poly1_final, new_poly2;

	//GJK 3D------------------------------------//
	CD_Polygon_3D poly_3d_1, poly_3d_2, poly_3d_1_projected;
	GJK_Simplex_3D gjk_simplex_3D;

	//collision functions-----------------------//
	void OBB_V_OBB_3D_Init();
	void OBB_V_OBB_3D_Update();
	void OBB_V_OBB_3D_Draw();

public:

	//CD----------------------------------------//
	Scene_Continuous();
	~Scene_Continuous();

	//Core--------------------------------------//
	virtual void Init();
	virtual void Update();
	virtual void Tmp_Draw();
	virtual void Tmp_DrawOnScreen();
	virtual void Exit();
	 
	//key pressed-------------------------------//
	virtual void Update_KeyPressed();
	virtual void Update_KeyReleased();
};

#endif