#include "InputManager.hpp"

#define NOMINMAX
#include <windows.h>

#include <Xinput.h>

#include <limits>

#include "../../Math.hpp"

#include "VirtualAxis.hpp"
#include "VibrationSetting.hpp"

//! Define the two different states
#define CURRENT_STATE 0
#define PREVIOUS_STATE 1
#define STATE_TOTAL 2

//! Provide a macro for converting to controller state index's
#define GET_IND(ID, STATE) ((ID) * STATE_TOTAL + (STATE))

//! Define the total number of axis
#define AXIS_TOTAL ((int)EControllerAxisCodes::Right_Y + 1)

//! Define the ID for all controllers connected
#define MAX_CONNECTED_CONTROLLER_ID 15

namespace SDL2_Engine {

	//! Define static variables
	Input* Input::mInstance = nullptr;

	/*
	 *		Name: ControllerState
	 *		Author: Mitchell Croft
	 *		Created: 31/01/2017
	 *		Modified: 31/01/2017
	 *		
	 *		Purpose:
	 *		Store a state for the connected controllers, in
	 *		order for the polling of different
	**/
	class Input::ControllerState {
	public:
		//! Store the current input ID for this state
		DWORD packetNumber;

		//! Store the axis information as separate floats
		float axisValues[AXIS_TOTAL];

		//! Store the bitmask for the controller buttons
		WORD buttonMask;

		//! Setup as blank
		ControllerState() : packetNumber(0), buttonMask(0) { memset(axisValues, 0, sizeof(float) * AXIS_TOTAL); }

		//! Remove the copy constructor
		ControllerState(const ControllerState&) = delete;

		//! Reset all state values back to default
		inline ControllerState& reset() { packetNumber = 0; buttonMask = 0; memset(axisValues, 0, sizeof(float) * AXIS_TOTAL); return *this; }

		//! Copy the values of another state
		ControllerState& operator=(const ControllerState& pCopy) {
			packetNumber = pCopy.packetNumber;
			memcpy_s(axisValues, sizeof(float) * AXIS_TOTAL, pCopy.axisValues, sizeof(float) * AXIS_TOTAL);
			buttonMask = pCopy.buttonMask;
			return *this;
		}

		//! Copy the values of a XINPUT_STATE
		ControllerState& operator=(const XINPUT_STATE& pState) {
			packetNumber = pState.dwPacketNumber;
			buttonMask = pState.Gamepad.wButtons;

			//Get the maximum value for the trigger values
			const float TRIGGER_MAX = (float)std::numeric_limits<BYTE>::max();

			// Convert the trigger values
			axisValues[(int)EControllerAxisCodes::Left_Trigger] = (float)pState.Gamepad.bLeftTrigger / TRIGGER_MAX;
			axisValues[(int)EControllerAxisCodes::Right_Trigger] = (float)pState.Gamepad.bRightTrigger / TRIGGER_MAX;

			//Get the maximum value for the thumbstick values
			const float THUMBSTICK_MAX = (float)std::numeric_limits<SHORT>::max();

			//Convert the thumbstick values
			axisValues[(int)EControllerAxisCodes::Left_X] = (float)pState.Gamepad.sThumbLX / THUMBSTICK_MAX;
			axisValues[(int)EControllerAxisCodes::Left_Y] = (float)pState.Gamepad.sThumbLY / THUMBSTICK_MAX;
			axisValues[(int)EControllerAxisCodes::Right_X] = (float)pState.Gamepad.sThumbRX / THUMBSTICK_MAX;
			axisValues[(int)EControllerAxisCodes::Right_Y] = (float)pState.Gamepad.sThumbRY / THUMBSTICK_MAX;

			return *this;
		}
	};

	/*
	 *		Name: VibrationValues
	 *		Author: Mitchell Croft
	 *		Created: 10/02/2017
	 *		Modified: 10/02/2017
	 *		
	 *		Purpose:
	 *		Store additional information that can be abstracted away 
	 *		from the user in the regular VibrationSetting struct object.
	**/
	class Input::VibrationValues {
	public:
		//! Store the vibration strength scale values (0 - 1)
		float leftVibration;
		float rightVibration;

		//! Store the duration for which the vibration should occur	(Seconds)
		float vibrationLength;

		//! Store a timer to track the time spent vibrating
		float vibrationTimer;

		//! Store a function pointer for optional scaling of value over time
		VibrationScaleFunc scaleFunc;

		//! Quick assignment values
		inline VibrationValues() : leftVibration(0.f), rightVibration(0.f), vibrationLength(0.f), vibrationTimer(0.f), scaleFunc(nullptr) {}
		inline VibrationValues(const VibrationSetting& pCopy) { *this = pCopy; }
		VibrationValues& operator=(const VibrationSetting& pCopy) {
			//Copy the vibration scales
			leftVibration = Math::clamp01(pCopy.leftVibration);
			rightVibration = Math::clamp01(pCopy.rightVibration);

			//Copy the duration value
			vibrationLength = Math::max(0.f, pCopy.vibrationLength);

			//Copy the scaling function
			scaleFunc = pCopy.scaleFunc;

			//Reset the timer
			vibrationTimer = 0.f;

			//Return itself
			return *this;
		}

		//! Delete unused values
		inline VibrationValues(const VibrationValues&) = delete;
		inline VibrationValues& operator=(const VibrationValues&) = delete;
	};

	/*
		Input : Constructor - Initialise with default values
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017
	*/
	Input::Input() : mConnectedControllers(0), mControllerStates(nullptr), mVibrationValues((int)EControllerID::TOTAL), mPollTimer(5.f), mPollInterval(5.f) { mControllerStates = new ControllerState[(int)EControllerID::TOTAL * STATE_TOTAL]; }

	/*
		Input : Destructor - Clear allocated memory
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017
	*/
	Input::~Input() { delete[] mControllerStates; }

	/*
		Input : init - Initialise the Input Manager with default values
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		return bool - Returns true if the Input Manager was created with this call
	*/
	bool Input::init() { return (!mInstance && (mInstance = new Input())); }

	/*
		Input : destroy - Delete the memory associated with the Input Manager
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017
	*/
	void Input::destroy() { if (mInstance) delete mInstance; mInstance = nullptr; }

	/*
		Input : update - Update the internal controller states and update the virtual axis
		Author: Mitchell Croft
		Created: 01/02/2017
		Modified: 01/02/2017

		param[in] pDelta - The delta time that is used to adjust the virtual axis values.
						   This can be scaled to replicate bullet time effects
		param[in] pRealDeltaTime - The unscaled delta time that is used to poll for new 
								   connected controllers at set intervals
	*/
	void Input::update(const float& pDelta, const float& pRealDelta) {
		#pragma region Find Connected Controllers
		//Check if there are disconnected controllers
		if (mInstance->mConnectedControllers != MAX_CONNECTED_CONTROLLER_ID) {
			//Add onto the timer value
			mInstance->mPollTimer += pRealDelta;

			//Check if polling should occur
			if (mInstance->mPollTimer >= mInstance->mPollInterval) {
				//Reset the poll timer
				mInstance->mPollTimer = 0.f;

				//Loop through to search for connected controllers
				DWORD result;
				for (DWORD i = 0; i < (int)EControllerID::TOTAL; i++) {
					//Check if there is already a controller connected
					if (mInstance->mConnectedControllers & (1 << i)) continue;

					//Clear previously stored struct information
					XINPUT_STATE state;
					ZeroMemory(&state, sizeof(XINPUT_STATE));

					//Get the current controller index's state
					result = XInputGetState(i, &state);

					//If a controller was found add it to the list
					if (result == ERROR_SUCCESS) mInstance->mConnectedControllers |= (1 << i);
				}
			}
		}
		#pragma endregion

		#pragma region Update State Information
		//Loop through all connected controllers
		DWORD result;
 		for (DWORD i = 0; i < (int)EControllerID::TOTAL; i++) {
			//Transfer the current state values to the previous states
			if (mInstance->mControllerStates[GET_IND(i, PREVIOUS_STATE)].packetNumber != mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].packetNumber)
				mInstance->mControllerStates[GET_IND(i, PREVIOUS_STATE)] = mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)];

			//Check if the controller is connected
			if (mInstance->mConnectedControllers & (1 << i)) {
				//Clear the previously stored struct information
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				//Get the current controller index's state
				result = XInputGetState(i, &state);

				//Check if the controller has been disconnected
				if (result != ERROR_SUCCESS) {
					//Reset the current controller state
					mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].reset();

					//Remove the current controllers bit from the connected controllers
					mInstance->mConnectedControllers ^= (1 << i);

					//Update the next controller
					continue;
				}

				//Update the current state information
				if (mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].packetNumber != state.dwPacketNumber)
					mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)] = state;
			}
		}
		#pragma endregion

		#pragma region Update Virtual Axis
		//Loop through all monitored axis
		for (uint i = 0; i < mInstance->mMonitorAxis.bucket_count(); i++) {
			//Get the first element
			auto bucket = mInstance->mMonitorAxis.begin(i);

			//Get the final iterator
			auto end = mInstance->mMonitorAxis.end(i);

			//Check there are elements in the bucket
			if (bucket == end) continue;

			//Store the name of the virtual axis
			const std::string V_AXIS = bucket->first;

			//Store the number of axis contributing to this value
			uint contributingAxis = 0U;

			//Store the strongest contributer
			float strongestAxis = 0.f;

			//Store the average gravity of all Virtual Axis
			float gravAvg = 0.f;

			//Copy the previous value of the axis 
			mInstance->mPreInputAxis[V_AXIS] = mInstance->mCurInputAxis[V_AXIS];

			//Loop through all monitored virtual axis with the same name
			for (; bucket != end; ++bucket) {
				//Add to the contributing axis counter
				contributingAxis++;

				//Get a reference to the monitored axis
				const VirtualAxis& axis = bucket->second;

				//Add the gravity to the running sum
				gravAvg += axis.gravity;

				//Store the strength of this input
				float axisStrength = 0.f;

				//If input is based on a axis
				if (axis.inputType == EAxisInputType::Axis) {
					//Get the axis strength value
					const float AXIS_VALUE = getAxisRaw(axis.aAxis, axis.controller);

					//Store the sign of the axis value
					const float SIGN = Math::sign(AXIS_VALUE);

					//Store the dead zone squared
					const float DEAD_ZONE_SQ = axis.aDeadZone * axis.aDeadZone;

					//Normalise the axis value
					const float AXIS_NORM_SQ = (AXIS_VALUE * AXIS_VALUE) - DEAD_ZONE_SQ;

					//Scale the axis strength based on the dead zone
					if (AXIS_NORM_SQ > 0.f) 
						axisStrength += (AXIS_NORM_SQ / (1.f - DEAD_ZONE_SQ)) * SIGN;
				}

				//If input is based on a button
				else {
					//Check for positive button press(es)
					if (getKey(axis.bPosBtn, axis.controller) || 
						getKey(axis.bAltPosBtn, axis.controller))
						axisStrength += 1.f;

					//Check for negative button press(es)
					if (getKey(axis.bNegBtn, axis.controller) ||
						getKey(axis.bAltNegBtn, axis.controller))
						axisStrength -= 1.f;
				}

				//Apply sensitivity and inversion flag
				axisStrength *= axis.sensitivity * (axis.invert ? -1.f : 1.f);

				//Check if the strength is stronger then the current best
				if (abs(axisStrength) > abs(strongestAxis)) strongestAxis = axisStrength;
			}

			//If there is a strength value to be included
			if (strongestAxis && abs(strongestAxis) >= abs(mInstance->mCurInputAxis[V_AXIS])) {
				//Add the strength value
				mInstance->mCurInputAxis[V_AXIS] += strongestAxis * pDelta;

				//Get the strongest value as a negative
				const float NEG_STRONGEST = Math::max((strongestAxis < 0.f ? strongestAxis : strongestAxis * -1.f), -1.f);
				const float POS_STRONGEST = Math::min(NEG_STRONGEST * -1.f, 1.f);

				//Clamp the value to the -1 - 1 range
				mInstance->mCurInputAxis[V_AXIS] = Math::clamp(mInstance->mCurInputAxis[V_AXIS], NEG_STRONGEST, POS_STRONGEST);
			}

			//Otherwise apply gravity
			else if (gravAvg && mInstance->mCurInputAxis[V_AXIS]) {
				//Get the inverse direction
				const float INV_DIR = Math::sign(mInstance->mCurInputAxis[V_AXIS]) * -1.f;

				//Take the average of the gravity value
				gravAvg /= (float)contributingAxis;

				//Get the value after gravity is applied
				float appliedVal = mInstance->mCurInputAxis[V_AXIS] + gravAvg * INV_DIR * pDelta;

				//Assign the axis value
				mInstance->mCurInputAxis[V_AXIS] = (Math::sign(appliedVal) == INV_DIR ? 0.f : appliedVal);
			}
		}
		#pragma endregion

		#pragma region Update Vibration Settings
		//Loop through all vibration values
		if (mInstance->mVibrationValues.size()) {
			for (auto it = mInstance->mVibrationValues.begin(); it != mInstance->mVibrationValues.end();) {
				//Get a reference to the vibration value object
				VibrationValues& val = it->second;

				//Add the delta time to the timer
				val.vibrationTimer += pDelta;

				//Check the controller is connected
				if (mInstance->mConnectedControllers & (1 << (int)it->first)) {
					//Store the scale factor 
					float scale;

					//Check if time is up 
					if (val.vibrationTimer >= val.vibrationLength) 
						scale = 0.f;

					//Otherwise get the scale from the settings
					else scale = (val.scaleFunc ? Math::clamp01(val.scaleFunc(Math::inverseLerp(0.f, val.vibrationLength, val.vibrationTimer))) : 1.f);

					//Create an XINPUT object
					XINPUT_VIBRATION vibration;
					ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

					//Setup the motor speeds
					vibration.wLeftMotorSpeed = (WORD)(val.leftVibration * scale * std::numeric_limits<WORD>::max());
					vibration.wRightMotorSpeed = (WORD)(val.rightVibration * scale * std::numeric_limits<WORD>::max());

					//Set the vibration state
					DWORD state = XInputSetState((int)it->first, &vibration);
				}
								   
				//Check for removal
				if (val.vibrationTimer >= val.vibrationLength) 
					mInstance->mVibrationValues.erase(it++);
				else ++it;
			}
		}
		#pragma endregion
	}

	/*
		Input : getVAxis - Get the value of a virtual axis from the input axis
		Author: Mitchell Croft
		Created: 31/10/2017
		Modified: 31/07/2017

		param[in] pAxis - A c-string representing the name of the virtual axis to check

		return float - Returns the virtual axis' value as a float (-1.f - 1.f)
	*/
	const float& Input::getVAxis(const char* pAxis) { return mInstance->mCurInputAxis[pAxis]; }

	/*
		Input : getAxisRaw - Get the raw axis value	from a connected controller, or the average across all connected
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017

		param[in] pAxis - A EControllerAxisCode value representing the controller axis to retrieve
		param[in] pID - The ID of the controller to check for input from (Default All)
		param[in] pAverage - Flags if the highest value should be taken across all connected remotes or if
							 an average should be taken (Default false, Only applies when pID == EControllerID::All)

		return float - Returns the axis value as a float (-1.f - 1.f)
	*/
	const float Input::getAxisRaw(const EControllerAxisCodes& pAxis, const EControllerID& pID /* = EControllerID::All */, const bool& pAverage /* = false */) {
		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers || pAxis == EControllerAxisCodes::Null_Axis) return false;

		//Check if the controller is a specific index
		if (pID != EControllerID::All && pID != EControllerID::TOTAL) {
			//Check the specified controller is connected
			if (mInstance->mConnectedControllers & (1 << (int)pID))
				return mInstance->mControllerStates[GET_IND((int)pID, CURRENT_STATE)].axisValues[(int)pAxis];

			//If controller is not connected return 0
			else return 0.f;
		}

		//Take the average of connected controllers
		else {
			//Store the number of connected controllers
			byte connectedControllers = 0;

			//Store the required value
			float storage = 0.f;

			//Loop through connected inputs
			float buffer;
			for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++) {
				//Check if this controller is connected
				if (mInstance->mConnectedControllers & (1 << i)) {
					//Increment the connected controller count
					connectedControllers++;

					//Check if the average is being taken
					if (pAverage)
						storage += mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].axisValues[(int)pAxis];

					//Otherwise check if the value is greater then the current
					else if (abs(buffer = mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].axisValues[(int)pAxis]) > abs(storage))
						storage = buffer;
				}
			}
			
			//Return the average of the total
			return (pAverage ? storage / (float)connectedControllers : storage);
		}
	}

	/*
		Input : getButton - Check a virtual axis like a virtual button, getting if it has been modified
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pAxis - A c-string representing the name of the virtual axis to check

		return bool - Returns true if the virtual axis value is not equal to 0
	*/
	bool Input::getButton(const char* pAxis) { return mInstance->mCurInputAxis[pAxis] != 0.f; }

	/*
		Input : getButtonDown - Check a virtual axis like a virtual button, getting if it has been modified this frame
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pAxis - A c-string representing the name of the virtual axis to check

		return bool - Returns true the first frame the virtual axis value is not equal to 0
	*/
	bool Input::getButtonDown(const char* pAxis) { return mInstance->mCurInputAxis[pAxis] && !mInstance->mPreInputAxis[pAxis]; }

	/*
		Input : getButtonUp - Check a virtual axis like a virtual button, getting if it has returned to 0 this frame
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pAxis - A c-string representing the name of the virtual axis to check

		return bool - Returns true the first frame the virtual axis value is equal to 0
	*/
	bool Input::getButtonUp(const char* pAxis) { return !mInstance->mCurInputAxis[pAxis] && mInstance->mPreInputAxis[pAxis]; }
	
	/*
		Input : getKey - Check if a controller button is down across one or all connected controllers
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017

		param[in] pCode - An EControllerKeyCode value representing the controller button to test
		param[in] pID - The ID of the controller to check for input from (Default All)

		return bool - Returns true if the specified controller(s) have the specified button down
	*/
	bool Input::getKey(const EControllerKeyCodes& pCode, const EControllerID& pID /* = EControllerID::All */) {
		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers || pCode == EControllerKeyCodes::Null_Input) return false;

		//Check if the controller is a specific index
		if (pID != EControllerID::All && pID != EControllerID::TOTAL) {
			//Check if the specified controller is connected
			if (mInstance->mConnectedControllers & (1 << (int)pID))
				return (mInstance->mControllerStates[GET_IND((int)pID, CURRENT_STATE)].buttonMask & (int)pCode) != 0;
			
			//If controller is not connected return false
			else return false;
		}

		//Check the connected controllers for the button press
		else {
			//Loop through connected inputs
			for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++) {
				//Check if the controller is connected
				if (mInstance->mConnectedControllers & (1 << i)) {
					//Check if the button has been pressed
					if (mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].buttonMask & (int)pCode)
						return true;
				}
			}
		}

		//No controller has pressed the button
		return false;
	}

	/*
		Input : getKeyDown - Check if a controller has been pressed across one or all connected controllers
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017

		param[in] pCode - An EControllerKeyCode value representing the controller button to test
		param[in] pID - The ID of the controller to check for input from (Default All)

		return bool - Returns true the first frame the specified controller(s) press the specified button
	*/
	bool Input::getKeyDown(const EControllerKeyCodes& pCode, const EControllerID& pID /* = EControllerID::All */) {
		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers || pCode == EControllerKeyCodes::Null_Input) return false;

		//Check if the controller is a specific index
		if (pID != EControllerID::All && pID != EControllerID::TOTAL) {
			//Check if the specified controller is connected
			if (mInstance->mConnectedControllers & (1 << (int)pID))
				return (mInstance->mControllerStates[GET_IND((int)pID, CURRENT_STATE)].buttonMask & (int)pCode &&
					    !(mInstance->mControllerStates[GET_IND((int)pID, PREVIOUS_STATE)].buttonMask & (int)pCode));

			//If controller is not connected return false
			else return false;
		}

		//Check the connected controllers for the button press
		else {
			//Loop through connected inputs
			for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++) {
				//Check if the controller is connected
				if (mInstance->mConnectedControllers & (1 << i)) {
					//Check if the button has been pressed
					if (mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].buttonMask & (int)pCode &&
						!(mInstance->mControllerStates[GET_IND(i, PREVIOUS_STATE)].buttonMask & (int)pCode))
						return true;
				}
			}
		}

		//No controller has pressed the button
		return false;
	}

	/*
		Input : getKeyUp - Check for a controller button release across one or all connected controllers
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017

		param[in] pCode - An EControllerKeyCode value representing the controller button to test
		param[in] pID - The ID of the controller to check for input from (Default All)

		return bool - Returns true the first frame the specified controller(s) released the specified button
	*/
	bool Input::getKeyUp(const EControllerKeyCodes& pCode, const EControllerID& pID /* = EControllerID::All */) {
		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers || pCode == EControllerKeyCodes::Null_Input) return false;

		//Check if the controller is a specific index
		if (pID != EControllerID::All && pID != EControllerID::TOTAL) {
			//Check if the specified controller is connected
			if (mInstance->mConnectedControllers & (1 << (int)pID))
				return (!(mInstance->mControllerStates[GET_IND((int)pID, CURRENT_STATE)].buttonMask & (int)pCode) &&
					mInstance->mControllerStates[GET_IND((int)pID, PREVIOUS_STATE)].buttonMask & (int)pCode);

			//If controller is not connected return false
			else return false;
		}

		//Check the connected controllers for the button release
		else {
			//Loop through connected inputs
			for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++) {
				//Check if the controller is connected
				if (mInstance->mConnectedControllers & (1 << i)) {
					//Check if the button has been released
					if (!(mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].buttonMask & (int)pCode) &&
						mInstance->mControllerStates[GET_IND(i, PREVIOUS_STATE)].buttonMask & (int)pCode)
						return true;
				}
			}
		}

		//No controller has released the button
		return false;
	}

	/*
		Input : addVirtualAxis - Add a new virtual axis object to be monitored by the Input Manager
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017

		param[in] pAxis - A VirtualAxis struct defining the properties of the new virtual axis to monitor
	*/
	void Input::addVirtualAxis(const VirtualAxis& pAxis) { mInstance->mMonitorAxis.insert(std::pair<const char*, VirtualAxis>(pAxis.name.c_str(), pAxis)); }

	/*
		Input : addVirtualAxis - Add an array of new VirtualAxis objects to be monitored by the Input Manager
		Author: Mitchell Croft
		Created: 01/02/2017
		Modified: 01/02/2017

		param[in] pArray - A pointer to the array of VirtualAxis objects to add to the monitor list
		param[in] pCount - The number of VirtualAxis objects in the array to include
	*/
	void Input::addVirtualAxis(const VirtualAxis* pArray, const uint& pCount) { for (uint i = 0; i < pCount;  mInstance->mMonitorAxis.insert(std::pair<const char*, VirtualAxis>(pArray[i].name.c_str(), pArray[i])), i++);	}

	/*
		Input : removeAxis - Remove all virtual axis with a specific name
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pAxis - A c-string representing the name of the virtual axis to remove
	*/
	void Input::removeAxis(const char* pAxis) { mInstance->mMonitorAxis.erase(pAxis); mInstance->mCurInputAxis.erase(pAxis); }

	/*
		Input : removeAxis - Remove all virtual axis' defined within the Input Manager
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017
	*/
	void Input::removeAxis() { mInstance->mMonitorAxis.clear(); mInstance->mCurInputAxis.clear(); }

	/*
		Input : setVibrationSetting - Apply a range of vibration settings to 1 or all connected controllers
		Author: Mitchell Croft
		Created: 10/02/2017
		Modified: 10/02/2017

		param[in] pSetting - A VibrationSetting object describing the 
	*/
	void Input::setVibrationSetting(const VibrationSetting& pSetting) {
		//Check if the controller is a specific index
		if (pSetting.controller != EControllerID::All && pSetting.controller != EControllerID::TOTAL)
			mInstance->mVibrationValues[pSetting.controller] = pSetting;

		//Add the setting to all of the controllers
		else for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++)
			mInstance->mVibrationValues[(EControllerID)i] = pSetting;
	}

	/*
		Input : setPollInterval - Set the time between polls for new connected controllers
		Author: Mitchell Croft
		Created: 01/02/2017
		Modified: 01/02/2017

		param[in] pInterval - The time in seconds between poll events
	*/
	void Input::setPollInterval(const float& pInterval) { mInstance->mPollInterval = pInterval; }
}