#include "Transform.h"

//helper mtx
Mtx44 translateMat, rotateMat, scaleMat;

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Transform::Transform()
{
	//default values
	localScale = scale = Vector3(1, 1, 1);
	changesMade = false;
	TRS.SetToIdentity();
	finalTRS.SetToIdentity();
	parent = NULL;
}

Transform::~Transform(){}

/*********************************************************************************************************
Translate
/*********************************************************************************************************/
void Transform::Translate(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
	translateMat.SetToTranslation(x, y, z);
	TRS = TRS * translateMat;
	changesMade = true;

	//translate children
	for (int i = 0; i < childList.size(); ++i)
		childList[i]->Translate_ByParent(x, y, z);
}

void Transform::SetTranslate(float x, float y, float z)
{
	translateMat.SetToTranslation(x - position.x, y - position.y, z - position.z);
	TRS = TRS * translateMat;
	changesMade = true;

	//translate children
	for (int i = 0; i < childList.size(); ++i)
		childList[i]->Translate_ByParent(x - position.x, y - position.y, z - position.z);

	position.x = x;
	position.y = y;
	position.z = z;
}

/*********************************************************************************************************
Rotate
/*********************************************************************************************************/
void Transform::Rotate(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	rotation.Clip_EulerAngles();
	rotateMat.SetToRotation(z, 0,0,1);
	TRS = TRS * rotateMat;
	changesMade = true;

	//rotate children
	for (int i = 0; i < childList.size(); ++i)
		childList[i]->Rotate_ByParent(x, y, z);
}

/*********************************************************************************************************
Set rotate
/*********************************************************************************************************/
void Transform::SetRotate(float x, float y, float z)
{
	Vector3 rotateVal(x - rotation.x, y - rotation.y, z - rotation.z);
	rotation.Set(x, y, z);
	rotation.Clip_EulerAngles();

	rotateMat.SetToRotation(rotateVal.z, 0, 0, 1);
	TRS = TRS * rotateMat;
	changesMade = true;

	//rotate children
	//child rotates by displacement
	for (int i = 0; i < childList.size(); ++i)
		childList[i]->Rotate_ByParent(rotateVal.x, rotateVal.y, rotateVal.z);
}

/*********************************************************************************************************
Scale
/*********************************************************************************************************/
void Transform::Scale(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
	scaleMat.SetToScale(x, y, z);
	TRS = TRS * scaleMat;
	changesMade = true;

	//scale children
	for (int i = 0; i < childList.size(); ++i)
		childList[i]->Scale_ByParent(x, y, z);
}

/*********************************************************************************************************
Transform by parent
/*********************************************************************************************************/
void Transform::Translate_ByParent(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;
}

void Transform::Rotate_ByParent(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	rotation.Clip_EulerAngles();
}

void Transform::Scale_ByParent(float x, float y, float z)
{
	scale.x += x;
	scale.y += y;
	scale.z += z;
}

/*********************************************************************************************************
cal TRS
/*********************************************************************************************************/
void Transform::CalculateTRS()
{
	//no changes made does not necessiate TRS calculation
	if (!changesMade)
	{
		//check if children needs transforming
		for (int i = 0; i < childList.size(); ++i)
		{
			if (childList[i]->changesMade)
				childList[i]->CalculateTRS_byParent(TRS);	//final TRS
		}
		return;
	}

	//update finalTRS with current frame's transformations
	finalTRS = TRS;

	//children TRS must update
	for (int i = 0; i < childList.size(); ++i)
	{
		childList[i]->CalculateTRS_byParent(finalTRS);
	}

	changesMade = false;	//reset
}

/*********************************************************************************************************
cal TRS by parent
/*********************************************************************************************************/
void Transform::CalculateTRS_byParent(Mtx44& parentFinal)
{
	//calculate with parent's final TRS first
	finalTRS = parentFinal * TRS;
	
	//children TRS must update
	for (int i = 0; i < childList.size(); ++i)
	{
		childList[i]->CalculateTRS_byParent(finalTRS);
	}

	changesMade = false;	//reset
}

/*********************************************************************************************************
Add as child
/*********************************************************************************************************/
void Transform::AddChild(Transform& addMe)
{
	if (addMe.parent)
		return;

	childList.push_back(&addMe);

	//calculate TRS inversed
	if (parent)
		addMe.TRS = parent->TRS.GetInverse() * addMe.TRS;
	addMe.TRS = TRS.GetInverse() * addMe.TRS;

	//values must be inversed

	
	addMe.parent = this;
}

/*********************************************************************************************************
Get TRS
/*********************************************************************************************************/
const Mtx44& Transform::Get_FinalTRS(){ return finalTRS; }
const Vector3& Transform::GetRotation(bool local)
{
	return rotation;
}

Vector3 Transform::GetPosition()
{
	position = finalTRS * Vector3(1, 1, 1);
	return position;
}

/*********************************************************************************************************
Gets the highest parent, if there's no parent, NULL is returned
/*********************************************************************************************************/
Transform* sdsdfsdfs = NULL;
Transform* Transform::Get_TopParent()
{
	sdsdfsdfs = this;
	while (sdsdfsdfs->parent)
		sdsdfsdfs = sdsdfsdfs->parent;
	return sdsdfsdfs;
}