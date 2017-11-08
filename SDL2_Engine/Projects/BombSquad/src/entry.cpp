#include <iostream>

//! Include the basic SDL2_Engine initialisation values
#include <Initialisation/Initialisation.hpp>
#include <Rendering/BasicSceneRenderer.hpp>
#include <Input/VirtualAxis.hpp>

using namespace SDL2_Engine::Initialisation;
using namespace SDL2_Engine::Input;

//! Include the functionality required to process loaded UI elements
#include <UI/Canvas.hpp>
#include <UI/UIElements/Interfaces/IUIAction.hpp>
#include <Globals.hpp>
#include <Scenes/SceneManager.hpp>

#include "Scenes/ExternalMenu.hpp"
#include "Scenes/PlayerSetupScene.hpp"

//! Use a vector to store Virtual Axis
#include <vector>

//! Include the starting Scene
#include "Scenes/SplashScene.hpp"

/*
	setupExternalUI - Apply basic logic to allow Action elements to progress to menus defined by their tag
	Author: Mitchell Croft
	Created: 02/11/2017
	Modified: 02/11/2017

	param[in] pObj - A pointer to the Actionable object to be setup
	param[in] pTag - The tag associated with the loaded Actionable object
*/
void setupExternalUI(SDL2_Engine::UI::UIElements::IUIAction* pObj, const SDL2_Engine::UI::uiTag& pTag) {
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////--------------------------------Check Predefined Scenes------------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Check for Game Setup
	if (pTag == "playerSetup") {
		pObj->setAction([&](SDL2_Engine::UI::UIElements::IUIAction* pItem, void* pData) {
			//Get a reference to the Scene Manager
			auto& scene = SDL2_Engine::Globals::get<SDL2_Engine::Scenes::SceneManager>();

			//Load the new External Menu
			scene.addScene<BombSquad::PlayerSetupScene>();

			//Remove any External Menu scenes
			scene.removeScenes<BombSquad::ExternalMenu>();
		});
	}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////--------------------------Default Assign UI Menu Called Tag--------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Set the action callback for the element
	else pObj->setAction([&](SDL2_Engine::UI::UIElements::IUIAction* pItem, void* pData) {
		//Get a reference to the Scene Manager
		auto& scene = SDL2_Engine::Globals::get<SDL2_Engine::Scenes::SceneManager>();
		
		//Remove any External Menu scenes
		scene.removeScenes<BombSquad::ExternalMenu>();

		//Load the new External Menu
		scene.addScene<BombSquad::ExternalMenu>(pTag.c_str());
	});
}

/*
	main - Initialise the application and create startup values
	Author: Mitchell Croft
	Created: 02/11/2017
	Modified: 02/11/2017

	param[in] pArgCount - The number of command line arguments currently in the pArgs array
	param[in] pArgs - An array of command line parameters passed to the program

	return int - Returns the status code for the programs execution
*/
int main(int pArgCount, char* pArgs[]) {
	//Create the initialisation object
	SDL2_Engine_Initialiser setup;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////---------------------------------Assign Version Values-------------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	setup.applicationVersion.major = 0;
	setup.applicationVersion.minor = 0;
	setup.applicationVersion.patch = 0;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////----------------------------------Setup Window Values--------------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Assign the title of the application
	setup.windowValues.title = "Bomb Squad";

	//Force the window to restricted size
	setup.windowValues.properties = SDL2_Engine::EWindowProperties::Style_Fixed;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////-----------------------------Process Command Line Arguments--------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	
	for (int i = 0; i < pArgCount; i++) {
		//Check if flag is output flag
		if (!strcmp("-log", pArgs[i])) 
			setup.loggerValues.storeLog = true;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////------------------------------------Initialise Renderer------------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Provide a Basic Scene Renderer for operation
	setup.rendererValues.sceneRenderer = new SDL2_Engine::Rendering::BasicSceneRenderer();

	//Create the Scene Manger Initialiser
	SceneManagerInitialiser sceneSetup(new BombSquad::SplashScene());

	//Bind the Scene Manager Initialiser to the setup object
	setup.sceneManagerValues = &sceneSetup;
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////---------------------------------Setup UI Navigation Axis----------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Initialise the UI element navigation virtual axis
	std::vector<SDL2_Engine::Input::VirtualAxis> uiAxis;

	//Horizontal
	VirtualAxis horizPad;
	horizPad.name = "horizontal";
	horizPad.inputType = EInputDevice::GamePad;
	horizPad.sensitivity = 1.f;
	horizPad.gravity = 10.f;
	horizPad.gamePads = EGamePadID::All;
	horizPad.gamePadInputType = EGamePadInputType::Axis;
	horizPad.aAxis = EGamePadAxisCode::Left_X;
	horizPad.aDeadZone = 0.1f;
	uiAxis.push_back(horizPad);

	VirtualAxis horizKey;
	horizKey.name = "horizontal";
	horizKey.inputType = EInputDevice::Keyboard;
	horizKey.sensitivity = 1.f;
	horizKey.gravity = 10.f;
	horizKey.kPosBtn = EKeyboardKeyCode::Arrow_Right;
	horizKey.kNegBtn = EKeyboardKeyCode::Arrow_Left;
	uiAxis.push_back(horizKey);

	//Vertical
	VirtualAxis vertPad;
	vertPad.name = "vertical";
	vertPad.inputType = EInputDevice::GamePad;
	vertPad.sensitivity = 1.f;
	vertPad.gravity = 10.f;
	vertPad.gamePads = EGamePadID::All;
	vertPad.gamePadInputType = EGamePadInputType::Axis;
	vertPad.aAxis = EGamePadAxisCode::Left_Y;
	vertPad.aDeadZone = 0.1f;
	uiAxis.push_back(vertPad);

	VirtualAxis vertKey;
	vertKey.name = "vertical";
	vertKey.inputType = EInputDevice::Keyboard;
	vertKey.sensitivity = 1.f;
	vertKey.gravity = 10.f;
	vertKey.kPosBtn = EKeyboardKeyCode::Arrow_Up;
	vertKey.kNegBtn = EKeyboardKeyCode::Arrow_Down;
	uiAxis.push_back(vertKey);

	//Confirm
	VirtualAxis confPad;
	confPad.name = "confirm";
	confPad.inputType = EInputDevice::GamePad;
	confPad.sensitivity = 1.f;
	confPad.gravity = 10.f;
	confPad.gamePads = EGamePadID::All;
	confPad.gamePadInputType = EGamePadInputType::Button;
	confPad.bPosBtn = EGamePadBtnCodes::A;
	confPad.bAltPosBtn = EGamePadBtnCodes::Start;
	uiAxis.push_back(confPad);

	VirtualAxis confKey;
	confKey.name = "confirm";
	confKey.inputType = EInputDevice::Keyboard;
	confKey.sensitivity = 1.f;
	confKey.gravity = 10.f;
	confKey.kPosBtn = EKeyboardKeyCode::Space;
	confKey.kAltPosBtn = EKeyboardKeyCode::Return;
	uiAxis.push_back(confKey);

	//Assign the uiAxis to the initial list
	setup.inputValues.defaultAxis = uiAxis.data();
	setup.inputValues.count = uiAxis.size();

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////--------------------------------Assign Setup Function Canvas-------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	setup.canvasValues.actionSetup = setupExternalUI;

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	////////------------------------------------Start Application--------------------------------////////
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	//Initialise the SDL2_Engine 
	auto errorNum = SDL2_Engine_Init(setup);

	//Terminate the program
	return (int)errorNum;
}