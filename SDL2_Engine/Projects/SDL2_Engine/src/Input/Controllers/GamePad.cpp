#include "GamePad.hpp"

//! Include useful types and functions
#include <cmath>
#include <windows.h>

//! Include the XInput API 
#include <Xinput.h>

//! Include Bitmask objects to be used with GamePad button checks
#include "../../Utilities/Bitmask.hpp"

//! Define useful information for managing GamePad input
#define GAMEPAD_AXIS_TOTAL		6
#define GAMEPAD_TRIGGER_MAX		255.f
#define GAMEPAD_THUMBSTICK_MAX	32767.f
#define GAMEPAD_VIBRATION_MAX	65535.f

//! Define simple clamp function
#define CLAMP01(X) ((X) < 0 ? 0 : ((X) > 1 ? 1 : (X)))

namespace SDL2_Engine {
	namespace Input {
		//! Simple enum for labeling the states
		enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

		/*
		 *		Name: InputState
		 *		Author: Mitchell Croft
		 *		Created: 10/02/2017
		 *		Modified: 24/07/2017
		 *		
		 *		Purpose:
		 *		Store converted XInput state information about a physical device
		**/
		struct _GamePad::InputState {
			//! Store the packet ID for the GamePad
			DWORD packetID;

			//! Store an array of floats for the axis values
			float axisValues[GAMEPAD_AXIS_TOTAL];

			//! Store a Bitmask for the controller buttons
			WORD buttonMask;

			/*
				InputState : Constructor - Initialise with default values
				Created: 10/02/2017
				Modified: 10/02/2017
			*/
			inline InputState() { reset(); }

			/*
				InputState : Reset - Reset the memory used by the InputState object to 0
				Created: 10/02/2017
				Modified: 10/02/2017
			*/
			inline void reset() { ZeroMemory(this, sizeof(InputState)); }

			/*
				InputState : Assignment Operator (InputState) - Copy the values of another InputState
				Created: 10/02/2017
				Modified: 10/02/2017

				param[in] pCopy - The InputState object to copy the values of

				return InputState& - Returns a reference to itself
			*/
			inline InputState& operator=(const InputState& pCopy) { memcpy_s(this, sizeof(InputState), &pCopy, sizeof(InputState)); return *this; }
			
			/*
				InputState : Assignment Operator (XINPUT_STATE) - Assign the values according to an XINPUT_STATE object
				Created: 10/02/2017
				Modified: 10/02/2017

				param[in] pState - The XINPUT_STATE object to extract values from

				return InputState& - Returns a reference to itself
			*/
			InputState& operator=(const XINPUT_STATE& pState) {
				//Copy the simple values
				packetID = pState.dwPacketNumber;
				buttonMask = pState.Gamepad.wButtons;

				//Convert the trigger values
				axisValues[(int)EGamePadAxisCode::Left_Trigger] = (float)pState.Gamepad.bLeftTrigger / GAMEPAD_TRIGGER_MAX;
				axisValues[(int)EGamePadAxisCode::Right_Trigger] = (float)pState.Gamepad.bRightTrigger / GAMEPAD_TRIGGER_MAX;

				//Convert the thumbstick axis
				axisValues[(int)EGamePadAxisCode::Left_X] = (float)pState.Gamepad.sThumbLX / GAMEPAD_THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCode::Left_Y] = (float)pState.Gamepad.sThumbLY / GAMEPAD_THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCode::Right_X] = (float)pState.Gamepad.sThumbRX / GAMEPAD_THUMBSTICK_MAX;
				axisValues[(int)EGamePadAxisCode::Right_Y] = (float)pState.Gamepad.sThumbRY / GAMEPAD_THUMBSTICK_MAX;

				//Return self
				return *this;
			}
		};

		/*
			_GamePad : btnDown - Checks if any of the buttons defined in the mask are currently down
			Created: 13/02/2017
			Modified: 24/07/2017

			param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

			return bool - Returns true if any of the provided buttons are currently down
		*/
		bool _GamePad::btnDown(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept { return (pBtns & mStates[STATE_CUR].buttonMask) != 0; }

		/*
			_GamePad : btnPressed - Checks if any of the buttons defined in the mask where pressed this cycle
			Created: 13/02/2017
			Modified: 24/07/2017

			param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

			return bool - Returns true if any of the provided buttons where pressed this cycle

			NOTE:
			All buttons must have been released simultaneously before this function can return true
		*/
		bool _GamePad::btnPressed(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept { return ((pBtns & mStates[STATE_CUR].buttonMask) && !(pBtns & mStates[STATE_PRE].buttonMask)); }

		/*
			_GamePad : btnReleased - Checks if any of the buttons defined in the mask where released this cycle
			Created: 13/02/2017
			Modified: 24/07/2017

			param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

			return bool - Returns true if any of the provided buttons where released this cycle

			NOTE:
			All buttons must have been pressed simultaneously before this function can return true
		*/
		bool _GamePad::btnReleased(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept { return (!(pBtns & mStates[STATE_CUR].buttonMask) && (pBtns & mStates[STATE_PRE].buttonMask)); }

		/*
			_GamePad : rawAxis - Retrieve the current raw axis value from the Game Pad
			Created: 13/02/2017
			Modified: 24/07/2017

			param[in] pAxis - An EGamePadAxisCodes value defining the axis to retrieve

			return const float& - Returns a constant reference to the raw axis value
		*/
		const float& _GamePad::rawAxis(const EGamePadAxisCode& pAxis) const noexcept { return mStates[STATE_CUR].axisValues[(int)pAxis]; }

		/*
			_GamePad : rawAxisDelta - Retrieve the raw change in axis value from the Game Pad
			Created: 13/02/2017
			Modified: 24/07/2017

			param[in] pAxis - An EGamePadAxisCodes value defining the axis to retrieve

			return float - Returns the change in axis value as float
		*/
		float _GamePad::rawAxisDelta(const EGamePadAxisCode& pAxis) const noexcept { return mStates[STATE_CUR].axisValues[(int)pAxis] - mStates[STATE_PRE].axisValues[(int)pAxis]; }

		/*
			_GamePad : vibrate - Set the vibration values for the connected controller
			Created: 13/02/2017
			Modified: 30/07/2017

			param[in] pLeftScale - The strength to use for the left rumble motor (0 - 1 scale)
			param[in] pRightScale - The strength to use for the right rumble motor (0 - 1 scale)
		*/
		void _GamePad::vibrate(const float& pLeftScale, const float& pRightScale) const noexcept {
			//Check the GamePad is connected
			if (!mConnected) return;

			//Create an XInput structure to describe the vibration
			XINPUT_VIBRATION vibration;
			ZeroMemory(&vibration, sizeof(XINPUT_VIBRATION));

			//Set the vibration values
			vibration.wLeftMotorSpeed = (WORD)(CLAMP01(pLeftScale) * GAMEPAD_VIBRATION_MAX);
			vibration.wRightMotorSpeed = (WORD)(CLAMP01(pRightScale) * GAMEPAD_VIBRATION_MAX);

			//Set the vibration state
			XInputSetState(mID, &vibration);
		}

		/*
			_GamePad : getGamePadMask - Get the EGamePadID mask value for the current GamePad
			Created: 13/02/2017
			Modified: 24/07/2017

			return EGamePadID - Returns an EGamePadID value representing the GamePad
		*/
		EGamePadID _GamePad::getGamePadMask() const noexcept { return (EGamePadID)(int)pow(2, mID); }

		/*
			_GamePad : Constructor - Initialise with default values
			Created: 24/07/2017
			Modified: 24/07/2017

			param[in] pID - The numerical index for the GamePad object to use
		*/
		_GamePad::_GamePad(const unsigned char& pID) :
			//Set the default values
			mStates(nullptr),
			mConnected(false),
			mID(pID) {

			//Create the state array
			mStates = new InputState[STATE_TOTAL];
		}

		/*
			_GamePad : Destructor - Unload memory allocated for state information
			Created: 24/07/2017
			Modified: 24/07/2017
		*/
		_GamePad::~_GamePad() { vibrate(0.f, 0.f); delete[] mStates; }

		/*
			_GamePad : update - Update the current state information for the controller
			Created: 13/02/2017
			Modified: 08/11/2017

			param[in] pReconnect - Flags if the GamePad should attempt to reconnect to a physical device

			return bool - Returns true if the GamePad is currently connected to a physical device
		*/
		bool _GamePad::update(const bool& pReconnect) {
			//If the GamePad is not connected and not reconnecting, don't bother
			if (!mConnected && !pReconnect) return mConnected;

			//Create a state to store the data in
			XINPUT_STATE state;
			ZeroMemory(&state, sizeof(XINPUT_STATE));

			//Get the current GamePad state
			DWORD result = XInputGetState(mID, &state);

			//If the GamePad is still connected
			if (result == ERROR_SUCCESS) {
				//Check if the controller was already connected
				if (!mConnected) mConnected = true;
			}

			//If the GamePad is no longer connected
			else {
				//Check if the controller was already disconnected
				if (mConnected) {
					//Set the connected flag
					mConnected = false;

					//Reset the state information
					memset(mStates, 0, sizeof(InputState) * STATE_TOTAL);
				}
			}

			//If the GamePad is connected update the state information
			if (mConnected) {
				//Copy the previous state information
				if (mStates[STATE_PRE].packetID != mStates[STATE_CUR].packetID)
					mStates[STATE_PRE] = mStates[STATE_CUR];

				//Check if there is a new state to take
				if (state.dwPacketNumber != mStates[STATE_CUR].packetID)
					mStates[STATE_CUR] = state;
			}

			//Return the connection flag
			return mConnected;
		}
	}
}