#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "CoreUtilities.h"

//Scenes include here-------------------------//
#include "Collision\Scene_Collision.h"

/*******************************************************************************************************
New and improved controller

Warning: input is run on a seperate thread, as such callbacks are used

Author: Tan Yie Cher
Date: 25/12/2016
/*******************************************************************************************************/
class Controller
{
protected:

	Controller();
	~Controller();

	//Scenes--------------------------------//
	Scene* scene;

	/********************** FPS **********************/
	const static unsigned char FPS; // FPS of this game (ONLY RELEASE MODE CAN RUN SMOOTHLY AT 170FPS MAX)
	const static unsigned int frameTime; // time for each frame

	/********************** FPS calculator **********************/
	double m_dElapsedTime;
	double m_dAccumulatedTime_thread1;
	double m_dAccumulatedTime_thread2;
	float fps;

	/******************** internal functions **********************/
	virtual void Create_Display() = 0;	//create Opengl display
	virtual void Call_EveryFrame() = 0;	//call once every frame
	virtual void SwapBuffers() = 0;	//call once every frame

public:

	/******************** core functions **********************/
	virtual void Init();
	virtual void Run() = 0;
	virtual void Exit() = 0;

	/******************** input functions (Callbacks) **********************/
	virtual void KeyPressed();
	virtual void KeyReleased();
};

#endif