#include <iostream>

#include <SDL.h>
#include <glm/glm.hpp>

#include <RakPeerInterface.h>

#include <string.h>

#include <dirent.h>

#include "SDL2_Engine/Utilities/Properties/Properties.h"

#include "SDL2_Engine/Time.h"
#include "SDL2_Engine/Debug.h"

int main(int pArgCount, char* pArgs[]) {

	glm::mat4 test;

	// Get current directory of the current file
	char result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);
	std::cout << result << '\n';

	/*//Test the time object
	SDL2_Engine::Time time;

	bool pressed = GetKeyState(VK_SPACE) < 0;
	while (pressed == GetKeyState(VK_SPACE) < 0) {
		time.update();

		if (time.realElapsedTime > 5.0) time.timeScale = sinf(time.realElapsedTime);

		//Output Values
		SDL2_Engine::Debug::log("---------- Time Values ----------\n" \
			"Time Scale: %f\n" \
			"Delta Time: %f (%i fps)\n" \
			"Real Delta Time: %f (%i fps)\n" \
			"Elapsed Time: %f\n" \
			"Real Elapsed Time: %f\n\n\n", time.timeScale.value(), time.deltaTime.value(), (int)(1.f / time.deltaTime), time.realDeltaTime.value(), (int)(1.f / time.realDeltaTime), time.elapsedTime.value(), time.realElapsedTime.value());
	}*/

	SDL2_Engine::Debug::log("Test Log %s", result);
	SDL2_Engine::Debug::log(SDL2_Engine::DebugColor::CYAN, "Test Log with color %s", result);
	SDL2_Engine::Debug::logWarning("Test Warning %s", result);
	SDL2_Engine::Debug::logError("Test Error %s", result);

	system("PAUSE");
	return 0;
}