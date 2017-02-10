#pragma once

#include "InputValues.hpp"

namespace SDL2_Engine {
	/*
	 *		Name: VibrationSetting
	 *		Author: Mitchell Croft
	 *		Created: 10/02/2017
	 *		Modified: 10/02/2017
	 *		
	 *		Purpose:
	 *		Provide a method for describing the vibration
	 *		settings to apply to a connected controller
	**/
	struct VibrationSetting {
		//! Store the ID of the controller to apply the vibration to
		EControllerID controller = EControllerID::All;

		//! Store the vibration strength scale values (0 - 1)
		float leftVibration = 0.f;
		float rightVibration = 0.f;

		//! Store the duration for which the vibration should occur	(Seconds)
		float vibrationLength = 0.f;

		//! Store a function pointer for optional scaling of value over time
		VibrationScaleFunc scaleFunc = nullptr;
	};
}