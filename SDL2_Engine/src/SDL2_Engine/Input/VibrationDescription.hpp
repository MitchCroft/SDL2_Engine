#pragma once

#include "InputValues.hpp"
#include "../Utilities/Bitmask.hpp"

namespace SDL2_Engine {
	namespace Input {

		/*
		*		Name: VibrationDescription
		*		Author: Mitchell Croft
		*		Created: 10/02/2017
		*		Modified: 13/02/2017
		*
		*		Purpose:
		*		Provide a method for describing the vibration
		*		values to apply to a connected controller
		**/
		struct VibrationDescription {
			//! A Bitmask for the 
			Utilities::Bitmask<EGamePadID> gamePad;

			//! Store the vibration strength scale values (0 - 1)
			float leftVibration;
			float rightVibration;

			//! Store the duration for which the vibration should occur	(Seconds)
			float vibrationLength;

			//! Store a function pointer for optional scaling of value over time
			VibrationScaleFunc scaleFunc;

			//! Simple constructor
			VibrationDescription();
		};
	}
}