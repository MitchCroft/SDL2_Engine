#include "TempProperties.hpp"

namespace BombSquad {
	/*
		TempProperties : getFuseLength - Get the current temporary fuse length
		Created: 10/11/2017
		Modified: 10/11/2017

		return float - Returns the temporary length as a float
	*/
	float TempProperties::getFuseLength() const {
		//Check if the fuse length is in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Fuse_Duration)) return 0.f;

		//Reverse look through properties to find the first
		for (int i = (int)mProperties.size() - 1; i >= 0; i--) {
			//Check the mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Fuse_Duration)
				return mProperties[i].powerup.getFuseLength();
		}

		//Default return 0 (shouldn't be hit)
		return 0.f;
	}

	/*
		TempProperties : getDeltaBlastRadius - Get the collective delta blast radius
		Created: 10/11/2017
		Modified: 11/11/2017

		return int - Returns the collective delta blast radius as an int
	*/
	int TempProperties::getDeltaBlastRadius() const {
		//Check if the delta blast radius in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Delta_Blast_Radius)) return 0;

		//Store the collective blast radius
		int collective = 0;

		//Loop through all properties
		for (size_t i = 0; i < mProperties.size(); i++) {
			//Check for inclusion in mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Delta_Blast_Radius)
				collective += mProperties[i].powerup.getDeltaBlastRadius();
		}

		//Return the collection
		return collective;
	}

	/*
		TempProperties : getDeltaBombCount - Get the collective delta bomb count
		Created: 10/11/2017
		Modified: 10/11/2017

		return int - Returns the collective delta bomb count as an int
	*/
	int TempProperties::getDeltaBombCount() const {
		//Check if the delta bomb count in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Delta_Bomb_Count)) return 0;

		//Store the collective blast radius
		int collective = 0;

		//Loop through all properties
		for (size_t i = 0; i < mProperties.size(); i++) {
			//Check for inclusion in mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Delta_Bomb_Count)
				collective += mProperties[i].powerup.getDeltaBombCount();
		}

		//Return the collection
		return collective;
	}

	/*
		TempProperties : getReverseControls - Get the current temporary control reversal state
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns the temporary state as a bool
	*/
	bool TempProperties::getReverseControls() const {
		//Check if the fuse length is in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Reverse_Controls)) return false;

		//Reverse look through properties to find the first
		for (int i = (int)mProperties.size() - 1; i >= 0; i--) {
			//Check the mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Reverse_Controls)
				return mProperties[i].powerup.getReverseControls();
		}

		//Default return false (shouldn't be hit)
		return false;
	}

	/*
		TempProperties : hasMines - Get the current temporary mine stats
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns the temporary state as a mine
	*/
	bool TempProperties::hasMines() const {
		//Check if the fuse length is in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Has_Mines)) return false;

		//Reverse look through properties to find the first
		for (int i = (int)mProperties.size() - 1; i >= 0; i--) {
			//Check the mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Has_Mines)
				return mProperties[i].powerup.hasMines();
		}

		//Default return false (shouldn't be hit)
		return false;
	}

	/*
		TempProperties : getSpeed - Get the collective speed
		Created: 10/11/2017
		Modified: 10/11/2017

		return float - Returns the collective speed as a float
	*/
	float TempProperties::getSpeed() const {
		//Check if the delta blast radius in the mask
		if (!(mCollectiveMask & EPowerupEffectFlags::Speed)) return 0.f;

		//Store the collective blast radius
		float collective = 0.f;

		//Loop through all properties
		for (size_t i = 0; i < mProperties.size(); i++) {
			//Check for inclusion in mask
			if (mProperties[i].powerup.getMask() & EPowerupEffectFlags::Speed)
				collective += mProperties[i].powerup.getSpeed();
		}

		//Return the collection
		return collective;
	}

	/*
		TempProperties : update - Update the internal timer values
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pDelta - The delta time for the current cycle
	*/
	void TempProperties::update(const float& pDelta) {
		//Flag if the properties have been changed
		bool flag = false;

		//Loop through and update the timer values
		for (int i = (int)mProperties.size(); i >= 0; i--) {
			//Update the timer
			mProperties[i].timer += pDelta;

			//Check if the time limit has expired
			if (mProperties[i].timer >= mProperties[i].powerup.getDuration()) {
				//Remove the object
				mProperties.erase(mProperties.begin() + i);

				//Flag a change
				flag = true;
			}
		}

		//Check if the the list has been modified
		if (flag) mCollectiveMask = constructMask();
	}

	/*
		TempProperties : Addition Assignment Operator - Add a new Powerup object to the internal list
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pPUp - The powerup object to add to the internal list

		return TempProperties& - Returns a reference to itself
	*/
	TempProperties& TempProperties::operator+=(const Powerup& pPUp) { mProperties.push_back(PowerupNode(pPUp)); mCollectiveMask = constructMask(); return *this; }

	/*
		TempProperties : Subtraction Assignment Operator - Remove all powerups with a specific mask value
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pMask - Bitmask object of EPowerupEffectFlags representing the powerup to remove

		return TempProperties& - Returns a reference to itself
	*/
	TempProperties& TempProperties::operator-=(const SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags>& pMask) {
		//Loop through and remove all with matching masks
		for (int i = (int)mProperties.size() - 1; i >= 0; i--) {
			//Check the mask
			if (mProperties[i].powerup.getMask() == pMask)
				mProperties.erase(mProperties.begin() + i);
		}

		//Update the mask
		mCollectiveMask = constructMask();

		//Return itself
		return *this;
	}

	/*
		TempProperties : constructMask - Get the collective BitMask of affected properties
		Created: 10/11/2017
		Modified: 10/11/2017

		return Bitmask<EPowerupEffectFlags> - Returns a Bitmask object made up of the temporary powerups bitmasks
	*/
	SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags> TempProperties::constructMask() const {
		//Store a bitmask
		SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags> mask;

		//Loop through and combine all of them
		for (size_t i = 0; i < mProperties.size(); i++)
			mask |= mProperties[i].powerup.getMask();

		//Return the mask
		return mask;
	}
}
