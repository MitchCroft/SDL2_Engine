#include "TestScene.hpp"

#include <Globals.hpp>
#include <Window/Window.hpp>
#include <Debug/Logger.hpp>
#include <Scenes/SceneManager.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/AxisInput.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Rendering/Renderer.hpp>
#include <Rendering/Colour.hpp>
#include <UI/Canvas.hpp>
#include <UI/UIElements/UIPanel.hpp>
#include <Time.hpp>
#include <Math.hpp>

#include <SDL.h>

using namespace SDL2_Engine;
using namespace SDL2_Engine::Rendering;
using namespace SDL2_Engine::Input;
using namespace SDL2_Engine::Scenes;
using namespace SDL2_Engine::UI;
using namespace SDL2_Engine::UI::UIElements;

/*
	TestScene : createScene - Initialise the values for the Test Scene
	Created: 11/10/2017
	Modified: 11/10/2017
	
	return bool - Returns true if the Scene was initialised successfully
*/
bool TestScene::createScene() {
	//Load the texture
	mTexture = Globals::get<Resources>().loadResource<Texture>("face.png");

	//Return if the texture was loaded correctly
	return (mTexture->status() == EResourceLoadStatus::Loaded);
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
	Globals::get<Debug::Logger>().log("Delta Time: ", (1.f / Globals::get<Time>().getDelta()));

	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCodes::Space)) 
		Globals::get<SceneManager>().quit();

	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCodes::Return)) {
		//Get the bounds of the screen
		const auto& bounds = Globals::get<Window>().getWindowDimensions();

		//Create the UI Panel
		auto panel = Globals::get<Canvas>().createUI<UIPanel>();

		//Set the position
		panel->setLocation({ rand() % bounds.x, rand() % bounds.y, 100, 100 });

		//Set the image
		panel->setImage(mTexture->texture);

		//Set random colour filter
		panel->setFilterColour(rand());
	}
}

/*
	TestScene : render - Render the Test Scene
	Created: 11/10/2017
	Modified: 11/10/2017
*/
void TestScene::render() {
	//Get the size of the window
	auto dim = Globals::get<Window>().getWindowDimensions();

	//Get the "test" virtual axis
	const float SCALE = (Globals::get<AxisInput>().getAxis("test") + 1) / 2.f;

	//Get the lerp colour
	auto lerp = Colour(Colour::White).lerp(Colour::Black, SCALE);

	//Get a reference to the renderer
	Renderer& rend = Globals::get<Renderer>();

	//Colour the background
	rend.drawRect({ 0, 0, dim.x, dim.y }, lerp, true);

	//Draw the image
	rend.drawTexture(mTexture->texture, { dim.x / 2 - 50, dim.y / 2 - 50, 100, 100 }, nullptr, Color::White, cosf((float)Globals::get<SDL2_Engine::Time>().getElapsed()) * Globals::get<Math>().Rad2Deg);
}
