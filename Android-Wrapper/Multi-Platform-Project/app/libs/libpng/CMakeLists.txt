add_library(libpng STATIC 
            png.c
			png.h
			pngconf.h
			pngconf.c
			pngdebug.h
			pngdebug.c
			pngerror.c
			pngget.c
			pnginfo.h
			pnginfo.c
			pnglibconf.h
			pnglibconf.c
			pngmem.c
			pngpread.c
			pngpriv.h
			pngpriv.c
			pngread.c
			pngrio.c
			pngrtran.c
			pngrutil.c
			pngset.c
			pngstruct.h
			pngstruct.c
			pngtrans.c
			pngwio.c
			pngwrite.c
			pngwtran.c
			pngwutil.c
			arm/arm_init.c
			arm/filter_neon.S
			arm/filter_neon_intrinsics.c
			)

target_include_directories(libpng PRIVATE
                           ${ANDROID_NDK}/sources/android/native_app_glue)
						   
target_link_libraries( # Specifies the target library.
                       libpng

                       # Links the target library to the log library
                       # included in the NDK.
					   z) #zlib
