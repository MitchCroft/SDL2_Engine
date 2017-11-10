#pragma once

//! Include the temporary properties object
#include "TempProperties.hpp"

namespace BombSquad {
	/*
	 *		Name: PlayerProperties 
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 10/11/2017
	 *		
	 *		Purpose:
	 *		Store permanent values that influence the way that the 
	 *		player acts during game play
	**/
	class PlayerProperties {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Construction-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerProperties : Constructor - Initialise with default values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline PlayerProperties() { reset(); }

		/*
			PlayerProperties : reset - Reset the Player Properties object back to its initial values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		void reset();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Getters----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerProperties : getFuseLength - Get the current fuse length of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return float - Returns the fuse length (in seconds) as a float 
		*/
		float getFuseLength() const;

		/*
			PlayerProperties : getBlastRadius - Get the current blast radius of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return int - Returns the blast radius as an int
		*/
		int getBlastRadius() const;

		/*
			PlayerProperties : getBombCount - Get the current bomb count of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return int - Returns the count as an int
		*/
		int getBombCount() const;

		/*
			PlayerProperties : isReversed - Get the reverse controls flag of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return bool - Returns the reversed state as a bool 
		*/
		bool isReversed() const;

		/*
			PlayerProperties : hasMines - Get the mines flag of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return bool - Returns the mine state as a bool
		*/
		bool hasMines() const;

		/*
			PlayerProperties : getSpeed - Get the speed of the player
			Created: 10/11/2017
			Modified: 10/11/2017

			return float - Returns the speed as a float
		*/
		float getSpeed() const;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Management-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerProperties : update - Pass time information to the internal TempProperties object
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDelta - The delta time for the current cycle
		*/
		inline void update(const float& pDelta) { mTempProperties.update(pDelta); }

		/*
			PlayerProperties : Addition Assignment Operator - Apply a new Powerup object to the properties
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pPUp - The powerup object to add to the internal list

			return PlayerProperties& - Returns a reference to itself
		*/
		PlayerProperties& operator+=(const Powerup& pPUp);

	private:
		//! Store the temporary properties values
		TempProperties mTempProperties;

		//! Store the duration of a bombs fuse
		float mFuseLength;

		//! Store the change in blast radius 
		int mBlastRadius;

		//! Store the change in bomb count
		int mBombCount;

		//! Store the reverse controls flag
		bool mReverseControls;

		//! Store the player has mines flag
		bool mMines;

		//! Store the tiles/second speed
		float mSpeed;
	};
}