#include "DebugCallback.hpp"

#include <stdio.h>
#include <stdarg.h>

//! Define the maximum length of a message that can be generated
#define DEBUG_MSG_MAX_LNG 1024

namespace SDL2_Engine {
	namespace Utilities {
		/*
			DebugCallback : Function Call Operator - Process a formatted string and supply it to the recipient callback
			Created: 13/07/2017
			Modified: 20/07/2017

			param[in] pFormat - A wide c-string that contains the text and formatting data to be output
			param[in] ... - The additional arguments to be inserted into the formatted string

			return bool - Returns true if the internal callback was raised

			Note:
			See http://www.cplusplus.com/reference/cstdio/printf/ for formatting options
		*/
		bool DebugCallback::operator()(const wchar_t* pFormat, ...) const {
			//Check if the format is valid and the callback is set
			if (!pFormat || !mPointer) return false;

			//Create a character buffer to hold the compiled message
			wchar_t* buffer = new wchar_t[DEBUG_MSG_MAX_LNG] { '\0' };

			//Create the parameter list 
			va_list args;
			va_start(args, pFormat);

			//Format the string into the buffer
			vswprintf_s(buffer, DEBUG_MSG_MAX_LNG, pFormat, args);

			//End the argument list
			va_end(args);

			//Send the message to the callback
			mPointer(buffer);

			//Delete the buffer
			delete[] buffer;

			//Return success
			return true;
		}
	}
}