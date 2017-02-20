#pragma once

#include <string>

#include "InputValues.hpp"

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
		//! Store the ID of the Axis to accept input from
		EGamePadID gamePad = EGamePadID::All;

		//! Store the name of the axis, used for accessing axis information
		std::string name = "";

		//! Store the additional strength applied to input movements
		float sensitivity = 1.f;

		//! Store the pull strength applied to the value returning to zero
		float gravity = 1.f;

		//! Flags if the input values should be inverted
		bool invert = false;

		//! Flags if the input type is a button or an axis control
		EGamePadInputType inputType = EGamePadInputType::Button;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////    Values below are only used when the input type is set to EAxisInputType::Button    ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the buttons which contribute a positive value to the axis
		EGamePadBtnCodes bPosBtn = EGamePadBtnCodes::Null_Input;
		EGamePadBtnCodes bAltPosBtn = EGamePadBtnCodes::Null_Input;

		//! Store the buttons which contribute a negative value to the axis
		EGamePadBtnCodes bNegBtn = EGamePadBtnCodes::Null_Input;
		EGamePadBtnCodes bAltNegBtn = EGamePadBtnCodes::Null_Input;

		///////////////////////////////////////////////////////////////////////////////////////////////
		////    Values below are only used when the input type is set to EAxisInputType::Axis      ////
		///////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the axis to take input from
		EGamePadAxisCodes aAxis = EGamePadAxisCodes::Null_Axis;

		//! Store the size of the dead zone applied to read axis (0 - 1)
		float aDeadZone = 0.1f;
	};
}