#include "SDL2_EngineInit.hpp"

#include "SDL2_Engine/SDL2_Engine.hpp"
#include "SDL2_Engine/State Manager/Objects/StateManager.hpp"
#include "SDL2_Engine/Resources/Resources.hpp"

using namespace SDL2_Engine;

bool init() {
	//TEMP
	Logger::log("Running the init function");
	//TEMP

	//Store the load success flag
	bool loadSuccess;

	//Complete a load of all systems
	if (loadSuccess = true /* TODO: Load SDL2 */) {
		if (loadSuccess = true /* TODO: Load Audio */) {
			if (loadSuccess = Resources::Resources::init("resources\\", (SDL_Renderer*)&loadSuccess /*VERY VERY TEMP*/, Logger::logWarning, Logger::logError)) {
				if (loadSuccess = true /* TODO: Load screen manager */) {
					loadSuccess = Input::init();
				}
			}
		}
	}

	//Return the load flag
	return loadSuccess;
}

//TEMP
inline float scaler(const float& pT) { return 1.f - pT; }
//TEMP

void updateLoop(Time& pTime) {
	//Update the input manager
	Input::update(pTime.deltaTime, pTime.realDeltaTime);

	//Check for game exit
	if (Input::getGamePadBtnPressed(EGamePadBtnCodes::Start) || Input::getKeyboardKeyPressed(EKeyboardKeyCodes::Escape))
		StateManager::StateManager::quit();

	//Check for button press
	if (int mask = Input::getGamePadBtnPressed(EGamePadBtnCodes::A)) {
		//Create the setting
		VibrationDescription setting;

		//Set the values
		setting.gamePad = mask;
		setting.leftVibration = 1.f;
		setting.rightVibration = 1.f;
		setting.vibrationLength = 2.f;

		//Add to input manager
		Input::applyVibration(setting);
	}

	if (int mask = Input::getGamePadBtnPressed(EGamePadBtnCodes::Y)) {
		//Create the setting
		VibrationDescription setting;

		//Set the values
		setting.gamePad = mask;
		setting.leftVibration = 1.f;
		setting.rightVibration = 0.5f;
		setting.vibrationLength = 2.f;
		setting.scaleFunc = scaler;

		//Add to input manager
		Input::applyVibration(setting);
	}
}

void drawLoop(Time& pTime) {

}

void destroy() {
	//TEMP
	Logger::log("Running the destroy function");
	//TEMP

	Input::destroy();

	//TODO: Destroy Screen manager

	Resources::Resources::destroy();

	//TODO: Destroy Audio

	//TODO: Destroy SDL2
}