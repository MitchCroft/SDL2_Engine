#include "VibrationDescription.hpp"

namespace SDL2_Engine {

	/*
		VibrationDescription : Constructor - Initialise with default values
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017
	*/
	VibrationDescription::VibrationDescription() :
		gamePad(EGamePadID::All),
		leftVibration(0.f),
		rightVibration(0.f),
		vibrationLength(0.f),
		scaleFunc(nullptr)
	{}
}