#pragma once

#include <string>

#include "InputValues.h"

namespace SDL2_Engine {
	/*
	*		Name: VirtualAxis
	*		Author: Mitchell Croft
	*		Created: 30/01/2017
	*		Modified: 31/01/2017
	*
	*		Purpose:
	*		Provide a method for creating virtual axis' that will
	*		be monitored by the Input Singleton. The resulting
	*		values can be read and used to create game input
	**/
	struct VirtualAxis {
		//! Store the controller to receive input from
		EControllerID controller = EControllerID::All;

		//! Store the name of the axis, used for accessing axis information
		std::string name = "";

		//! Store the additional strength applied to input movements
		float sensitivity = 1.f;

		//! Store the pull strength applied to the value returning to zero
		float gravity = 1.f;

		//! Flags if the input values should be inverted
		bool invert = false;

		//! Flags if the input type is a button or an axis control
		EAxisInputType inputType = EAxisInputType::Button;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////    Values below are only used when the input type is set to EAxisInputType::Button    ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the buttons which contribute a negative value to the axis
		EControllerKeyCodes bPosBtn = EControllerKeyCodes::Null_Input;
		EControllerKeyCodes bAltPosBtn = EControllerKeyCodes::Null_Input;

		//! Store the buttons which contribute a negative value to the axis
		EControllerKeyCodes bNegBtn = EControllerKeyCodes::Null_Input;
		EControllerKeyCodes bAltNegBtn = EControllerKeyCodes::Null_Input;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////    Values below are only used when the input type is set to EAxisInputType::Axis      ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the axis to take input from
		EControllerAxisCodes aAxis = EControllerAxisCodes::Null_Axis;

		//! Store the size of the dead zone applied to read axis (0 - 1)
		float aDeadZone = 0.1f;
	};
}