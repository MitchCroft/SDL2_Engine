#include <iostream>

#include <Initialisation/Initialisation.hpp>
#include <Rendering/BasicSceneRenderer.hpp>
#include <Input/VirtualAxis.hpp>
#include "TestScene.hpp"

#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include <UI/Canvas.hpp>

#include <UI/UIElements/Interfaces/IUIAction.hpp>
#include <UI/UIElements/UITextbox.hpp>

#include <vector>
using std::vector;

using namespace SDL2_Engine::Initialisation;
using namespace SDL2_Engine::Input;

void setupUIAction(SDL2_Engine::UI::UIElements::IUIAction* pObject, const SDL2_Engine::UI::uiTag& pTag) {
	//Check the assigned tag
	if (pTag == "submission") {
		pObject->setAction([](SDL2_Engine::UI::UIElements::IUIAction* pItem, void* pData) {
			//Get the user input box
			SDL2_Engine::UI::UIElements::UITextbox* txtBox = dynamic_cast<SDL2_Engine::UI::UIElements::UITextbox*>(SDL2_Engine::Globals::get<SDL2_Engine::UI::Canvas>().getUI("userInput"));

			//Get the logger object
			const SDL2_Engine::Debug::Logger& LOG = SDL2_Engine::Globals::get<SDL2_Engine::Debug::Logger>();

			//Check if the UI element was found
			if (!txtBox) LOG.log("Textbox with tag 'userInput' was not found...");

			//Otherwise output the text
			else LOG.log("Textbox 'userInput': ", txtBox->getText());
		});
	}
}

int main() {
	//Create an initialisation object
	SDL2_Engine_Initialiser setup;

	//Set the version number
	setup.applicationVersion.major = 0;
	setup.applicationVersion.minor = 0;
	setup.applicationVersion.patch = 0;

	//Provide a Basic Scene Renderer for operation
	setup.rendererValues.sceneRenderer = new SDL2_Engine::Rendering::BasicSceneRenderer();

	//Create the Scene Manger Initialiser
	SceneManagerInitialiser sceneSetup(new TestScene());

	//Bind the Scene Manage Initialiser
	setup.sceneManagerValues = &sceneSetup;

	//Setup the Input Manager Initialiser
	vector<VirtualAxis> virtualAxis;

	//Horizontal
	VirtualAxis horizPad;
	horizPad.inputType = EInputDevice::GamePad;
	horizPad.name = "horizontal";
	horizPad.sensitivity = 1.f;
	horizPad.gravity = 10.f;
	horizPad.gamePads = EGamePadID::One;
	horizPad.gamePadInputType = EGamePadInputType::Axis;
	horizPad.aAxis = EGamePadAxisCode::Left_X;
	horizPad.aDeadZone = 0.1f;
	virtualAxis.push_back(horizPad);

	VirtualAxis horizKey;
	horizKey.inputType = EInputDevice::Keyboard;
	horizKey.name = "horizontal";
	horizKey.sensitivity = 1.f;
	horizKey.gravity = 10.f;
	horizKey.kPosBtn = EKeyboardKeyCode::Arrow_Right;
	horizKey.kNegBtn = EKeyboardKeyCode::Arrow_Left;
	virtualAxis.push_back(horizKey);

	//Vertical
	VirtualAxis vertPad;
	vertPad.inputType = EInputDevice::GamePad;
	vertPad.name = "vertical";
	vertPad.sensitivity = 1.f;
	vertPad.gravity = 10.f;
	vertPad.gamePads = EGamePadID::One;
	vertPad.gamePadInputType = EGamePadInputType::Axis;
	vertPad.aAxis = EGamePadAxisCode::Left_Y;
	vertPad.aDeadZone = 0.1f;
	virtualAxis.push_back(vertPad);

	VirtualAxis vertKey;
	vertKey.inputType = EInputDevice::Keyboard;
	vertKey.name = "vertical";
	vertKey.sensitivity = 1.f;
	vertKey.gravity = 10.f;
	vertKey.kPosBtn = EKeyboardKeyCode::Arrow_Up;
	vertKey.kNegBtn = EKeyboardKeyCode::Arrow_Down;
	virtualAxis.push_back(vertKey);

	//Confirm
	VirtualAxis confPad;
	confPad.inputType = EInputDevice::GamePad;
	confPad.name = "confirm";
	confPad.sensitivity = 1.f;
	confPad.gravity = 10.f;
	confPad.gamePads = EGamePadID::One;
	confPad.gamePadInputType = EGamePadInputType::Button;
	confPad.bPosBtn = EGamePadBtnCodes::A;
	virtualAxis.push_back(confPad);

	VirtualAxis confKey;
	confKey.inputType = EInputDevice::Keyboard;
	confKey.name = "confirm";
	confKey.sensitivity = 1.f;
	confKey.gravity = 10.f;
	confKey.kAltPosBtn = EKeyboardKeyCode::Return;
	virtualAxis.push_back(confKey);

	setup.inputValues.defaultAxis = virtualAxis.data();
	setup.inputValues.count = virtualAxis.size();

	setup.canvasValues.actionSetup = setupUIAction;

	//Initialise the SDL2_Engine
	auto errorNum = SDL2_Engine_Init(setup);

	//Check the return value
	if (errorNum != EInitialisationError::Success)
		system("PAUSE");

	//Return the error code
	return (int)errorNum;
}