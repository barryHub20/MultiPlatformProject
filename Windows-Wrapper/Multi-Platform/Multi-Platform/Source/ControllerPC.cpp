#include "ControllerPC.h"
Controller_PC* Controller_PC::instance = NULL;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

/********************************************************************************
Init stuff
********************************************************************************/
void Controller_PC::Init()
{
	//call first!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Create_Display();

	//pre-CU stuff init---------------------------------------------------------------//

	Controller::Init();
}

Controller_PC* Controller_PC::Instance()
{
	if (!instance)
		instance = new Controller_PC;
	return instance;
}

void Controller_PC::Create_Display()
{
	//window-------------------------------------------------------------------------//
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL


	//Create a window and create its OpenGL context
	m_window = glfwCreateWindow(Screen::SCREEN_WIDTH, Screen::SCREEN_HEIGHT, "Computer Graphics", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread.
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK)
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}
}

GLFWwindow* Controller_PC::Get_windows()
{
	return m_window;
}

/********************************************************************************
Run stuff
********************************************************************************/
void Controller_PC::Run()
{
	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame

	while (!glfwWindowShouldClose(m_window) && !CU::input.isKeyPressed(InputState::keycodes::Q))
	{
		if (!CU::initialized_resources)
			continue;

		//=============================================================================================================================================//
		//get the elapsed time--------------------------------------//
		CU::dt = m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_thread1 += m_dElapsedTime;
		m_dAccumulatedTime_thread2 += m_dElapsedTime;
		//=============================================================================================================================================//

		//=============================================================================================================================================//
		//fps------------------------------------------------//
		CU::fps = fps = (float)(1.f / m_dElapsedTime);

		//=============================================================================================================================================//

		//Call once every frame (platform independent)------------------------------------------------//
		Call_EveryFrame();

		m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.
	}
}

/********************************************************************************
Call once every frame
********************************************************************************/
void Controller_PC::Call_EveryFrame()
{
	//pre-update
	CU::PreUpdate();

	//update
	scene->Update();

	//render
	CU::view.ClearScreen();	
	CU::DrawInWorld();
	scene->Tmp_Draw();
	CU::DrawOnScreen();
	scene->Tmp_DrawOnScreen();
	CU::view.PostRender();

	//swap buffers------------------------------------------------//
	SwapBuffers();
}

/********************************************************************************
Swap buffers
********************************************************************************/
void Controller_PC::SwapBuffers()
{
	//Swap buffers
	glfwSwapBuffers(m_window);
	//Get and organize events, like keyboard and mouse input, window resizing, etc...
	glfwPollEvents();
}

/********************************************************************************
Exit stuff
********************************************************************************/
void Controller_PC::Exit()
{
	//CU exit------------------------------------//
	CU::Exit();

	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);

	//Finalize and clean up GLFW
	glfwTerminate();
	m_window = NULL;

	delete instance;
}