#include <iostream>

#include <Initialisation/Initialisation.hpp>
#include <Rendering/BasicSceneRenderer.hpp>
#include <Input/VirtualAxis.hpp>
#include "TestScene.hpp"

using namespace SDL2_Engine::Initialisation;
using namespace SDL2_Engine::Input;

int main() {
	//Create an initialisation object
	SDL2_Engine_Initialiser setup;

	//Provide a Basic Scene Renderer for operation
	setup.rendererValues.sceneRenderer = new SDL2_Engine::Rendering::BasicSceneRenderer();

	//Create the Scene Manger Initialiser
	SceneManagerInitialiser sceneSetup(new TestScene());

	//Bind the Scene Manage Initialiser
	setup.sceneManagerValues = &sceneSetup;

	//Setup the Input Manager Initialiser
	VirtualAxis def[4];

	def[0].inputType = EInputDevice::GamePads;
	strcpy_s(def[0].name, "test");
	def[0].sensitivity = 1;
	def[0].gravity = 1;
	def[0].gamePads = EGamePadID::One;
	def[0].gamePadInputType = EGamePadInputType::Button;
	def[0].bPosBtn = EGamePadBtnCodes::A;
	def[0].bNegBtn = EGamePadBtnCodes::B;

	def[1].inputType = EInputDevice::GamePads;
	strcpy_s(def[1].name, "test");
	def[1].sensitivity = 1;
	def[1].gravity = 1;
	def[1].gamePads = EGamePadID::One;
	def[1].gamePadInputType = EGamePadInputType::Axis;
	def[1].aAxis = EGamePadAxisCodes::Right_X;
	def[1].aDeadZone = 0.1f;

	def[2].inputType = EInputDevice::Keyboard;
	strcpy_s(def[2].name, "test");
	def[2].sensitivity = 1;
	def[2].gravity = 1;
	def[2].kPosBtn = EKeyboardKeyCodes::Arrow_Up;
	def[2].kNegBtn = EKeyboardKeyCodes::Arrow_Down;

	def[3].inputType = EInputDevice::Mouse;
	strcpy_s(def[3].name, "test");
	def[3].sensitivity = 1;
	def[3].gravity = 1;
	def[3].mAxis = EMouseAxis::X;

	setup.inputValues.defaultAxis = def;
	setup.inputValues.count = 4;

	//Initialise the SDL2_Engine
	auto errorNum = SDL2_Engine_Init(setup);

	//Check the return value
	if (errorNum != EInitialisationError::Success)
		system("PAUSE");

	//Return the error code
	return (int)errorNum;
}