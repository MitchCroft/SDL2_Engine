#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

namespace BombSquad {
	/*
	 *		Name: GameScene 
	 *		Author: Mitchell Croft
	 *		Created: 11/11/2017
	 *		Modified: 11/11/2017
	 *		
	 *		Purpose:
	 *		Render the game elements to the window for the player(s) to see
	**/
	class GameScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			GameScene : Constructor - Initialise with default values
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		GameScene();

		/*
			GameScene : createScene - Load the values required for the Scene to operate
			Created: 12/11/2017
			Modified: 12/11/2017

			return bool - Returns true if the Scene was initialised successfully
		*/
		bool createScene() override;

		/*
			GameScene : destroyScene - Destroy the values used to setup the Scene
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		void destroyScene() override;

		/*
			GameScene : update - Check for game completion
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		void update() override;

		/*
			GameScene : render - Render the Scene to the Window
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		void render() override;

	private:
		//! Flag if the Game Manager should be destroyed with the scene
		bool mDestroyGM;

		//! Store a timer to delay progression to score scene
		float mEndTimer;

		//! Store the background for the game
		SDL2_Engine::LocalResource<SDL2_Engine::Texture> mBackground;

		//! Store the spritesheet used for the game
		SDL2_Engine::LocalResource<SDL2_Engine::Texture> mSpritesheet;
	};
}