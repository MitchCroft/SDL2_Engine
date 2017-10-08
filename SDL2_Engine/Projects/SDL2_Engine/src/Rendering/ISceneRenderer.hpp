#pragma once

#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace Rendering {
		/*
		 *		Name: ISceneRenderer
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 09/10/2017
		 *		
		 *		Purpose:
		 *		Provide basic implementation interfaces for Scene Renderers to implement
		**/
		class SDL2_LIB_INC ISceneRenderer {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Rendering Functionality------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Construction/Destruction------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				ISceneRenderer : Destructor - Virtual function for inherited classes
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			inline virtual ~ISceneRenderer() {}

			/*
				ISceneRenderer : createRenderer - Setup the Scene Renderer
				Created: 09/10/2017
				Modified: 09/10/2017

				return bool - Returns true if the Scene Renderer was created successfully
			*/
			virtual bool createRenderer() = 0;

			/*
				ISceneRenderer : destroyRenderer - Free memory and resources for the Scene Renderer
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			virtual void destroyRenderer() = 0;
		};
	}
}