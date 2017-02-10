#include <iostream>

#include "SDL2_Engine/State Manager/StateManager.hpp"

#include "SDL2_EngineInit.hpp"

#include "SDL2_Engine/Root/Logger.hpp"

#include <SDL.h>

int main(int pArgCount, char* pArgs[]) {
	//Create the StateManager settings object
	SDL2_Engine::StateManager::StateManagerInitValues vals;

	//Setup with values
	vals.commandArgs = pArgs;
	vals.commandArgsCount = pArgCount;
	vals.initFunc = init;
	vals.mainFunc = drawLoop;
	vals.threadFunc = updateLoop;
	vals.destroyFunc = destroy;
	vals.warningCallback = SDL2_Engine::Logger::logWarning;
	vals.errorCallback = SDL2_Engine::Logger::logError;

	SDL_WindowEvent;

	//Initialise the State Manager
	if (SDL2_Engine::StateManager::StateManager::init(vals))
		SDL2_Engine::StateManager::StateManager::run();

	//Destroy the State Manager
	SDL2_Engine::StateManager::StateManager::destroy();

	#ifdef _DEBUG
	system("PAUSE");
	#endif
	return 0;
}