#include "SDL2_EngineInit.hpp"

//! For initialising different managers
#include "SDL2_Engine/InitIncludes.hpp"

//! To enable using the time values
#include "SDL2_Engine/Time/Time.hpp"

//! To enable creating vibration descriptions
#include "SDL2_Engine/Input/VibrationDescription.hpp"

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
					loadSuccess = Input::Input::init();
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

void updateLoop(Time::Time& pTime) {
	//Update the input manager
	Input::Input::update(pTime.deltaTime, pTime.realDeltaTime);

	//Check for game exit
	if (Input::Input::getGamePadBtnPressed(Input::EGamePadBtnCodes::Start) || Input::Input::getKeyboardKeyPressed(Input::EKeyboardKeyCodes::Escape))
		StateManager::StateManager::quit();

	//Check for button press
	if (int mask = Input::Input::getGamePadBtnPressed(Input::EGamePadBtnCodes::A)) {
		//Create the setting
		Input::VibrationDescription setting;

		//Set the values
		setting.gamePad = mask;
		setting.leftVibration = 1.f;
		setting.rightVibration = 1.f;
		setting.vibrationLength = 2.f;

		//Add to input manager
		Input::Input::applyVibration(setting);
	}

	if (int mask = Input::Input::getGamePadBtnPressed(Input::EGamePadBtnCodes::Y)) {
		//Create the setting
		Input::VibrationDescription setting;

		//Set the values
		setting.gamePad = mask;
		setting.leftVibration = 1.f;
		setting.rightVibration = 0.5f;
		setting.vibrationLength = 2.f;
		setting.scaleFunc = scaler;

		//Add to input manager
		Input::Input::applyVibration(setting);
	}
}

void drawLoop(Time::Time& pTime) {

}

void destroy() {
	//TEMP
	Logger::log("Running the destroy function");
	//TEMP

	Input::Input::destroy();

	//TODO: Destroy Screen manager

	Resources::Resources::destroy();

	//TODO: Destroy Audio

	//TODO: Destroy SDL2
}