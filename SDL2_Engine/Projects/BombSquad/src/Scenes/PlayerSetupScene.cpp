#include "PlayerSetupScene.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include <Input/StringToInputValues.hpp>
using namespace SDL2_Engine;
using namespace SDL2_Engine::Input;

//! Include the Objx Parser to read control scheme data
#include <Objx_node.hpp>
#include <Objx_document.hpp>
#include <Objx_reader.hpp>

namespace BombSquad {
	/*
		PlayerSetupScene : Constructor - Initialise with default values
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	PlayerSetupScene::PlayerSetupScene() {}

	/*
		PlayerSetupScene : createScene - Load the values required for the Scene to operate
		Created: 06/11/2017
		Modified: 06/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool PlayerSetupScene::createScene() {
		//Load the spritesheet or whatever is required to display the characters
		//TODO
		
		//Retrieve the logging object
		Debug::Logger& logger = Globals::get<Debug::Logger>();
		
		//Create the Objx Reader
		bool flag = false;
		Objx::Reader reader([&](const Objx::EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
			//Output error header
			if (!flag) logger.logError("The Player Setup Scene encountered an error/errors when attempting to parse the control scheme list located at 'resources/controls.objx'");

			//Output the error information
			logger.logError("Line: %zu\tColumn: %zu\t\t%s\t(%s, %i)", pLine, pColumn, pMsg, Objx::statusCodeToString(pCode), (int)pCode);

			//Toggle the flag
			flag = true;
		}, 2);

		//Attempt to parse the loaded data
		const Objx::Document DOC = reader.cparse("resources/controls.objx");

		//Check if an error was encountered
		if (flag) return false;

		//Check there are control schemes loaded
		else if (!DOC.size()) {
			//Output error message
			logger.logError("The Player Setup Scene loaded no Control Schemes from the file 'resources/controls.objx'. There needs to be a minimum of four to proceed");

			//Return failure
			return false;
		}

		//Store the names of the objects to load
		const char* NAMES[] = { "verticalMove", "horizontalMove", "place", "detonate" };
		const EPlayerAction ACTIONS[] = { EPlayerAction::Move_Vertical, EPlayerAction::Move_Horizontal, EPlayerAction::Place_Bomb, EPlayerAction::Detonate_Mine };

		//Store the number of actions to process
		const size_t ACTION_COUNT = sizeof(ACTIONS) / sizeof(EPlayerAction);

		//Process all loaded control Schemes
		for (auto iter = DOC.cbegin(); iter != DOC.cend(); ++iter) {
			//Create a new Control Scheme object
			ControlScheme newScheme;

			//Switch on the physical device being used
			switch (stringToInputType(iter->readProp("inputDevice").readVal<Objx::xstring>().c_str())) {
			case EInputDevice::GamePad: {
				//Get the ID of the GamePad to setup for
			 	Utilities::Bitmask<EGamePadID> gamePadID = stringToGamePadID(iter->readProp("id").readVal<Objx::xstring>().c_str());

				//Loop through and find the Axis' that need setting up
				for (int player = 0; player < 4; player++) {
					//Check if this Game Pad is in the mask
					if (gamePadID & (1 << player)) {
						//Assign the GamePad ID to the Control Scheme
						newScheme.setGamePadID((EGamePadID)(1 << player));

						//Loop through the actions to process
						for (size_t i = 0; i < ACTION_COUNT; i++) {
							//Setup for the action object
							const Objx::Node actionObj = iter->readProp(NAMES[i]).readVal<Objx::xobjx>();
							VirtualAxis actionAxis; actionAxis.inputType = EInputDevice::GamePad; actionAxis.gamePads = (1 << player);

							//Read the action object values
							actionAxis.sensitivity		= (float)actionObj["sensitivity"].readVal<Objx::xdec>(1000.0);
							actionAxis.gravity			= (float)actionObj["gravity"].readVal<Objx::xdec>(1000.0);
							actionAxis.invert			= actionObj["invert"].readVal<Objx::xbool>(false);
							actionAxis.gamePadInputType	= stringToGamePadInputType(actionObj["inputType"].readVal<Objx::xstring>().c_str());
							actionAxis.bPosBtn			= stringToGamePadBtn(actionObj["posBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bAltPosBtn		= stringToGamePadBtn(actionObj["altPosBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bNegBtn			= stringToGamePadBtn(actionObj["negBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bAltNegBtn		= stringToGamePadBtn(actionObj["altNegBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.aAxis			= stringToGamePadAxis(actionObj["axis"].readVal<Objx::xstring>().c_str());
							actionAxis.aDeadZone		= (float)actionObj["deadZone"].readVal<Objx::xdec>(0.1);

							//Add action movement object to the scheme
							newScheme.addActionControl(ACTIONS[i], actionAxis);
						}
					}
				}
				break; }
			case EInputDevice::Keyboard: {
				//Loop through the actions to process
				for (size_t i = 0; i < ACTION_COUNT; i++) {
					//Setup for the action object
					const Objx::Node actionObj = iter->readProp(NAMES[i]).readVal<Objx::xobjx>();
					VirtualAxis actionAxis; actionAxis.inputType = EInputDevice::Keyboard;

					//Read the action object values
					actionAxis.sensitivity				= (float)actionObj["sensitivity"].readVal<Objx::xdec>(1000.0);
					actionAxis.gravity					= (float)actionObj["gravity"].readVal<Objx::xdec>(1000.0);
					actionAxis.invert					= actionObj["invert"].readVal<Objx::xbool>(false);
					actionAxis.kPosBtn					= stringToKeyboardKey(actionObj["posBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kAltPosBtn				= stringToKeyboardKey(actionObj["altPosBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kNegBtn					= stringToKeyboardKey(actionObj["negBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kAltNegBtn				= stringToKeyboardKey(actionObj["altNegBtn"].readVal<Objx::xstring>().c_str());

					//Add action movement object to the scheme
					newScheme.addActionControl(ACTIONS[i], actionAxis);
				}
				break; }
			default:
				//Log the error
				logger.logError("Player Setup Scene encountered an unrecognised 'inputDevice' value '%s'. Please use either 'GamePad' or 'Keyboard'", iter->readProp("inputDevice").readVal<Objx::xstring>().c_str());
			}

			//Add the new scheme to the unclaimed list
			mUnclaimedSchemes.push_back(newScheme);
		}

		//Check there at least four control schemes loaded
		if (mUnclaimedSchemes.size() < 4) {
			//Output error message
			logger.logError("The Player Setup Scene loaded %zu Control Schemes from the file 'resources/controls.objx'. There needs to be a minimum of four to proceed", mUnclaimedSchemes.size());

			//Return failure
			return false;
		}

		//Bind all control schemes to demonstration setups
		for (size_t i = 0; i < mUnclaimedSchemes.size(); i++)
			mUnclaimedSchemes[i].bindControlScheme((EPlayerNumber)i);

		//Return success
		return true;
	}

	/*
		PlayerSetupScene : update - Update the internal values corresponding to player actions
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	void PlayerSetupScene::update() {
		//Check for exit back to main menu
		//TODO

		//Check if there is a need to check for more unclaimed options
		if (mClaimedSchemes.size() < 4 && mUnclaimedSchemes.size()) {
			//Loop through the unclaimed schemes to see if any have been activated
			for (int i = (int)mUnclaimedSchemes.size() - 1; i >= 0 && mClaimedSchemes.size() < 4; i--) {
				//Check if the place bomb action button was pressed
				if (mUnclaimedSchemes[i].actionPressed(EPlayerAction::Place_Bomb)) {
					//Move the unclaimed scheme to the claimed list
					mClaimedSchemes.push_back(mUnclaimedSchemes[i]);

					//Play a short vibration if it's a game pad
					mUnclaimedSchemes[i].deathVibration();

					//Remove the scheme from the unclaimed list
					mUnclaimedSchemes.erase(mUnclaimedSchemes.begin() + i);
				}
			}
		}

		//Check for progression to powerup selections
		//TODO

		//Check for changing the animation of claimed players
		//TODO
	}

	/*
		PlayerSetupScene : render - Render the Scene to the Window
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	void PlayerSetupScene::render() {
		//TODO
	}
}