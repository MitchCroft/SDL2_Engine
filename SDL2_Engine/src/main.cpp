#include <iostream>

#include <SDL.h>
#include <glm/glm.hpp>

#include <RakPeerInterface.h>

#include <string.h>

#include <dirent.h>

#include "SDL2_Engine/Utilities/Properties/Properties.h"

#include "SDL2_Engine/Time.h"

int main(int pArgCount, char* pArgs[]) {

	glm::mat4 test;

	// Get current directory of the current file
	char result[MAX_PATH];
	GetModuleFileName(NULL, result, MAX_PATH);
	std::cout << result << '\n';

	//Test the time object
	SDL2_Engine::Time time;

	bool pressed = GetKeyState(VK_SPACE) < 0;
	while (pressed == GetKeyState(VK_SPACE) < 0) {
		time.update();

		if (time.realElapsedTime > 5.0) time.timeScale = sinf(time.realElapsedTime);

		//Output Values
		printf("---------- Time Values ----------\n" \
			"Time Scale: %f\n" \
			"Delta Time: %f\n" \
			"Real Delta Time: %f\n" \
			"Elapsed Time: %f\n" \
			"Real Elapsed Time: %f\n\n\n", time.timeScale.value(), time.deltaTime.value(), time.realDeltaTime.value(), time.elapsedTime.value(), time.realElapsedTime.value());
	}

	system("PAUSE");
	return 0;
}