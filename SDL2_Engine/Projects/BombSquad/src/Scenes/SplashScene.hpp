#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

//! Prototype the UILabel object
namespace SDL2_Engine { namespace UI { namespace UIElements { class UILabel; } } }

namespace BombSquad {
	/*
	 *		Name: SplashScene
	 *		Author: Mitchell Croft
	 *		Created: 02/11/2017
	 *		Modified: 02/11/2017
	 *		
	 *		Purpose:
	 *		Provide an initial informational display on the game
	 *		window as the application starts
	**/
	class SplashScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			SplashScene : Constructor - Initialise with default values
			Created: 02/11/2017
			Modified: 02/11/2017
		*/
		SplashScene();

		/*
			SplashScene : createScene - Load and setup the Scene for rendering
			Created: 02/11/2017
			Modified: 02/11/2017

			return bool - Returns true if the Scene was created successfully
		*/
		bool createScene() override;

		/*
			SplashScene : destroyScene - Function to facilitate the deallocation of resources for a Scene
			Created: 02/11/2017
			Modified: 02/11/2017
		*/
		void destroyScene() override;

		/*
			SplashScene : update - Function to facilitate the updating of values once per cycle
			Created: 02/11/2017
			Modified: 02/11/2017
		*/
		void update() override;

	private:
		//! Store the font used to render the introduction text
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mOpeningFont;

		//! Store a pointer to the UI Label
		SDL2_Engine::UI::UIElements::UILabel** mLabels;
		size_t mLabelCount;

		//! Store a timer to display the screen for
		float mTimer;
	};
}