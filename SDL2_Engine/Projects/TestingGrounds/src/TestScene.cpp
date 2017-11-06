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
#include <Utilities/VersionDescriptor.hpp>

#include <SDL.h>

using namespace SDL2_Engine;
using namespace SDL2_Engine::Rendering;
using namespace SDL2_Engine::Input;
using namespace SDL2_Engine::Scenes;
using namespace SDL2_Engine::UI;
using namespace SDL2_Engine::UI::UIElements;
using namespace SDL2_Engine::Utilities;

/*
	TestScene : createScene - Initialise the values for the Test Scene
	Created: 11/10/2017
	Modified: 02/11/2017
	
	return bool - Returns true if the Scene was initialised successfully
*/
bool TestScene::createScene() { 
	//Load the cursor object
	mCursor = Globals::get<Resources>().loadResource<Cursor>("cursor.objx");

	//Check the cursor was loaded correctly
	if (mCursor->status() != EResourceLoadStatus::Loaded) return false;

	//Assign the cursor to the Mouse
	Globals::get<Mouse>().useCursor(mCursor);

	//Load the font file
	mFont = Globals::get<Resources>().loadResource<Font>("font.ttf", 16);

	//Check the font was loaded correctly
	if (mFont->status() != EResourceLoadStatus::Loaded) return false;

	//Load the menu UI elements
	if (!Globals::get<Canvas>().loadCanvasFromObjx("menuUI.Objx")) return false;

	//Create the version label
	auto versionLabel = Globals::get<Canvas>().createUI<UILabel>();

	//Assign the font to the label
	versionLabel->setFont(mFont->font);

	//Set the position
	versionLabel->setLocation({ 0, 0 });

	//Set the alignment
	versionLabel->setAlignment(ETextAlignment::Left);

	//Get the version for the application
	const auto& VERSION = Globals::getApplicationVersion();

	//Set the text for the label
	versionLabel->setText((std::to_string(VERSION.major) + "." + std::to_string(VERSION.minor) + "." + std::to_string(VERSION.patch) + " (" + std::to_string(VERSION.versionID) + ")").c_str());

	//Return success
	return true;
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
	//Quit Application
	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCode::Escape)) 
		Globals::get<SceneManager>().quit();
}

/*
	TestScene : render - Render the Test Scene
	Created: 11/10/2017
	Modified: 16/10/2017
*/
void TestScene::render() {}
