#include "TestScene.hpp"

#include <Globals.hpp>
#include <Window/Window.hpp>
#include <Debug/Logger.hpp>
#include <Scenes/SceneManager.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>
#include <Resources/ResourceTypes/LocalResourceCursor.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/Mouse/Mouse.hpp>
#include <Input/AxisInput.hpp>
#include <Rendering/Renderer.hpp>
#include <Rendering/Colour.hpp>
#include <UI/Canvas.hpp>
#include <UI/UIElements/UIPanel.hpp>
#include <UI/UIElements/UILabel.hpp>
#include <UI/UIElements/UIButton.hpp>
#include <UI/UIElements/UITextbox.hpp>
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
	Modified: 17/10/2017
	
	return bool - Returns true if the Scene was initialised successfully
*/
bool TestScene::createScene() { 
	//Load the cursor object
	mCursor = Globals::get<Resources>().loadResource<Cursor>("cursor.objx");

	//Check the cursor was loaded correctly
	if (mCursor->status() != EResourceLoadStatus::Loaded) return false;

	//Assign the cursor to the Mouse
	Globals::get<Mouse>().useCursor(mCursor);

	//Load the menu UI elements
	return Globals::get<Canvas>().loadCanvasFromObjx("menuUI.Objx"); 
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
	Modified: 16/10/2017
*/
void TestScene::update() {
	//Output Delta Time
	//Globals::get<Debug::Logger>().log("Delta Time: ", (1.f / Globals::get<Time>().getDelta()));

	//Quit Application
	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCodes::Escape)) 
		Globals::get<SceneManager>().quit();
}

/*
	TestScene : render - Render the Test Scene
	Created: 11/10/2017
	Modified: 16/10/2017
*/
void TestScene::render() {}
