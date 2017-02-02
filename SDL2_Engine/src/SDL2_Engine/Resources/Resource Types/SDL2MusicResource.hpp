#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the Mix_Music object
struct _Mix_Music;

namespace SDL2_Engine {
	namespace Resources {
		/*
		 *		Name: SDL2Resource (Music)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Store music data read from an external file
		**/
		template<>
		class SDL2Resource<Music> : public ResourceBase {
			//! Assign as a friend of the Resource Manager
			friend class Resources;

			//! Store a pointer to the Mix_Music object
			_Mix_Music* mMusic;

			//! Constructor / Destructor
			inline SDL2Resource(const resourceID& pID) :
				ResourceBase(pID, EResourceType::Music),
				mMusic(nullptr), music(mMusic)
			{}
			inline ~SDL2Resource() override = default;

		public:
			//! Provide property access to the Mix_Music
			Utilities::Properties::ReadOnlyProperty<const _Mix_Music*> music;

			//! Override clear memory operation
			void freeMemory() override;
		};
	}
}