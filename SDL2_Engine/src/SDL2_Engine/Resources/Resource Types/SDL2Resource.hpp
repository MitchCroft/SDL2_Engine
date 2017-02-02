#pragma once

#include "ResourceBase.hpp"

namespace SDL2_Engine {
	namespace Resources {
		/*
		 *		Name: SDL2Resource (Default)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Setup the default generic SDL2Resource to force a 
		 *		compiler error. The proper resource specifications can 
		 *		be created from this.
		**/
		template<typename T>
		class SDL2Resource : public ResourceBase { 
			static_assert(false, "Can not create a ResourceBase object that " \
								 "is not based on the prototypes listed in " \
								 "the ResourceTypePrototypes.hpp file"); 
		};
	}
}