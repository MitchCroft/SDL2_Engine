#pragma once

//! Specify the method of inclusion used when handling the library
#ifdef _BUILD_SDL2_ENGINE_LIB_
		//! Remove previously defined values for the library include macro
		#ifdef SDL2_LIB_INC
			#undef SDL2_LIB_INC
		#endif
	#define SDL2_LIB_INC __declspec(dllexport)
#else
	#define SDL2_LIB_INC __declspec(dllimport)
#endif