#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

//! Prototype the SDL Font type
struct _TTF_Font;

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (Font)
		 *		Author: Mitchell Croft
		 *		Created: 05/10/2017
		 *		Modified: 05/10/2017
		 *		
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on TTF_Font objects
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<Font> : public ILocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store a pointer to the created font
			_TTF_Font* font;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Font) : Destructor - Deallocate memory
				Created: 05/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (Font) : dispose - Unload SDL2 resource information
				Created: 05/10/2017
				Modified: 05/10/2017
			*/
			void dispose() override;

		private:

			//! Assign as a friend of the Resources Manager
			friend class Resources;

			/*
				LocalResource (Font) : Constructor - Initialise with default values
				Created: 05/10/2017
				Modified: 05/10/2017

				param[in] pPath - The path of the font file to load
				param[in] pSize - The point size to use for the font
				param[in] pIndex - Optional index of the font face to load from the file (Default 0)
			*/
			__LocalResource(const char* pPath, const int& pSize, const long& pIndex = 0);
		};
	}
}