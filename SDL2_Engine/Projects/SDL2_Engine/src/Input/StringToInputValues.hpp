#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"

#include "AxisInputValues.hpp"
#include "Controllers/ControllerValues.hpp"
#include "Keyboard/KeyboardValues.hpp"
#include "Mouse/MouseValues.hpp"

//! Define the maximum length for the comparator string
#define INPUT_COMPARE_STRING_MAX 20

namespace SDL2_Engine {
	namespace Input {
		//! Export the required FString implementation
		template class SDL2_LIB_INC Utilities::FString<INPUT_COMPARE_STRING_MAX>;

		//! Create a type define for the compare String
		typedef Utilities::FString<INPUT_COMPARE_STRING_MAX> compareStr;

		/*
			stringToInputType - Convert a String value to a corresponding input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EInputDevice value

			return EInputDevice - Returns an EInputDevice value representing the passed in String value or EInputDevice::Null if not recognised
		*/
		extern SDL2_LIB_INC EInputDevice stringToInputType(const compareStr& pString);

		/*
			stringToGamePadID - Convert a String value to a corresponding GamePad ID value
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadID value
			
			return EGamePadID - Returns an EGamePadID value representing the passed in String value or EGamePadID::Null if not recognised
		*/
		extern SDL2_LIB_INC EGamePadID stringToGamePadID(const compareStr& pString);

		/*
			stringToGamePadInputType - Convert a String value to a corresponding GamePad input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadInputType value

			return EGamePadInputType - Returns an EGamePadInputType value representing the passed in String value or EGamePadInputType::Null if not recognised
		*/
		extern SDL2_LIB_INC EGamePadInputType stringToGamePadInputType(const compareStr& pString);

		/*
			stringToGamePadAxis - Convert a String value to a corresponding GamePad axis code
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadAxisCode value

			return EGamePadAxisCode - Returns an EGamePadAxisCode value representing the passed in String value or EGamePadAxisCode::Null if not recognised
		*/
		extern SDL2_LIB_INC EGamePadAxisCode stringToGamePadAxis(const compareStr& pString);

		/*
			stringToGamePadBtn - Convert a String value to a corresponding GamePad button 
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EGamePadBtnCodes value

			return EGamePadBtnCodes - Returns an EGamePadBtnCodes value representing the passed in String value or EGamePadBtnCodes::Null_Input if not recognised
		*/
		extern SDL2_LIB_INC EGamePadBtnCodes stringToGamePadBtn(const compareStr& pString);

		/*
			stringToKeyboardKey - Convert a String value to a corresponding Keyboard Key
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EKeyboardKeyCode value

			return EKeyboardKeyCode - Returns an EKeyboardKeyCode value representing the passed in String value or EKeyboardKeyCode::Null if not recognised
		*/
		extern SDL2_LIB_INC EKeyboardKeyCode stringToKeyboardKey(const compareStr& pString);

		/*
			stringToMouseInputType - Convert a String value to a corresponding Mouse Input type
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseInputType value

			return EMouseInputType - Returns an EMouseInputType value representing the passed in String value or EMouseInputType::Null if not recognised
		*/
		extern SDL2_LIB_INC EMouseInputType stringToMouseInputType(const compareStr& pString);

		/*
			stringToMouseButton - Convert a String value to a corresponding Mouse Button
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseButton value

			return EMouseButton - Returns an EMouseButton value representing the passed in String value or EMouseButton::Null if not recognised
		*/
		extern SDL2_LIB_INC EMouseButton stringToMouseButton(const compareStr& pString);

		/*
			stringToMouseAxis - Convert a String value to a corresponding Mouse Axis
			Author: Mitchell Croft
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pString - The String value to be converted to an EMouseAxis value

			return EMouseAxis - Returns an EMouseButton value representing the passed in String value or EMouseAxis::Null if not recognised
		*/
		extern SDL2_LIB_INC EMouseAxis stringToMouseAxis(const compareStr& pString);
	}
}