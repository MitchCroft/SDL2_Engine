#pragma once

#include <vector>

//! Prototype the SDL_Cursor type
struct SDL_Cursor;

namespace SDL2_Engine {
	namespace Resources {
		namespace Custom_Resources {
			/*
			 *		Name: Cursor_Container
			 *		Author: Mitchell Croft
			 *		Created: 02/02/2017
			 *		Modified: 02/02/2017
			 *		
			 *		Purpose:
			 *		Store the information relevant to an animated
			 *		cursor through SDL
			**/
			struct Cursor_Container {
				//! Store the current progress through the animation
				unsigned int currentFrame;

				//! Store the total number frames in the cursor
				unsigned int totalFrames;

				//! Store the playback speed of the animation
				float fps;

				//! Store a timer for tracking animation progress
				float animationTimer;

				//! Store a vector of the cursors that make up the animation
				std::vector<SDL_Cursor*> frameList;

				//! Constructor
				inline Cursor_Container() : currentFrame(0), totalFrames(0), fps(0.f), animationTimer(0.f) {}
			};
		}
	}
}