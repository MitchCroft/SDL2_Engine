#pragma once

#include <string>

//! Include all Resource objects
#include "ResourceTypes/SDL2TextureResource.hpp"
#include "ResourceTypes/SDL2SFXResource.hpp"
#include "ResourceTypes/SDL2MusicResource.hpp"
#include "ResourceTypes/SDL2CursorResource.hpp"
//TODO: Add Font 
#include "ResourceTypes/SDL2GenericResource.hpp"

/*
 *		Name: resourceToString
 *		Author: Mitchell Croft
 *		Created: 03/02/2017
 *		Modified: 03/02/2017
 *		
 *		Purpose:
 *		Provide a number of specialised functions to convert the SDL2Resource 
 *		objects into std::string labels
**/

namespace SDL2_Engine {
	namespace Resources {
			//! Default
			template<typename T> inline std::string resourceToString() { static_assert(false, "Can not convert a non SDL2Resource item to a std::string"); }

			//! Texture
			template<> inline std::string resourceToString<Texture>() { return "TEXTURE"; }

			//! SFX
			template<> inline std::string resourceToString<SFX>() { return "SFX"; }

			//! Music
			template<> inline std::string resourceToString<Music>() { return "MUSIC"; }

			//! Cursor
			template<> inline std::string resourceToString<Cursor>() { return "CURSOR"; }

			//! Font
			//template<> inline std::string resourceToString<Font>() { return "FONT"; }
			
			//! Generic
			template<> inline std::string resourceToString<Generic>() { return "GENERIC"; }
	}
}