#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

//! Prototype the SDL Mix_Chunk object
struct Mix_Chunk;

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (SFX)
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 05/10/2017
		 *		
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on Mix_Chunk objects
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<SFX> : public LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store a pointer to the created sound effect
			Mix_Chunk* sfx;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (SFX) : Destructor - Deallocate memory
				Created: 04/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (SFX) : dispose - Unload SDL2 resource information
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			void dispose() override;

		private:

			//! Assign as a friend of the Resources Manager
			friend class Resources;

			/*
				LocalResource (SFX) : Constructor - Initialise with default values
				Created: 04/10/2017
				Modified: 04/10/2017

				param[in] pPath - The path of the sound effect file to load
			*/
			__LocalResource(const char* pPath);
		};
	}
}