#include "Keyboard.hpp"

//! Include the global values required to get time information
#include "../../Globals.hpp"
#include "../../Time.hpp"
#include "../../Window/Window.hpp"

//! Include the required STL objects
#include <bitset>
#include <array>

//! Define the size of a Keyboard state
#define KEYBOARD_STATE_SIZE 256U

//! Define the mask value for checking keyboard input
#define PRESSED_MASK 0x80
#define TOGGLED_MASK 0x01

//! Define the type to hold keyboard data
typedef unsigned char BYTE;

//! Label the different states to be handled
enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: KeyboardInternalData
		 *		Author: Mitchell Croft
		 *		Created: 25/05/2017
		 *		Modified: 22/09/2017
		 *		
		 *		Purpose:
		 *		Abstract the STL objects away from the main Keyboard
		 *		object to allow for simpler management of memory
		**/
		struct Keyboard::KeyboardInternalData {
			//! Store the keyboard state data
			BYTE keyboardStates[STATE_TOTAL][KEYBOARD_STATE_SIZE];

			//! Store timing information for the inputing of repeat values
			std::array<float, KEYBOARD_STATE_SIZE> repeatTimers;
			std::bitset<KEYBOARD_STATE_SIZE> repeatFlags;
		};

		/*
			Keyboard : verifyKeyboardInput - Verify the key value that is being tested for appending to the passed in string
			Created: 20/07/2017
			Modified: 15/10/2017

			param[in/out] pString - A reference to the standard string object to fill
			param[in] pKey - The virtual key that is being tested for inclusion
			param[in] pMaxLength - The maximum length that the string can be
			param[in] pFlags - Bitmask of EKeyboardInputFlags that define what characters are allowed

			return bool - Returns true if the pString object was modified in anyway
		*/
		bool Keyboard::verifyKeyboardInput(std::string& pString, const EKeyboardKeyCodes& pKey, const int& pMaxLength, const Utilities::Bitmask<EKeyboardInputFlags>& pFlags) const {
			//Check if the string has reached capacity
			if (pMaxLength >= 0 && pString.length() >= (unsigned int)pMaxLength && pKey != EKeyboardKeyCodes::Backspace)
				return false;

			//Store the size of the string prior to modification
			const size_t STRING_SIZE = pString.length();

			//Check if the key is a backspace
			if (pKey == EKeyboardKeyCodes::Backspace) {
				//Check if there are characters to remove
				if (STRING_SIZE) pString = pString.substr(0, (int)STRING_SIZE - 1);
			}

			//Otherwise convert the key to text
			else {
				//Create a buffer to hold the character
				WORD buffer = 0;

				//Convert the key
				ToAscii((int)pKey,
					MapVirtualKey((int)pKey, MAPVK_VK_TO_VSC),
					mData->keyboardStates[STATE_CUR],
					&buffer,
					0);

				//Check the flags to validate the character
				if (isprint(buffer) &&
					((isalpha(buffer) && pFlags & EKeyboardInputFlags::Alphabetical) ||
					(isdigit(buffer) && pFlags & EKeyboardInputFlags::Numerical) ||
						(buffer == VK_SPACE && pFlags & EKeyboardInputFlags::Space) ||
					pFlags & EKeyboardInputFlags::Special))
					pString += (char)buffer;
			}

			//Return if string length is different
			return (pString.length() != STRING_SIZE);
		}

		/*
			Keyboard : Constructor - Initialise with default values
			Created: 19/07/2017
			Modified: 22/09/2017
		*/
		Keyboard::Keyboard() : mData(nullptr) {}

		/*
			Keyboard : createInterface - Allocate the resources required for the Keyboard object
			Created: 19/07/2017
			Modified: 22/09/2017

			return bool - Returns true if the Keyboard object was created successfully
		*/
		bool Keyboard::createInterface() {
			//Create the internal data storage object
			mData = new KeyboardInternalData();

			//Return success 
			return true;
		}

		/*
			Keyboard : destroyInterface - Deallocate the resources used by the Keyboard object
			Created: 19/07/2017
			Modified: 25/07/2017
		*/
		void Keyboard::destroyInterface() { if (mData) delete mData; }

		/*
			Keyboard : update - Update the Keyboard input states
			Created: 19/07/2017
			Modified: 11/10/2017
		*/
		void Keyboard::update() {
			//Check that the Window has focus
			if (!Globals::get<Window>().hasProperty(EWindowProperties::Focus)) return;

			//Get the time object
			Time& time = Globals::get<Time>();

			//Poll keyboard update messages
			GetKeyState(NULL);

			//Copy the current keyboard state data
			memcpy_s(mData->keyboardStates[STATE_PRE], sizeof(BYTE) * KEYBOARD_STATE_SIZE, mData->keyboardStates[STATE_CUR], sizeof(BYTE) * KEYBOARD_STATE_SIZE);

			//Get the current keyboard state
			GetKeyboardState(mData->keyboardStates[STATE_CUR]);

			//Update the repeat key timers
			for (int i = (int)mData->repeatTimers.size() - 1; i >= 0; i--) {
				//Ensure the value isn't already at 0
				if (mData->repeatTimers[i]) {
					//Subtract the delta time
					mData->repeatTimers[i] -= time.getRealDelta();

					//Floor the value at 0
					if (mData->repeatTimers[i] < 0.f)
						mData->repeatTimers[i] = 0.f;
				}
			}
		}

		/*
			Keyboard : keyDown - Determine if a specified key value has been pressed
			Created: 19/07/2017
			Modified: 25/07/2017

			param[in] pKey - The EKeyboardKeyCodes value representing the key to check

			return bool - Returns true if the key is currently down
		*/
		bool Keyboard::keyDown(const EKeyboardKeyCodes& pKey) const noexcept { return ((mData->keyboardStates[STATE_CUR][(int)pKey] & PRESSED_MASK) != 0); }

		/*
			Keyboard : keyPressed - Determine if a specified key value was pressed this cycle
			Created: 19/07/2017
			Modified: 25/07/2017

			param[in] pKey - The EKeyboardKeyCodes value representing the key to check

			return bool - Returns true the first cycle where the key is pressed down
		*/
		bool Keyboard::keyPressed(const EKeyboardKeyCodes& pKey) const noexcept {
			return ((mData->keyboardStates[STATE_CUR][(int)pKey] & PRESSED_MASK) &&
				!(mData->keyboardStates[STATE_PRE][(int)pKey] & PRESSED_MASK));
		}

		/*
			Keyboard : keyReleased - Determine if a specified key value was released this cycle
			Created: 19/07/2017
			Modified: 25/07/2017

			param[in] pKey - The EKeyboardKeyCodes value representing the key to check

			return bool - Returns true the first cycle where the key has been released
		*/
		bool Keyboard::keyReleased(const EKeyboardKeyCodes& pKey) const noexcept {
			return (!(mData->keyboardStates[STATE_CUR][(int)pKey] & PRESSED_MASK) &&
				(mData->keyboardStates[STATE_PRE][(int)pKey] & PRESSED_MASK));
		}

		/*
			Keyboard : keyToggled - Determine if a specified key value is currently toggled
			Created: 19/07/2017
			Modified: 25/07/2017

			param[in] pKey - The EKeyboardKeyCodes value representing the key to check

			return bool - Returns true if the keys toggle state is active
		*/
		bool Keyboard::keyToggled(const EKeyboardKeyCodes& pKey) const noexcept { return ((mData->keyboardStates[STATE_CUR][(int)pKey] & TOGGLED_MASK) != 0); }

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
		bool Keyboard::modifyStringByKeyboard(std::string& pString, const int& pMaxLength, const Utilities::Bitmask<EKeyboardInputFlags>& pFlags) const {
			//Store a flag monitoring if the string has been changed
			bool modified = false;

			//Loop through a keys that need checking
			for (size_t i = 0; i < KEYBOARD_STATE_SIZE; i++) {
				//Test to see if the key has been pressed
				if (keyDown((EKeyboardKeyCodes)i)) {
					//Check to see if the timer is up for the key to be processed again
					if (!mData->repeatTimers[i] || !mData->repeatFlags[i]) {
						//Test to see if the string was modified with this press
						if (verifyKeyboardInput(pString, (EKeyboardKeyCodes)i, pMaxLength, pFlags))
							modified = true;

						//Set the repeat delay timer
						mData->repeatTimers[i] = (mData->repeatFlags[i] ? 0.05f : 0.5f);

						//Set the bit flag
						mData->repeatFlags[i] = true;
					}
				}

				//Otherwise reset the bit flag
				else mData->repeatFlags[i] = false;
			}

			//Return the modified flag
			return modified;
		}
	}
}