#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

//! Prototype the SDL2 Mix_Music object
struct _Mix_Music;

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (Music)
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 05/10/2017
		 *		
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on Mix_Music objects
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<Music> : public LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store a pointer to the created sound effect
			_Mix_Music* music;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Music) : Destructor - Deallocate memory
				Created: 04/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (Music) : dispose - Unload SDL2 resource information
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			void dispose() override;

		private:

			//! Assign as a friend of the Resources Manager
			friend class Resources;

			/*
				LocalResource (Music) : Constructor - Initialise with default values
				Created: 04/10/2017
				Modified: 04/10/2017

				param[in] pPath - The path of the music file to load
			*/
			__LocalResource(const char* pPath);
		};
	}
}