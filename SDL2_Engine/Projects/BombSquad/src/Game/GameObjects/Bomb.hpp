#pragma once

//! Include the Player ID values
#include "GameObjectValues.hpp"

namespace BombSquad {
	/*
	 *		Name: Bomb
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 10/11/2017
	 *		
	 *		Purpose:
	 *		Store values related to the in game bomb object
	**/
	struct Bomb {
		//! Store the ID of the Player that placed the bomb
		EPlayerID owner;

		//! Flag if the bomb should be treated like a mine
		bool isMine;

		//! Store the length of the fuse
		float fuseLength;

		//! Store a timer for tracking how long the bomb has been on the map
		float timer;
	};
}