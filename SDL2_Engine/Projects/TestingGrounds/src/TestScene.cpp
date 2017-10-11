#include "TestScene.hpp"

#include <Globals.hpp>
#include <Window/Window.hpp>
#include <Scenes/SceneManager.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Rendering/Renderer.hpp>
#include <Time.hpp>
#include <Math.hpp>

#include <SDL.h>

/*
	TestScene : createScene - Initialise the values for the Test Scene
	Created: 11/10/2017
	Modified: 11/10/2017
	
	return bool - Returns true if the Scene was initialised successfully
*/
bool TestScene::createScene() {
	//Load the texture
	mTexture = SDL2_Engine::Globals::get<SDL2_Engine::Resources>().loadResource<SDL2_Engine::Texture>("face.png");

	//Return if the texture was loaded correctly
	return (mTexture->status() == SDL2_Engine::EResourceLoadStatus::Loaded);
}

/*
	TestScene : destroyScene - Deallocate memory used for the Test Scene
	Created: 11/10/2017
	Modified: 11/10/2017
*/
void TestScene::destroyScene() {}

/*
	TestScene : update - Update the TestScene values
	Created: 11/10/2017
	Modified: 11/10/2017
*/
void TestScene::update() {
	if (SDL2_Engine::Globals::get<SDL2_Engine::Input::Keyboard>().keyPressed(SDL2_Engine::Input::EKeyboardKeyCodes::Space)) 
		SDL2_Engine::Globals::get<SDL2_Engine::Scenes::SceneManager>().quit();
}

/*
	TestScene : render - Render the Test Scene
	Created: 11/10/2017
	Modified: 11/10/2017
*/
void TestScene::render() {
	//Get the size of the window
	auto dim = SDL2_Engine::Globals::get<SDL2_Engine::Window>().getWindowDimensions();

	//Draw the image
	SDL2_Engine::Globals::get<SDL2_Engine::Rendering::Renderer>().drawTexture(mTexture->texture, { dim.x / 2 - 50, dim.y / 2 - 50, 100, 100 }, nullptr, SDL2_Engine::Rendering::Color::White, cosf((float)SDL2_Engine::Globals::get<SDL2_Engine::Time>().getElapsed()) * SDL2_Engine::Globals::get<SDL2_Engine::Math>().Rad2Deg);
}
