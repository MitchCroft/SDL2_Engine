#pragma once
#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the SDL_Surface
struct SDL_Surface;

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
			/*
			 *		Name: SDL2Resource (FontString)
			 *		Author: Mitchell Croft
			 *		Created: 08/03/2017
			 *		Modified: 08/03/2017
			 *		
			 *		Purpose:
			 *		Manage a surface created by rendering text via 
			 *		a SDL2Resource<Font> object
			**/
			template<>
			class __SDL2Resource<FontString> : public ResourceBase {
				//! Assign as a friend of the Font resource
				friend class __SDL2Resource<Font>;

				//! Store a pointer to the surface
				SDL_Surface* mText;

				//! Constructor
				inline __SDL2Resource() : 
					ResourceBase(0, EResourceType::FontString) ,
					mText(nullptr), text(mText)
				{}

			public:
				//! Provide access to the text surface
				Utilities::Properties::ReadOnlyProperty<const SDL_Surface*> text;

				//! Override clear memory function
				void freeMemory() override;

				//! Destructor is public to allow for destruction
				inline ~__SDL2Resource() override = default;
			};
		}
	}
}