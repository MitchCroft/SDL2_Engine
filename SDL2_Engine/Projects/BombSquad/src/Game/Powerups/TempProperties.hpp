#pragma once

//! Include the required objects
#include <vector>
#include "Powerup.hpp"

namespace BombSquad {
	/*
	 *		Name: TempProperties
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 12/11/2017
	 *		
	 *		Purpose:
	 *		Store temporary powerup values that are used to modify the player stats
	**/
	class TempProperties {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Getters----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			TempProperties : getMask - Get the collective BitMask of affected properties
			Created: 10/11/2017
			Modified: 10/11/2017

			return const Bitmask<EPowerupEffectFlags>& - Returns a constant reference to the internal value
		*/
		inline const SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags>& getMask() const { return mCollectiveMask; }
		
		/*
			TempProperties : getFuseLength - Get the current temporary fuse length
			Created: 10/11/2017
			Modified: 10/11/2017

			return float - Returns the temporary length as a float
		*/
		float getFuseLength() const;

		/*
			TempProperties : getDeltaBlastRadius - Get the collective delta blast radius
			Created: 10/11/2017
			Modified: 11/11/2017

			return int - Returns the collective delta blast radius as an int
		*/
		int getDeltaBlastRadius() const;

		/*
			TempProperties : getDeltaBombCount - Get the collective delta bomb count
			Created: 10/11/2017
			Modified: 10/11/2017

			return int - Returns the collective delta bomb count as an int
		*/
		int getDeltaBombCount() const;

		/*
			TempProperties : getReverseControls - Get the current temporary control reversal state
			Created: 10/11/2017
			Modified: 10/11/2017

			return bool - Returns the temporary state as a bool
		*/
		bool getReverseControls() const;

		/*
			TempProperties : hasMines - Get the current temporary mine stats
			Created: 10/11/2017
			Modified: 10/11/2017

			return bool - Returns the temporary state as a mine
		*/
		bool hasMines() const;

		/*
			TempProperties : getSpeed - Get the collective speed 
			Created: 10/11/2017
			Modified: 10/11/2017

			return float - Returns the collective speed as a float
		*/
		float getSpeed() const;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Management-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			TempProperties : update - Update the internal timer values 
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDelta - The delta time for the current cycle
		*/
		void update(const float& pDelta);

		/*
			TempProperties : clear - Remove all temporary powerups stored
			Created: 10/11/2017
			Modified: 12/11/2017
		*/
		inline void clear() { mProperties.clear(); mCollectiveMask = 0; }

		/*
			TempProperties : Addition Assignment Operator - Add a new Powerup object to the internal list
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pPUp - The powerup object to add to the internal list

			return TempProperties& - Returns a reference to itself
		*/
		TempProperties& operator+=(const Powerup& pPUp);

		/*
			TempProperties : Subtraction Assignment Operator - Remove all powerups with a specific mask value
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pMask - Bitmask object of EPowerupEffectFlags representing the powerup to remove

			return TempProperties& - Returns a reference to itself
		*/
		TempProperties& operator-=(const SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags>&  pMask);

	private:
		//! Define an internal node for tracking powerup and timer values
		struct PowerupNode {
			//! Store a set of powerup values
			Powerup powerup;

			//! Store the timer for the powerup
			float timer;

			//! Simple constructor
			inline PowerupNode(const Powerup& pType) : powerup(pType), timer(0.f) {}
		};

		//! Store a collective Bitmask
		SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags> mCollectiveMask;

		//! Store an vector of the temporary properties
		std::vector<PowerupNode> mProperties;

		//! Construct the internal mask
		SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags> constructMask() const;
	};
}