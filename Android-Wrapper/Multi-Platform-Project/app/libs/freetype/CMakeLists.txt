
#add the prebuild library-----------------------------------------------//
ADD_DEFINITIONS (-DFT2_BUILD_LIBRARY)

					   
#add the header files-----------------------------------------------//
add_library(freetype-headers STATIC 

		#src-----------------------------------//
		src/base/ftsystem.c
		src/base/ftdebug.c
		src/base/ftinit.c
		src/base/ftbbox.c
		src/base/ftbitmap.c
		src/base/ftcid.c
		src/base/ftadvanc.c
		src/base/ftcalc.c
		src/base/ftdbgmem.c
		src/base/ftgloadr.c
		src/base/ftobjs.c
		src/base/ftoutln.c
		src/base/ftrfork.c
		src/base/ftsnames.c
		src/base/ftstream.c
		src/base/fttrigon.c
		src/base/ftutil.c
		src/base/ftfstype.c
		src/base/ftgasp.c
		src/base/ftglyph.c
		src/base/ftgxval.c
		src/base/ftlcdfil.c
		src/base/ftmm.c
		src/base/ftotval.c
		src/base/ftpatent.c
		src/base/ftpfr.c
		src/base/ftstroke.c
		src/base/ftsynth.c
		src/base/fttype1.c
		src/base/ftwinfnt.c
		src/truetype/truetype.c
		src/cff/cff.c
		src/cid/type1cid.c
		src/pfr/pfr.c
		src/type42/type42.c
		src/winfonts/winfnt.c
		src/pcf/pcf.c
		src/sfnt/sfnt.c
		src/autofit/autofit.c
		src/pshinter/pshinter.c
		src/raster/raster.c
		src/smooth/smooth.c
		src/cache/ftcache.c
		src/gzip/ftgzip.c
		src/lzw/ftlzw.c
		src/bzip2/ftbzip2.c
		src/psnames/psmodule.c
				
		#main---------------------------------//
		ft2build.cpp
		ft2build.h
		
		#include/freetype/config--------------------//
		freetype/config/ftconfig.cpp
		freetype/config/ftconfig.h
		freetype/config/ftheader.cpp
		freetype/config/ftheader.h
		freetype/config/ftmodule.cpp
		freetype/config/ftmodule.h
		freetype/config/ftoption.cpp
		freetype/config/ftoption.h
		freetype/config/ftstdlib.cpp
		freetype/config/ftstdlib.h
		
		#include/freetype-------------------------//
		freetype/freetype.cpp
		freetype/freetype.h
		freetype/ftadvanc.cpp
		freetype/ftadvanc.h
		freetype/ftautoh.cpp
		freetype/ftautoh.h
		freetype/ftbbox.cpp
		freetype/ftbbox.h
		freetype/ftbdf.cpp
		freetype/ftbdf.h
		freetype/ftbitmap.cpp
		freetype/ftbitmap.h
		freetype/ftbzip2.cpp
		freetype/ftbzip2.h
		freetype/ftcache.cpp
		freetype/ftcache.h
		freetype/ftcffdrv.cpp
		freetype/ftcffdrv.h
		freetype/ftchapters.cpp
		freetype/ftchapters.h
		freetype/ftcid.cpp
		freetype/ftcid.h
		freetype/fterrdef.cpp
		freetype/fterrdef.h
		freetype/fterrors.cpp
		freetype/fterrors.h
		freetype/ftfntfmt.cpp
		freetype/ftfntfmt.h
		freetype/ftgasp.cpp
		freetype/ftgasp.h
		freetype/ftglyph.cpp
		freetype/ftglyph.h
		freetype/ftgxval.cpp
		freetype/ftgxval.h
		freetype/ftgzip.cpp
		freetype/ftgzip.h
		freetype/ftimage.cpp
		freetype/ftimage.h
		freetype/ftincrem.cpp
		freetype/ftincrem.h
		freetype/ftlcdfil.cpp
		freetype/ftlcdfil.h
		freetype/ftlist.cpp
		freetype/ftlist.h
		freetype/ftlzw.cpp
		freetype/ftlzw.h
		freetype/ftmac.cpp
		freetype/ftmac.h
		freetype/ftmm.cpp
		freetype/ftmm.h
		freetype/ftmodapi.cpp
		freetype/ftmodapi.h
		freetype/ftmoderr.cpp
		freetype/ftmoderr.h
		freetype/ftotval.cpp
		freetype/ftotval.h
		freetype/ftoutln.cpp
		freetype/ftoutln.h
		freetype/ftpfr.cpp
		freetype/ftpfr.h
		freetype/ftrender.cpp
		freetype/ftrender.h
		freetype/ftsizes.cpp
		freetype/ftsizes.h
		freetype/ftsnames.cpp
		freetype/ftsnames.h
		freetype/ftstroke.cpp
		freetype/ftstroke.h
		freetype/ftsynth.cpp
		freetype/ftsynth.h
		freetype/ftsystem.cpp
		freetype/ftsystem.h
		freetype/fttrigon.cpp
		freetype/fttrigon.h
		freetype/ftttdrv.cpp
		freetype/ftttdrv.h
		freetype/fttypes.cpp
		freetype/fttypes.h
		freetype/ftwinfnt.cpp
		freetype/ftwinfnt.h
		freetype/ftxf86.h
		freetype/t1tables.cpp
		freetype/t1tables.h
		freetype/ttnameid.cpp
		freetype/ttnameid.h
		freetype/tttables.cpp
		freetype/tttables.h
		freetype/tttags.cpp
		freetype/tttags.h
		freetype/ttunpat.cpp
		freetype/ttunpat.h

		#include/freetype/internal-----------------------//
		freetype/internal/autohint.cpp
		freetype/internal/autohint.h
		freetype/internal/ftcalc.cpp
		freetype/internal/ftcalc.h
		freetype/internal/ftdebug.cpp
		freetype/internal/ftdebug.h
		freetype/internal/ftdriver.cpp
		freetype/internal/ftdriver.h
		freetype/internal/ftgloadr.cpp
		freetype/internal/ftgloadr.h
		freetype/internal/fthash.cpp
		freetype/internal/fthash.h
		freetype/internal/ftmemory.cpp
		freetype/internal/ftmemory.h
		freetype/internal/ftobjs.cpp
		freetype/internal/ftobjs.h
		freetype/internal/ftpic.cpp
		freetype/internal/ftpic.h
		freetype/internal/ftrfork.cpp
		freetype/internal/ftrfork.h
		freetype/internal/ftserv.cpp
		freetype/internal/ftserv.h
		freetype/internal/ftstream.cpp
		freetype/internal/ftstream.h
		freetype/internal/fttrace.cpp
		freetype/internal/fttrace.h
		freetype/internal/ftvalid.cpp
		freetype/internal/ftvalid.h
		freetype/internal/internal.cpp
		freetype/internal/internal.h
		freetype/internal/pcftypes.cpp
		freetype/internal/pcftypes.h
		freetype/internal/psaux.cpp
		freetype/internal/psaux.h
		freetype/internal/pshints.cpp
		freetype/internal/pshints.h
		freetype/internal/sfnt.cpp
		freetype/internal/sfnt.h
		freetype/internal/t1types.cpp
		freetype/internal/t1types.h
		freetype/internal/tttypes.cpp
		freetype/internal/tttypes.h
		
		#include/freetype/internal/services-------------------------//
		freetype/internal/services/svbdf.cpp
		freetype/internal/services/svbdf.h
		freetype/internal/services/svcid.cpp
		freetype/internal/services/svcid.h
		freetype/internal/services/svfntfmt.cpp
		freetype/internal/services/svfntfmt.h
		freetype/internal/services/svgldict.cpp
		freetype/internal/services/svgldict.h
		freetype/internal/services/svgxval.cpp
		freetype/internal/services/svgxval.h
		freetype/internal/services/svkern.cpp
		freetype/internal/services/svkern.h
		freetype/internal/services/svmetric.cpp
		freetype/internal/services/svmetric.h
		freetype/internal/services/svmm.cpp
		freetype/internal/services/svmm.h
		freetype/internal/services/svotval.cpp
		freetype/internal/services/svotval.h
		freetype/internal/services/svpfr.cpp
		freetype/internal/services/svpfr.h
		freetype/internal/services/svpostnm.cpp
		freetype/internal/services/svpostnm.h
		freetype/internal/services/svprop.cpp
		freetype/internal/services/svprop.h
		freetype/internal/services/svpscmap.cpp
		freetype/internal/services/svpscmap.h
		freetype/internal/services/svpsinfo.cpp
		freetype/internal/services/svpsinfo.h
		freetype/internal/services/svsfnt.cpp
		freetype/internal/services/svsfnt.h
		freetype/internal/services/svttcmap.cpp
		freetype/internal/services/svttcmap.h
		freetype/internal/services/svtteng.cpp
		freetype/internal/services/svtteng.h
		freetype/internal/services/svttglyf.cpp
		freetype/internal/services/svttglyf.h
		freetype/internal/services/svwinfnt.cpp
		freetype/internal/services/svwinfnt.h
		freetype/internal/services/svxf86nm.cpp
		freetype/internal/services/svxf86nm.h

			)

set(freetype_dir ../freetype)
set(freetype2_dir ../freetype/freetype)
set(freetype_config_dir ../freetype/freetype/config)
set(freetype_internal_dir ../freetype/freetype/internal)
set(freetype_internal_services_dir ../freetype/freetype/internal/services)
			
#link/glue the headers into a lib--------------------------------------------------//
target_include_directories(freetype-headers PRIVATE
                           ${ANDROID_NDK}/sources/android/native_app_glue
						   ${freetype_dir}
						   ${freetype2_dir}
						   ${freetype_config_dir}
						   )
						   
#link the headers lib with the src lib-----------------------------------------------//
target_link_libraries( # Specifies the target library.
                       freetype-headers

                       # Links the target library to the log library
                       # included in the NDK.
					   #freetype-lib
					   #z
					   log
					   #m
					   ) #zlib
