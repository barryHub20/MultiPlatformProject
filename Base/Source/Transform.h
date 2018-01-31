#ifndef TRANSFORMATION_H
#define TRANSFORMATION_H
#include "Vector3.h"
#include "Mtx44.h"

/*******************************************************************************************************
Transformation class

Author: Tan Yie Cher
Date: 1/1/2017

Description:
-TRS: the current transformations

Flow:
-your transformations/adding/removing chil
-CalculateTRS();

Edits:
-1/1/17: basic transformations to be integrated with basic Entity/Component, 2D rotation
-1/8/17: Framerate Proof collision transformation

Improvements:
-Use handles instead of pointers, slows down
/*******************************************************************************************************/
class Transform
{
public:

	Vector3 position;
	Vector3 localPosition;

	Vector3 rotation;
	Vector3 localRotation;

	Vector3 scale;
	Vector3 localScale;

	bool sticky;	//transform affects parent

public:

	Transform* parent;

	Mtx44 TRS, finalTRS;

	bool changesMade;	//changes made requires TRS recalculation

	std::vector<Transform*> childList;

	//Transformation by parent (only update values, not TRS)------------------------------//
	void Translate_ByParent(float x, float y, float z);
	virtual void Rotate_ByParent(float x, float y, float z);
	void Scale_ByParent(float x, float y, float z);

public:

	//CD--------------------------------//
	Transform();
	~Transform();

	//Transformation------------------------------//
	void Translate(float x, float y, float z);
	void SetTranslate(float x, float y, float z);
	virtual void Rotate(float x, float y, float z);
	virtual void SetRotate(float x, float y, float z);
	void Scale(float x, float y, float z);

	//Core------------------------------//
	void CalculateTRS();
	virtual void CalculateTRS_byParent(Mtx44& parentFinal);

	//Parent/child------------------------------//
	void AddChild(Transform& addMe);

	//Get------------------------------//
	const Mtx44& Get_FinalTRS();
	const Vector3& GetRotation(bool local);
	Vector3 GetPosition();

	//misc---------------------------------//
	Transform* Get_TopParent();
};

#endif