#pragma once

//! Include the required objects for player input and display
#include "../Animation/PlayerAnimator.hpp"
#include "../Input/ControlScheme.hpp"
#include "../Powerups/PlayerProperties.hpp"
#include "GameObjectValues.hpp"

namespace BombSquad {
	/*
	 *		Name: Player 
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 12/11/2017
	 *		
	 *		Purpose:
	 *		Store values related to the in game player object
	**/
	struct Player {
		//! Store the ID of the player
		EPlayerID id;

		//! Store the Animator that the player is using
		PlayerAnimator animator;

		//! Store the control scheme that the player is using
		ControlScheme controls;

		//! Store the properties affecting the player
		PlayerProperties properties;

		//! Flag if the player is currently alive
		bool alive;

		//! Store the current position of the player
		coord position;

		//! Store the movement progression values from one tile to another
		glm::vec2 moveProg;

		//! Store the number of placed bombs 
		size_t placedBombs;

		//! Store game values
		size_t wins, losses;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Construction------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Player : Constructor - Initialise with default values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline Player() : alive(false), wins(0), losses(0)	{}
	};
}