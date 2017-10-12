#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "Controllers/ControllerValues.hpp"
#include "Keyboard/KeyboardValues.hpp"
#include "Mouse/MouseValues.hpp"
#include "../Utilities/Bitmask.hpp"
#include "../Utilities/FString.hpp"

//! Define a maximum length for Virtual Axis names
#define VIRTUAL_AXIS_NAME_MAX 32

namespace SDL2_Engine {
	namespace Input {
		//! Flag the input devices the Virtual Axis can use
		enum class EInputDevice { GamePads, Keyboard, Mouse };

		//! Flag the different input types available to the Gamepad
		enum class EGamePadInputType { Button, Axis };

		/*
		 *		Name: VirtualAxis
		 *		Author: Mitchell Croft
		 *		Created: 30/01/2017
		 *		Modified: 12/10/2017
		 *
		 *		Purpose:
		 *		Store value used to monitor a named virtual axis across either
		 *		gamepads, keyboard(s) or mice
		**/
		struct SDL2_LIB_INC VirtualAxis {
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Common Values-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store the Input device to monitor for this Virtual Axis
			EInputDevice inputType;

			//! Store the 'name' given to this Virtual Axis
			Utilities::FString<VIRTUAL_AXIS_NAME_MAX> name;

			//! Store the additional strength scale applied to the Input movements
			float sensitivity;

			//! Store the pull strength applied to the value when lacking Input
			float gravity;

			//! Flags if the input values should be inverted
			bool invert;

			union {
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////----------------------------------GamePad Values-------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				struct {
					//! Store the GamePads to check the input values from. Axis can only read from a single GamePad 
					Utilities::Bitmask<EGamePadID> gamePads;

					//! Store the GamePds input type 
					EGamePadInputType gamePadInputType;

					/////////////////////////////////////////////////////////////////////////////////////////////////////
					////////-----------------------------------Button Values-------------------------------------////////
					/////////////////////////////////////////////////////////////////////////////////////////////////////

					//! Store the buttons which contribute a positive value to the axis
					EGamePadBtnCodes bPosBtn;
					EGamePadBtnCodes bAltPosBtn;

					//! Store the buttons which contribute a negative value to the axis
					EGamePadBtnCodes bNegBtn;
					EGamePadBtnCodes bAltNegBtn;

					/////////////////////////////////////////////////////////////////////////////////////////////////////
					////////-------------------------------------Axis Values-------------------------------------////////
					/////////////////////////////////////////////////////////////////////////////////////////////////////
					
					//! Store the axis to take input from
					EGamePadAxisCodes aAxis;

					//! Store the size of the dead zone applied to read axis 
					float aDeadZone;
				};


				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////----------------------------------Keyboard Values------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				struct {
					//! Store the buttons which contribute a positive value to the axis
					EKeyboardKeyCodes kPosBtn;
					EKeyboardKeyCodes kAltPosBtn;

					//! Store the buttons which contribute a negative value to the axis
					EKeyboardKeyCodes kNegBtn;
					EKeyboardKeyCodes kAltNegBtn;
				};

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////------------------------------------Mouse Values-------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				
				//! Store the Mouse Axis to read from
				EMouseAxis mAxis;
			};

			/*
				VirtualAxis : Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			VirtualAxis();

			/*
				VirtualAxis : Copy Constructor - Initialise with the values of another VirtualAxis
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pCopy - The Virtual Axis object to copy
			*/
			VirtualAxis(const VirtualAxis& pCopy);
		};
	};
}
