#include <iostream>

#include <SDL.h>

#include "SDL2_Engine/InitIncludes.hpp"
#include "SDL2_EngineInit.hpp"

int main(int pArgCount, char* pArgs[]) {
	//Create the StateManager settings object
	SDL2_Engine::StateManagerInitValues vals;

	//Setup with values
	vals.commandArgs = pArgs;
	vals.commandArgsCount = pArgCount;
	vals.initFunc = init;
	vals.mainFunc = drawLoop;
	vals.threadFunc = updateLoop;
	vals.destroyFunc = destroy;
	vals.warningCallback = SDL2_Engine::Logger::logWarning;
	vals.errorCallback = SDL2_Engine::Logger::logError;

	//Initialise the State Manager
	if (SDL2_Engine::StateManager::init(vals))
		SDL2_Engine::StateManager::run();
	
	//Destroy the State Manager
	SDL2_Engine::StateManager::destroy();

	#ifdef _DEBUG
	system("PAUSE");
	#endif
	return 0;
}