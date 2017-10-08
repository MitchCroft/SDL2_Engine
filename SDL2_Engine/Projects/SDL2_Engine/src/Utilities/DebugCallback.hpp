#pragma once

#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace Utilities {
		/*
		 *		Name: DebugCallback
		 *		Author: Mitchell Croft
		 *		Created: 13/07/2017
		 *		Modified: 20/07/2017
		 *
		 *		Purpose:
		 *		Provide a uniform interface for providing formatted string
		 *		information to an unknown location via a user defined callback
		**/
		class SDL2_LIB_INC DebugCallback {
		public:
			//! Define the signature to be used for callback functions
			typedef void(*signature)(const wchar_t*);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Constructors-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				DebugCallback : Constructor - Initialise with default values
				Created: 13/07/2017
				Modified: 13/07/2017

				param[in] pFunc - A function to be used to receive the formatted string (Default nullptr)
			*/
			DebugCallback(const signature& pFunc = nullptr) noexcept : mPointer(pFunc) {}

			/*
				DebugCallback : Copy Constructor - Copy the values of another DebugCallback object
				Created: 13/07/2017
				Modified: 13/07/2017

				param[in] pCopy - The DebugCallback object to copy the values of
			*/
			DebugCallback(const DebugCallback& pCopy) noexcept : mPointer(pCopy.mPointer) {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Operators--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

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
			bool operator() (const wchar_t* pFormat, ...) const;

			/*
				DebugCallback : Assignment Operator - Copy the values of another DebugCallback object
				Created: 13/07/2017
				Modified: 13/07/2017

				param[in] pCopy - The DebugCallback object to copy the values of

				return DebugCallback& - Returns a reference to the DebugCallback object being assigned
			*/
			inline DebugCallback& operator=(const DebugCallback& pCopy) noexcept { mPointer = pCopy.mPointer; return *this; }

			/*
				DebugCallback : Assignment Operator - Assign the callback pointer to a new function
				Created: 13/07/2017
				Modified: 13/07/2017

				param[in] pFunc - A function pointer that matches the DebugCallbacks signature

				return DebugCallback& - Returns a reference to the DebugCallback object being assigned
			*/
			inline DebugCallback& operator=(const signature& pFunc) noexcept { mPointer = pFunc; return *this; }

			/*
				DebugCallback : Bool Cast Operator - Implicit conversion for determining if the callback function is set
				Created: 13/07/2017
				Modified: 13/07/2017

				return bool - Returns true if the callback function has been set
			*/
			inline operator bool() const noexcept { return (mPointer != nullptr); }

		private:

			//! Store a pointer to the callback function
			signature mPointer;
		};
	}
}