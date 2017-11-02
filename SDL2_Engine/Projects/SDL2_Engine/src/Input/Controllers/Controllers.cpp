#include "Controllers.hpp"

//! Include the global values required to get time information
#include "../../Globals.hpp"
#include "../../Time.hpp"
#include "../../Math.hpp"
#include "../../Window/Window.hpp"

//! Include the STL objects used to track keyed values
#include <unordered_map>

//! Include the required SDL2_Engine objects
#include "GamePad.hpp"
#include "VibrationDescription.hpp"

//! Declare a simple enum to store GamePad indexes
enum EGamePadIndexs : unsigned char { GAMEPAD_ONE, GAMEPAD_TWO, GAMEPAD_THREE, GAMEPAD_FOUR, GAMEPAD_TOTAL };

//! Label the different states to be handled
enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

//! Macro for converting SINGLE EGamePadID value to an index
#define ID2IND(X) (int)(log((int)(X)) / log(2))

/*
	fsqrt - Assembly fast square root function
	Author: Unknown

	param[in] n - The number to take the square root of

	return double - Returns the answer as a double value

	NOTE:
	Function was found at https://www.codeproject.com/Articles/69941/Best-Square-Root-Method-Algorithm-Function-Precisi
*/
double inline __declspec (naked) __fastcall fsqrt(double n) {
	_asm fld qword ptr[esp + 4]
		_asm fsqrt
	_asm ret 8
}

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
			Modified: 02/11/2017

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
					mData->gamepads[i].btnDown(pBtns))	//GamePad has the buttons down
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : btnPressed - Test to see if the specified GamePad(s) have just pressed at least one of the specified buttons after all buttons were previously released
			Created: 22/09/2017
			Modified: 02/11/2017

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
					mData->gamepads[i].btnPressed(pBtns))	//GamePad has pressed a button
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : btnReleased - Test to see if the specified GamePad(s) have just released at least one of the specified buttons after all buttons were previously pressed
			Created: 22/09/2017
			Modified: 02/11/2017

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
					mData->gamepads[i].btnReleased(pBtns))	//GamePad has released a button
					mask |= (1 << i);
			}

			//Return the mask
			return mask;
		}

		/*
			Controllers : rawAxis - Retrieve the raw axis value from a specific or multiple GamePads
			Created: 22/09/2017
			Modified: 02/11/2017

			param[in] pAxis - The EGamePadAxisCode value to retrieve
			param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

			return const float - Returns the average axis value as a const float within the range -1.f to 1.f
		*/
		const float Controllers::rawAxis(const EGamePadAxisCodes& pAxis, const Utilities::Bitmask<EGamePadID>& pIDs /*= EGamePadID::All*/) const noexcept { 
			//Check if the mask is multiple or a single ID
			int s = fsqrt(pIDs.getMask());
			if ((s * s) == pIDs.getMask()) 
				return mData->gamepads[ID2IND(pIDs.getMask())].rawAxis(pAxis);

			//Store a axis strength value and counter for contributing GamePads
			float strength = 0.f, contributing = 0.f;

			//Loop through and take the average for the different controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				//Check the ID is in the mask
				if (pIDs & (1 << i) &&						//GamePad is in the pIDs mask
					mData->gamepads[i].rawAxis(pAxis)) {	//GamePad has input on this axis currently
					//Increment the contributing count
					contributing++;

					//Add the strength value
					strength += mData->gamepads[i].rawAxis(pAxis);
				}
			}

			//Return the averaged value
			return (contributing ? strength / contributing : 0.f);
		}

		/*
			Controllers : rawAxisDelta - Retrieve the change in raw axis value from a specific or multiple GamePads
			Created: 22/09/2017
			Modified: 02/11/2017

			param[in] pAxis - The EGamePadAxisCode value to retrieve
			param[in] pIDs - A Bitmask of EGamePadID values that make up the various GamePad(s) to check

			return const float - Returns a float value containing the average change in the specified axis' value
		*/
		const float Controllers::rawAxisDelta(const EGamePadAxisCodes& pAxis, const Utilities::Bitmask<EGamePadID>& pIDs /*= EGamePadID::All*/) const noexcept { 
			//Check if the mask is multiple or a single ID
			int s = fsqrt(pIDs.getMask());
			if ((s * s) == pIDs.getMask())
				return mData->gamepads[ID2IND(pIDs.getMask())].rawAxisDelta(pAxis);

			//Store a axis delta value and counter for contributing GamePads
			float delta = 0.f, contributing = 0.f;

			//Loop through and take the average for the different controllers
			for (int i = GAMEPAD_ONE; i < GAMEPAD_TOTAL; i++) {
				//Check the ID is in the mask
				if (pIDs & (1 << i) &&							//GamePad is in the pIDs mask
					mData->gamepads[i].rawAxisDelta(pAxis)) {	//GamePad has had a change of axis value
					//Increment the contributing count
					contributing++;

					//Add the axis delta value
					delta += mData->gamepads[i].rawAxisDelta(pAxis);
				}
			}

			//Return the averaged value
			return (contributing ? delta / contributing : 0.f);
		}

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
			Modified: 12/10/2017
		*/
		void Controllers::update() {
			//Check that the Window has focus
			if (!Globals::get<Window>().hasProperty(EWindowProperties::Focus)) return;

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

			//Check there are Vibration Descriptions to process
			if (mData->vibrationValues.size()) {
				//Add to the vibration timer
				mData->vibrationTimer += time.getRealDelta();

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
						val.timer += mData->vibrationFrequency * integral * time.getScale();

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
								scale = math.clamp01(scale);
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