#include "AxisInput.hpp"

//! Include the SDL2_Engine objects
#include "../Globals.hpp"

#include "../Math.hpp"
#include "../Time.hpp"

#include "../Window/Window.hpp"

#include "VirtualAxis.hpp"
#include "AxisInputInitialiser.hpp"

#include "Controllers/Controllers.hpp"
#include "Keyboard/Keyboard.hpp"
#include "Mouse/Mouse.hpp"

//! Include the required STL objects
#include <unordered_map>
#include <string>

//! Label the different states to be handled
enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: AxisInputInternalData
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 12/10/2017
		 *		
		 *		Purpose:
		 *		Store the internal data for the Input object
		**/
		struct AxisInput::AxisInputInternalData {
			//! Temporary storage of the Initialiser
			const Initialisation::AxisInputInitialiser* setup = nullptr;

			//! Store a map of the Virtual Axis' that need monitoring
			std::unordered_multimap<std::string, VirtualAxis> monitoredAxis;

			//! Store separate states of information for the Virtual Axis
			std::unordered_map<std::string, float> inputAxis[STATE_TOTAL];
		};

		/*
			AxisInput : getAxis - Retrieve the value of a Virtual Axis being monitored
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return const float& - Returns the value of the Virtual Axis as a const float reference
		*/
		const float& AxisInput::getAxis(const char* pAxis) const { return mData->inputAxis[STATE_CUR][pAxis]; }

		/*
			AxisInput : getAxisDelta - Retrieve the change in axis value since the last cycle
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return float - Returns a float value containing the change in the Virtual Axis' value
		*/
		float AxisInput::getAxisDelta(const char* pAxis) const { return (mData->inputAxis[STATE_CUR][pAxis] - mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			AxisInput : btnDown - Treat the Virtual Axis as a button and check if it is currently 'down'
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true if the Virtual Axis is not equal to zero
		*/
		bool AxisInput::btnDown(const char* pAxis) const { return (mData->inputAxis[STATE_CUR][pAxis] != 0.f); }

		/*
			AxisInput : btnPressed - Treat the Virtual Axis as a button and check if it was 'pressed'
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true the first cycle the Virtual Axis is not equal to zero
		*/
		bool AxisInput::btnPressed(const char* pAxis) const { return (mData->inputAxis[STATE_CUR][pAxis] && !mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			AxisInput : btnReleased - Treat the Virtual Axis as a button and check if it was 'released'
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true the first cycle the Virtual Axis is equal to zero
		*/
		bool AxisInput::btnReleased(const char* pAxis) const { return (!mData->inputAxis[STATE_CUR][pAxis] && mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			AxisInput : addAxis - Add a new Virtual Axis description to the monitor list
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - The Virtual Axis object describing the new Virtual Axis
		*/
		void AxisInput::addAxis(const VirtualAxis& pAxis) { mData->monitoredAxis.insert(std::pair<std::string, VirtualAxis>(pAxis.name.c_str(), pAxis)); }

		/*
			AxisInput : addAxis -  Add an array of Virtual Axis descriptions to the monitor list
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pAxis - A pointer to the array of Virtual Axis objects to monitor
			param[in] pCount - The number of Virtual Axis objects stored in the array
		*/
		void AxisInput::addAxis(const VirtualAxis* pAxis, const size_t& pCount) {
			for (size_t i = 0; i < pCount; i++)
				mData->monitoredAxis.insert(std::pair<std::string, VirtualAxis>(pAxis[i].name.c_str(), pAxis[i]));
		}

		/*
			AxisInput : removeAxis - Clear all Virtual Axis' with a specific name
			Created: 22/09/2017
			Modified: 12/10/2017

			param[in] pAxis - A string defining the name of the Axis to remove
		*/
		void AxisInput::removeAxis(const char* pAxis) {
			//Clear all traces of the axis
			mData->monitoredAxis.erase(pAxis);
			mData->inputAxis[STATE_CUR].erase(pAxis);
			mData->inputAxis[STATE_PRE].erase(pAxis);
		}

		/*
			AxisInput : removeAxis - Clear all Virtual Axis'
			Created: 22/09/2017
			Modified: 12/10/2017
		*/
		void AxisInput::removeAxis() {
			//Clear all Virtual Axis
			mData->monitoredAxis.clear();
			mData->inputAxis[STATE_CUR].clear();
			mData->inputAxis[STATE_PRE].clear();
		}

		/*
			AxisInput : Constructor - Initialise with default values
			Created: 11/10/2017
			Modified: 11/10/2017

			param[in] pSetup - Defines how the Input Manager should be setup
		*/
		AxisInput::AxisInput(const Initialisation::AxisInputInitialiser& pSetup) : mData(nullptr) {
			//Create the data object
			mData = new AxisInputInternalData();

			//Stow the Initialiser values
			mData->setup = &pSetup;
		}

		/*
			AxisInput : createInterface - Verify and setup starting information
			Created: 11/10/2017
			Modified: 11/10/2017

			return bool - Returns true if the Resources Manager was setup correctly
		*/
		bool AxisInput::createInterface() {
			//Get the initialiser object
			auto setup = mData->setup;
			mData->setup = nullptr;

			//Check if there are default Virtual Axis
			if (setup->defaultAxis) addAxis(setup->defaultAxis, setup->count);

			//Return success
			return true;
		}

		/*
			AxisInput : destroyInterface - Deallocate internal memory allocated
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void AxisInput::destroyInterface() { if (mData) delete mData; mData = nullptr; }

		/*
			AxisInput : update - Update the Virtual Axis
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void AxisInput::update() {
			//Check that the Window has focus
			if (!Globals::get<Window>().hasProperty(EWindowProperties::Focus)) return;

			//Get the time and Math objects
			const Time& time = Globals::get<Time>();
			const Math& math = Globals::get<Math>();

			//Get the other Input Managers
			const Controllers& controllers = Globals::get<Controllers>();
			const Keyboard& keyboard = Globals::get<Keyboard>();
			const Mouse& mouse = Globals::get<Mouse>();

			//Loop through all monitored axis
			for (int i = (int)mData->monitoredAxis.bucket_count() - 1; i >= 0; --i) {
				//Get the first element
				auto bucket = mData->monitoredAxis.begin(i);

				//Get the final iterator
				auto end = mData->monitoredAxis.end(i);

				//Check there are elements in the bucket
				if (bucket == end) continue;

				//Store the name of the Virtual axis
				const std::string& V_AXIS_NAME = bucket->first;

				//Store the number of Virtual axis' contributing to these values
				size_t contributingAxis = 0U;

				//Store the strongest contributer
				float strongestAxis = 0.f;

				//Store the average gravity of all Virtual Axis for this name
				float gravAvg = 0.f;

				//Copy the previous value of the axis
				mData->inputAxis[STATE_PRE][V_AXIS_NAME] = mData->inputAxis[STATE_CUR][V_AXIS_NAME];

				//Loop through all monitored Virtual Axis with the same name
				for (; bucket != end; ++bucket) {
					//Add to the contributing Axis' counter
					++contributingAxis;

					//Get a reference to the current Virtual Axis
					const VirtualAxis& axis = bucket->second;

					//Add the gravity to the running sum
					gravAvg += axis.gravity;

					//Store the strength of this input
					float axisStrength = 0.f;
					
					//Switch based on the Input Device
					switch (axis.inputType) {
					case EInputDevice::GamePads: {
						///////////////////////////////////////////////////////////////////////////////////////////////
						////                                Input Is Based on Axis                                 ////
						///////////////////////////////////////////////////////////////////////////////////////////////
						if (axis.gamePadInputType == EGamePadInputType::Axis) {
							//Get the axis strength value
							const float AXIS_VALUE = controllers.rawAxis(axis.aAxis, axis.gamePads.getValue());

							//Store the sign of the axis value
							const float SIGN = math.sign(AXIS_VALUE);

							//Store the dead zone 
							const float DEAD_ZONE_SQ = axis.aDeadZone * axis.aDeadZone;

							//Normalise the axis value
							const float AXIS_NORM_SQ = (AXIS_VALUE * AXIS_VALUE) - DEAD_ZONE_SQ;

							//Scale the axis strength based on the dead zone
							if (AXIS_NORM_SQ) axisStrength += (AXIS_NORM_SQ / (1.f - DEAD_ZONE_SQ)) * SIGN;
						}

						///////////////////////////////////////////////////////////////////////////////////////////////
						////                              Input Is Based on Buttons                                ////
						///////////////////////////////////////////////////////////////////////////////////////////////
						else {
							//Check for positive key press(es)
							if (controllers.btnDown({ axis.bPosBtn, axis.bAltPosBtn }, axis.gamePads))
								axisStrength += 1.f;

							//Check for negative key press(es)
							if (controllers.btnDown({ axis.bNegBtn, axis.bAltNegBtn }, axis.gamePads))
								axisStrength -= 1.f;
						}

						break; }
					case EInputDevice::Keyboard: {
						//Check for positive key press(es)
						if (keyboard.keyDown(axis.kPosBtn) || keyboard.keyDown(axis.kAltPosBtn))
							axisStrength += 1.f;

						//Check for negative key press(es)
						if (keyboard.keyDown(axis.kNegBtn) || keyboard.keyDown(axis.kAltNegBtn))
							axisStrength -= 1.f;

						break; }
					case EInputDevice::Mouse: {
						//Use the delta movement for the specified axis
						axisStrength += mouse.getDelta(axis.mAxis);
						break; }
					}

					//Apply the sensitivity and inversion flags
					axisStrength *= axis.sensitivity * (axis.invert ? -1.f : 1.f);

					//Check if the strength is stronger then the current
					if (abs(axisStrength) > abs(strongestAxis)) strongestAxis = axisStrength;
				}

				//Add the strength to the current Input state
				if (strongestAxis && abs(strongestAxis) > abs(mData->inputAxis[STATE_CUR][V_AXIS_NAME]))
					mData->inputAxis[STATE_CUR][V_AXIS_NAME] = math.clamp(mData->inputAxis[STATE_CUR][V_AXIS_NAME] + strongestAxis * time.getDelta(), -1.f, 1.f);

				//Otherwise apply gravity
				else if (gravAvg && mData->inputAxis[STATE_CUR][V_AXIS_NAME]) {
					//Get the inverse direction
					const float INV_DIR = math.sign(mData->inputAxis[STATE_CUR][V_AXIS_NAME]) * -1.f;

					//Average out the gravity values
					gravAvg /= (float)contributingAxis;

					//Get the value after gravity is applied
					const float APP_GRAV_VAL = mData->inputAxis[STATE_CUR][V_AXIS_NAME] + gravAvg * INV_DIR * time.getDelta();

					//Assign the axis value
					mData->inputAxis[STATE_CUR][V_AXIS_NAME] = (math.sign(APP_GRAV_VAL) == INV_DIR ? 0.f : APP_GRAV_VAL);
				}
			}
		}
	}
}
