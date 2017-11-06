#pragma once

//! Include the Controller values
#include "ControllerValues.hpp"

//! Include Utility functionality
#include "../../Utilities/Action.hpp"
#include "../../Utilities/Bitmask.hpp"

namespace SDL2_Engine {
	namespace Input {
		//! Export the required Action implementation
		template class SDL2_LIB_INC Utilities::Action<float(const float&)>;

		/*
		 *		Name: VibrationDescription
		 *		Author: Mitchell Croft
		 *		Created: 10/02/2017
		 *		Modified: 22/09/2017
		 *		
		 *		Purpose:
		 *		Provide a method for describing the vibration values to 
		 *		apply to a connected GamePad over a period of time
		**/
		struct VibrationDescription {
			//! Store the GamePad(s) effected as a bitmask
			Utilities::Bitmask<EGamePadID> gamePad = EGamePadID::All;

			//! Store the vibration strength scales
			float leftVibration = 1.f;
			float rightVibration = 1.f;

			//! Store the duration of the Vibration (in seconds)
			float vibrationLength = 1.f;

			//! Store an action callback for scaling vibration values over time
			Utilities::Action<float(const float&)> scaleFunc;
		};
	}
}