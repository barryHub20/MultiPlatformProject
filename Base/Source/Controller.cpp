#include "Controller.h"
const unsigned char Controller::FPS = 1000; // FPS of this game
const unsigned int Controller::frameTime = 1000 / FPS; // time for each frame

/*********************************************************************************************************
C/D
/*********************************************************************************************************/
Controller::Controller()
{
}

Controller::~Controller()
{
	delete scene;
}

/********************************************************************************
Init stuff
********************************************************************************/
void Controller::Init()
{
	//Timers: set both timers to 0-----------------------------//
	m_dElapsedTime = 0.0;
	m_dAccumulatedTime_thread1 = 0.0;
	m_dAccumulatedTime_thread2 = 0.0;

	//rand
	//srand(time(NULL));

	//Core Utilities------------------------------------------//
	CU::Init();

	//scene---------------------------------------------------//
	scene = new Scene_Pathfinding;
	scene->Init();
}


/********************************************************************************
input callbacks
********************************************************************************/
void Controller::KeyPressed()
{
	scene->Update_KeyPressed();
}

void Controller::KeyReleased()
{
	scene->Update_KeyReleased();
}
