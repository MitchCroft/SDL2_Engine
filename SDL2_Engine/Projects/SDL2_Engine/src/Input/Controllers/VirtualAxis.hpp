#pragma once

#include "ControllerValues.hpp"

#include <string>

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: VirtualAxis
		 *		Author: Mitchell Croft
		 *		Created: 30/01/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Provide a method for creating virtual axis' that will be monitored 
		 *		by the global Controllers object. The resulting values can be read
		 *		and used to create game input
		**/
		struct VirtualAxis {
			//! Store the ID the GamePad's to accept input from
			EGamePadID gamePad = EGamePadID::All;

			//! Store the name of the axis, used for accessing axis information
			std::string name = "";

			//! Store the additional strength applied to the input movements
			float sensitivity = 1.f;

			//! Store the pull strength applied to the value returning to zero
			float gravity = 1.f;

			//! Flags if the input should be inverted
			bool invert = false;

			//! Flags if the input type is a button or axis
			EGamePadInputType inputType = EGamePadInputType::Button;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Button Values-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			 
			//! Store the buttons which contribute a positive value to the axis
			EGamePadBtnCodes bPosBtn = EGamePadBtnCodes::Null_Input;
			EGamePadBtnCodes bAltPosBtn = EGamePadBtnCodes::Null_Input;

			//! Store the buttons which contribute a negative value to the axis
			EGamePadBtnCodes bNegBtn = EGamePadBtnCodes::Null_Input;
			EGamePadBtnCodes bAltNegBtn = EGamePadBtnCodes::Null_Input;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Axis Values-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//! Store the axis to take input from
			EGamePadAxisCodes aAxis = EGamePadAxisCodes::Null_Axis;

			//! Store the size of the dead zone applied to read axis (0 - 1)
			float aDeadZone = 0.1f;
		};
	}
}