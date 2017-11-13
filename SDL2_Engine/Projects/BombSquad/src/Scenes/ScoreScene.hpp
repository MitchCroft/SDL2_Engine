#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

//! Include the required Game Objects
#include "../Game/GameObjects/Player.hpp"

namespace BombSquad {
	/*
	 *		Name: ScoreScene
	 *		Author: Mitchell Croft
	 *		Created: 13/11/2017
	 *		Modified: 13/11/2017
	 *		
	 *		Purpose:
	 *		Display the player scores after a round
	**/
	class ScoreScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			ScoreScene : Constructor - Initialise with default values
			Created: 13/11/2017
			Modified: 13/11/2017
		*/
		ScoreScene();

		/*
			ScoreScene : createScene - Load the values required for the Scene to operate
			Created: 13/11/2017
			Modified: 13/11/2017

			return bool - Returns true if the Scene was initialised successfully
		*/
		bool createScene() override;

		/*
			ScoreScene : destroyScene - Destroy the values used to setup the Scene
			Created: 13/11/2017
			Modified: 13/11/2017
		*/
		void destroyScene() override;

		/*
			ScoreScene : update - Update the internal values corresponding to player actions
			Created: 13/11/2017
			Modified: 13/11/2017
		*/
		void update() override;

		/*
			ScoreScene : render - Render the Scene to the Window 
			Created: 13/11/2017
			Modified: 13/11/2017
		*/
		void render() override;

	private:
		//! Flag if the Game Manager should be destroyed on quit
		bool mDestroyGM;

		//! Store a copy of the player objects from the game
		std::vector<Player> mPlayers;

		//! Store the image used for the background panel
		SDL2_Engine::LocalResource<SDL2_Engine::Texture> mBackground;

		//! Store the fonts used to display information
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mScoreFont;
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mActionFont;
	};
}