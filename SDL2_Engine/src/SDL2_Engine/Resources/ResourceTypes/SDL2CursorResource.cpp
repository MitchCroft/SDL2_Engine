#include "SDL2CursorResource.hpp"

//! Include the Cursor description object
#include "Descriptors/Cursor.hpp"

//! Get access to the SDL functions
#include <SDL.h>

namespace SDL2_Engine {
	namespace Resources {
		namespace ResourceTypes {
			/*
				__SDL2Resource (Cursor) : update - Update the animation of the current Cursor resource
				Author: Mitchell Croft
				Created: 02/02/2017
				Modified: 03/02/2017

				param[in] pDelta - The delta time to update this cursor with
			*/
			void __SDL2Resource<Cursor>::update(const float& pDelta) {
				//Check the resource has been loaded
				if (mStatus != EResourceLoadStatus::Loaded) return;

				//Check that there are animation frames
				else if (mCursor->fps <= 0.f) return;

				//Add the delta time to the delta time
				mCursor->animationTimer += pDelta;

				//Calculate the time per frame
				const float TIME_PER_FRAME = 1.f / mCursor->fps;

				//Store the number of frames consumed this update
				float usedFrames = 0.f;

				//Calculate the number of frames to advance
				mCursor->animationTimer = modf(mCursor->animationTimer, &usedFrames);

				//Advance the currently active frame
				mCursor->currentFrame = (mCursor->currentFrame + (unsigned int)usedFrames) % mCursor->totalFrames;
			}

			/*
				__SDL2Resource (Cursor) : getCurrentFrame - Get the currently active frame of the Cursor animation
				Author: Mitchell Croft
				Created: 02/02/2017
				Modified: 03/02/2017

				return const SDL_Cursor* - Returns a constant pointer to the current frame for this Cursor
			*/
			const SDL_Cursor* __SDL2Resource<Cursor>::getCurrentFrame() const {
				//Check the status of the resource
				if (mStatus != EResourceLoadStatus::Loaded) return nullptr;

				//Check the frame is valid
				return (mCursor->currentFrame < mCursor->totalFrames ? mCursor->frameList[mCursor->currentFrame] : nullptr);
			}

			/*
				__SDL2Resource (Cursor) : freeMemory - Clear the managed SDL_Cursor resources
				Author: Mitchell Croft
				Created: 02/02/2016
				Modified: 03/02/2016
			*/
			void __SDL2Resource<Cursor>::freeMemory() {
				//Check the cursor exists before removing it
				if (mCursor) {
					//Loop through the Cursor frames
					for (auto& frame : mCursor->frameList)
						SDL_FreeCursor(frame);

					//Delete the cursor object
					delete mCursor;

					//Reset the cursor pointer
					mCursor = nullptr;

					//Update the resource status
					mStatus = EResourceLoadStatus::Freed;
				}
			}
		}
	}
}