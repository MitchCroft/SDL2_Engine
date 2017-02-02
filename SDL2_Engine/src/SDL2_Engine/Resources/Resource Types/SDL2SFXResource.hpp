#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the Mix_Chunk object
struct Mix_Chunk;

namespace SDL2_Engine {
	namespace Resources {
		/*
		 *		Name: SDL2Resource (SFX)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Store sound effect data read from an external file
		**/
		template<>
		class SDL2Resource<SFX> : public ResourceBase {
			//! Assign as a friend of the Resource Manager
			friend class Resources;

			//! Store a pointer to the Mix_Chunk object
			Mix_Chunk* mSFX;

			//! Constructor / Destructor
			inline SDL2Resource(const resourceID& pID) :
				ResourceBase(pID, EResourceType::SFX),
				mSFX(nullptr), sfx(mSFX)
			{}
			inline ~SDL2Resource() override = default;

		public:
			//! Provide property access to the Mix_Chunk
			Utilities::Properties::ReadOnlyProperty<const Mix_Chunk*> sfx;

			//! Override clear memory operation
			void freeMemory() override;
		};
	}
}