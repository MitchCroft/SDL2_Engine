#include "InputManager.hpp"

#define NOMINMAX
#include <windows.h>

#include <Xinput.h>
#include <limits>

#include "VirtualAxis.hpp"

//! Define the two different states
#define CURRENT_STATE 0
#define PREVIOUS_STATE 1
#define STATE_TOTAL 2

//! Provide a macro for converting to controller state index's
#define GET_IND(ID, STATE) (ID * STATE_TOTAL + STATE)

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
		Input : Constructor - Initialise with default values
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 01/02/2017
	*/
	Input::Input() : mConnectedControllers(0), mControllerStates(nullptr), mPollTimer(5.f), mPollInterval(5.f) { mControllerStates = new ControllerState[(int)EControllerID::TOTAL * STATE_TOTAL]; }

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

		return float - Returns the axis value as a float (-1.f - 1.f)
	*/
	const float Input::getAxisRaw(const EControllerAxisCodes& pAxis, const EControllerID& pID /* = EControllerID::All */) {
		//Check the axis is valid
		if (pAxis == EControllerAxisCodes::Null_Axis) throw std::runtime_error("Can not check the state of the Null_Axis, axis is not valid");

		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers) return 0.f;

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

			//Store the running total of axis values
			float total = 0.f;

			//Loop through connected inputs
			for (int i = (int)EControllerID::One; i < (int)EControllerID::TOTAL; i++) {
				//Check if this controller is connected
				if (mInstance->mConnectedControllers & (1 << i)) {
					//Increment the connected controller count
					connectedControllers++;

					//Add onto the total
					total += mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].axisValues[(int)pAxis];
				}
			}
			
			//Return the average of the total
			return (total / (float)connectedControllers);
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
		//Check the key is valid
		if (pCode == EControllerKeyCodes::Null_Input) throw std::runtime_error("Can not check the state of the Null_Input, button is not valid");

		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers) return false;

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
		//Check the key is valid
		if (pCode == EControllerKeyCodes::Null_Input) throw std::runtime_error("Can not check the state of the Null_Input, button is not valid");

		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers) return false;

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
						!(mInstance->mControllerStates[GET_IND(i, CURRENT_STATE)].buttonMask & (int)pCode))
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
		//Check the key is valid
		if (pCode == EControllerKeyCodes::Null_Input) throw std::runtime_error("Can not check the state of the Null_Input, button is not valid");

		//Check there is at least one connected controller
		if (!mInstance->mConnectedControllers) return false;

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
		Input : addAxis - Add a new virtual axis object to be monitored by the Input Manager
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pAxis - A VirtualAxis struct defining the properties of the new virtual axis to monitor
	*/
	void Input::addAxis(const VirtualAxis& pAxis) { mInstance->mMonitorAxis.insert(std::pair<const char*, VirtualAxis>(pAxis.name.c_str(), pAxis)); }

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
		Input : setPollInterval - Set the time between polls for new connected controllers
		Author: Mitchell Croft
		Created: 01/02/2017
		Modified: 01/02/2017

		param[in] pInterval - The time in seconds between poll events
	*/
	void Input::setPollInterval(const float& pInterval) { mInstance->mPollInterval = pInterval; }
}