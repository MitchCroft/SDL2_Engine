#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the SDL_Cursor object
struct SDL_Cursor;

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Cursor_Container object
		namespace Custom_Resources { struct Cursor_Container; }

		/*
		 *		Name: SDL2Resource (Cursor)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Store cursor data read from an external file
		**/
		template<>
		class SDL2Resource<Cursor> : public ResourceBase {
			//! Assign as a friend of the Resource Manager
			friend class Resources;

			//! Store a pointer to the Cursor_Container
			Custom_Resources::Cursor_Container* mCursor;

			//! Construct / Destructor
			inline SDL2Resource(const resourceID& pID) :
				ResourceBase(pID, EResourceType::Cursor),
				mCursor(nullptr)
			{}

		public:
			//! Update the current animation frame over time
			void update(const float& pDelta);

			//! Get the current cursor frame
			const SDL_Cursor* getCurrentFrame() const;

			//! Override clear memory operation
			void freeMemory() override;
		};
	}
}