#pragma once
#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Include the Color object
#include "../../Rendering/Color.hpp"

//! Prototype the SDL font
struct _TTF_Font;

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Resources singleton
		class ResourceManager;

		namespace ResourceTypes {
			/*
			 *		Name: SDL2Resource (Font)
			 *		Author: Mitchel Croft
			 *		Created: 08/03/2017
			 *		Modified: 15/03/2017
			 *		
			 *		Purpose:
			 *		Store and manage an SDL true type font
			 *		object, and allow for the rendering of text
			**/
			template<>
			class __SDL2Resource<Font> : public ResourceBase {
				//! Assign as a friend of the Resource Manager
				friend class ResourceManager;

				//! Store a pointer to the True Type Font object
				_TTF_Font* mFont;

				//! Constructor 
				inline __SDL2Resource(const resourceID& pID, const unsigned int& pSize, const Rendering::Color& pColor = Rendering::Color::White, const long& pIndex = 0) :
					ResourceBase(pID, EResourceType::Font),
					mFont(nullptr), size(pSize), index(pIndex) {}

			public:
				//! Store the size of the font
				const unsigned int size;

				//! Store the type of font 
				const long& index;

				//! Override clear memory function
				void freeMemory() override;

				//! Render a string to a SDL2Resource object
				SDL2Resource<FontString> renderString(const char* pString, const bool& pTransparency = false);

				//! Destructor is public to allow for destruction
				inline ~__SDL2Resource() override = default;
			};
		}
	}
}