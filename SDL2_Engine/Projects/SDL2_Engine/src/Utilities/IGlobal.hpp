#pragma once

//! Include the library management definitions
#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace Utilities {
		/*
		 *		Name: IGlobal
		 *		Author: Mitchell Croft
		 *		Created: 19/07/2017
		 *		Modified: 22/09/2017
		 *
		 *		Purpose:
		 *		Abstract Interface for instantiating and destroying
		 *		global instances of management objects
		**/
		class SDL2_LIB_INC IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				IGlobal : Destructor - Default virtual destructor
				Created: 19/07/2017
				Modified: 22/09/2017
			*/
			virtual ~IGlobal() {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Update Function-----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				IGlobal : update - Execute the update function stored for the Singleton interface
				Created: 19/07/2017
				Modified: 19/07/2017
			*/
			virtual void update() = 0;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Interface Functions---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				IGlobal : createInterface - Virtual method for creating the interface object for use
				Created: 19/07/2017
				Modified: 19/07/2017

				return bool - Function should return true if the interface was created successfully
			*/
			virtual bool createInterface() = 0;

			/*
				IGlobal : destroyInterface - Virtual method for clearing up allocated memory in an interface
				Created: 19/07/2017
				Modified: 19/07/2017
			*/
			virtual void destroyInterface() = 0;
		};
	}
}