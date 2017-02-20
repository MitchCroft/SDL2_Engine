#include "InputManager.hpp"

#include "GamePad.hpp"
#include "VirtualAxis.hpp"
#include "VibrationDescription.hpp"

#include "../../Math.hpp"

#include <windows.h>

//! Define the size of a Keyboard state
#define KEYBOARD_STATE_SIZE 256U

//! Define the mask value for checking keyboard input
#define PRESSED_MASK 0x80
#define TOGGLED_MASK 0x01

namespace SDL2_Engine {

	//! Declare a simple enum to store GamePad indexes
	enum EGamePadIndexs { GAMEPAD_ONE, GAMEPAD_TWO, GAMEPAD_THREE, GAMEPAD_FOUR, GAMEPAD_TOTAL };

	//! Define a simple enum for tracking the states
	enum EState { STATE_CURRENT, STATE_PREVIOUS, STATE_TOTAL };

	//! Declare static values
	Input* Input::mInstance = nullptr;

	/*
	 *		Name: VibrationValues
	 *		Author: Mitchell Croft
	 *		Created: 13/02/2017
	 *		Modified: 13/02/2017
	 *
	 *		Purpose:
	 *		 Store additional information that can be abstracted
	 *		 from the user in the regular VibrationDescription
	 *		 object
	**/
	struct Input::VibrationValues {
		//! Store the vibration strength values (0 - 1)
		float leftVibration;
		float rightVibration;

		//! Store the duration for which the vibration should occur (Seconds)
		float vibrationLength;

		//! Store a timer to track the time spent vibrating
		float vibrationTimer;

		//! Store a function pointer for optional scaling of value over time
		VibrationScaleFunc scaleFunc;

		//! Quick use functions
		inline VibrationValues() : leftVibration(0.f), rightVibration(0.f), vibrationLength(0.f), vibrationTimer(0.f), scaleFunc(nullptr) {}
		inline VibrationValues(const VibrationDescription& pCopy) { *this = pCopy; }
		VibrationValues& operator=(const VibrationDescription& pCopy) {
			//Copy the vibration scales
			leftVibration = Math::clamp01(pCopy.leftVibration);		
			rightVibration = Math::clamp01(pCopy.rightVibration);	

			//Copy the duration value
			vibrationLength = pCopy.vibrationLength;

			//Copy the scaling function
			scaleFunc = pCopy.scaleFunc;

			//Reset the timer
			vibrationTimer = 0.f;

			//Return self
			return *this;
		}
	};

	/*
		Input : Constructor - Initialise with default values
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 14/02/2017
	*/
	Input::Input() :
		mGamePads(nullptr),
		mKeyboardStates(nullptr),
		mPollTimer(5.f),
		mPollInterval(5.f),
		INPUTABLE_KEY_VALUES({
		//! Alpha
		EKeyboardKeyCodes::A, EKeyboardKeyCodes::B, EKeyboardKeyCodes::C, EKeyboardKeyCodes::D,
		EKeyboardKeyCodes::E, EKeyboardKeyCodes::F, EKeyboardKeyCodes::G, EKeyboardKeyCodes::H,
		EKeyboardKeyCodes::I, EKeyboardKeyCodes::J, EKeyboardKeyCodes::K, EKeyboardKeyCodes::L,
		EKeyboardKeyCodes::M, EKeyboardKeyCodes::N, EKeyboardKeyCodes::O, EKeyboardKeyCodes::P,
		EKeyboardKeyCodes::Q, EKeyboardKeyCodes::R, EKeyboardKeyCodes::S, EKeyboardKeyCodes::T,
		EKeyboardKeyCodes::U, EKeyboardKeyCodes::V, EKeyboardKeyCodes::W, EKeyboardKeyCodes::X,
		EKeyboardKeyCodes::Y, EKeyboardKeyCodes::Z,

		//! Numerical
		EKeyboardKeyCodes::Num0, EKeyboardKeyCodes::Num1, EKeyboardKeyCodes::Num2, EKeyboardKeyCodes::Num3,
		EKeyboardKeyCodes::Num4, EKeyboardKeyCodes::Num5, EKeyboardKeyCodes::Num6, EKeyboardKeyCodes::Num7,
		EKeyboardKeyCodes::Num8, EKeyboardKeyCodes::Num9, EKeyboardKeyCodes::Num_Pad0,
		EKeyboardKeyCodes::Num_Pad1, EKeyboardKeyCodes::Num_Pad2, EKeyboardKeyCodes::Num_Pad3,
		EKeyboardKeyCodes::Num_Pad4, EKeyboardKeyCodes::Num_Pad5, EKeyboardKeyCodes::Num_Pad6,
		EKeyboardKeyCodes::Num_Pad7, EKeyboardKeyCodes::Num_Pad8, EKeyboardKeyCodes::Num_Pad9,

		//! Special Characters
		EKeyboardKeyCodes::Space, EKeyboardKeyCodes::Num_Pad_Multiply, EKeyboardKeyCodes::Num_Pad_Subtract, EKeyboardKeyCodes::Num_Pad_Decimal,
		EKeyboardKeyCodes::Num_Pad_Divide, EKeyboardKeyCodes::Semi_Colon, EKeyboardKeyCodes::Plus,
		EKeyboardKeyCodes::Comma, EKeyboardKeyCodes::Minus,	EKeyboardKeyCodes::Period,	EKeyboardKeyCodes::Slash_Forward,
		EKeyboardKeyCodes::Slash_Backward, EKeyboardKeyCodes::Tilde, EKeyboardKeyCodes::Square_Left,
		EKeyboardKeyCodes::Square_Right, EKeyboardKeyCodes::Quote,

		//! Control
		EKeyboardKeyCodes::Backspace,
	})
	{}

	/*
		Input : verifyKeyboardInput - Verify the key value that is being tested for appending to the passed std:;string
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in\out] pString - A reference to the standard string object to fill
		param[in] pKey - The Virtual Key that is being tested for inclusion
		param[in] pMaxLength - The maximum length that the string can be
		param[in] pFlags - Bitmask of EKeyboardInputFlags that define what characters are allowed
						   to be entered into the string

		return bool - Returns true if the pString reference was modified was in anyway
	*/
	bool Input::verifyKeyboardInput(std::string& pString, const EKeyboardKeyCodes& pKey, const int& pMaxLength, const Utilities::Bitmask<EKeyboardInputFlags, char>& pFlags) const {
		//Check if the string has reached capacity
		if (pMaxLength >= 0 && pString.size() >= (unsigned int)pMaxLength && pKey != EKeyboardKeyCodes::Backspace)
			return false;

		//Store the size of the string as it currently exists
		const unsigned int STRING_SIZE = pString.size();

		//If key is backspace
		if (pKey == EKeyboardKeyCodes::Backspace) {
			//Check if there are characters to remove
			if (STRING_SIZE) pString = pString.substr(0, (int)pString.size() - 1);
		}

		//Otherwise convert the key to Ascii
		else {
			//Create a buffer for the new character
			WORD buffer = 0;

			//Convert the virtual key to Ascii
			ToAscii((int)pKey,
				MapVirtualKey((int)pKey, MAPVK_VK_TO_VSC),
				mKeyboardStates[STATE_CURRENT],
				&buffer,
				0);

			//Check the flags to validate the character
			if ((isalpha(buffer) && pFlags & EKeyboardInputFlags::Alpha) ||
				((isdigit(buffer) && pFlags & EKeyboardInputFlags::Numerical)) ||
				((pFlags & EKeyboardInputFlags::Special)))
				pString += (char)buffer;
		}

		//Return string modification
		return (pString.size() != STRING_SIZE);
	}

	/*
		Input : init - Create and setup the singleton instance
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 14/02/2017

		return bool - Returns true if the Input singleton was created successfully
	*/
	bool Input::init() {
		//Check if singleton already exists
		if (mInstance) return false;

		//Create the singleton
		mInstance = new Input();

		//Create the GamePad objects
		mInstance->mGamePads = new _GamePad[4]{ 0, 1, 2, 3 };

		//Create the keyboard states
		mInstance->mKeyboardStates = new BYTE*[STATE_TOTAL];
		for (int i = STATE_CURRENT; i < STATE_TOTAL; i++) {
			mInstance->mKeyboardStates[i] = new BYTE[KEYBOARD_STATE_SIZE];
			memset(mInstance->mKeyboardStates[i], 0, sizeof(BYTE) * KEYBOARD_STATE_SIZE);
		}

		//Create the GamePad objects
		return ((mInstance->mGamePads = new _GamePad[4]{ 0, 1, 2, 3 }) != nullptr);
	}

	/*
		Input : destroy - De-allocate the memory associated with Input singleton
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 14/02/2017
	*/
	void Input::destroy() {
		//Check the singleton was created
		if (mInstance) {
			//Delete the Keyboard states
			if (mInstance->mKeyboardStates) {
				//Delete the individual states
				for (int i = STATE_CURRENT; i < STATE_TOTAL; i++)
					delete[] mInstance->mKeyboardStates[i];

				//Delete the group
				delete[] mInstance->mKeyboardStates;
			}

			//Delete the GamePads
			if (mInstance->mGamePads) delete[] mInstance->mGamePads;

			//Delete the singleton instance
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/*
		Input : update - Update the internal GamePad states and Virtual Axis values
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 14/02/2017

		param[in] pDelta - The delta time that is used to adjust the virtual axis values.
						   This can be scaled to replicate bullet time effects
		param[in] pRealDeltaTime - The unscaled delta time that is used to poll for new
								   connected GamePads at set intervals
	*/
	void Input::update(const float& pDelta, const float& pRealDelta) {
		//Update the polling timer
		mInstance->mPollTimer += pRealDelta;

		//Check if new GamePads should be found
		bool attemptReconnect = mInstance->mPollTimer >= mInstance->mPollInterval;

		//Reset the poll timer
		if (attemptReconnect) mInstance->mPollTimer = 0.f;

#pragma region Update Input Values
		//Loop through and update all controllers
		for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++)
			mInstance->mGamePads[i].update(attemptReconnect);

		//Poll keyboard update messages
		GetKeyState(NULL);

		//Copy previous state data 
		memcpy_s(mInstance->mKeyboardStates[STATE_PREVIOUS], sizeof(BYTE) * KEYBOARD_STATE_SIZE, mInstance->mKeyboardStates[STATE_CURRENT], sizeof(BYTE) * KEYBOARD_STATE_SIZE);

		//Get the current keyboard state
		GetKeyboardState(mInstance->mKeyboardStates[STATE_CURRENT]);

		//Update the repeat key timers
		for (int i = (int)mInstance->mRepeatKeyTimers.size() - 1; i >= 0; i--) {
			//Check the value isn't already at zero
			if (mInstance->mRepeatKeyTimers[i]) {
				//Subtract the delta time
				mInstance->mRepeatKeyTimers[i] -= pRealDelta;

				//Floor the value at 0
				if (mInstance->mRepeatKeyTimers[i] < 0.f)
					mInstance->mRepeatKeyTimers[i] = 0.f;
			}
		}
#pragma endregion

#pragma region Update Virtual Axis
		//Loop through all monitored axis
		for (size_t i = 0; i < mInstance->mMonitor.bucket_count(); i++) {
			//Get the first element
			auto bucket = mInstance->mMonitor.begin(i);

			//Get the final iterator
			auto end = mInstance->mMonitor.end(i);

			//Check there are elements in the bucket
			if (bucket == end) continue;

			//Store the name of the Virtual Axis
			const std::string V_AXIS_NAME = bucket->first;

			//Store the number of Axis contributing to this value
			size_t contributingAxis = 0U;

			//Store the strongest contributer
			float strongestAxis = 0.f;

			//Store the average gravity of all Virtual Axis for this name
			float gravAvg = 0.f;

			//Copy the previous value of the axis
			mInstance->mPreInputAxis[V_AXIS_NAME] = mInstance->mCurInputAxis[V_AXIS_NAME];

			//Loop through all monitored Virtual Axis with the same name
			for (; bucket != end; ++bucket) {
				//Add to the contributing Axis counter
				contributingAxis++;

				//Get A reference to the monitored axis
				const VirtualAxis& axis = bucket->second;

				//Add the gravity to the running sum
				gravAvg += axis.gravity;

				//Store the strength of this input
				float axisStrength = 0.f;

				///////////////////////////////////////////////////////////////////////////////////////////////
				////                                Input Is Based on Axis                                 ////
				///////////////////////////////////////////////////////////////////////////////////////////////
				if (axis.inputType == EGamePadInputType::Axis) {
					//Get the axis strength value
					const float AXIS_VALUE = getGamePadAxis(axis.aAxis, axis.gamePad);

					//Store the sign of the axis value
					const float SIGN = Math::sign(AXIS_VALUE); 

					//Store the dead zone squared
					const float DEAD_ZONE_SQ = axis.aDeadZone * axis.aDeadZone;

					//Normalise the axis value
					const float AXIS_NORM_SQ = (AXIS_VALUE * AXIS_VALUE) - DEAD_ZONE_SQ;

					//Scale the Axis strength based on the dead zone
					if (AXIS_NORM_SQ > 0.f)
						axisStrength += (AXIS_NORM_SQ / (1.f - DEAD_ZONE_SQ)) * SIGN;
				}

				///////////////////////////////////////////////////////////////////////////////////////////////
				////                              Input Is Based on Buttons                                ////
				///////////////////////////////////////////////////////////////////////////////////////////////
				else {
					//Check for positive button press(es)
					if (getGamePadBtnDown({ axis.bPosBtn, axis.bAltPosBtn }, axis.gamePad))
						axisStrength += 1.f;

					//Check for negative button press(es)
					if (getGamePadBtnDown({ axis.bNegBtn, axis.bAltNegBtn }, axis.gamePad))
						axisStrength -= 1.f;
				}

				//Apply the sensitivity and inversion flag
				axisStrength *= axis.sensitivity * (axis.invert ? -1.f : 1.f);

				//Check if the strength is stronger then the current best
				if (abs(axisStrength) > abs(strongestAxis)) strongestAxis = axisStrength;
			}

			//If there is a strength value to be included
			if (strongestAxis && abs(strongestAxis) >= abs(mInstance->mCurInputAxis[V_AXIS_NAME])) {
				//Add the strength value
				mInstance->mCurInputAxis[V_AXIS_NAME] += strongestAxis * pDelta;

				//Clamp the value to -1 to 1 range
				if (abs(mInstance->mCurInputAxis[V_AXIS_NAME]) > 1.f)
					mInstance->mCurInputAxis[V_AXIS_NAME] /= abs(mInstance->mCurInputAxis[V_AXIS_NAME]);
			}

			//Otherwise apply gravity
			else if (gravAvg && mInstance->mCurInputAxis[V_AXIS_NAME]) {
				//Get the inverse direction
				const float INV_DIR = Math::sign(mInstance->mCurInputAxis[V_AXIS_NAME]) * -1.f; 

				//Take the average of the gravity value
				gravAvg /= (float)contributingAxis;

				//Get the value after gravity is applied
				float appliedVal = mInstance->mCurInputAxis[V_AXIS_NAME] + gravAvg * INV_DIR * pDelta;

				//Assign the Axis value
				mInstance->mCurInputAxis[V_AXIS_NAME] = (Math::sign(appliedVal) == INV_DIR ? 0.f : appliedVal); 
			}
		}
#pragma endregion

#pragma region Update Vibration Values
		//Check there are vibration values to process
		if (mInstance->mVibrationValues.size()) {
			//Add to the vibration timer
			mInstance->mVibrationTimer += pRealDelta;

			//Check if the timer is over the frequency value
			if (mInstance->mVibrationTimer >= mInstance->VIBRATION_FREQUANCY) {
				//Spend the built up time
				float integral;
				mInstance->mVibrationTimer = modf(mInstance->mVibrationTimer / mInstance->VIBRATION_FREQUANCY, &integral) * mInstance->VIBRATION_FREQUANCY;

				//Loop through all vibration values
				for (auto it = mInstance->mVibrationValues.begin(); it != mInstance->mVibrationValues.end();) {
					//Get a reference to the Vibration Value object
					VibrationValues& val = it->second;

					//Add the elapsed time to the timer
					val.vibrationTimer += mInstance->VIBRATION_FREQUANCY * integral;

					//Get the controller that is being accessed
					GamePad gamePad = getGamePad(it->first);

					//Check the controller is connected
					if (gamePad->mConnected) {
						//Store the scale factor
						float scale;

						//Check if the time is up
						if (val.vibrationTimer >= val.vibrationLength)
							scale = 0.f;

						//Otherwise get the scale from the settings
						else scale = (val.scaleFunc ? Math::clamp01(val.scaleFunc(val.vibrationTimer / val.vibrationLength)) : 1.f); 

						//Apply the vibration to the Game Pad
						gamePad->vibrate(val.leftVibration * scale, val.rightVibration * scale);

						//Check for removal
						if (val.vibrationTimer >= val.vibrationLength)
							mInstance->mVibrationValues.erase(it++);
						else ++it;
					}
				}
			}
		}
#pragma endregion
	}

	/*
		Input : setPollInterval - Set the amount of time between checks for recently connected GamePads
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pInterval - A float value defining the time (in seconds) between GamePad polling
	*/
	void Input::setPollInterval(const float& pInterval) { mInstance->mPollInterval = (pInterval >= 0.f ? pInterval : 0.f); }

	/*
		Input : getVAxis - Retrieve the value of a Virtual Axis being monitored
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to retrieve

		return const float& - Returns the value of the Virtual Axis as a const float reference
	*/
	const float& Input::getVAxis(const char* pAxis) { return mInstance->mCurInputAxis[pAxis]; }

	/*
		Input : getVAxisDelta - Retrieve the change in axis value since the last cycle
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to retrieve

		return float - Returns a float value containing the change in the Virtual Axis' value
	*/
	float Input::getVAxisDelta(const char* pAxis) { return (mInstance->mCurInputAxis[pAxis] - mInstance->mPreInputAxis[pAxis]); }

	/*
		Input : getVBtnDown - Treat the Virtual Axis as a button and check if it is currently 'down'
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to retrieve

		return bool - Returns true if the Virtual Axis is not equal to zero
	*/
	bool Input::getVBtnDown(const char* pAxis) { return (mInstance->mCurInputAxis[pAxis] != 0.f); }

	/*
		Input : getVBtnPressed - Treat the Virtual Axis as a button and check if it was 'pressed'
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to retrieve

		return bool - Returns true the first cycle the Virtual Axis is not equal to zero
	*/
	bool Input::getVBtnPressed(const char* pAxis) { return ((mInstance->mCurInputAxis[pAxis] != 0.f) && (mInstance->mPreInputAxis[pAxis] == 0.f)); }

	/*
		Input : getVBtnReleased - Treat the Virtual Axis as a button and check if it was 'released'
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to retrieve

		return bool - Returns true the first cycle the Virtual Axis is equal to zero
	*/
	bool Input::getVBtnReleased(const char* pAxis) { return ((mInstance->mCurInputAxis[pAxis] == 0.f) && (mInstance->mPreInputAxis[pAxis] != 0.f)); }

	/*
		Input : addVirtualAxis - Add a new Virtual Axis description to the monitor list
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - The VirtualAxis object describing the new Virtual Axis
	*/
	void Input::addVirtualAxis(const VirtualAxis& pAxis) { mInstance->mMonitor.insert(std::pair<std::string, VirtualAxis>(pAxis.name, pAxis)); }

	/*
		Input : addVirtualAxis - Add an array of new Virtual Axis descriptions to the monitor list
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pArray - A pointer to the array of VirtualAxis objects to monitor
		param[in] pCount - The number of VirtualAxis objects there are to add
	*/
	void Input::addVirtualAxis(const VirtualAxis* pArray, const size_t& pCount) { for (size_t i = 0; i < pCount; i++) mInstance->mMonitor.insert(std::pair<std::string, VirtualAxis>(pArray[i].name, pArray[i])); }

	/*
		Input : removeVirtualAxis - Clear all VirtualAxis objects with specific name
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - A c-string defining the name of the Axis to clear
	*/
	void Input::removeVirtualAxis(const char* pAxis) { mInstance->mMonitor.erase(pAxis); mInstance->mPreInputAxis.erase(pAxis); mInstance->mCurInputAxis.erase(pAxis); }

	/*
		Input : removeAxis - Clear all virtual axis within the Input singleton
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017
	*/
	void Input::removeVirtualAxis() { mInstance->mMonitor.clear(); mInstance->mPreInputAxis.clear(); mInstance->mCurInputAxis.clear(); }

	/*
		Input : getGamePad - Get a pointer to specific GamePad object
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pID - The EGamePadID of the GamePad you want a reference to

		return GamePad - Returns a GamePad (const _GamePad*) object to be used for testing input
	*/
	GamePad Input::getGamePad(EGamePadID pID) { return &(mInstance->mGamePads[(int)(log((int)pID) / log(2))]); }

	/*
		Input : appllyVibration - Add a vibration description to the Input singleton, describing how GamePad(s)
								  should vibrate
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pDesc - A VibrationDescription object describing how the Vibration will occur
	*/
	void Input::applyVibration(const VibrationDescription& pDesc) {
		//Loop through the bitmask
		for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
			//Check if the GamePad is in the mask
			if (pDesc.gamePad & (1 << i))
				mInstance->mVibrationValues.insert(std::pair<EGamePadID, VibrationValues>((EGamePadID)(1 << i), pDesc));
		}
	}

	/*
		Input : resetVibration - Reset the vibration of the specified GamePad(s)
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 13/02/2017

		param[in] pIDs - A Bitmask of the EGamePadID values to reset
	*/
	void Input::resetVibration(const Utilities::Bitmask<EGamePadID>& pIDs) {
		//Create a simple description
		VibrationDescription desc;

		//Apply the mask
		desc.gamePad = pIDs;

		//Apply the vibration to those GamePads
		applyVibration(desc);
	}

	/*
		Input : getGamePadAxis - Retrieve a raw axis value from a specific GamePad
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pAxis - The EGamePadAxisCode value to retrieve
		param[in] pID - The EGamePadID value defining the controller to retrieve the input from

		return const float& - Returns the axis value as a const float reference with the range -1 to 1
	*/
	const float& Input::getGamePadAxis(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) { return mInstance->mGamePads[(int)(log((int)pID) / log(2))].getRawAxis(pAxis); }

	/*
		Input : getGamePadAxisDelta - Retrieve the change in raw axis value from a specific GamePad
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pAxis - The EGamePadAxisCode value to check
		param[in] pID - The EGamePadID value defining the controller to retrieve the input from

		return float - Returns a float value containing the change in the specified axis' value
	*/
	float Input::getGamePadAxisDelta(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) { return mInstance->mGamePads[(int)(log((int)pID) / log(2))].getRawAxisDelta(pAxis); }

	/*
		Input : getGamePadBtnDown - Test to see if the specified GamePads are pressing at least one
									of the specified buttons
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
		param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePads to check

		return int - Returns an int bitmask that flags that different controllers that are pressing
					 at least one of the specified buttons
	*/
	int Input::getGamePadBtnDown(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EControllerID::All*/) {
		//Create bitmask
		int mask = 0;

		//Test the controllers 
		for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
			if (pIDs & (1 << i) &&						//GamePad is in the mask
				mInstance->mGamePads[i].mConnected &&	//GamePad is connected
				mInstance->mGamePads[i].btnDown(pBtns)) //GamePad has the buttons down
				mask |= (1 << i);
		}

		//Return the mask
		return mask;
	}

	/*
		Input : getGamePadBtnPressed - Test to see if the specified GamePads have just pressed at least one
									   of the specified buttons after all buttons were previously released
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
		param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePads to check

		return int - Returns an int bitmask that flags that different controllers that are have just
					 pressed at least one of the specified buttons
	*/
	int Input::getGamePadBtnPressed(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EControllerID::All*/) {
		//Create bitmask
		int mask = 0;

		//Test the controllers 
		for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
			if (pIDs & (1 << i) &&							//GamePad is in the mask
				mInstance->mGamePads[i].mConnected &&		//GamePad is connected
				mInstance->mGamePads[i].btnPressed(pBtns))	//GamePad has pressed a button
				mask |= (1 << i);
		}

		//Return the mask
		return mask;
	}

	/*
		Input : getGamePadBtnReleased - Test to see if the specified GamePads have just released at least one
										of the specified buttons after all buttons were previously pressed
		Author: Mitchell Croft
		Created: 13/02/2017
		Modified: 13/02/2017

		param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
		param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePads to check

		return int - Returns an int bitmask that flags that different controllers that are have just
					 released at least one of the specified buttons
	*/
	int Input::getGamePadBtnReleased(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EControllerID::All*/) {
		//Create bitmask
		int mask = 0;

		//Test the controllers 
		for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
			if (pIDs & (1 << i) &&							//GamePad is in the mask
				mInstance->mGamePads[i].mConnected &&		//GamePad is connected
				mInstance->mGamePads[i].btnReleased(pBtns))	//GamePad has released a button
				mask |= (1 << i);
		}

		//Return the mask
		return mask;
	}

	/*
		Input : getKeyboardKeyDown - Test to see if a specific key is currently down on the keyboard
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pKey - An EKeyboardKeyCode specifying the key to check

		return bool - Returns true if the key is currently down
	*/
	bool Input::getKeyboardKeyDown(const EKeyboardKeyCodes& pKey) { return ((mInstance->mKeyboardStates[STATE_CURRENT][(int)pKey] & PRESSED_MASK) != 0); }

	/*
		Input : getKeyboardKeyPressed - Test to see if the specified key was pressed this cycle
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pKey - An EKeyboardKeyCode specifying the key to check

		return bool - Returns true the first cycle this key is pressed down
	*/
	bool Input::getKeyboardKeyPressed(const EKeyboardKeyCodes& pKey) {
		return ((mInstance->mKeyboardStates[STATE_CURRENT][(int)pKey] & PRESSED_MASK) &&
			!(mInstance->mKeyboardStates[STATE_PREVIOUS][(int)pKey] & PRESSED_MASK));
	}

	/*
		Input : getKeyboardKeyReleased - Test to see if the specified key was released this cycle
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pKey - An EKeyboardKeyCode specifying the key to check

		return bool - Returns true the first cycle this key is released
	*/
	bool Input::getKeyboardKeyReleased(const EKeyboardKeyCodes& pKey) {
		return (!(mInstance->mKeyboardStates[STATE_CURRENT][(int)pKey] & PRESSED_MASK) &&
			(mInstance->mKeyboardStates[STATE_PREVIOUS][(int)pKey] & PRESSED_MASK));
	}

	/*
		Input : getKeyboardKeyToggled - Test to see if the specified key has been toggled
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in] pKey - An EKeyboardKeyCode specifying the key to check

		return bool - Returns true if the specified key has the toggle flag
	*/
	bool Input::getKeyboardKeyToggled(const EKeyboardKeyCodes& pKey) { return ((mInstance->mKeyboardStates[STATE_CURRENT][(int)pKey] & TOGGLED_MASK) != 0); }

	/*
		Input : modifyStringByKeyboard - Add characters to a standard string object based on the
										 keyboard keys pressed
		Author: Mitchell Croft
		Created: 14/02/2017
		Modified: 14/02/2017

		param[in\out] pString - A reference to the standard string object to fill
		param[in] pMaxLength - The maximum length that the string can be (Less then 0 means
							   no character limit, Default -1)
		param[in] pFlags - Bitmask of EKeyboardInputFlags that define what characters are allowed
						   to be entered into the string (Default EKeyboardInputFlags::All)

		return bool - Returns true if the pString reference was modified was in anyway
	*/
	bool Input::modifyStringByKeyboard(std::string& pString, const int& pMaxLength /*= -1*/, const Utilities::Bitmask<EKeyboardInputFlags, char>& pFlags /*= EKeyboardInputFlags::All*/) {
		//Store a flag monitoring if the string has been changed
		bool modified = false;

		//Loop through all Keys to check
		for (int i = (int)mInstance->INPUTABLE_KEY_VALUES.size() - 1; i >= 0; i--) {
			//Test to see if the key has been pressed
			if (mInstance->mKeyboardStates[STATE_CURRENT][(int)mInstance->INPUTABLE_KEY_VALUES[i]] & PRESSED_MASK) {
				//Check if the timer is up for the key to be processed again
				if (!mInstance->mRepeatKeyTimers[i] || !mInstance->mRepeatFlags[i]) {
					//Test to see if the string was modified with this press
					if (mInstance->verifyKeyboardInput(pString, mInstance->INPUTABLE_KEY_VALUES[i], pMaxLength, pFlags))
						modified = true;

					//Set the the repeat timer delay
					mInstance->mRepeatKeyTimers[i] = (mInstance->mRepeatFlags[i] ? 0.05f : 0.5f);

					//Set the bit flag
					mInstance->mRepeatFlags[i] = true;
				}
			}

			//Otherwise reset the bit flag
			else mInstance->mRepeatFlags[i] = false;
		}

		//Return the modified flag
		return modified;
	}
}
