#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

//! Prototype the SDL_Cursor type
struct SDL_Cursor;

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (Cursor)
		 *		Author: Mitchell Croft
		 *		Created: 05/10/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on Objx files that
		 *		describe (animated) mouse cursors
		 *		
		 *		Notes:
		 *		The Cursor resource is loaded through an Objx file (https://github.com/MitchCroft/ObjxTextParser)
		 *		which is then parsed by the specialised Local Resource. These files can describe the following 
		 *		information:
		 *		
		 *		source		(string)	 - The filepath relative to the Objx file that contains the image to be used as a spritesheet
		 *		frameWidth	(integer)	 - The width (in pixels) of each frame of the animation (must be > 0)
		 *		frameHeight (integer)	 - The height (in pixels) of each frame of the animation (must be > 0)
		 *		fps			(decimal)	 - The number of frames per second to playback the animation at (must be >= 0)
		 *		frames		(Objx array) - An array of Objx which dictate the source x, y, w (width) and h (height)
		 *								   pixel values for the individual frames in the animation (Omit these to use entire image)
		 *		hotX		(decimal)	 - A 0-1 scale that dictates the active 'hot zone' of the cursor on the X axis (for clicking)
		 *		hotY		(decimal)	 - A 0-1 scale that dictates the active 'hot zone' of the cursor on the Y axis (for clicking)
		 *		
		 *		Example:
		 *		
		 *		cursor = {
		 *			source = "example.png"
		 *			
		 *			frameWidth = 32
		 *			frameHeight = 32
		 *			
		 *			fps = 29.97
		 *			
		 *			hotX = 0.5
		 *			hotY = 0.5
		 *			
		 *			frames = [{
		 *				x = 0
		 *				y = 0
		 *				w = 64
		 *				h = 64
		 *			}, {
		 *				x = 64
		 *				y = 0
		 *				w = 64
		 *				h = 64
		 *			}]
		 *		}
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<Cursor> : public LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				LocalResource (Cursor) : getFrame - Retrieve a specified animation frame
				Created: 05/10/2017
				Modified: 06/10/2017

				param[in] pFrame - The index of the frame to retrieve

				return SDL_Cursor* - Returns a SDL_Cursor pointer of nullptr if pFrame is out of range
			*/
			inline SDL_Cursor* getFrame(const size_t& pFrame) const { return (pFrame < mFrameCount ? mFrames[pFrame] : nullptr); }

			/*
				LocalResource (Cursor) : frameCount - Get the number of frames that are contained in the animation
				Created: 05/10/2017
				Modified: 05/10/2017

				return const size_t& - Returns a constant reference to the count value
			*/			
			inline const size_t& frameCount() const { return mFrameCount; }

			/*
				LocalResource (Cursor) : fps - Get the frames per second for the cursors playback
				Created: 05/10/2017
				Modified: 05/10/2017

				return const float& - Returns a constant reference to the loaded frames per second value
			*/
			inline const float& fps() const { return mFPS; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Cursor) : Destructor - Deallocate memory
				Created: 05/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (Cursor) : dispose - Unload resource information
				Created: 05/10/2017
				Modified: 05/10/2017
			*/
			void dispose() override;

		private:
			//! Assign as a friend of the Resources Manager
			friend class Resources;

			//! Store an array of the cursor frames that make up animation
			SDL_Cursor** mFrames;

			//! Store the number of frames in the array
			size_t mFrameCount;

			//! Store the animation playback speed
			float mFPS;

			/*
				LocalResource (Cursor) : Constructor - Initialise with default values
				Created: 05/10/2017
				Modified: 05/10/2017

				param[in] pPath - The path of the Objx file to load
			*/
			__LocalResource(const char* pPath);
		};
	}
}