#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the SDL_Texture object
struct SDL_Texture;

namespace SDL2_Engine {
	namespace Resources {
		/*
		 *		Name: SDL2Resource (Texture)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Store the image data read from an external file
		**/
		template<>
		class SDL2Resource<Texture> : public ResourceBase {
			//! Assign as a friend of the Resource Manager
			friend class Resources;

			//! Store a pointer to the SDL_Texture object
			SDL_Texture* mTexture;

			//! Constructor / Destructor
			inline SDL2Resource(const resourceID& pID) :
				ResourceBase(pID, EResourceType::Texture),
				mTexture(nullptr), texture(mTexture)
			{}
			inline ~SDL2Resource() override = default;

		public:
			//! Provide property access to the SDL_Texture
			Utilities::Properties::ReadOnlyProperty<const SDL_Texture*> texture;

			//!! Override clear memory operation
			void freeMemory() override;
		};
	}
}