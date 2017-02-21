#include "GamePad.hpp"

#include <limits>

#define NOMINMAX
#include <windows.h>
#include <Xinput.h>

#include "../_Root/Math.hpp"
#include "../Utilities/Bitmask.hpp"

//! Define the maximum number of Controller Axis values
#define AXIS_TOTAL 6

namespace SDL2_Engine {
	namespace Input {
		//! Define a simple enum for tracking the states
		enum EState { STATE_CURRENT, STATE_PREVIOUS, STATE_TOTAL };

		//! Store the constant values used for converting XInput values
		const float TRIGGER_MAX = (float)std::numeric_limits<BYTE>::max();
		const float THUMBSTICK_MAX = (float)std::numeric_limits<SHORT>::max();
		const float VIBRATION_MAX = (float)std::numeric_limits<WORD>::max();


		/*
		 *		Name: InputState
		 *		Author: Mitchell Croft
		 *		Created: 10/02/2017
		 *		Modified: 10/02/2017
		 *
		 *		Purpose:
		 *		Store converted state information about the
		 *		controllers input values
		**/
		struct _GamePad::InputState {
			//! Store the packet ID for the controller
			DWORD packetID;

			//! Store an array of floats for the axis
			float axisValues[AXIS_TOTAL];

			//! Store a bitmask for the controller buttons
			WORD buttonMask;

			//! Constructor
			inline InputState() { reset(); }

			//! Wipe memory values
			inline void reset() { ZeroMemory(this, sizeof(InputState)); }

			//! Assignment operator values
			InputState& operator=(const InputState& pCopy) {
				memcpy_s(this, sizeof(InputState), &pCopy, sizeof(InputState));
				return *this;
			}
			InputState& operator=(const XINPUT_STATE& pState) {
				//! Copy the simple values
				packetID = pState.dwPacketNumber;
				buttonMask = pState.Gamepad.wButtons;

				//Convert the trigger values
				axisValues[(int)EGamePadAxisCodes::Left_Trigger] = (float)pState.Gamepad.bLeftTrigger / TRIGGER_MAX;
				axisValues[(int)EGamePadAxisCodes::Right_Trigger] = (float)pState.Gamepad.bRightTrigger / TRIGGER_MAX;

				//Convert the thumbstick axis
				axisValues[(int)EGamePadAxisCodes::Left_X] = (float)pState.Gamepad.sThumbLX / THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCodes::Left_Y] = (float)pState.Gamepad.sThumbLY / THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCodes::Right_X] = (float)pState.Gamepad.sThumbRX / THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCodes::Right_Y] = (float)pState.Gamepad.sThumbRY / THUMBSTICK_MAX;

				//Return this
				return *this;
			}
		};

		/*
			_GamePad : Constructor - Initialise with default values
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pID - The controller ID of the current _GamePad object
		*/
		_GamePad::_GamePad(const unsigned int& pID) :
			mConnected(false),
			mID(pID),
			mStates(nullptr) {
			mStates = new InputState[STATE_TOTAL];
		}

		/*
			_GamePad : Destructor - Cancel out any vibration on connected controllers
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017
		*/
		_GamePad::~_GamePad() { if (mConnected) vibrate(0.f, 0.f); }

		/*
			_GamePad : update - Update the state information for the current _GamePad
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/03/2017

			param[in] pReconnect - Flags if the GamePad should attempt to reconnect if it is disconnected

			return bool - Returns true if the GamePad is currently connected
		*/
		bool _GamePad::update(const bool& pReconnect) {
			//Check if the GamePad is connected
			if (!mConnected && pReconnect) {
				//Create a state to store the return in
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				//Get the current GamePads state
				DWORD result = XInputGetState(mID, &state);

				//If the controller is connected
				if (result == ERROR_SUCCESS) {
					//Set the connected flag
					mConnected = true;

					//Apply small vibration
					vibrate(0.25f, 0.25f);
				}
			}

			//If the GamePad is connected, update the state information
			if (mConnected) {
				//Check if the current state is different from the previous
				if (mStates[STATE_PREVIOUS].packetID != mStates[STATE_CURRENT].packetID)
					mStates[STATE_PREVIOUS] = mStates[STATE_CURRENT];

				//Create a state to store the return in
				XINPUT_STATE state;
				ZeroMemory(&state, sizeof(XINPUT_STATE));

				//Get the current GamePads state
				DWORD result = XInputGetState(mID, &state);

				//Check if the GamePad has become disconnected
				if (result != ERROR_SUCCESS) {
					//Reset all state information
					memset(mStates, 0, sizeof(InputState) * 2);

					//Reset connected flag
					mConnected = false;
				}

				//Otherwise update the current state
				else if (mStates[STATE_CURRENT].packetID != state.dwPacketNumber)
					mStates[STATE_CURRENT] = state;
			}

			//Return the connection status
			return mConnected;
		}

		/*
			_GamePad : getGamePadID - Returns the Bitmask GamePadID value for the current GamePad
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			return EGamePadID - Returns a EGamePadID value corresponding the the _GamePads connection place
		*/
		const EGamePadID _GamePad::getGamePadID() const { return (EGamePadID)(int)(pow(2, mID)); }

		/*
			_GamePad : vibrate - Set the vibration values for the connected controller
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pLeftScale - The strength of the left rumble motor to use (0 - 1 scale)
			param[in] pRightScale - The strength of the right rumble motor to use (0 - 1 scale)
		*/
		void _GamePad::vibrate(float pLeftScale, float pRightScale) const {
			//Check GamePad is connected
			if (!mConnected) return;

			//Create an XInput structure to define the vibration
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			//Set the vibration values
			vibration.wLeftMotorSpeed = (WORD)(Math::clamp01(pLeftScale) * VIBRATION_MAX);
			vibration.wRightMotorSpeed = (WORD)(Math::clamp01(pRightScale) * VIBRATION_MAX);

			//Set the vibration state
			XInputSetState(mID, &vibration);
		}

		/*
			_GamePad : getRawAxis - Retrieve the raw axis values for a specified Axis
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pAxis - An EControllerAxisCode value defining the axis to retrieve

			return const float& - Returns a constant reference to this _GamePads raw axis value
		*/
		const float& _GamePad::getRawAxis(const EGamePadAxisCodes& pAxis) const { return mStates[STATE_CURRENT].axisValues[(int)pAxis]; }

		/*
			_GamePad : getRawAxisDelta - Retrieve the change in a raw axis value
			Author: Mitchell Croft
			Created: 14/02/2017
			Modified: 14/02/2017

			param[in] pAxis - The EGamePadAxisCode value to check

			return float - Returns a float value containing the change in _GamePad raw axis' value
		*/
		float _GamePad::getRawAxisDelta(const EGamePadAxisCodes& pAxis) const { return (mStates[STATE_CURRENT].axisValues[(int)pAxis] - mStates[STATE_PREVIOUS].axisValues[(int)pAxis]); }

		/*
			_GamePad : btnDown - Check if any of the supplied button mask codes are currently down
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pBtn - A Bitmask of EControllerBtnCode values, containing the buttons to check

			return bool - Returns true if any of the buttons defined in the Bitmask are currently down
		*/
		bool _GamePad::btnDown(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const { return (mStates[STATE_CURRENT].buttonMask & pBtn) != 0; }

		/*
			_GamePad : btnPressed - Check if any of the supplied button mask values were pressed after all previously
									being up
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pBtn - A Bitmask of EControllerBtnCode values, containing the buttons to check

			return bool - Returns true if any of the buttons defined in the Bitmask have been pressed
		*/
		bool _GamePad::btnPressed(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const { return ((mStates[STATE_CURRENT].buttonMask & pBtn) && !(mStates[STATE_PREVIOUS].buttonMask & pBtn)); }

		/*
			_GamePad : btnReleased - Check if any of the supplied button mask values were released after all previously
									 being down
			Author: Mitchell Croft
			Created: 13/02/2017
			Modified: 13/02/2017

			param[in] pBtn - A Bitmask of EControllerBtnCode values, containing the buttons to check

			return bool - Returns true if any of the buttons defined in the Bitmask have been released
		*/
		bool _GamePad::btnReleased(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const { return (!(mStates[STATE_CURRENT].buttonMask & pBtn) && (mStates[STATE_PREVIOUS].buttonMask & pBtn)); }
	}
}
