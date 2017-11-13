#include "GameLoadingScene.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Time.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>

#include <Rendering/Renderer.hpp>

#include <Window/Window.hpp>

#include <UI/Canvas.hpp>

#include <Scenes/SceneManager.hpp>
#include "GameScene.hpp"

using namespace SDL2_Engine;

#include <SDL.h>

//! Include the Game Manager
#include "../Game/Management/GameManager.hpp"

namespace BombSquad {
	/*
		GameLoadingScene : Constructor - Initialise with default values
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	GameLoadingScene::GameLoadingScene() : 
		mDestroyGM(true) 
	{}

	/*
		GameLoadingScene : createScene - Load the values required for the Scene to operate
		Created: 12/11/2017
		Modified: 13/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool GameLoadingScene::createScene() {
		//Clear the previous UI elements
		Globals::get<UI::Canvas>().clearCanvas();

		//Load the font
		mMsgFont = Globals::get<Resources>().loadResource<Font>("resources/Fonts/Ebrima.ttf", 36);

		//Check the font loaded correctly
		if (mMsgFont->status() != EResourceLoadStatus::Loaded) return false;

		//Default not completed
		mLoadedFlag.store(false);

		//Create the worker thread
		mWorker = std::thread([&]() { 
			//Load the new map
			GM::setupNewGame(); 

			//Flag completed
			mLoadedFlag.store(true);
		});

		//Return success
		return true;
	}

	/*
		GameLoadingScene : destroyScene - Destroy the values used to setup the Scene
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	void GameLoadingScene::destroyScene() {
		//Check for worker join
		if (mWorker.get_id() != std::thread::id()) mWorker.join();

		//Check if the Game Manager should be destroyed
		if (mDestroyGM) GM::destroy();
	}

	/*
		GameLoadingScene : update - Check for loading completion
		Created: 12/11/2017
		Modified: 13/11/2017
	*/
	void GameLoadingScene::update() {
		//Check if the thread has finished
		if (mLoadedFlag.load()) {
			//Shutdown the current scene
			shutdown();

			//Prevent Game Manager destruction
			mDestroyGM = false;

			//Load the Game Scene
			Globals::get<Scenes::SceneManager>().addScene<GameScene>();
		}
	}

	/*
		GameLoadingScene : render - Render the Scene to the Window
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	void GameLoadingScene::render() {
		//Get the current Time object
		const Time& TIME = Globals::get<Time>();

		//Get the elapsed time as divided by three
		size_t buffer = (size_t)(floor(TIME.getRealElapsed())) % 3;
		
		//Create a string buffer for the loading text
		std::string textBuffer = "Loading";

		//Add 'animation'
		for (size_t i = 0; i < buffer; i++)
			textBuffer += '.';

		//Get the window dimensions
		const auto DIM = Globals::get<Window>().getWindowDimensions();

		//Render the text to the window
		Globals::get<Rendering::Renderer>().drawText(textBuffer.c_str(), mMsgFont->font, { DIM.x / 2, DIM.y / 2 }, Rendering::Colour::Wheat, Rendering::ETextAlignment::Center);
	}
}