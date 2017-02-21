#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

//! Prototype the SDL_Cursor object
struct SDL_Cursor;

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Resources singleton
		class Resources;

		namespace ResourceTypes {

			//! Prototype the Cursor descriptor
			namespace Descriptors { struct Cursor; }

			/*
			 *		Name: SDL2Resource (Cursor)
			 *		Author: Mitchell Croft
			 *		Created: 02/02/2017
			 *		Modified: 03/02/2017
			 *
			 *		Purpose:
			 *		Store cursor data read from an external file
			 *
			 *		Notes:
			 *		The Cursor resource is loaded through a JSON file which is then parsed
			 *		by the Resource Manager. These files should describe the following
			 *		information:
			 *
			 *		src - The filepath relative to this JSON file that contains the image to be used as a spritesheet
			 *		frameWidth - The width (in pixels) of each frame of the animation (Must be > 0)
			 *		frameHeight - The height (in pixels) of each frame of the animation (Must be > 0)
			 *		fps - The number of frames per second to playback the animation at (double)
			 *		frames - An array of simple objects which dictate the the x and y pixel coordinates
			 *				 as well as the source width and height of each individual frame
			 *
			 *		Example:
			 *		{
			 *			"src":"example.png",
			 *			"frameWidth":32,
			 *			"frameHeight":32,
			 *			"fps":29.97,
			 *			"frames": [ { "x":0, "y":0, "w":32, "h":32 }, { "x":32, "y":0, "w":32, "h":32 } ]
			 *		}
			**/
			template<>
			class __SDL2Resource<Cursor> : public ResourceBase {
				//! Assign as a friend of the Resource Manager
				friend class Resources;

				//! Store a pointer to the Cursor_Container
				Descriptors::Cursor* mCursor;

				//! Construct / Destructor
				inline __SDL2Resource(const resourceID& pID, const float& pHotX,
					const float& pHotY, const float& pScale = 1.f) :
					ResourceBase(pID, EResourceType::Cursor),
					mCursor(nullptr), xHot(pHotX), yHot(pHotY),
					scale(pScale < 0.f ? 0.f : pScale) {}

			public:
				//! Store a size scale value to be applied to the Cursor size
				const float scale;

				//! Store the 'hot' zone percentage values (0 - 1)
				const float xHot;
				const float yHot;

				//! Update the current animation frame over time
				void update(const float& pDelta);

				//! Get the current cursor frame
				const SDL_Cursor* getCurrentFrame() const;

				//! Override clear memory operation
				void freeMemory() override;

				//! Destructor is public to allow for resource destruction
				inline ~__SDL2Resource() override = default;
			};
		}
	}
}