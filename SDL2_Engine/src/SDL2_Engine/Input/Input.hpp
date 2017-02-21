#pragma once

//! Include the Input values
#include "InputValues.hpp"

//! Allow defaulting of GamePad ID values
#include "../Utilities/Bitmask.hpp"

#include <unordered_map>
#include <string>
#include <bitset>
#include <array>

namespace SDL2_Engine {
	namespace Input {
		//! Prototype the Virtual Axis class
		struct VirtualAxis;

		//! Prototype the Vibration Description
		struct VibrationDescription;

		//! Define the type to hold keyboard data
		typedef unsigned char BYTE;

		/*
		 *		Name: Input
		 *		Author: Mitchell Croft
		 *		Created: 10/02/2017
		 *		Modified: 13/02/2017
		 *		
		 *		Purpose:
		 *		Track and manage the GamePads connected the system
		 *		in order to test for input values for both them and
		 *		a connected keyboard
		**/
		class Input {
			//! Internal object used to store all relevant information to timed vibrations
			struct VibrationValues;

			//! Used to determine how often the Input manager updates the vibration values on GamePads
			const float VIBRATION_FREQUANCY = 1.f / 20.f;

			//! Store a constant array of Input keys to check for input
			const std::array<EKeyboardKeyCodes, 63> INPUTABLE_KEY_VALUES;

			//! Singleton values
			static Input* mInstance;
			Input();
			~Input() = default;

			//! Store an array of the GamePads
			_GamePad* mGamePads;

			//! Store an array of the Keyboard States
			BYTE** mKeyboardStates;

			//! Store a timer for applying the vibration values
			float mVibrationTimer;

			//! Store a map of Vibration Descriptions to apply
			std::unordered_map<EGamePadID, VibrationValues> mVibrationValues;

			//! Store a map of the Virtual Axis to monitor
			std::unordered_multimap<std::string, VirtualAxis> mMonitor;

			//! Store the virtual axis values as they are modified over time
			std::unordered_map<std::string, float> mPreInputAxis;
			std::unordered_map<std::string, float> mCurInputAxis;

			//! Store timing information for the inputing of repeat values
			std::array<float, 63> mRepeatKeyTimers;
			std::bitset<63> mRepeatFlags;

			//! Store controller poll timer values
			float mPollTimer;
			float mPollInterval;

			//! Simple function for converting Virtual Keycodes and appending them to a std::string
			bool verifyKeyboardInput(std::string& pString, const EKeyboardKeyCodes& pKey, const int& pMaxLength, const Utilities::Bitmask<EKeyboardInputFlags, char>& pFlags) const;

		public:
			//! Main functions
			static bool init();
			static void destroy();
			static void update(const float& pDelta, const float& pRealDelta);

			//! General
			static void setPollInterval(const float& pInterval);

			//! Virtual Axis'								
			static const float& getVAxis(const char* pAxis);

			static float getVAxisDelta(const char* pAxis);

			static bool getVBtnDown(const char* pAxis);
			static bool getVBtnPressed(const char* pAxis);
			static bool getVBtnReleased(const char* pAxis);

			static void addVirtualAxis(const VirtualAxis& pAxis);
			static void addVirtualAxis(const VirtualAxis* pArray, const size_t& pCount);

			static void removeVirtualAxis(const char* pAxis);
			static void removeVirtualAxis();

			//! GamePads
			static GamePad getGamePad(EGamePadID pID);

			static void applyVibration(const VibrationDescription& pDesc);
			static void resetVibration(const Utilities::Bitmask<EGamePadID>& pIDs);

			static const float& getGamePadAxis(const EGamePadAxisCodes& pAxis, const EGamePadID& pID);

			static float getGamePadAxisDelta(const EGamePadAxisCodes& pAxis, const EGamePadID& pID);

			static int getGamePadBtnDown(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All);
			static int getGamePadBtnPressed(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All);
			static int getGamePadBtnReleased(const Utilities::Bitmask<EGamePadBtnCodes, unsigned short>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All);

			//! Keyboard 
			static bool getKeyboardKeyDown(const EKeyboardKeyCodes& pKey);
			static bool getKeyboardKeyPressed(const EKeyboardKeyCodes& pKey);
			static bool getKeyboardKeyReleased(const EKeyboardKeyCodes& pKey);

			static bool getKeyboardKeyToggled(const EKeyboardKeyCodes& pKey);

			static bool modifyStringByKeyboard(std::string& pString, const int& pMaxLength = -1, const Utilities::Bitmask<EKeyboardInputFlags, char>& pFlags = EKeyboardInputFlags::All);
		};
	}
}