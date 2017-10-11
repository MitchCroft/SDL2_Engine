#include <iostream>

#include <Initialisation/Initialisation.hpp>
#include <Rendering/BasicSceneRenderer.hpp>
#include "TestScene.hpp"

using namespace SDL2_Engine::Initialisation;

int main() {
	//Create an initialisation object
	SDL2_Engine_Initialiser setup;

	//Provide a Basic Scene Renderer for operation
	setup.rendererValues.sceneRenderer = new SDL2_Engine::Rendering::BasicSceneRenderer();

	//Create the Scene Manger Initialiser
	SceneManagerInitialiser sceneSetup(new TestScene());

	//Bind the Scene Manage Initialiser
	setup.sceneManagerValues = &sceneSetup;

	//Initialise the SDL2_Engine
	auto errorNum = SDL2_Engine_Init(setup);

	//Check the return value
	if (errorNum != EInitialisationError::Success)
		system("PAUSE");

	//Return the error code
	return (int)errorNum;
}