# Sets the minimum version of CMake required to build the native
# library. You should either keep the default value or only pass a
# value of 3.4.0 or lower.

cmake_minimum_required(VERSION 3.4.1)

# build native_app_glue as a static lib (Android includes it as source code only, must built into a .so lib)
add_library(app-glue STATIC
            ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c)
			
add_library(cpufeatures STATIC
            ${ANDROID_NDK}/sources/android/cpufeatures/cpu-features.c)
			
			
add_library( # Sets the name of the library.
             dependency-lib

             # Sets the library as a shared library.
             STATIC
			
			 src/main/cpp/Android_dep/Android_Dependencies.h
			 src/main/cpp/Android_dep/Android_Dependencies.cpp
			 )
			 
target_include_directories(dependency-lib PRIVATE
							${ANDROID_NDK}/sources/android/cpufeatures
                           ${ANDROID_NDK}/sources/android/native_app_glue
						   src/main/LearnOpenGL/LearnOpenGL/Source)
						   
target_link_libraries( # Specifies the target library.
                       dependency-lib			 
						app-glue
						cpufeatures
						android)			 
			 
#add RUNTIME dependency file here 
#file(src/main/LearnOpenGL/LearnOpenGL/Source/Android_source/Android_Dependencies.h)
SET(CMAKE_CXX_FLAGS "-include ../../../../src/main/cpp/Android_dep/Android_Dependencies.h")
SET(CMAKE_C_FLAGS "-include ../../../../src/main/cpp/Android_dep/Android_Dependencies.h")
#add_definitions(-include src/main/LearnOpenGL/LearnOpenGL/Source/Android_source/Android_Dependencies.h)
#ADD_DEFINITIONS (-Dfopen(fname, mode)=android_fopen(fname, mode))
#include_directories(SYSTEM src/main/LearnOpenGL/LearnOpenGL/Source/Android_dep/Android_Dependencies.h)	

#local CPP flags
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -CURR_PLAT_PC=0 -CURR_PLAT_ANDROID=1 -CURR_PLAT_IOS=0 -CURR_PLAT_WEB=0")

#call after native-lib since there are some system functions overwrite
# build the ndk-helper library--------------------------------------------------------------------//
set(ndk_helper_dir libs/ndk_helper)	#start location is location of CMakeLists.txt
add_subdirectory(${ndk_helper_dir} ndk_helper) #Set ndk_helper_dir as a variable?

# build the libpng library--------------------------------------------------------------------//
set(libpng_dir libs/libpng)	#start location is location of CMakeLists.txt
add_subdirectory(${libpng_dir} libpng) #Set ndk_helper_dir as a variable?

# build the freetype library--------------------------------------------------------------------//
set(freetype_dir libs/freetype)	#start location is location of CMakeLists.txt
add_subdirectory(${freetype_dir} freetype-headers) #Set ndk_helper_dir as a variable?		 
			 
			 
			 
			 
# build the native library--------------------------------------------------------------------//			 
ADD_DEFINITIONS (-DANDROID_VERSION)	
ADD_DEFINITIONS (-DCURR_PLAT_PC=0)	
ADD_DEFINITIONS (-DCURR_PLAT_ANDROID=1)
ADD_DEFINITIONS (-DCURR_PLAT_IOS=0)
ADD_DEFINITIONS (-DCURR_PLAT_WEB=0)

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

			#platform-independent files
			../../../Base/Source/AssetManager.cpp
			../../../Base/Source/AssetManager.h
			../../../Base/Source/Camera.cpp
			../../../Base/Source/Camera.h
			../../../Base/Source/Controller.cpp
			../../../Base/Source/Controller.h
			../../../Base/Source/FPS_Camera.cpp
			../../../Base/Source/FPS_Camera.h
			../../../Base/Source/GridMesh.cpp
			../../../Base/Source/GridMesh.h
			../../../Base/Source/Input.cpp
			../../../Base/Source/Input.h
			../../../Base/Source/libpng_demo.cpp
			../../../Base/Source/libpng_demo.h
			../../../Base/Source/Mesh.cpp
			../../../Base/Source/Mesh.h
			../../../Base/Source/Scene.cpp
			../../../Base/Source/Scene.h
			../../../Base/Source/shader.hpp
			../../../Base/Source/TextRenderer.cpp
			../../../Base/Source/TextRenderer.h
			../../../Base/Source/TextureLoader.cpp
			../../../Base/Source/TextureLoader.h
			../../../Base/Source/Transform.cpp
			../../../Base/Source/Transform.h
			../../../Base/Source/UsefulUtils.cpp
			../../../Base/Source/UsefulUtils.h
			../../../Base/Source/Vertex.cpp
			../../../Base/Source/Vertex.h
			../../../Base/Source/View.cpp
			../../../Base/Source/View.h
			
			../../../Base/Source/Collision/GJK_Simplex3D.cpp
			../../../Base/Source/Collision/GJK_Simplex3D.h
			../../../Base/Source/Collision/Polygon_3D.cpp
			../../../Base/Source/Collision/Polygon_3D.h
			../../../Base/Source/Collision/Scene_Collision.cpp
			../../../Base/Source/Collision/Scene_Collision.h
			
			../../../Base/Source/Shared/CoreUtilities.cpp
			../../../Base/Source/Shared/CoreUtilities.h
			../../../Base/Source/Shared/Header.cpp
			../../../Base/Source/Shared/Header.h
			
			../../../Base/Source/Voxel/Chunk.cpp
			../../../Base/Source/Voxel/Chunk.h
			../../../Base/Source/Voxel/Scene_Voxel.cpp
			../../../Base/Source/Voxel/Scene_Voxel.h

			../../../Base/Source/2D/Broadphrase.cpp
			../../../Base/Source/2D/NavMesh.cpp
			../../../Base/Source/2D/NavMesh.h
			../../../Base/Source/2D/NPC.cpp
			../../../Base/Source/2D/NPC.h
			../../../Base/Source/2D/Pathfinding.cpp
			../../../Base/Source/2D/Pathfinding.h
			../../../Base/Source/2D/PathFollower.cpp
			../../../Base/Source/2D/PathFollower.h
			../../../Base/Source/2D/Scene_Pathfinding.cpp
			../../../Base/Source/2D/Scene_Pathfinding.h
			../../../Base/Source/2D/SpriteRenderer.cpp
			../../../Base/Source/2D/SpriteRenderer.h
			../../../Base/Source/2D/TileMap.cpp
			../../../Base/Source/2D/TileMap.h

			#common
			../../../Base/Libraries/Common/MathUtility.cpp
			../../../Base/Libraries/Common/MathUtility.h
			../../../Base/Libraries/Common/MatrixStack.cpp
			../../../Base/Libraries/Common/MatrixStack.h
			../../../Base/Libraries/Common/Mtx44.cpp
			../../../Base/Libraries/Common/Mtx44.h
			../../../Base/Libraries/Common/MyMath.h
			../../../Base/Libraries/Common/Vector2.cpp
			../../../Base/Libraries/Common/Vector2.h
			../../../Base/Libraries/Common/Vector3.cpp
			../../../Base/Libraries/Common/Vector3.h
             
			#Android-dependent files
			src/main/cpp/AndroidUtils.cpp
			src/main/cpp/AndroidUtils.h
			src/main/cpp/ControllerAndroid.cpp
			src/main/cpp/ControllerAndroid.h
			src/main/cpp/Input_Android.cpp
			src/main/cpp/Input_Android.h
			src/main/cpp/ShaderAndroid.cpp
			src/main/cpp/Main.cpp
			 )
			 
#set_target_properties(native-lib PROPERTIES COMPILE_DEFINITIONS "-DCURR_PLAT_PC=0")
#set_target_properties(native-lib PROPERTIES COMPILE_DEFINITIONS "-DCURR_PLAT_ANDROID=1") 
			 
#add pre-processor defines--------------------------------------------------------------------//
#target_compile_definitions(native-lib PRIVATE 
#							CURR_PLAT_PC 0
#							CURR_PLAT_ANDROID 1
#							CURR_PLAT_IOS 0
#							CURR_PLAT_WEB 0)
#set(CMAKE_CXX_FLAGS "${LOCAL_CXX_FLAGS} -DCURR_PLAT_PC=0")
#set(CMAKE_CXX_FLAGS "${LOCAL_CXX_FLAGS} -DCURR_PLAT_ANDROID=1")
#set_target_properties(native-lib PROPERTIES COMPILE_DEFINITIONS "CURR_PLAT_PC 0")
#set_target_properties(native-lib PROPERTIES COMPILE_DEFINITIONS "DCURR_PLAT_ANDROID 1")
			 
# Searches for a specified prebuilt library and stores the path as a
# variable. Because system libraries are included in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
			  
#as the name implies
target_include_directories(native-lib PRIVATE
                           ${ANDROID_NDK}/sources/android/cpufeatures
                           ${ANDROID_NDK}/sources/android/native_app_glue
						   src/main/cpp
						   ../../../Base/Source
						   ../../../Base/Source/Shared
						   ../../../Base/Libraries/Common
						   ${ndk_helper_dir}
						   ${libpng_dir}
						   ${freetype_dir}
						   )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in the
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib

                       # Links the target library to the log library
                       # included in the NDK.
					   android
                       cpufeatures
                       app-glue
					   atomic
                       EGL
                       GLESv3
					   ndk-helper
					   libpng
					   freetype-headers
					   dependency-lib
                       ${log-lib} )#log-lib is special, others no need ${}
