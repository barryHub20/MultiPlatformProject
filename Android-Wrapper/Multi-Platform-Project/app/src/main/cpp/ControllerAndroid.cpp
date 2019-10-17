#include "ControllerAndroid.h"
Controller_Android* Controller_Android::instance = NULL;
android_app* Controller_Android::app = NULL;

vector<int32_t> Controller_Android::pointerList;
Vector2 Controller_Android::pointerPosList[Controller_Android::MAX_TOUCH_EVENTS];
Vector2 Controller_Android::pointerPos;

float Controller_Android::new_cameraWidth = 0.f;

/*******************************************************************************************************************
instance
*******************************************************************************************************************/
Controller_Android* Controller_Android::Instance()
{
    if(!instance)
    {
        instance = new Controller_Android;
    }
    return instance;
}

/*******************************************************************************************************************
Set app
*******************************************************************************************************************/
void Controller_Android::PreInit(android_app* app)
{
    this->app = app;
    initialized_resources_ = false;
    has_focus_ = false;
    gl_context_ = ndk_helper::GLContext::GetInstance();
}

/*******************************************************************************************************************
Non-display init
*******************************************************************************************************************/
void Controller_Android::Init()
{
    //Timers: set both timers to 0-----------------------------//
    mFrameClock.SetMaxDelta(1.0f / 60.f);  //60fps (input is milliseconds)

    //Controller init------------------------------------------//
    Controller::Init();
}

/*******************************************************************************************************************
Create display
*******************************************************************************************************************/
void Controller_Android::Create_Display()
{
    //Display------------------------------------------------------------------//
        if (!initialized_resources_)
        {
            gl_context_->Init(app->window);
            Screen::SCREEN_WIDTH = gl_context_->GetScreenWidth();
            Screen::SCREEN_HEIGHT = gl_context_->GetScreenHeight();
            X_to_Y_ratio = Screen::SCREEN_WIDTH / Screen::SCREEN_HEIGHT;

            //assume width always more than height
            //we want width to match the scren ratio
            if(Screen::SCREEN_WIDTH > Screen::SCREEN_HEIGHT)
                new_cameraWidth = Screen::CAMERA_HEIGHT * X_to_Y_ratio;

            Init();

            initialized_resources_ = true;
        }
        else
        {
            // initialize OpenGL ES and EGL
            if (EGL_SUCCESS != gl_context_->Resume(app->window))
            {
                CU::Exit(); //don't term display
                Init();
            }
        }

        // Initialize GL state.
        //glEnable(GL_CULL_FACE);
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL);

        //Note that screen size might have been changed RATIO: 4:3
        float game_ScreenWidth = Screen::SCREEN_HEIGHT * Screen::CAM_X_TO_Y_RATIO; //4:3
        float excessWidth = Screen::SCREEN_WIDTH - game_ScreenWidth;

        glViewport(excessWidth * 0.5f,
                   0.f, game_ScreenWidth, gl_context_->GetScreenHeight());
        CU::view.ClearScreen();

        //reset timer to start now
        mFrameClock.Reset();
}

/*******************************************************************************************************************
call once every frame
*******************************************************************************************************************/
void Controller_Android::Call_EveryFrame()
{
    ///pre-update
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

/*******************************************************************************************************************
call once every frame
*******************************************************************************************************************/
void Controller_Android::SwapBuffers()
{
     //if (EGL_SUCCESS != gl_context_->Swap()) {
     //}
    gl_context_->Swap();
}

/********************************************************************************
constantly update touch
********************************************************************************/
void Controller_Android::Update_Touch(AInputEvent* event)
{
    pthread_mutex_lock(&CU::mutex_);

    for(int i = 0; i < pointerList.size(); ++i)
    {
        pointerPos = GetPointerPos(event, FindIndex(event, pointerList[i]));
        if(pointerPosList[i].Same(pointerPos))
            continue;
        pointerPosList[i] = pointerPos;
        CU::input.cursor_KeyHeld(pointerList[i], pointerPosList[i], i);
    }

    pthread_mutex_unlock(&CU::mutex_);
}

/********************************************************************************
Get pointer index of the ID
 (READ ONLY) no need thread safety measures
********************************************************************************/
int32_t Controller_Android::FindIndex(const AInputEvent* event, int32_t id) {

    for (auto i = 0; i < pointerList.size(); ++i) {
        if (id == AMotionEvent_getPointerId(event, i)) return i;
    }
    return -1;
}

int32_t Controller_Android::Find_ID(const AInputEvent* event, int32_t index)
{
    for(int i = 0; i < pointerList.size(); ++i)
    {
        if(pointerList[i] == AMotionEvent_getPointerId(event, index))
            return pointerList[i];
    }
    return -1;
}

void Controller_Android::Remove_ID(int32_t id)
{
	pthread_mutex_lock(&CU::mutex_);

    auto it = pointerList.begin();
    auto it_end = pointerList.end();

    for (; it != it_end; ++it) {
        if (*it == id) {
            pointerList.erase(it);
            break;
        }
    }

	pthread_mutex_unlock(&CU::mutex_);
}

/********************************************************************************
Get pointer pos
 (DO NOT have thread-safe lock here)
********************************************************************************/
Vector2 Controller_Android::GetPointerPos(AInputEvent* event, int32_t pointer_index)
{
    pthread_mutex_lock(&CU::mutex_);

    Vector2 returnVal;
    returnVal.x = AMotionEvent_getX(event, pointer_index);
    returnVal.y = AMotionEvent_getY(event, pointer_index);

    returnVal.x = (returnVal.x / Screen::SCREEN_WIDTH) * new_cameraWidth;
    returnVal.y = Screen::CAMERA_HEIGHT - ((returnVal.y / Screen::SCREEN_HEIGHT) * Screen::CAMERA_HEIGHT);

    //tmp raycast to world
    returnVal.x -= new_cameraWidth * 0.5f;
    returnVal.y -= Screen::CAMERA_HEIGHT * 0.5f;

    pthread_mutex_unlock(&CU::mutex_);

    return returnVal;
}

/*******************************************************************************************************************
Handle input
*******************************************************************************************************************/
int32_t Controller_Android::engine_handle_input(struct android_app* app, AInputEvent* event)
{
    Controller_Android* engine = (Controller_Android*)app->userData;
        int return_val = 0;

        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION) {

            float xPos = AMotionEvent_getX(event, 0);
            float yPos = AMotionEvent_getY(event, 0);
            return_val = 1;
        }

        CU::input.SetCalledKey(InputState::keycodes::TAP, false);

        //detect tap input------------------------------------------//
        int32_t action = AMotionEvent_getAction(event);

        int32_t pointerIndex = -1;
        int32_t pointer_ID = -1;

        unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;
        switch (flags) {
            case AMOTION_EVENT_ACTION_DOWN:

                //param event, param index
                pointerList.push_back(AMotionEvent_getPointerId(event, 0)); //always 0
                pointerPos = GetPointerPos(event, 0);
                CU::input.cursor_KeyDown(InputState::keycodes::TAP, pointerList.back(), pointerPos, 0);
                pointerPosList[0] = pointerPos;

                return_val = 1;

                break;
            case AMOTION_EVENT_ACTION_UP: { //TAP DETECTED (PRIMARY CURSOR)

                pointer_ID = Find_ID(event, 0); //confirm 0
                Remove_ID(pointer_ID);  //remove that ID from tracker
                CU::input.cursor_KeyUp(InputState::keycodes::TAP, pointer_ID, 0);


                return_val = 1;
                break;

                case AMOTION_EVENT_ACTION_MOVE:

                CU::input.SetCalledKey(InputState::keycodes::TAP, true);
                Update_Touch(event);

                return_val = 1;
                break;

                case AMOTION_EVENT_ACTION_POINTER_DOWN: //(SECONDARY CURSORS)

                    //get the index of this new pointer
                    pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                    pointerList.push_back(AMotionEvent_getPointerId(event, pointerIndex));
                    pointerPos = GetPointerPos(event, pointerIndex);
                    CU::input.cursor_KeyDown(InputState::keycodes::TAP, pointerList.back(), pointerPos, pointerIndex);
                    pointerPosList[pointerIndex] = pointerPos;

                return_val = 1;
                break;
                case AMOTION_EVENT_ACTION_POINTER_UP:

                    //get the index of the released pointer
                pointerIndex = (action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;
                pointer_ID = Find_ID(event, pointerIndex);
                Remove_ID(pointer_ID);  //remove that ID from tracker
                CU::input.cursor_KeyUp(InputState::keycodes::TAP, pointer_ID, pointerIndex);

                return_val = 1;
                break;
            }
        }

        return return_val;
}

/*******************************************************************************************************************
Handle input
*******************************************************************************************************************/
void Controller_Android::engine_handle_cmd(struct android_app* app, int32_t cmd)
{
    switch (cmd) {
        case APP_CMD_SAVE_STATE:

            break;
        case APP_CMD_INIT_WINDOW:
            // The window is being shown, get it ready.
            if (Controller_Android::Instance()->app->window != NULL) {
                Controller_Android::Instance()->Create_Display();
                Controller_Android::Instance()->Call_EveryFrame();
            }
            break;
        case APP_CMD_TERM_WINDOW:
            // The window is being hidden or closed, clean it up.
            Controller_Android::Instance()->TermDisplay();
            Controller_Android::Instance()->has_focus_ = false;
            break;
        case APP_CMD_GAINED_FOCUS:
            Controller_Android::Instance()->has_focus_ = true;
            break;
        case APP_CMD_LOST_FOCUS:

            // Also stop animating.
            Controller_Android::Instance()->has_focus_ = false;
            break;
        case APP_CMD_LOW_MEMORY:
            //Free up GL resources
            Controller_Android::Instance()->TrimMemory();
            break;
    }
}

/*******************************************************************************************************************
main loop
*******************************************************************************************************************/
void Controller_Android::Run()
{
// loop waiting for stuff to do.
    while (1) {

        // Read all pending events.
        int id;
        int events;
        struct android_poll_source* source;

        // If not animating, we will block forever waiting for events.
        // If animating, we loop until all events are read, then continue
        // to draw the next frame of animation.
        while ((id = ALooper_pollAll(IsReady() ? 0 : -1, NULL, &events, (void**)&source))
                       >= 0) {

            // Process this event.
            if (source != NULL) {
                source->process(app, source);
            }

            // Check if we are exiting.
            if (app->destroyRequested != 0) {

                TermDisplay();
                return;
            }
        }

        if (IsReady()) {

            //calculate dt----------------------------------------//
            CU::dt = mFrameClock.ReadDelta();
            CU::fps = (float)(1.f / CU::dt);

            // Drawing is throttled to the screen update rate, so there
            // is no need to do timing here.
            Call_EveryFrame();
        }
    }
}

/*******************************************************************************************************************
exit
*******************************************************************************************************************/
void Controller_Android::Exit()
{
    //CU exit------------------------------------//
    CU::Exit();

    TermDisplay();
}

/*******************************************************************************************************************
system functions
*******************************************************************************************************************/
bool Controller_Android::IsReady()
{
    if (has_focus_)
        return true;
    return false;
}

void Controller_Android::TermDisplay()
{
    gl_context_->Suspend();
}

void Controller_Android::TrimMemory()
{
    gl_context_->Invalidate();
}

/*******************************************************************************************************************
Get/Set functions
*******************************************************************************************************************/
const android_app* Controller_Android::GetApp()
{
    return app;
}