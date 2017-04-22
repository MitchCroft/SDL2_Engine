#pragma once

/*
 *		Name: ResourceTypePrototypes
 *		Author: Mitchell Croft
 *		Created: 02/02/2017
 *		Modified: 02/02/2017
 *		
 *		Purpose:
 *		Provide struct prototypes that can be fed to the Resource 
 *		Manager in order to describe the type of resource being loaded
**/
namespace SDL2_Engine {
	namespace Resources {
		//! Define a struct prototype for every possible resource
		struct Texture;
		struct SFX;
		struct Music;
		struct Cursor;
		struct Font;
		struct FontString;
		struct Generic;
	}
}