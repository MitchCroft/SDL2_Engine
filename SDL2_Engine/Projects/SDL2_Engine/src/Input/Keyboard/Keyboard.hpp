#pragma once

//! Include the required SDL2_Engine objects
#include "../../__LibraryManagement.hpp"
#include "../../Utilities/IGlobal.hpp"
#include "../../Utilities/Bitmask.hpp"
#include "KeyboardValues.hpp"

//! Include the string object
#include <string>

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: Keyboard
		 *		Author: Mitchell Croft
		 *		Created: 19/07/2017
		 *		Modified: 15/10/2017
		 *
		 *		Purpose:
		 *		Handle changes in keyboard input used for the modification of
		 *		strings of information
		**/
		class SDL2_LIB_INC Keyboard : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Get Input Values---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Keyboard : keyDown - Determine if a specified key value has been pressed
				Created: 19/07/2017
				Modified: 25/07/2017

				param[in] pKey - The EKeyboardKeyCodes value representing the key to check

				return bool - Returns true if the key is currently down
			*/
			bool keyDown(const EKeyboardKeyCodes& pKey) const noexcept;

			/*
				Keyboard : keyPressed - Determine if a specified key value was pressed this cycle
				Created: 19/07/2017
				Modified: 25/07/2017

				param[in] pKey - The EKeyboardKeyCodes value representing the key to check

				return bool - Returns true the first cycle where the key is pressed down
			*/
			bool keyPressed(const EKeyboardKeyCodes& pKey) const noexcept;

			/*
				Keyboard : keyReleased - Determine if a specified key value was released this cycle
				Created: 19/07/2017
				Modified: 25/07/2017

				param[in] pKey - The EKeyboardKeyCodes value representing the key to check

				return bool - Returns true the first cycle where the key has been released
			*/
			bool keyReleased(const EKeyboardKeyCodes& pKey) const noexcept;

			/*
				Keyboard : keyToggled - Determine if a specified key value is currently toggled
				Created: 19/07/2017
				Modified: 25/07/2017

				param[in] pKey - The EKeyboardKeyCodes value representing the key to check

				return bool - Returns true if the keys toggle state is active
			*/
			bool keyToggled(const EKeyboardKeyCodes& pKey) const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Text Modification Function----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Keyboard : modifyStringByKeyboard - Add characters to a standard wide string object based on the keys pressed
				Created: 20/07/2017
				Modified: 15/10/2017

				param[in/out] pString - A reference to the string object to me modified
				param[in] pMaxLength - The maximum length that the string can be 
									   (Negative indicates no character limit, Default -1)
				param[in] pFlags - Bitmask of EKeyboardInputFlags that define what characters are
								   allowed to be entered into the string (Default EKeyboardInputFlags::All)

				return bool - Returns true if the pString object was modified in any way
			*/
			bool modifyStringByKeyboard(std::string& pString, 
										const int& pMaxLength = -1, 
										const Utilities::Bitmask<EKeyboardInputFlags>& pFlags = EKeyboardInputFlags::All) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Keyboard : Constructor - Initialise with default values
				Created: 19/07/2017
				Modified: 22/09/2017
			*/
			Keyboard();

			/*
				Keyboard : createInterface - Allocate the resources required for the Keyboard object
				Created: 19/07/2017
				Modified: 22/09/2017

				return bool - Returns true if the Keyboard object was created successfully
			*/
			bool createInterface() override;

			/*
				Keyboard : destroyInterface - Deallocate the resources used by the Keyboard object
				Created: 19/07/2017
				Modified: 25/07/2017
			*/
			void destroyInterface() override;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Main Function----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Keyboard : update - Update the Keyboard input states
				Created: 19/07/2017
				Modified: 11/10/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements of the Keyboard
			struct KeyboardInternalData;
			KeyboardInternalData* mData;

			//! Convert key values to text and append to a string
			bool verifyKeyboardInput(std::string& pString, const EKeyboardKeyCodes& pKey, const int& pMaxLength, const Utilities::Bitmask<EKeyboardInputFlags>& pFlags) const;
		};
	}
}