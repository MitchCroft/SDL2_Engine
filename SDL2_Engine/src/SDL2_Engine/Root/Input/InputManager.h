#pragma once

#include "../../Types.h"

#include "InputForwardDeclares.h"
#include "../Time/TimeForwardDeclares.h"

#include "InputValues.h"

#include <unordered_map>

namespace SDL2_Engine {

	/*
	*		Name: Input
	*		Author: Mitchell Croft
	*		Created: 30/01/2017
	*		Modified: 31/01/2017
	*
	*		Purpose:
	*		Monitor and store input values involving controller's connected
	*		to the computer that is running the program
	**/
	class Input {
		//! Store the singleton values
		static Input* mInstance;
		Input();
		~Input();

		//! Prototype a container for controller information
		struct ControllerState;

		//! Store a bitmask to store the connected controllers
		byte mConnectedControllers;

		//! Store controller state objects in order to store input data
		ControllerState* mControllerStates;

		//! Store a map of the different axis' objects to monitor and update
		std::unordered_multimap<const char*, VirtualAxis> mMonitorAxis;

		//! Store the axis values as they have been modified by the InputAxis values
		std::unordered_map<const char*, float> mPreInputAxis;
		std::unordered_map<const char*, float> mCurInputAxis;

	public:
		//! Initialisation / Destruction
		static bool init();
		static void destroy();

		//! Main function
		static void update(const Time& pTime);

		//! Getters
		static const float& getVAxis(const char* pAxis);

		static const float getAxisRaw(const EControllerAxisCodes& pAxis, const EControllerID& pID = EControllerID::All);

		static bool getButton(const char* pAxis);
		static bool getButtonDown(const char* pAxis);
		static bool getButtonUp(const char* pAxis);

		static bool getKey(const EControllerKeyCodes& pCode, const EControllerID& pID = EControllerID::All);
		static bool getKeyDown(const EControllerKeyCodes& pCode, const EControllerID& pID = EControllerID::All);
		static bool getKeyUp(const EControllerKeyCodes& pCode, const EControllerID& pID = EControllerID::All);

		//! Setters
		static void addAxis(const VirtualAxis& pAxis);
		static void removeAxis(const char* pAxis);
		static void removeAxis();
	};
}