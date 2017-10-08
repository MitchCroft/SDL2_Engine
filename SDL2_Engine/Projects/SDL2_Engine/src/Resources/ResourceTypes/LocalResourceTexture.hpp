#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

//! Prototype the required SDL types
struct SDL_Renderer;
struct SDL_Texture;

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (Texture)
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 05/10/2017
		 *
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on SDL2_Texture objects
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<Texture> : public LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store a pointer to the created SDL_Texture
			SDL_Texture* texture;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Texture) : Destructor - Deallocate memory
				Created: 04/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (Texture) : dispose - Unload SDL2 resource information
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			void dispose() override;

		private:

			//! Assign as a friend of the Resources Manager
			friend class Resources;

			/*
				LocalResource (Texture) : Constructor - Initialise with default values
				Created: 04/10/2017
				Modified: 04/10/2017

				param[in] pRenderer - The SDL2 renderer object that will be used to create the texture
				param[in] pPath - The path of the image file to load
				param[in] pBlendMode - The SDL_BlendMode value to be applied to the texture (Default none)
			*/
			__LocalResource(SDL_Renderer* pRenderer, const char* pPath, const int& pBlendMode = 0);
		};
	}
}