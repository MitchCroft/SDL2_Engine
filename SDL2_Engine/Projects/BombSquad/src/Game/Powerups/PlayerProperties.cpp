#include "PlayerProperties.hpp"

//! Define very simple math macros
#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define CLAMP(X, Y, Z) ((X) < (Y) ? (Y) : ((X) > (Z) ? (Z) : (X)))

namespace BombSquad {
	/*
		PlayerProperties : reset - Reset the Player Properties object back to its initial values
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	void PlayerProperties::reset() {
		//Empty the temporary properties
		mTempProperties.clear();

		//Reset the starting values (Dodgy hard coded again....)
		mFuseLength = 2;
		mBlastRadius = 2;
		mBombCount = 1;
		mReverseControls = false;
		mMines = false;
		mSpeed = 10.f;
	}

	/*
		PlayerProperties : getFuseLength - Get the current fuse length of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return float - Returns the fuse length (in seconds) as a float
	*/
	float PlayerProperties::getFuseLength() const { 
		//Get the fuse length value
		const float LENGTH = (mTempProperties.getMask() & EPowerupEffectFlags::Fuse_Duration ? mTempProperties.getFuseLength() : mFuseLength);

		//Apply clamping
		return CLAMP(LENGTH, 0.f, 30.f); 
	}

	/*
		PlayerProperties : getBlastRadius - Get the current blast radius of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return int - Returns the blast radius as an int
	*/
	int PlayerProperties::getBlastRadius() const {
		//Get the blast radius
		const int RADIUS = mBlastRadius + (mTempProperties.getMask() & EPowerupEffectFlags::Delta_Blast_Radius ? mTempProperties.getDeltaBlastRadius() : 0);

		//Apply clamping
		return CLAMP(RADIUS, 1, 9999);
	}

	/*
		PlayerProperties : getBombCount - Get the current bomb count of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return int - Returns the count as an int
	*/
	int PlayerProperties::getBombCount() const {
		//Get the bomb count
		const int COUNT = mBombCount + (mTempProperties.getMask() & EPowerupEffectFlags::Delta_Bomb_Count ? mTempProperties.getDeltaBombCount() : 0);

		//Apply clamping
		return CLAMP(COUNT, 1, 100);
	}

	/*
		PlayerProperties : isReversed - Get the reverse controls flag of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns the reversed state as a bool
	*/
	bool PlayerProperties::isReversed() const { return (mTempProperties.getMask() & EPowerupEffectFlags::Reverse_Controls ? mTempProperties.getReverseControls() : mReverseControls); }

	/*
		PlayerProperties : hasMines - Get the mines flag of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns the mine state as a bool
	*/
	bool PlayerProperties::hasMines() const { return (mTempProperties.getMask() & EPowerupEffectFlags::Has_Mines ? mTempProperties.hasMines() : mMines); }

	/*
		PlayerProperties : getSpeed - Get the speed of the player
		Created: 10/11/2017
		Modified: 10/11/2017

		return float - Returns the speed as a float
	*/
	float PlayerProperties::getSpeed() const {
		//Get the speed value
		const float SPEED = mSpeed + (mTempProperties.getMask() & EPowerupEffectFlags::Speed ? mTempProperties.getSpeed() : 0.f);

		//Apply minimum 
		return MAX(SPEED, 1.f);
	}

	/*
		PlayerProperties : Addition Assignment Operator - Apply a new Powerup object to the properties
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pPUp - The powerup object to add to the internal list

		return PlayerProperties& - Returns a reference to itself
	*/
	PlayerProperties& PlayerProperties::operator+=(const Powerup& pPUp) {
		//Check if the powerup is temporary
		if (pPUp.getDuration() > 0.f) mTempProperties += pPUp;

		//Otherwise apply settings
		else {
			//Check if the powerup affects the values before assigning
			if (pPUp.getMask() & EPowerupEffectFlags::Fuse_Duration)
				mFuseLength = CLAMP(pPUp.getFuseLength(), 0.f, 30.f);
			if (pPUp.getMask() & EPowerupEffectFlags::Delta_Blast_Radius)
				mBlastRadius = CLAMP(mBlastRadius + pPUp.getDeltaBombCount(), 1, 9999);
			if (pPUp.getMask() & EPowerupEffectFlags::Delta_Bomb_Count)
				mBombCount = CLAMP(mBombCount + pPUp.getDeltaBombCount(), 1, 100);
			if (pPUp.getMask() & EPowerupEffectFlags::Reverse_Controls)
				mReverseControls = pPUp.getReverseControls();
			if (pPUp.getMask() & EPowerupEffectFlags::Has_Mines)
				mMines = pPUp.hasMines();
			if (pPUp.getMask() & EPowerupEffectFlags::Speed)
				mSpeed = MAX(mSpeed + pPUp.getSpeed(), 1.f);
		}

		//Return itself
		return *this;
	}
}
