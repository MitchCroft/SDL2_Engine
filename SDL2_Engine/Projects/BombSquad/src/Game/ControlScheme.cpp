#include "ControlScheme.hpp"

//! Include the remaining SDL2_Engine objects
#include <Globals.hpp>
#include <Input/AxisInput.hpp>

#include <Input/Controllers/VibrationDescription.hpp>
#include <Input/Controllers/Controllers.hpp>

namespace BombSquad {
	/*
		playerNumberToString - Convert an EPlayerNumber value to a string
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pNum - The EPlayerNumber value to convert

		return std::string - Returns a string object containing the converted player number
	*/
	std::string playerNumberToString(const EPlayerNumber& pNum) {
		switch (pNum) {
		case EPlayerNumber::Unassigned: return "Unassigned";
		case EPlayerNumber::One: return "One";
		case EPlayerNumber::Two: return "Two";
		case EPlayerNumber::Three: return "Three";
		case EPlayerNumber::Four: return "Four";
		default: return "Demonstration" + std::to_string((int)pNum);
		}
	}

	/*
		reverseScale - Reverse a 0-1 scale float value
		Author: Mitchell Croft
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pVal - An assumed 0-1 scale value

		return float - Returns the reversed value as a float
	*/
	inline float reverseScale(const float& pVal) { return (1.f - pVal); }

	/*
		ControlScheme : Constructor - Initialise with default values
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	ControlScheme::ControlScheme() :
		mGamePadID(SDL2_Engine::Input::EGamePadID::Null),
		mPlayerID(EPlayerNumber::Unassigned)
	{}

	/*
		ControlScheme : setGamePadID - Set the Game Pad ID used for the control scheme
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pID - The new EGamePadID to give to the Control Scheme

		return bool - Returns true if the GamePadID was set. GamePadID can not be set if the scheme has been bound
	*/
	bool ControlScheme::setGamePadID(const SDL2_Engine::Input::EGamePadID& pID) {
		//Check scheme is not in use
		if (mPlayerID != EPlayerNumber::Unassigned) return false;

		//Set the Game Pad ID
		mGamePadID = pID;
	}

	/*
		ControlScheme : addActionControl - Add a Virtual Axis that will be used to control a specific action
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pAction - An EPlayerAction value that defines what action the should be controlled by pAxis
		param[in] pAxis - A SDL2_Engine Virtual Axis that defines the controls for the action

		return bool - Returns true if the action was set. Action controls can not be set if the scheme has been bound
	*/
	bool ControlScheme::addActionControl(const EPlayerAction& pAction, const SDL2_Engine::Input::VirtualAxis& pAxis) {
		//Check scheme is not in use
		if (mPlayerID != EPlayerNumber::Unassigned) return false;

		//Add the Virtual Axis to the map
		mActionControls[pAction] = pAxis;

		//Return success
		return true;
	}

	/*
		ControlScheme : bindControlScheme - Bind the currently contained controls to the SDL2_Engine Axis Input Manager
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pPlayerNum - A EPlayerNumber value defining the player the control scheme will be used for

		return bool - Returns true if the control scheme was successfully bound
	*/
	bool ControlScheme::bindControlScheme(const EPlayerNumber& pPlayerNum) {
		//Check scheme is not in use
		if (mPlayerID != EPlayerNumber::Unassigned) return false;

		//Store the new player ID value
		mPlayerID = pPlayerNum;

		//Store the formatted first section
		std::string firstSec = playerNumberToString(mPlayerID) + std::string("_");

		//Store the Action Lookup strings
		mActionLookup[EPlayerAction::Move_Vertical]		= firstSec + "Vertical";
		mActionLookup[EPlayerAction::Move_Horizontal]	= firstSec + "Horizontal";
		mActionLookup[EPlayerAction::Place_Bomb]		= firstSec + "Place";
		mActionLookup[EPlayerAction::Detonate_Mine]		= firstSec + "Detonate";

		//Store the names in the stored virtual axis'
		mActionControls[EPlayerAction::Move_Vertical].name = mActionLookup[EPlayerAction::Move_Vertical].c_str();
		mActionControls[EPlayerAction::Move_Horizontal].name = mActionLookup[EPlayerAction::Move_Horizontal].c_str();
		mActionControls[EPlayerAction::Place_Bomb].name = mActionLookup[EPlayerAction::Place_Bomb].c_str();
		mActionControls[EPlayerAction::Detonate_Mine].name = mActionLookup[EPlayerAction::Detonate_Mine].c_str();

		//Get the Axis Input object
		SDL2_Engine::Input::AxisInput& axis = SDL2_Engine::Globals::get<SDL2_Engine::Input::AxisInput>();

		//Store the axis values in the Axis Input object
		axis.addAxis(mActionControls[EPlayerAction::Move_Vertical]);
		axis.addAxis(mActionControls[EPlayerAction::Move_Horizontal]);
		axis.addAxis(mActionControls[EPlayerAction::Place_Bomb]);
		axis.addAxis(mActionControls[EPlayerAction::Detonate_Mine]);

		//Return success
		return true;
	}

	/*
		ControlScheme : unbindControlScheme - Unbind the currently contained controls from the SDL2_Engine Axis Input Manager
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	void ControlScheme::unbindControlScheme() {
		//Check scheme is not in use
		if (mPlayerID == EPlayerNumber::Unassigned) return;

		//Get the Axis Input object
		SDL2_Engine::Input::AxisInput& axis = SDL2_Engine::Globals::get<SDL2_Engine::Input::AxisInput>();

		//Remove the virtual Axis
		axis.removeAxis(mActionLookup[EPlayerAction::Move_Vertical].c_str());
		axis.removeAxis(mActionLookup[EPlayerAction::Move_Horizontal].c_str());
		axis.removeAxis(mActionLookup[EPlayerAction::Place_Bomb].c_str());
		axis.removeAxis(mActionLookup[EPlayerAction::Detonate_Mine].c_str());

		//Clear the Action Lookups
		mActionLookup.clear();

		//Reset the player ID
		mPlayerID = EPlayerNumber::Unassigned;
	}

	/*
		ControlScheme : actionPressed - Check to see if the contained action has been pressed (Button behavior)
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pAction - An EPlayerAction value that defines the Input Values to poll

		return bool - Returns true if the action input was changed this cycle
	*/
	bool ControlScheme::actionPressed(const EPlayerAction& pAction) { return SDL2_Engine::Globals::get<SDL2_Engine::Input::AxisInput>().btnPressed(mActionLookup[pAction].c_str()); }

	/*
		ControlScheme : actionAxis - Retrieve the axis value for the specified action (Axis behavior)
		Created: 06/11/2017
		Modified: 06/11/2017

		param[in] pAction - An EPlayerAction value that defines the Input Values to poll

		return const float& - Returns a constant reference to the Virtual Axis value (-1 to 1)
	*/
	const float& ControlScheme::actionAxis(const EPlayerAction& pAction) { return SDL2_Engine::Globals::get<SDL2_Engine::Input::AxisInput>().getAxis(mActionLookup[pAction].c_str()); }

	/*
		ControlScheme : deathVibration - If this Control Scheme is tied to a GamePad, cause the death vibration
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	void ControlScheme::deathVibration() const {
		//Check if the Control Scheme has been assigned to a GamePad
		if (mGamePadID == SDL2_Engine::Input::EGamePadID::Null) return;

		//Create the Vibration Description
		SDL2_Engine::Input::VibrationDescription desc;

		//Store the GamePad ID
		desc.gamePad = mGamePadID;

		//Shorten the time the vibration plays
		desc.vibrationLength = 0.75f;

		//Apply the scale function
		desc.scaleFunc = reverseScale;

		//Play the vibration
		SDL2_Engine::Globals::get<SDL2_Engine::Input::Controllers>().applyVibration(desc);
	}
}
