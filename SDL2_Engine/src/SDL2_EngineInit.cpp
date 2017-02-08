#include "SDL2_EngineInit.hpp"

#include "SDL2_Engine/SDL2_Engine.hpp"

#include "SDL2_Engine/State Manager/Objects/StateManager.hpp"

using namespace SDL2_Engine;

#include <glm/glm.hpp>

glm::vec2 position;

bool init() {
	//TEMP
	Logger::log("Running the init function");
	//TEMP

	position = glm::vec2(0, 0);

	VirtualAxis axis[2];
	axis[0].name = "X"; axis[1].name = "Y";
	axis[0].inputType = axis[1].inputType = EAxisInputType::Axis;
	axis[0].aDeadZone = axis[1].aDeadZone = 0.1f;
	axis[0].aAxis = EControllerAxisCodes::Right_X;
	axis[1].aAxis = EControllerAxisCodes::Right_Y;

	bool inited = Input::init();

	if (inited) {
		Input::addVirtualAxis(axis, 2);
	}

	return inited;
}

void updateLoop(Time& pTime) {
	//Update the input values
	Input::update(pTime.deltaTime, pTime.realDeltaTime);

	position += glm::vec2(Input::getVAxis("X"), Input::getVAxis("Y")) * pTime.deltaTime.value();

	if (Input::getKeyDown(EControllerKeyCodes::Start))
		StateManager::StateManager::quit();
}

void drawLoop(Time& pTime) {
	Logger::log("Position (X:", position.x, ", Y:", position.y, ")");
}

void destroy() {
	Input::destroy();

	//TEMP
	Logger::log("Running the destroy function");
	//TEMP
}