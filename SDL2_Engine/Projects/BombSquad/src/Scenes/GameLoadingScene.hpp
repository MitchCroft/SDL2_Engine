#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

//! Include the thread objects
#include <thread>
#include <atomic>

namespace BombSquad {
	/*
	 *		Name: GameLoadingScene
	 *		Author: Mitchell Croft
	 *		Created: 12/11/2017
	 *		Modified: 13/11/2017
	 *		
	 *		Purpose:
	 *		Provide a visual indication of the behind the scenes process
	**/
	class GameLoadingScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			GameLoadingScene : Constructor - Initialise with default values
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		GameLoadingScene();

		/*
			GameLoadingScene : createScene - Load the values required for the Scene to operate
			Created: 12/11/2017
			Modified: 13/11/2017

			return bool - Returns true if the Scene was initialised successfully
		*/
		bool createScene() override;

		/*
			GameLoadingScene : destroyScene - Destroy the values used to setup the Scene
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		void destroyScene() override;

		/*
			GameLoadingScene : update - Check for loading completion
			Created: 12/11/2017
			Modified: 13/11/2017
		*/
		void update() override;

		/*
			GameLoadingScene : render - Render the Scene to the Window
			Created: 12/11/2017
			Modified: 12/11/2017
		*/
		void render() override;

	private:
		//! Flag if the Game Manager should be destroyed with the scene
		bool mDestroyGM;

		//! Store the font used to display the loading message
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mMsgFont;

		//! Store the thread being used to load the map
		std::thread mWorker;

		//! Flag when the loading operation has been completed
		std::atomic<bool> mLoadedFlag;
	};
}