#include <iostream>

#define NOMINMAX
#include "SDL2_Engine/Math.hpp"

#include <SDL.h>
#include <glm/glm.hpp>

#include <RakPeerInterface.h>

#include <string.h>

#include <dirent.h>

#include "SDL2_Engine/SDL2_Engine.hpp"

int main(int pArgCount, char* pArgs[]) {

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
		//This shouldn't happen execute
		if (SDL2_Engine::Input::init()) return -1;

		//Create a virtual axis
		SDL2_Engine::VirtualAxis axis[2];

		//Set up the virtual axis
		axis[0].name = axis[1].name = "Test";
		axis[0].sensitivity = 2.5f;
		axis[0].gravity = 5.f;

		axis[0].inputType = SDL2_Engine::EAxisInputType::Axis;

		axis[0].aAxis = SDL2_Engine::EControllerAxisCodes::Right_Y;
		axis[0].aDeadZone = 0.1f;

		axis[1].controller = SDL2_Engine::EControllerID::Two;
		axis[1].inputType = SDL2_Engine::EAxisInputType::Button;

		axis[1].bPosBtn = SDL2_Engine::EControllerKeyCodes::Y;
		axis[1].bAltPosBtn = SDL2_Engine::EControllerKeyCodes::B;
		axis[1].bNegBtn = SDL2_Engine::EControllerKeyCodes::A;
		axis[1].bAltNegBtn = SDL2_Engine::EControllerKeyCodes::X;

		//Add the virtual axis to Input Manager
		SDL2_Engine::Input::addVirtualAxis(axis, 2);

		//Loop until loop is left		
		while (!SDL2_Engine::Input::getKeyDown(SDL2_Engine::EControllerKeyCodes::Start)) {
			//Update the current time
			time.update();

			//Update the input manager
			SDL2_Engine::Input::update(time.deltaTime, time.realDeltaTime);

			//Output axis value
			SDL2_Engine::Logger::log("Virtual Axis 'Test' has a value of ", SDL2_Engine::Input::getVAxis("Test"), " FPS: ", 1.f /  time.realDeltaTime);
		}
	}

	//Destroy the input manager
	SDL2_Engine::Input::destroy();

	system("PAUSE");
	return 0;
}