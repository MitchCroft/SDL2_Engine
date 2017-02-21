#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the SDL_Texture object
struct SDL_Texture;

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Resources singleton
		class Resources;

		namespace ResourceTypes {
			/*
			 *		Name: SDL2Resource (Texture)
			 *		Author: Mitchell Croft
			 *		Created: 02/02/2017
			 *		Modified: 03/02/2017
			 *
			 *		Purpose:
			 *		Store the image data read from an external file
			**/
			template<>
			class __SDL2Resource<Texture> : public ResourceBase {
				//! Assign as a friend of the Resource Manager
				friend class Resources;

				//! Store a pointer to the SDL_Texture object
				SDL_Texture* mTexture;

				//! Constructor 
				inline __SDL2Resource(const resourceID& pID, const int& pBlend = 0) :
					ResourceBase(pID, EResourceType::Texture),
					mTexture(nullptr), blendMode(pBlend),
					texture(mTexture) {}

			public:
				//! Store the Blend Mode to apply to the Texture
				const int blendMode;

				//! Provide property access to the SDL_Texture
				Utilities::Properties::ReadOnlyProperty<const SDL_Texture*> texture;

				//!! Override clear memory operation
				void freeMemory() override;

				//! Destructor is public to allow for resource destruction
				inline ~__SDL2Resource() override = default;
			};
		}
	}
}