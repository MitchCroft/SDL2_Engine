#include "InputManager.h"

#include <windows.h>
#include <Xinput.h>

#include "../Time.h"

#include "VirtualAxis.h"

//! Define the two different states
#define CURRENT_STATE 0
#define PREVIOUS_STATE 1
#define STATE_TOTAL 2

//! Define the total number of axis
#define AXIS_TOTAL ((int)EControllerAxisCodes::Right_Y + 1)

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
	struct Input::ControllerState {
		//! Store the current input ID for this state
		DWORD packetNumber;

		//! Store the axis information as separate floats
		float axisValues[AXIS_TOTAL];

		//! Store the bitmask for the controller buttons
		WORD buttonMask;
	};

	/*
		Input : Constructor - Initialise with default values
		Author: Mitchell Croft
		Created: 31/01/2017
		Modified: 31/01/2017
	*/
	Input::Input() : mConnectedControllers(0), mControllerStates(nullptr) { mControllerStates = new ControllerState[(int)EControllerID::TOTAL * STATE_TOTAL]; }

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
		Created: 31/01/2017
		Modified: 31/01/2017

		param[in] pTime - A constant reference to an SDL2_Engine::Time object, for use in updating
						  the virtual axis values
	*/
	void Input::update(const Time& pTime) {
		pTime.realDeltaTime;
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
		Modified: 31/01/2017

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
				return mInstance->mControllerStates[(int)pID * STATE_TOTAL + CURRENT_STATE].axisValues[(int)pAxis];

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
					total += mInstance->mControllerStates[i * STATE_TOTAL + CURRENT_STATE].axisValues[(int)pAxis];
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
		Modified: 31/01/2017

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
				return (mInstance->mControllerStates[(int)pID * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode) == 1;
			
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
					if (mInstance->mControllerStates[i * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode)
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
		Modified: 31/01/2017

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
				return (mInstance->mControllerStates[(int)pID * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode &&
					    !(mInstance->mControllerStates[(int)pID * STATE_TOTAL + PREVIOUS_STATE].buttonMask & (int)pCode));

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
					if (mInstance->mControllerStates[i * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode &&
						!(mInstance->mControllerStates[i * STATE_TOTAL + PREVIOUS_STATE].buttonMask & (int)pCode))
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
		Modified: 31/01/2017

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
				return (!(mInstance->mControllerStates[(int)pID * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode) &&
					mInstance->mControllerStates[(int)pID * STATE_TOTAL + PREVIOUS_STATE].buttonMask & (int)pCode);

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
					if (!(mInstance->mControllerStates[i * STATE_TOTAL + CURRENT_STATE].buttonMask & (int)pCode) &&
						mInstance->mControllerStates[i * STATE_TOTAL + PREVIOUS_STATE].buttonMask & (int)pCode)
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
}