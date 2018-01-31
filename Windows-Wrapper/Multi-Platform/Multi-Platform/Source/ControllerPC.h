#ifndef CONTROLLERPC_H
#define CONTROLLERPC_H

#include "Controller.h"

/*************************************************************
PC version

Author: Tan Yie Cher
Date: 25/12/2016
/*************************************************************/
class Controller_PC : public Controller
{
public:

	GLFWwindow* m_window;

private:

	Controller_PC(){}
	~Controller_PC(){}

	static Controller_PC* instance;

	/********************** stopwatch **********************/
	StopWatch m_timer;

	/******************** internal functions **********************/
	void Create_Display() override;
	void Call_EveryFrame() override;	//call once every frame
	void SwapBuffers() override;

public:

	static Controller_PC* Instance();

	/******************** core functions **********************/
	void Init() override;
	void Run() override;
	void Exit() override;

	/******************** get/set functions **********************/
	GLFWwindow* Get_windows();
};

#endif