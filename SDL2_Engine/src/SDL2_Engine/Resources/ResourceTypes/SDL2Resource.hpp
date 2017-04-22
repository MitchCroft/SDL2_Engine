#pragma once

#include <memory>

#include "ResourceBase.hpp"

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
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
			class __SDL2Resource : public ResourceBase {};
		}

		//! Define a simple shared pointer alias for resource types
		template<class T>
		using SDL2Resource = std::shared_ptr<ResourceTypes::__SDL2Resource<T>>;
	}
}