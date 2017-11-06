#pragma once

//! Include the required SDL2_Engine values
#include <Input/VirtualAxis.hpp>

//! Include the required STL objects
#include <unordered_map>
#include <string>

namespace BombSquad {
	//! Store the ID of the player that uses a control scheme
	enum class EPlayerNumber { Unassigned = -5, Four, Three, Two, One, Demonstration };

	//! Store the different actions that can be performed by the player
	enum class EPlayerAction { Move_Vertical, Move_Horizontal, Place_Bomb, Detonate_Mine };

	/*
	 *		Name: ControlScheme
	 *		Author: Mitchell Croft
	 *		Created: 06/11/2017
	 *		Modified: 06/11/2017
	 *		
	 *		Purpose:
	 *		Store the control values used to control the player during game play
	**/
	class ControlScheme {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------Construction/Setup----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			ControlScheme : Constructor - Initialise with default values
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		ControlScheme();

		/*
			ControlScheme : setGamePadID - Set the Game Pad ID used for the control scheme
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pID - The new EGamePadID to give to the Control Scheme

			return bool - Returns true if the GamePadID was set. GamePadID can not be set if the scheme has been bound
		*/
		bool setGamePadID(const SDL2_Engine::Input::EGamePadID& pID);

		/*
			ControlScheme : addActionControl - Add a Virtual Axis that will be used to control a specific action
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pAction - An EPlayerAction value that defines what action the should be controlled by pAxis
			param[in] pAxis - A SDL2_Engine Virtual Axis that defines the controls for the action

			return bool - Returns true if the action was set. Action controls can not be set if the scheme has been bound
		*/
		bool addActionControl(const EPlayerAction& pAction, const SDL2_Engine::Input::VirtualAxis& pAxis);

		/*
			ControlScheme : bindControlScheme - Bind the currently contained controls to the SDL2_Engine Axis Input Manager
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pPlayerNum - A EPlayerNumber value defining the player the control scheme will be used for

			return bool - Returns true if the control scheme was successfully bound
		*/
		bool bindControlScheme(const EPlayerNumber& pPlayerNum);

		/*
			ControlScheme : unbindControlScheme - Unbind the currently contained controls from the SDL2_Engine Axis Input Manager
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		void unbindControlScheme();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Usability--------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			ControlScheme : actionPressed - Check to see if the contained action has been pressed (Button behavior)
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pAction - An EPlayerAction value that defines the Input Values to poll

			return bool - Returns true if the action input was changed this cycle
		*/
		bool actionPressed(const EPlayerAction& pAction);

		/*
			ControlScheme : actionAxis - Retrieve the axis value for the specified action (Axis behavior)
			Created: 06/11/2017
			Modified: 06/11/2017

			param[in] pAction - An EPlayerAction value that defines the Input Values to poll

			return const float& - Returns a constant reference to the Virtual Axis value (-1 to 1)
		*/
		const float& actionAxis(const EPlayerAction& pAction);

		/*
			ControlScheme : deathVibration - If this Control Scheme is tied to a GamePad, cause the death vibration
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		void deathVibration() const;

	private:
		//! Store the ID of the Game Pad this control scheme belongs to
		SDL2_Engine::Input::EGamePadID mGamePadID;

		//! Store the Player Number associated with the control scheme
		EPlayerNumber mPlayerID;

		//! Store a map of the established virtual axis corresponding to the player actions
		std::unordered_map<EPlayerAction, SDL2_Engine::Input::VirtualAxis> mActionControls;

		//! Store a map of the virtual axis names corresponding to the player ID and action enumeration
		std::unordered_map<EPlayerAction, std::string> mActionLookup;
	};
}