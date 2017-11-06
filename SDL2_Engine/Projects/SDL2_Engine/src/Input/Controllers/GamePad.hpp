#pragma once

#include "../../__LibraryManagement.hpp"
#include "ControllerValues.hpp"

namespace SDL2_Engine {
	//! Prototype the Bitmask object
	namespace Utilities { template<typename T> class Bitmask; }

	namespace Input {
		/*
		 *		Name: _GamePad
		 *		Author: Mitchell Croft
		 *		Created: 10/02/2017
		 *		Modified: 24/07/2017
		 *
		 *		Purpose:
		 *		Manage the values associated with a single GamePad object
		 *		and provide an interface for checking input states
		**/
		class SDL2_LIB_INC _GamePad {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Input Information-----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				_GamePad : btnDown - Checks if any of the buttons defined in the mask are currently down
				Created: 13/02/2017
				Modified: 24/07/2017

				param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

				return bool - Returns true if any of the provided buttons are currently down
			*/
			bool btnDown(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept;

			/*
				_GamePad : btnPressed - Checks if any of the buttons defined in the mask where pressed this cycle
				Created: 13/02/2017
				Modified: 24/07/2017

				param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

				return bool - Returns true if any of the provided buttons where pressed this cycle

				NOTE:
				All buttons must have been released simultaneously before this function can return true
			*/
			bool btnPressed(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept;

			/*
				_GamePad : btnReleased - Checks if any of the buttons defined in the mask where released this cycle
				Created: 13/02/2017
				Modified: 24/07/2017

				param[in] pBtns - A Bitmask of EControllerBtnCodes values, dictating the buttons to check

				return bool - Returns true if any of the provided buttons where released this cycle

				NOTE:
				All buttons must have been pressed simultaneously before this function can return true
			*/
			bool btnReleased(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns) const noexcept;

			/*
				_GamePad : rawAxis - Retrieve the current raw axis value from the Game Pad
				Created: 13/02/2017
				Modified: 24/07/2017

				param[in] pAxis - An EGamePadAxisCodes value defining the axis to retrieve

				return const float& - Returns a constant reference to the raw axis value
			*/
			const float& rawAxis(const EGamePadAxisCode& pAxis) const noexcept;

			/*
				_GamePad : rawAxisDelta - Retrieve the raw change in axis value from the Game Pad
				Created: 13/02/2017
				Modified: 24/07/2017

				param[in] pAxis - An EGamePadAxisCodes value defining the axis to retrieve

				return float - Returns the change in axis value as float
			*/
			float rawAxisDelta(const EGamePadAxisCode& pAxis) const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Feedback Functionality--------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				_GamePad : vibrate - Set the vibration values for the connected controller
				Created: 13/02/2017
				Modified: 30/07/2017

				param[in] pLeftScale - The strength to use for the left rumble motor (0 - 1 scale)
				param[in] pRightScale - The strength to use for the right rumble motor (0 - 1 scale)
			*/
			void vibrate(const float& pLeftScale, const float& pRightScale) const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------GamePad Information---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				_GamePad : getID - Get the numerical index representing the current GamePad
				Created: 13/02/2017
				Modified: 24/07/2017

				return const unsigned char& - Returns the index as a reference to a constant unsigned char value
			*/
			inline const unsigned char& getID() const noexcept { return mID; }

			/*
				_GamePad : getGamePadMask - Get the EGamePadID mask value for the current GamePad
				Created: 13/02/2017
				Modified: 24/07/2017

				return EGamePadID - Returns an EGamePadID value representing the GamePad
			*/
			EGamePadID getGamePadMask() const noexcept;

			/*
				_GamePad : isConnected - Returns the connection flag of the current GamePad
				Created: 13/02/2017
				Modified: 13/02/2017

				return const bool& - Returns the connection state as a constant reference to a bool
			*/
			inline const bool& isConnected() const noexcept { return mConnected; }

		private:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------GamePad Values-----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Set the GamePad as a friend of the Controllers global object
			friend class Controllers;

			//! Prototype a container for the XInput state information
			struct InputState;

			//! Store the state information for the Game Pad
			InputState* mStates;

			//! Flag if the controller is currently connected
			bool mConnected;

			//! Store the numerical index of the GamePad
			const unsigned char mID;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Private Functions--------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//! Construction / Destruction
			_GamePad(const unsigned char& pID);
			~_GamePad();

			//! Update Input State Information
			bool update(const bool& pReconnect);
		};

		//! Define a simplified type define for 'everyday' use
		typedef const _GamePad* GamePad;
	}
}