#include <iostream>

#include <SDL.h>
#include <glm/glm.hpp>

#include <RakPeerInterface.h>

#include <string.h>

#include <dirent.h>

#include "SDL2_Engine/SDL2_Engine.hpp"

int main(int pArgCount, char* pArgs[]) {

	glm::mat4 test;

	// Get current directory of the current file
	char result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);
	std::cout << result << '\n';

	//Test the time object
	SDL2_Engine::Time time;

	/*bool pressed = GetKeyState(VK_SPACE) < 0;
	while (pressed == GetKeyState(VK_SPACE) < 0) {
		time.update();

		if (time.realElapsedTime > 5.0) time.timeScale = sinf(time.realElapsedTime);

		//Output Values
		SDL2_Engine::Logger::log("---------- Time Values ----------\n" \
			"Time Scale: %f\n" \
			"Delta Time: %f (%i fps)\n" \
			"Real Delta Time: %f (%i fps)\n" \
			"Elapsed Time: %f\n" \
			"Real Elapsed Time: %f\n\n\n", time.timeScale.value(), time.deltaTime.value(), (int)(1.f / time.deltaTime), time.realDeltaTime.value(), (int)(1.f / time.realDeltaTime), time.elapsedTime.value(), time.realElapsedTime.value());
	}*/

	/*std::string testString = " test to see if the log works";

	SDL2_Engine::Logger::log("This is the ", 1, testString);
	SDL2_Engine::Logger::logFormatted("Test Log %s", result);
	SDL2_Engine::Logger::logFormatted(SDL2_Engine::DebugColor::CYAN | SDL2_Engine::DebugColor::MAGENTA_FILL, "Test Log with color %s", result);
	SDL2_Engine::Logger::logWarning("Test Warning %s", result);
	SDL2_Engine::Logger::logError("Test Error %s", result);*/

	/*//Loop through all possible controller inputs
	DWORD dwResult;
	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		//Clear previously stored struct information
		XINPUT_STATE state;
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		//Get the current controller index's state
		dwResult = XInputGetState(i, &state);

		//Check the controllers connection
		SDL2_Engine::Logger::logFormatted(dwResult == ERROR_SUCCESS ? SDL2_Engine::DebugColor::GREEN : SDL2_Engine::DebugColor::RED,
			"Controller index %i is %s...\n",
			i, dwResult == ERROR_SUCCESS ? "connected" : "not connected");

		if (dwResult == ERROR_SUCCESS) {
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));
			vibration.wLeftMotorSpeed = 65535;
			vibration.wRightMotorSpeed = 65535;
			XInputSetState(i, &vibration);
		}
	}*/

	//Try to initialise the Input manager
	if (SDL2_Engine::Input::init()) {
		if (SDL2_Engine::Input::init()) return -1;

		//Loop until loop is left		
		bool pressed = GetKeyState(VK_SPACE) < 0;
		while (pressed == GetKeyState(VK_SPACE) < 0) {
			//Update the current time
			time.update();

			//Update the input manager
			SDL2_Engine::Input::update(time);

			SDL2_Engine::Logger::log("Controllers ", (SDL2_Engine::Input::getKey(SDL2_Engine::EControllerKeyCodes::A) ? "have" : "have not") , " pressed the 'A' button");
		}
	}

	//Destroy the input manager
	SDL2_Engine::Input::destroy();

	system("PAUSE");
	return 0;
}