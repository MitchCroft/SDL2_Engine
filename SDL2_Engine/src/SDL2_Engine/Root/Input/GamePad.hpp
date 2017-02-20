#pragma once

//! Include the input values
#include "InputValues.hpp"

namespace SDL2_Engine {

	//! Prototype the Bitmask object
	namespace Utilities {
		template<typename T, typename Val>
		class Bitmask;
	}

	class _GamePad {
		//! Set the GamePad as a friend of the Input singleton
		friend class Input;

		//! Prototype a container for the XInput state information
		struct InputState;

		//! Flag if the controller is connected
		bool mConnected;

		//! Store the index ID of the controller object
		const unsigned int mID;

		//! Store the state information for the game pads 
		InputState* mStates;

		//! Constructor / Destructor 
		_GamePad(const unsigned int& pID);
		~_GamePad();

		//! Update the internal state information
		bool update(const bool& pReconnect);

	public:
		//! Get the ID of the current controller
		inline const unsigned int& getID() const { return mID; }
		const EGamePadID getGamePadID() const;

		//! Get the connection status of the GamePad
		inline const bool& isConnected() const { return mConnected; }

		//! Cause a vibration in the controller
		void vibrate(float pLeftScale, float pRightScale) const;

		//! Get axis information
		const float& getRawAxis(const EGamePadAxisCodes& pAxis) const;
		float getRawAxisDelta(const EGamePadAxisCodes& pAxis) const;

		//! Get button information
		bool btnDown(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const;
		bool btnPressed(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const;
		bool btnReleased(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtn) const;
	};

	//! Create a simplified typedef for the user to use
	typedef const _GamePad* GamePad;
}