//
// Created by Kenneth Tan on 29/1/2017.
//
#include "ControllerAndroid.h"
#include "Android_dep/Android_dependencies.h"

#define HELPER_CLASS_NAME \
  "apple/pie/multi_platform_project/helper/NDKHelper"  // Class name of helper function

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "native-activity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "native-activity", __VA_ARGS__))

void android_main( android_app* state )
{
        // Make sure glue isn't stripped.
        app_dummy();


        state->userData = Controller_Android::Instance();
        ndk_helper::JNIHelper::Init( state->activity, HELPER_CLASS_NAME );
        android_fopen_set_asset_manager(state->activity->assetManager);

        state->onAppCmd = Controller_Android::engine_handle_cmd;
        state->onInputEvent = Controller_Android::engine_handle_input;
	    Controller_Android::Instance()->PreInit(state);


        Controller_Android::Instance()->Run();
}