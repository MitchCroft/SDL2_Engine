#include "Controllers.hpp"

//! Include the global values required to get time information
#include "../../Globals.hpp"
#include "../../Time.hpp"
#include "../../Math.hpp"

//! Include the STL objects used to track keyed values
#include <unordered_map>
#include <string>

//! Include the required SDL2_Engine objects
#include "GamePad.hpp"
#include "VirtualAxis.hpp"
#include "VibrationDescription.hpp"

//! Declare a simple enum to store GamePad indexes
enum EGamePadIndexs : unsigned char { GAMEPAD_ONE, GAMEPAD_TWO, GAMEPAD_THREE, GAMEPAD_FOUR, GAMEPAD_TOTAL };

//! Label the different states to be handled
enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

//! Macro for converting SINGLE EGamePadID value to an index
#define ID2IND(X) (int)(log((int)(X)) / log(2))

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: VibrationProgress
		 *		Author: Mitchell Croft
		 *		Created: 25/07/2017
		 *		Modified: 22/09/2017
		 *		
		 *		Purpose:
		 *		Store additional timer information along with the values
		 *		described in a VibrationDescription object
		**/
		struct VibrationProgress {
			//! Store a copy of the original Vibration Description
			VibrationDescription desc;

			//! Store the timer for the current 'animation'
			float timer;

			/*
				VibrationProgress : Constructor - Initialise with default values
				Created: 22/09/2017
				Modified: 22/09/2017

				param[in] pDesc - A VibrationDescription object defining the attributes of the vibration
			*/
			VibrationProgress(const VibrationDescription& pDesc = VibrationDescription()) :
				desc(pDesc),
				timer(0.f)
			{}
		};

		/*
		 *		Name: ControllersInternalData
		 *		Author: Mitchell Croft
		 *		Created: 25/07/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Abstract the STL objects away from the main Controllers
		 *		object to allow for simpler management of memory
		**/
		struct Controllers::ControllersInternalData {
			//! Store an array of the possible physical devices
			_GamePad gamepads[GAMEPAD_TOTAL]{ 0, 1, 2, 3 };

			//! Store the values required to intermittently poll for connected physical devices
			float pollInterval = 5.f;
			float pollTimer = 5.f;

			//! Store the values required update vibration 'animations' on physical devices
			float vibrationFrequency = 1.f / 20.f;
			float vibrationTimer = 0.f;

			//! Store the Vibration Progress values for each individual physical device
			std::unordered_map<EGamePadID, VibrationProgress> vibrationValues;

			//! Store a map of the Virtual Axis' that need monitoring
			std::unordered_multimap<std::string, VirtualAxis> monitoredAxis;

			//! Store separate states of information for the Virtual Axis
			std::unordered_map<std::string, float> inputAxis[STATE_TOTAL];
		};

		/*
			Controllers : getGamePad - Get a pointer to a specific GamePad object
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pID - The EgamePadID of the GamePad desired

			return GamePad - Returns a GamePad (const _GamePad*) object to be used for testing input
		*/
		GamePad Controllers::getGamePad(const EGamePadID& pID) const noexcept { return &(mData->gamepads[ID2IND(pID)]); }

		/*
			Controllers : btnDown - Test to see if the specified GamePad(s) are pressing at least one of the specified buttons
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
			param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

			return int - Returns an int bitmask that flags the different GamePad(s) that are pressing at least one of the specified buttons
		*/
		int Controllers::btnDown(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EGamePadID::All*/) const noexcept {
			//Create a bitmask
			int mask = 0;

			//Test the controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				if (pIDs & (1 << i) &&					//GamePad is in the pIDs mask
					mData->gamepads[i].mConnected &&	//GamePad is connected
					mData->gamepads[i].btnDown(pBtns))	//GamePad has the buttons down
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : btnPressed - Test to see if the specified GamePad(s) have just pressed at least one of the specified buttons after all buttons were previously released
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
			param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

			return int - Returns an int bitmask that flags the different GamePad(s) that have just pressed at least on the specified buttons
		*/
		int Controllers::btnPressed(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EGamePadID::All*/) const noexcept {
			//Create a bitmask
			int mask = 0;

			//Test the controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				if (pIDs & (1 << i) &&						//GamePad is in the pIDs mask
					mData->gamepads[i].mConnected &&		//GamePad is connected
					mData->gamepads[i].btnPressed(pBtns))	//GamePad has pressed a button
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : btnReleased - Test to see if the specified GamePad(s) have just released at least one of the specified buttons after all buttons were previously pressed
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pBtns - A Bitmask of EGamePadBtnCodes that make up the various buttons to check
			param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

			return int - Returns an int bitmask that flags the different GamePad(s) that have just released at least on the specified buttons
		*/
		int Controllers::btnReleased(const Utilities::Bitmask<EGamePadBtnCodes>& pBtns, const Utilities::Bitmask<EGamePadID>& pIDs /*= EGamePadID::All*/) const noexcept {
			//Create a bitmask
			int mask = 0;

			//Test the controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				if (pIDs & (1 << i) &&						//GamePad is in the pIDs mask
					mData->gamepads[i].mConnected &&		//GamePad is connected
					mData->gamepads[i].btnReleased(pBtns))	//GamePad has released a button
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : rawAxis - Retrieve the raw axis value from a specific GamePad
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pAxis - The EGamePadAxisCode value to retrieve
			param[in] pID - The EGamePadID value defining the controller to retrieve input from

			return const float& - Returns the axis value as a const float reference within the range -1.f to 1.f
		*/
		const float& Controllers::rawAxis(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) const noexcept { return mData->gamepads[ID2IND(pID)].rawAxis(pAxis); }

		/*
			Controllers : rawAxisDelta - Retrieve the change in raw axis value from a specified GamePad
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pAxis - The EGamePadAxisCode value to retrieve
			param[in] pID - The EGamePadID value defining the controller to retrieve input from

			return float - Returns a float value containing the change in the specified axis' value
		*/
		const float Controllers::rawAxisDelta(const EGamePadAxisCodes& pAxis, const EGamePadID& pID) const noexcept { return mData->gamepads[ID2IND(pID)].rawAxisDelta(pAxis); }

		/*
			Controllers : applyVibration - Add a vibration description to the Controller Manager
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pDesc - A VibrationDescription object describing how the vibration will occur
		*/
		void Controllers::applyVibration(const VibrationDescription& pDesc) const noexcept {
			//Loop through the Bitmask IDs
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				//Check if the GamePad is in the mask
				if (pDesc.gamePad & (1 << i))
					mData->vibrationValues.insert(std::pair<EGamePadID, VibrationProgress>((EGamePadID)(1 << i), { pDesc }));
			}
		}

		/*
			Controllers : resetVibration - Reset the vibration of the specified GamePad(s)
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pIDs - A Bitmask of the EGamePadID value(s) to reset
		*/
		void Controllers::resetVibration(const Utilities::Bitmask<EGamePadID>& pIDs) const noexcept {
			//Create an empty description
			VibrationDescription desc;

			//Apply the mask
			desc.gamePad = pIDs;

			//Apply the description
			applyVibration(desc);
		}

		/*
			Controllers : vAxis - Retrieve the value of a Virtual Axis being monitored
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return const float& - Returns the value of the Virtual Axis as a const float reference
		*/
		const float& Controllers::vAxis(const std::string& pAxis) const noexcept { return mData->inputAxis[STATE_CUR][pAxis]; }

		/*
			Controllers : vAxisDelta - Retrieve the change in axis value since the last cycle
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return float - Returns a float value containing the change in the Virtual Axis' value
		*/
		float Controllers::vAxisDelta(const std::string& pAxis) const noexcept { return (mData->inputAxis[STATE_CUR][pAxis] - mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			Controllers : vBtnDown - Treat the Virtual Axis as a button and check if it is currently 'down'
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true if the Virtual Axis is not equal to zero
		*/
		bool Controllers::vBtnDown(const std::string& pAxis) const noexcept { return (mData->inputAxis[STATE_CUR][pAxis] != 0.f); }

		/*
			Controllers : vBtnPressed - Treat the Virtual Axis as a button and check if it was 'pressed'
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true the first cycle the Virtual Axis is not equal to zero
		*/
		bool Controllers::vBtnPressed(const std::string& pAxis) const noexcept { return (mData->inputAxis[STATE_CUR][pAxis] && !mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			Controllers : vBtnReleased - Treat the Virtual Axis as a button and check if it was 'released'
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to retrieve

			return bool - Returns true the first cycle the Virtual Axis is equal to zero
		*/
		bool Controllers::vBtnReleased(const std::string& pAxis) const noexcept { return (!mData->inputAxis[STATE_CUR][pAxis] && mData->inputAxis[STATE_PRE][pAxis]); }

		/*
			Controllers : addVirtualAxis - Add a new Virtual Axis description to the monitor list
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - The Virtual Axis object describing the new Virtual Axis
		*/
		void Controllers::addVirtualAxis(const VirtualAxis& pAxis) const noexcept { mData->monitoredAxis.insert(std::pair<std::string, VirtualAxis>(pAxis.name, pAxis)); }

		/*
			Controllers : addVirtualAxis - Add an array of Virtual Axis descriptions to the monitor list
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pAxis - A pointer to the array of Virtual Axis objects to monitor
			param[in] pCount - The number of Virtual Axis objects stored in the array
		*/
		void Controllers::addVirtualAxis(const VirtualAxis*& pAxis, const size_t& pCount) const noexcept {
			for (size_t i = 0; i < pCount; i++) 
				mData->monitoredAxis.insert(std::pair<std::string, VirtualAxis>(pAxis[i].name, pAxis[i]));
		}

		/*
			Controllers : removeVirtualAxis - Clear all Virtual Axis' with a specific name
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pAxis - A string defining the name of the Axis to remove
		*/
		void Controllers::removeVirtualAxis(const std::string& pAxis) const noexcept {
			//Clear all traces of the axis
			mData->monitoredAxis.erase(pAxis); 
			mData->inputAxis[STATE_CUR].erase(pAxis); 
			mData->inputAxis[STATE_PRE].erase(pAxis);
		}

		/*
			Controllers : removeVirtualAxis - Clear all Virtual Axis'
			Created: 22/09/2017
			Modified: 22/09/2017
		*/
		void Controllers::removeVirtualAxis() const noexcept {
			//Clear all Virtual Axis
			mData->monitoredAxis.clear();
			mData->inputAxis[STATE_CUR].clear();
			mData->inputAxis[STATE_PRE].clear();
		}

		/*
			Controllers : getPollInterval - Get the amount of time between checks for recently connected GamePads
			Created: 22/09/2017
			Modified: 22/09/2017

			return const float& - Returns a const float reference to the poll interval value
		*/
		const float& Controllers::getPollInterval() const noexcept { return mData->pollInterval; }

		/*
			Controllers : setPollInterval - Set the amount of time between checks for recently connected GamePads
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pInt - A float value defining the time (seconds) between GamePad polling
		*/
		void Controllers::setPollInterval(const float& pInt) const noexcept { mData->pollInterval = (pInt > 0.f ? pInt : 0.f); }

		/*
			Controllers : getVibrationFrequancy - Get the refresh rate of the GamePad Vibration Descriptions
			Created: 22/09/2017
			Modified: 22/09/2017

			return const float - Returns a const float value containing the refresh rate of the Vibration Descriptions
		*/
		const float Controllers::getVibrationFrequancy() const noexcept { return (1.f / mData->vibrationFrequency); }

		/*
			Controllers : setVibrationFrequancy - Set the refresh rate of the GamePad Vibration Descriptions
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pFreq - A float value defining the refresh rate (hertz) of the Vibration Descriptions
		*/
		void Controllers::setVibrationFrequancy(const float& pFreq) const noexcept { mData->vibrationFrequency = (1.f / (pFreq > 1.f ? pFreq : 1.f)); }

		/*
			Controllers : Constructor - Initialise with default values
			Created: 25/07/2017
			Modified: 22/09/2017
		*/
		Controllers::Controllers() : mData(nullptr) {}

		/*
			Controllers : createInterface - Allocate the resources required for the Controllers object
			Created: 25/07/2017
			Modified: 22/09/2017

			return bool - Returns true if the Keyboard object was created successfully
		*/
		bool Controllers::createInterface() {
			//Create the internal data storage object
			mData = new ControllersInternalData();

			//Return success
			return true;
		}

		/*
			Controllers : destroyInterface - Deallocate the resources used by the Controllers object
			Created: 25/07/2017
			Modified: 22/09/2017
		*/
		void Controllers::destroyInterface() { if (mData) delete mData; }

		/*
			Controllers : update - Update the GamePads input states
			Created: 25/07/2017
			Modified: 22/09/2017
		*/
		void Controllers::update() {
			//Get the Global Objects
			const Time& time = Globals::get<Time>();
			const Math& math = Globals::get<Math>();

			//Update the poll timer
			mData->pollTimer += time.getRealDelta();

			//Check if new GamePads should be found
			bool attemptReconnect = (mData->pollTimer >= mData->pollInterval);

			//Reset the poll timer
			if (attemptReconnect) mData->pollTimer = 0.f;

			//Loop through and update all controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++)
				mData->gamepads[i].update(attemptReconnect);

			//Update the internal values
			updateVirtualAxis(time, math);
			updateVibrations(time, math);
		}

		/*
			Controllers : updateVirtualAxis - Update the Virtual Axis that are being monitored and their respective values
			Created: 22/09/2017
			Modified: 06/10/2017

			param[in] pTime - The Time object in use for this cycle
			param[in] pMath - The Math object in use for this cycle
		*/
		void Controllers::updateVirtualAxis(const Time& pTime, const Math& pMath) const {
			//Loop through all monitored Axis'
			for (int i = (int)mData->monitoredAxis.bucket_count() - 1; i >= 0; i--) {
				//Get the first element
				auto bucket = mData->monitoredAxis.begin(i);

				//Get the final iterator
				auto end = mData->monitoredAxis.end(i);

				//Check there are elements in the bucket
				if (bucket == end) continue;

				//Store the name of the Virtual Axis
				const std::string& V_AXIS_NAME = bucket->first;

				//Store the number of Axis' contributing to these values
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

					//Get a reference to current Virtual Axis
					const VirtualAxis& axis = bucket->second;

					//Add the gravity to the running sum
					gravAvg += axis.gravity;

					//Store the strength of this input
					float axisStrength = 0.f;

					///////////////////////////////////////////////////////////////////////////////////////////////
					////                                Input Is Based on Axis                                 ////
					///////////////////////////////////////////////////////////////////////////////////////////////
					if (axis.inputType == EGamePadInputType::Axis) {
						//Get the axis strength value
						const float AXIS_VALUE = rawAxis(axis.aAxis, axis.gamePad);

						//Store the sign of the axis value
						const float SIGN = pMath.sign(AXIS_VALUE);

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
						//Get the GamePad
						GamePad pad = getGamePad(axis.gamePad);

						//Check for positive button press(es)
						if (pad->btnDown({ axis.bPosBtn, axis.bAltPosBtn }))
							axisStrength += 1.f;

						//Check for negative button press(es)
						if (pad->btnDown({ axis.bNegBtn, axis.bAltNegBtn }))
							axisStrength -= 1.f;
					}

					//Apply the sensitivity and inversion flags
					axisStrength *= axis.sensitivity * (axis.invert ? -1.f : 1.f);

					//Check if the strength is stronger then the current
					if (abs(axisStrength) > abs(strongestAxis)) strongestAxis = axisStrength;
				}

				//Add the strength to the current input state
				if (strongestAxis && abs(strongestAxis) > abs(mData->inputAxis[STATE_CUR][V_AXIS_NAME])) 
					mData->inputAxis[STATE_CUR][V_AXIS_NAME] = pMath.clamp(mData->inputAxis[STATE_CUR][V_AXIS_NAME] + strongestAxis * pTime.getDelta(), -1.f, 1.f);

				//Otherwise apply gravity
				else if (gravAvg && mData->inputAxis[STATE_CUR][V_AXIS_NAME]) {
					//Get the inverse direction
					const float INV_DIR = pMath.sign(mData->inputAxis[STATE_CUR][V_AXIS_NAME]) * -1.f;

					//Average out the gravity values
					gravAvg /= (float)contributingAxis;

					//Get the value after gravity is applied
					const float APP_GRAV_VAL = mData->inputAxis[STATE_CUR][V_AXIS_NAME] + gravAvg * INV_DIR * pTime.getDelta();

					//Assign the axis value
					mData->inputAxis[STATE_CUR][V_AXIS_NAME] = (pMath.sign(APP_GRAV_VAL) == INV_DIR ? 0.f : APP_GRAV_VAL);
				}
			}
		}

		/*
			Controllers : updateVibrations - Update the GamePads Vibration Values
			Created: 22/09/2017
			Modified: 22/09/2017

			param[in] pTime - The Time object in use for this cycle
			param[in] pMath - The Math object in use for this cycle
		*/
		void Controllers::updateVibrations(const Time& pTime, const Math& pMath) const {
			//Check there are Vibration Descriptions to process
			if (mData->vibrationValues.size()) {
				//Add to the vibration timer
				mData->vibrationTimer += pTime.getRealDelta();

				//Check if the timer is over the frequency value
				if (mData->vibrationTimer >= mData->vibrationFrequency) {
					//Spend the built up time
					float integral;
					mData->vibrationTimer = modf(mData->vibrationTimer / mData->vibrationFrequency, &integral) * mData->vibrationFrequency;

					//Loop through the Vibration Descriptions
					for (auto it = mData->vibrationValues.begin(); it != mData->vibrationValues.end();) {
						//Get a reference to the Vibration Description
						VibrationProgress& val = it->second;

						//Add the elapsed time 
						val.timer += mData->vibrationFrequency * integral * pTime.getScale();

						//Get the GamePad that is being accessed
						GamePad pad = getGamePad(it->first);

						//Check if the GamePad is connected
						if (pad->mConnected) {
							//Store the scale factor
							float scale;

							//Check if the time is up
							if (val.timer >= val.desc.vibrationLength)
								scale = 0;

							//Check if there an action
							else if (val.desc.scaleFunc) {
								//Apply the function to the value
								val.desc.scaleFunc(scale, val.timer / val.desc.vibrationLength);

								//Clamp the value to 0 - 1 
								scale = pMath.clamp01(scale);
							}

							//Otherwise scale constantly
							else scale = 1.f;

							//Apply the vibration to the GamePad
							pad->vibrate(val.desc.leftVibration * scale, val.desc.rightVibration * scale);
						}

						//Check for removal
						if (val.timer >= val.desc.vibrationLength)
							mData->vibrationValues.erase(it++);
						else ++it;
					}
				}
			}
		}
	}
}