#pragma once

//! Include the required values
#include <Utilities/Bitmask.hpp>
#include <string>

#include "PowerupValues.hpp"

namespace BombSquad {
	/*
	 *		Name: Powerup
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 10/11/2017
	 *		
	 *		Purpose:
	 *		Store the values loaded from an Objx powerup descriptor
	**/
	class Powerup {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Construction-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Powerup : Constructor - Initialise with default values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline Powerup() : 
			mMask(EPowerupEffectFlags::Null),
			mActor(EPowerupActor::Null),
			mName("Unassigned"),
			mDescription("Undescribed"),
			mDuration(0.f),
			mFuseLength(0),
			mDeltaBlastRadius(0),
			mDeltaBombCount(0),
			mDetonateAll(false),
			mReverseControls(false),
			mTeleport(false),
			mMines(false),
			mSpeed(0.f) {
			memset(&mTextureID, 0, sizeof(UUID));
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Getters----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Powerup : getMask - Get the EPowerupEffectFlags mask for the Powerup object
			Created: 10/11/2017
			Modified: 10/11/2017

			return const Bitmask<EPowerupEffectFlags>& - Returns a constant reference to the internal mask object
		*/
		inline const SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags>& getMask() const { return mMask; }

		/*
			Powerup : getActor - Get the Actor that the powerup will effect
			Created: 10/11/2017
			Modified: 10/11/2017

			return const EPowerupActor& - Returns a constant reference to the internal value
		*/
		inline const EPowerupActor& getActor() const { return mActor; }

		/*
			Powerup : getTextureID - Get the UUID used to represent the powerups texture
			Created: 10/11/2017
			Modified: 10/11/2017

			return UUID& - Returns a constant reference to the internal value
		*/
		inline const UUID& getTextureID() const { return mTextureID; }

		/*
			Powerup : getName - Get the name of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const std::string& - Returns a constant reference to the internal value
		*/
		inline const std::string& getName() const { return mName; }

		/*
			Powerup : getDescription - Get the description of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const std::string& - Returns a constant reference to the internal value
		*/
		inline const std::string& getDescription() const { return mDescription; }

		/*
			Powerup : getDuration - Get the duration of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const float& - Returns a constant reference to the internal value
		*/
		inline const float& getDuration() const { return mDuration; }

		/*
			Powerup : getFuseLength - Get the fuse length of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const float& - Returns a constant reference to the internal value
		*/
		inline const float& getFuseLength() const { return mFuseLength; }

		/*
			Powerup : getDeltaBlastRadius - Get the delta blast radius of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const int& - Returns a constant reference to the internal value
		*/
		inline const int& getDeltaBlastRadius() const { return mDeltaBlastRadius; }

		/*
			Powerup : getDetonateAll - Get the detonate all flag of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const bool& - Returns a constant reference to the internal value
		*/
		inline const bool& getDetonateAll() const { return mDetonateAll; }

		/*
			Powerup : getDeltaBombCount - Get the delta bomb count of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const int& - Returns a constant reference to the internal value
		*/
		inline const int& getDeltaBombCount() const { return mDeltaBombCount; }

		/*
			Powerup : getReverseControls - Get the reverse controls flag of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const bool& - Returns a constant reference to the internal value
		*/
		inline const bool& getReverseControls() const { return mReverseControls; }

		/*
			Powerup : getTeleport - Get the teleport flag of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const bool& - Returns a constant reference to the internal value
		*/
		inline const bool& getTeleport() const { return mTeleport; }

		/*
			Powerup : hasMines - Get the mines flag of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const bool& - Returns a constant reference to the internal value
		*/
		inline const bool& hasMines() const { return mMines; }

		/*
			Powerup : getSpeed - Get the speed of the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			return const float& - Returns a constant reference to the internal value
		*/
		inline const float& getSpeed() const { return mSpeed; }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Setters----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Powerup : setActor - Set the Actor the powerup affects
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pActor - The EPowerupActor to assign
		*/
		inline void setActor(const EPowerupActor& pActor) { mActor = pActor; }

		/*
			Powerup : setTextureID - Set the ID used for texture identification
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pID - The UUID object to assign
		*/
		inline void setTextureID(const UUID& pID) { mTextureID = pID; }

		/*
			Powerup : setName - Set the name used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pName - A string to assign to the powerup
		*/
		inline void setName(const std::string& pName) { mName = pName; }

		/*
			Powerup : setDescription - Set the description used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDesc - A string to assign to the powerup
		*/
		inline void setDescription(const std::string& pDesc) { mDescription = pDesc; }

		/*
			Powerup : setDuration - Set the duration used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDur - A float indicating the new duration (where <= 0 is permanant)
		*/
		inline void setDuration(const float& pDur) { mDuration = pDur; }

		/*
			Powerup : setFuseLength - Set the fuse length used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pLength - A float indicating the length (in seconds) of the fuse
		*/
		inline void setFuseLength(const float& pLength) { mFuseLength = pLength; mMask |= EPowerupEffectFlags::Fuse_Duration; }

		/*
			Powerup : setDeltaBlastRadius - Set the delta blast radius used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDelta - An int value defining the new delta
		*/
		inline void setDeltaBlastRadius(const int& pDelta) { mDeltaBlastRadius = pDelta; mMask |= EPowerupEffectFlags::Delta_Blast_Radius; }

		/*
			Powerup : setDetonateAll - Set the detonate all flag used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline void setDetonateAll() { mDetonateAll = true; mMask |= EPowerupEffectFlags::Detonate_All; }

		/*
			Powerup : setDeltaBombCount - Set the delta bomb count used for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDelta - An int value defining the new delta
		*/
		inline void setDeltaBombCount(const int& pDelta) { mDeltaBombCount = pDelta; mMask |= EPowerupEffectFlags::Delta_Bomb_Count; }

		/*
			Powerup : setReverseControls - Set the reverse controls flag for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pState - A bool value defining the new state
		*/
		inline void setReverseControls(const bool& pState) { mReverseControls = pState; mMask |= EPowerupEffectFlags::Reverse_Controls; }

		/*
			Powerup : setTeleport - Set the teleport flag for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline void setTeleport() { mTeleport = true; mMask |= EPowerupEffectFlags::Teleport; }

		/*
			Powerup : setMines - Set the mine flag for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pState - A bool value defining the new state
		*/
		inline void setMines(const bool& pState) { mMines = pState; mMask |= EPowerupEffectFlags::Has_Mines; }

		/*
			Powerup : setSpeed - Set the speed for the powerup
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pSpeed - A float value defining the new speed
		*/
		inline void setSpeed(const float& pSpeed) { mSpeed = pSpeed; mMask |= EPowerupEffectFlags::Speed; }

	private:
		//! Store a mask of the values the powerup effects
		SDL2_Engine::Utilities::Bitmask<EPowerupEffectFlags> mMask;

		//! Store the actor the powerup effects
		EPowerupActor mActor;

		//! Store the texture that is used to represent the powerup
		UUID mTextureID;

		//! Store the name of the powerup
		std::string mName;

		//! Store a description of what the powerup does
		std::string mDescription;

		//! Store the duration the powerup lasts for
		float mDuration;

		//! Store the duration of a bombs fuse
		float mFuseLength;

		//! Store the change in blast radius 
		int mDeltaBlastRadius;

		//! Store the detonate all flag
		bool mDetonateAll;

		//! Store the change in bomb count
		int mDeltaBombCount;

		//! Store the reverse controls flag
		bool mReverseControls;

		//! Store the teleport player flag
		bool mTeleport;

		//! Store the player has mines flag
		bool mMines;

		//! Store the tiles/second speed
		float mSpeed;
	};
}