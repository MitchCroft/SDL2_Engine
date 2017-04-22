#pragma once

//! Include the basic types
#include "../_Root/Types.hpp"

//! Prototype the SDL Renderer
struct SDL_Renderer;

namespace SDL2_Engine {
	namespace Rendering {
		//! Prototype the RenderSet object
		struct RenderSet;

		/*
		 *		Name: ScreenRenderer
		 *		Author: Mitchell Croft
		 *		Created: 07/03/2017
		 *		Modified: 07/03/2017
		 *		
		 *		Purpose:
		 *		Provide default screen rendering and allow for 
		 *		custom extensions of the ScreenRenderer to allow 
		 *		for visual effects
		**/
		class ScreenRenderer {
		public:
			//! Store a simple identifier value
			const int ID;

			//! Constructor / Destructor
			inline ScreenRenderer(const int& pID = 0) : ID(pID) {}
			virtual ~ScreenRenderer() = default;

			//! Receive RenderSet's from the Renderer in order to display images
			virtual int processRenderSets(/* TODO: Pass in Renderer */, const RenderSet* pSets, const uint& pCount);
		};
	}
}