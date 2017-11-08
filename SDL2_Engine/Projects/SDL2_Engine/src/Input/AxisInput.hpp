#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"

namespace SDL2_Engine {
	//! Prototype the Input Initialiser object
	namespace Initialisation { struct AxisInputInitialiser; }

	namespace Input {
		//! Prototype the Virtual axis object
		struct VirtualAxis;

		/*
		 *		Name: AxisInput
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 08/11/2017
		 *		
		 *		Purpose:
		 *		Provide an interface for managing and evaluating Virtual Axis
		 *		across all three input types
		**/
		class SDL2_LIB_INC AxisInput : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Virtual Axis Functions------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				AxisInput : getAxis - Retrieve the value of a Virtual Axis being monitored
				Created: 22/09/2017
				Modified: 12/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return const float& - Returns the value of the Virtual Axis as a const float reference
			*/
			const float& getAxis(const char* pAxis) const;

			/*
				AxisInput : getAxisDelta - Retrieve the change in axis value since the last cycle
				Created: 22/09/2017
				Modified: 12/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return float - Returns a float value containing the change in the Virtual Axis' value
			*/
			float getAxisDelta(const char* pAxis) const;

			/*
				AxisInput : btnDown - Treat the Virtual Axis as a button and check if it is currently 'down'
				Created: 22/09/2017
				Modified: 12/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true if the Virtual Axis is not equal to zero
			*/
			bool btnDown(const char* pAxis) const;

			/*
				AxisInput : btnPressed - Treat the Virtual Axis as a button and check if it was 'pressed'
				Created: 22/09/2017
				Modified: 12/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true the first cycle the Virtual Axis is not equal to zero
			*/
			bool btnPressed(const char* pAxis) const;

			/*
				AxisInput : btnReleased - Treat the Virtual Axis as a button and check if it was 'released'
				Created: 22/09/2017
				Modified: 12/10/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true the first cycle the Virtual Axis is equal to zero
			*/
			bool btnReleased(const char* pAxis) const;

			/*
				AxisInput : addAxis - Add a new Virtual Axis description to the monitor list
				Created: 22/09/2017
				Modified: 08/11/2017

				param[in] pAxis - The Virtual Axis object describing the new Virtual Axis
			*/
			void addAxis(const VirtualAxis& pAxis);

			/*
				AxisInput : addAxis -  Add an array of Virtual Axis descriptions to the monitor list
				Created: 22/09/2017
				Modified: 08/11/2017

				param[in] pAxis - A pointer to the array of Virtual Axis objects to monitor
				param[in] pCount - The number of Virtual Axis objects stored in the array
			*/
			void addAxis(const VirtualAxis* pAxis, const size_t& pCount);

			/*
				AxisInput : hasAxis - Check to see if a Virtual Axis exists in the monitor list
				Created: 13/10/2017
				Modified: 08/11/2017

				param[in] pAxis - A string defining the name of the Axis to retrieve

				return bool - Returns true if the specified axis name exists
			*/
			bool hasAxis(const char* pAxis) const;

			/*
				AxisInput : removeAxis - Clear all Virtual Axis' with a specific name
				Created: 22/09/2017
				Modified: 08/11/2017

				param[in] pAxis - A string defining the name of the Axis to remove
			*/
			void removeAxis(const char* pAxis);

			/*
				AxisInput : removeAxis - Clear all Virtual Axis'
				Created: 22/09/2017
				Modified: 08/11/2017
			*/
			void removeAxis();

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				AxisInput : Constructor - Initialise with default values
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pSetup - Defines how the Input Manager should be setup
			*/
			AxisInput(const Initialisation::AxisInputInitialiser& pSetup);

			/*
				AxisInput : createInterface - Verify and setup starting information
				Created: 11/10/2017
				Modified: 11/10/2017

				return bool - Returns true if the Resources Manager was setup correctly
			*/
			bool createInterface() override;

			/*
				AxisInput : destroyInterface - Deallocate internal memory allocated
				Created: 11/10/2017
				Modified: 11/10/2017
			*/
			void destroyInterface() override;

			/*
				AxisInput : update - Update the Virtual Axis
				Created: 11/10/2017
				Modified: 08/11/2017
			*/
			void update() override;

		private:
			//! Define the internal protected elements for the Renderer
			struct AxisInputInternalData;
			AxisInputInternalData* mData;
		};
	}
}