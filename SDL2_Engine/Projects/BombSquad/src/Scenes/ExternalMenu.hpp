#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>
#include <Utilities/FString.hpp>

namespace BombSquad {
	/*
	 *		Name: ExternalMenu
	 *		Author: Mitchell Croft
	 *		Created: 02/11/2017
	 *		Modified: 02/11/2017
	 *		
	 *		Purpose:
	 *		Load a UI based menu from the external 'MenuDesc.objx' file
	**/
	class ExternalMenu : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			ExternalMenu : Constructor - Initialise with default values
			Created: 02/11/2017
			Modified: 02/11/2017

			param[in] pName - The name of the object within 'MenuDesc.objx' file to load UI elements from
		*/
		ExternalMenu(const char* pName);

		/*
			ExternalMenu : createScene - Load and setup the Scene for rendering
			Created: 02/11/2017
			Modified: 02/11/2017

			return bool - Returns true if the Scene was created successfully
		*/
		bool createScene() override;

		/*
			ExternalMenu : destroyScene - Function to facilitate the deallocation of resources for a Scene
			Created: 02/11/2017
			Modified: 02/11/2017
		*/
		inline void destroyScene() override {}

	private:
		//! Store the name of the UI element to load from the file
		SDL2_Engine::Utilities::FString<128> mLoadName;

		//! Store the font used to display the version
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mVersionFont;
	};
}