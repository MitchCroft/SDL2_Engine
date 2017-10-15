#include "TestScene.hpp"

#include <Globals.hpp>
#include <Window/Window.hpp>
#include <Debug/Logger.hpp>
#include <Scenes/SceneManager.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>
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
	outputText - Output the Text from a UITextbox object
	Author: Mitchell Croft
	Created: 15/10/2017
	Modified: 15/10/2017

	param[in] pObj - The UI object that triggered the action
	param[in] pData - The data associated with the UI element for processing
*/
void outputUITextboxText(IUIAction* pObj, void* pData) {
	//Cast the object to a UITextbox object
	auto txtbox = dynamic_cast<UITextbox*>(pObj);

	//If no object leave
	if (!txtbox) return;

	//Log the text contained text
	Globals::get<Debug::Logger>().log("Textbox: ", txtbox->getText());
}

/*
	buttonTestCallback - Test the action responses of UI buttons
	Created: 14/10/2017
	Modified: 14/10/2017

	param[in] pObj - The UI object that triggered the action
	param[in] pData - The data associated with the UI element for processing
*/
void buttonTestCallback(IUIAction* pObj, void* pData) {
	//Get the Canvas object
	Canvas& canvas = Globals::get<Canvas>();

	//Get the window dimensions
	const Dimension2D& DIM = Globals::get<Window>().getWindowDimensions();

	//Come up with a random UI interactable element
	if (rand() % 2) {
		//Create the UI Button
		auto UI = canvas.createUI<UIButton>();

		//Set the font
		UI->setFont(Globals::get<Resources>().loadResource<Font>("font.ttf", 24)->font);

		//Set the text
		UI->setText("I am the best");

		//Set a random position
		UI->setLocation({ rand() % DIM.x, rand() % DIM.y, 250, 50 });

		//Set the action callback
		UI->setAction(buttonTestCallback);
	}

	else {
		//Create the UI Textbox
		auto UI = canvas.createUI<UITextbox>();

		//Set the font
		UI->setFont(Globals::get<Resources>().loadResource<Font>("font.ttf", 24)->font);

		//Set a random position
		UI->setLocation({ rand() % DIM.x, rand() % DIM.y, 250, 50 });

		//Set the action callback
		UI->setAction(outputUITextboxText);
	}

	//Rebuild the interaction map
	canvas.rebuildInteractionMap();
}


/*
	TestScene : createScene - Initialise the values for the Test Scene
	Created: 11/10/2017
	Modified: 14/10/2017
	
	return bool - Returns true if the Scene was initialised successfully
*/
bool TestScene::createScene() {
	//Load the texture
	mTexture = Globals::get<Resources>().loadResource<Texture>("face.png");

	//Load the font
	mFont = Globals::get<Resources>().loadResource<Font>("font.ttf", 24);

	//Return if the texture was loaded correctly
	return ((EResourceLoadStatus)((int)mTexture->status() | (int)mFont->status()) == EResourceLoadStatus::Loaded);
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
	Modified: 14/10/2017
*/
void TestScene::update() {
	//Output Delta Time
	//Globals::get<Debug::Logger>().log("Delta Time: ", (1.f / Globals::get<Time>().getDelta()));

	//Quit Application
	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCodes::Escape)) 
		Globals::get<SceneManager>().quit();

	//Add new UI Button
	if (Globals::get<Keyboard>().keyPressed(EKeyboardKeyCodes::Return))
		buttonTestCallback(nullptr, nullptr);
}

/*
	TestScene : render - Render the Test Scene
	Created: 11/10/2017
	Modified: 14/10/2017
*/
void TestScene::render() {
	//Get the size of the window
	auto dim = Globals::get<Window>().getWindowDimensions();

	//Get a reference to the renderer
	Renderer& rend = Globals::get<Renderer>();

	//Draw the image
	rend.drawTexture(mTexture->texture, { dim.x / 2 - 50, dim.y / 2 - 50, 100, 100 }, nullptr, Color::White, cosf((float)Globals::get<SDL2_Engine::Time>().getElapsed()) * Globals::get<Math>().Rad2Deg);
}
