#pragma once

//! Include the required SDL2_Engine objects
#include "../../__LibraryManagement.hpp"
#include "../../Utilities/IGlobal.hpp"
#include "../../Utilities/Bitmask.hpp"

#include "VirtualAxis.hpp"


namespace SDL2_Engine {
	//! Prototype the Time and Math objects
	class Time; class Math;

	namespace Input {
		//! Prototype the GamePad object
		class _GamePad;
		typedef const _GamePad* GamePad;

		//! Prototype the container objects used by the Controllers global object
		struct VibrationDescription;

		/*
		 *		Name: Controllers
		 *		Author: Mitchell Croft
		 *		Created: 25/07/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Track and manage connected XInput physical devices as
		 *		well as provide an interface for polling input states
		**/
		class SDL2_LIB_INC Controllers : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------GamePad Inputs----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Controllers : getGamePad - Get a pointer to a specific GamePad object
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pID - The EgamePadID of the GamePad desired

				return GamePad - Returns a GamePad (const _GamePad*) object to be used for testing input
			*/
			GamePad getGamePad(const EGamePadID& pID) const noexcept;

			/*
				Controllers : btnDown - Test to see if the specified GamePad(s) are pressing at least one of the specified buttons
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
				param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

				return int - Returns an int bitmask that flags the different GamePad(s) that are pressing at least one of the specified buttons
			*/
			int btnDown(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All) const noexcept;

			/*
				Controllers : btnPressed - Test to see if the specified GamePad(s) have just pressed at least one of the specified buttons after all buttons were previously released
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
				param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

				return int - Returns an int bitmask that flags the different GamePad(s) that have just pressed at least on the specified buttons
			*/
			int btnPressed(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All) const noexcept;

			/*
				Controllers : btnReleased - Test to see if the specified GamePad(s) have just released at least one of the specified buttons after all buttons were previously pressed
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
				param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

				return int - Returns an int bitmask that flags the different GamePad(s) that have just released at least on the specified buttons
			*/
			int btnReleased(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs = EGamePadID::All) const noexcept;

			/*
				 Controllers : rawAxis - Retrieve the raw axis value from a specific GamePad
				 Created: 22/09/2017
				 Modified: 22/09/2017

				 param[in] pAxis - The EGamePadAxisCode value to retrieve
				 param[in] pID - The EGamePadID value defining the controller to retrieve input from

				 return const float& - Returns the axis value as a const float reference within the range -1.f to 1.f
			*/
			const float& rawAxis(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) const noexcept;

			/*
				Controllers : rawAxisDelta - Retrieve the change in raw axis value from a specified GamePad
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pAxis - The EGamePadAxisCode value to retrieve
				param[in] pID - The EGamePadID value defining the controller to retrieve input from

				return float - Returns a float value containing the change in the specified axis' value
			*/
			const float rawAxisDelta(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) const noexcept;

			/*
				Controllers : applyVibration - Add a vibration description to the Controller Manager
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pDesc - A VibrationDescription object describing how the vibration will occur
			*/	
			void applyVibration(const VibrationDescription& pDesc) const noexcept;

			/*
				Controllers : resetVibration - Reset the vibration of the specified GamePad(s)
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pIDs - A Bitmask of the EGamePadID value(s) to reset
			*/
			void resetVibration(const Utilities::Bitmask<EGamePadID>& pIDs) const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Virtual Inputs----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Controllers : vAxis - Retrieve the value of a Virtual Axis being monitored
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return const float& - Returns the value of the Virtual Axis as a const float reference
			*/
			const float& vAxis(const std::string& pAxis) const noexcept;

			/*
				Controllers : vAxisDelta - Retrieve the change in axis value since the last cycle
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return float - Returns a float value containing the change in the Virtual Axis' value
			*/
			float vAxisDelta(const std::string& pAxis) const noexcept;

			/*
				Controllers : vBtnDown - Treat the Virtual Axis as a button and check if it is currently 'down'
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true if the Virtual Axis is not equal to zero
			*/
			bool vBtnDown(const std::string& pAxis) const noexcept;

			/*
				Controllers : vBtnPressed - Treat the Virtual Axis as a button and check if it was 'pressed'
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true the first cycle the Virtual Axis is not equal to zero
			*/
			bool vBtnPressed(const std::string& pAxis) const noexcept;

			/*
				Controllers : vBtnReleased - Treat the Virtual Axis as a button and check if it was 'released'
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true the first cycle the Virtual Axis is equal to zero
			*/
			bool vBtnReleased(const std::string& pAxis) const noexcept;

			/*
				Controllers : addVirtualAxis - Add a new Virtual Axis description to the monitor list
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pAxis - The Virtual Axis object describing the new Virtual Axis
			*/
			void addVirtualAxis(const VirtualAxis& pAxis) const noexcept;

			/*
				Controllers : addVirtualAxis - Add an array of Virtual Axis descriptions to the monitor list
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pAxis - A pointer to the array of Virtual Axis objects to monitor
				param[in] pCount - The number of Virtual Axis objects stored in the array
			*/
			void addVirtualAxis(const VirtualAxis*& pAxis, const size_t& pCount) const noexcept;

			/*
				Controllers : removeVirtualAxis - Clear all Virtual Axis' with a specific name
				Created: 22/09/2017
				Modified: 06/10/2017

				param[in] pAxis - A string defining the name of the Axis to remove
			*/
			void removeVirtualAxis(const std::string& pAxis) const noexcept;

			/*
				Controllers : removeVirtualAxis - Clear all Virtual Axis'
				Created: 22/09/2017
				Modified: 22/09/2017
			*/
			void removeVirtualAxis() const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Timing Values-----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Controllers : getPollInterval - Get the amount of time between checks for recently connected GamePads
				Created: 22/09/2017
				Modified: 22/09/2017

				return const float& - Returns a const float reference to the poll interval value
			*/
			const float& getPollInterval() const noexcept;

			/*
				Controllers : setPollInterval - Set the amount of time between checks for recently connected GamePads
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pInt - A float value defining the time (seconds) between GamePad polling
			*/
			void setPollInterval(const float& pInt) const noexcept;

			/*
				Controllers : getVibrationFrequancy - Get the refresh rate of the GamePad Vibration Descriptions
				Created: 22/09/2017
				Modified: 22/09/2017

				return const float - Returns a const float value containing the refresh rate of the Vibration Descriptions
			*/
			const float getVibrationFrequancy() const noexcept;

			/*
				Controllers : setVibrationFrequancy - Set the refresh rate of the GamePad Vibration Descriptions
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pFreq - A float value defining the refresh rate (hertz) of the Vibration Descriptions
			*/
			void setVibrationFrequancy(const float& pFreq) const noexcept;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Controllers : Constructor - Initialise with default values
				Created: 25/07/2017
				Modified: 22/09/2017
			*/
			Controllers();

			/*
				Controllers : createInterface - Allocate the resources required for the Controllers object
				Created: 25/07/2017
				Modified: 22/09/2017

				return bool - Returns true if the Keyboard object was created successfully
			*/
			bool createInterface() override;

			/*
				Controllers : destroyInterface - Deallocate the resources used by the Controllers object
				Created: 25/07/2017
				Modified: 22/09/2017
			*/
			void destroyInterface() override;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Main Function----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Controllers : update - Update the GamePads input states
				Created: 25/07/2017
				Modified: 22/09/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements for the Controllers
			struct ControllersInternalData;
			ControllersInternalData* mData;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Update Internal---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			void updateVirtualAxis(const Time& pTime, const Math& pMath) const;
			void updateVibrations(const Time& pTime, const Math& pMath) const;
		};
	}
}