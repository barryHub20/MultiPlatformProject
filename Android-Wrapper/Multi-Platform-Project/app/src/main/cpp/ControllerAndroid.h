#ifndef CONTROLLERANDROID_H
#define CONTROLLERANDROID_H

#include <NDKHelper.h>
#include "Controller.h"
#include "AndroidUtils.h"

#include <jni.h>
#include <errno.h>

#include <EGL/egl.h>
#include <GLES/gl.h>

#include <android/sensor.h>
#include <android/log.h>
#include <android_native_app_glue.h>

/**********************************************************************************************
Android version

Author: Tan Yie Cher
Date: 25/12/2016 (volatile)
MAJOR REVISION: 17/2/2017
/**********************************************************************************************/
class Controller_Android : public Controller
{

    //internal override-----------------------------------------------------------//
    void Create_Display() override;	//create Opengl display
    void Call_EveryFrame() override;	//call once every frame
    void SwapBuffers() override;	//call once every frame

    //system functions-----------------------------------------------------------//
    void TermDisplay(); //terminate display
    void TrimMemory();
    bool IsReady();

    //input functions-----------------------------------------------------------//
    static void Update_Touch(AInputEvent* event);
    static Vector2 GetPointerPos(AInputEvent* event, int32_t pointer_ID);
    static int32_t FindIndex(const AInputEvent* event, int32_t id);
    static int32_t Find_ID(const AInputEvent* event, int32_t index);
    static void Remove_ID(int32_t id);

    //C/D------------------------------------------------------------------//
    Controller_Android(){}
    ~Controller_Android(){}

private:

	static android_app* app;

	//input
    static const int MAX_TOUCH_EVENTS = 15;
    static vector<int32_t> pointerList; //store the unique pointer lists
    static Vector2 pointerPosList[MAX_TOUCH_EVENTS];
    static Vector2 pointerPos;  //shared

	EGLDisplay display;
	EGLSurface surface;
	EGLContext context;
	int32_t width;
	int32_t height;
	bool initialized_resources_;
	bool has_focus_;

    //display
    float X_to_Y_ratio; //display X to Y ratio (NOT GAME SCREEN!!)
    static float new_cameraWidth;  //store new cam. width instead of screen

	// frame clock -- it computes the deltas between successive frames so we can update stuff properly
    DeltaClock mFrameClock;

    ndk_helper::GLContext* gl_context_;

	static Controller_Android* instance;


public:

    void PreInit(android_app* app);

	//core---------------------------------------------------------------//
	static Controller_Android* Instance();

	//override---------------------------------------------------------------//
	void Init() override;
	void Run() override;
	void Exit() override;

	//callback---------------------------------------------------------------//
	static int32_t engine_handle_input(struct android_app* app, AInputEvent* event);
	static void engine_handle_cmd(struct android_app* app, int32_t cmd);

	//get app---------------------------------------------------------------//
	static const android_app* GetApp();
};

#endif