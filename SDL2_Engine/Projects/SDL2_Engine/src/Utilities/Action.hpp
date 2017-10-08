#pragma once

namespace SDL2_Engine {
	namespace Utilities {
		/*
		 *		Name: Action (Default)
		 *		Author: Mitchell Croft
		 *		Created: 30/06/2017
		 *		Modified: 13/07/2017
		 *
		 *		Purpose:
		 *		Provide a base point for the class specialisations to follow
		 *		on from. This is not a valid type and should not be used
		**/
		template<typename T>
		class Action {
		public:
			/*
				Action (Default) : Constructor - Unused, inclusion is to provide user help on failed compile
				Created: 13/07/2017
				Modified: 13/07/2017
			*/
			inline Action() {
				static_assert(false, "The Action object requires a template signature in the form of a function. E.g. Action<int(const char*, int, float)>");
			}
		};

		/*
		 *		Name: Action (Type Return)
		 *		Author: Mitchell Croft
		 *		Created: 30/06/2017
		 *		Modified: 13/07/2017
		 *
		 *		Purpose:
		 *		Store a function pointer that has a return type and an
		 *		indiscriminate amount of parameters
		**/
		template<typename TReturn, typename ... TArgs>
		class Action<TReturn(TArgs...)> {
		public:
			//! Define a type for the Action object
			typedef TReturn(*signature)(TArgs...);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Constructors-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Action (Type Return) : Constructor - Initialise with default values
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pFunc - A function to be used to receive the callback information
			*/
			inline Action(const signature& pFunc = nullptr) noexcept : mPointer(pFunc) {}

			/*
				Action (Type Return) : Copy Constructor - Copy the values of another Action object
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pCopy - The Action object to copy the values of
			*/
			inline Action(const Action& pCopy) noexcept : mPointer(pCopy.mPointer) {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Operators--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Action (Type Return) : Function Call Operator - Transfer data to the callback function (if set)
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pReturn - A reference to a value where the return of the function can be stored
				param[in] pArgs - The required parameters required to call the function

				return bool - Returns true if a callback was raised by this function
			*/
			inline bool operator() (TReturn& pReturn, TArgs&& ... pArgs) const { if (!mPointer) return false; pReturn = mPointer(pArgs...); return true; }

			/*
				Action (Type Return) : Assignment Operator - Copy the values of another Action object
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pCopy - The Action object to copy the values of

				return Action& - Returns a reference to the Action object being assigned
			*/
			inline Action& operator=(const Action& pCopy) noexcept { mPointer = pCopy.mPointer; return *this; }

			/*
				Action (Type Return) : Assignment Operator - Assign the callback pointer to a new function
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pFunc - A function pointer that matches the Actions signature

				return Action& - Returns a reference to the Action object being assigned
			*/
			inline Action& operator=(const signature& pFunc) noexcept { mPointer = pFunc; return *this; }

			/*
				Action (Type Return) : Bool Cast Operator - Implicit conversion for determining if the callback function is set
				Created: 30/06/2017
				Modified: 13/07/2017

				return bool - Returns true if the callback function has been set
			*/
			inline operator bool() const noexcept { return (mPointer != nullptr); }

		private:
			//! Store a pointer to the callback function
			signature mPointer;
		};

		/*
		 *		Name: Action (Void Return)
		 *		Author: Mitchell Croft
		 *		Created: 30/06/2017
		 *		Modified: 13/07/2017
		 *
		 *		Purpose:
		 *		Store a function pointer that has no return type and an
		 *		indiscriminate amount of parameters
		**/
		template<typename ... TArgs>
		class Action<void(TArgs...)> {
		public:
			//! Define a type for the Action object
			typedef void(*signature)(TArgs...);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Constructors-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Action (Void Return) : Constructor - Initialise with default values
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pFunc - A function to be used to receive the callback information
			*/
			inline Action(const signature& pFunc = nullptr) noexcept : mPointer(pFunc) {}

			/*
				Action (Void Return) : Copy Constructor - Copy the values of another Action object
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pCopy - The Action object to copy the values of
			*/
			inline Action(const Action& pCopy) noexcept : mPointer(pCopy.mPointer) {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Operators--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Action (Void Return) : Function Call Operator - Transfer data to the callback function (if set)
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pArgs - The required parameters required to call the function

				return bool - Returns true if a callback was raised by this function
			*/
			inline bool operator() (TArgs ... pArgs) const { if (!mPointer) return false; mPointer(pArgs...); return true; }

			/*
				Action (Void Return) : Assignment Operator - Copy the values of another Action object
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pCopy - The Action object to copy the values of

				return Action& - Returns a reference to the Action object being assigned
			*/
			inline Action& operator=(const Action& pCopy) noexcept { mPointer = pCopy.mPointer; return *this; }

			/*
				Action (Void Return) : Assignment Operator - Assign the callback pointer to a new function
				Created: 30/06/2017
				Modified: 13/07/2017

				param[in] pFunc - A function pointer that matches the Actions signature

				return Action& - Returns a reference to the Action object being assigned
			*/
			inline Action& operator=(const signature& pFunc) noexcept { mPointer = pFunc; return *this; }

			/*
				Action (Void Return) : Bool Cast Operator - Implicit conversion for determining if the callback function is set
				Created: 30/06/2017
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