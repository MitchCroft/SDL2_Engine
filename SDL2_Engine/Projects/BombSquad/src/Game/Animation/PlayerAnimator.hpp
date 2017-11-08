#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Resources/Resources.hpp>

//! Include the required STL objects
#include <vector>
#include <unordered_map>

//! Define the number of animation sets to create
#define ANI_SET_TOTAL 2U

namespace BombSquad {
	//! Store enumeration labels for the two animation sets
	enum class EAnimationSet { Head, Body };

	//! Store enumeration labels for the four animations
	enum class EAnimation { Down, Up, Left, Right, Cheer, Death };

	/*
	 *		Name: AniFrame 
	 *		Author: Mitchell Croft
	 *		Created: 08/11/2017
	 *		Modified: 08/11/2017
	 *		
	 *		Purpose:
	 *		Store a single animation frames dimensions
	**/
	struct AniFrame { int x, y, w, h; };

	/*
	 *		Name: PlayerAnimator
	 *		Author: Mitchell Croft
	 *		Created: 08/11/2017
	 *		Modified: 08/11/2017
	 *		
	 *		Purpose:
	 *		Provide a unified interface for managing the animations that
	 *		are played during the game
	**/
	class PlayerAnimator {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------Construction/Initialisation-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerAnimator : Constructor - Initialise with default values
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pLocation - The filepath location of the Objx file containing the animation descriptions (Default nullptr)
			param[in] pName - The name of the contained Objx object to load animation values from (Default nullptr)
		*/
		PlayerAnimator(const char* pLocation = nullptr, const char* pName = nullptr);

		/*
			PlayerAnimator : loadAnimations - Load the animations from an external Objx file
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pLocation - The filepath location of the Objx file containing the animation descriptions
			param[in] pName - The name of the contained Objx object to load animation values from

			return bool - Returns true if the animations where loaded
		*/
		bool loadAnimations(const char* pLocation, const char* pName);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Interface----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerAnimator : setAnimation - Set the current animation that is playing for an animation set
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pSet - An EAnimationSet value defining the set to modify the current animation of
			param[in] pAnimation - An EAnimation value defining the animation to start playing
		*/
		void setAnimation(const EAnimationSet& pSet, const EAnimation& pAnimation);

		/*
			PlayerAnimator : getFrame - Get the current frame for a specific animation set
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pSet - An EAnimationSet value defining the set to retrieve the active frame of

			return AniFrame - Return an AniFrame object holding the animation frame location
		*/
		AniFrame getFrame(const EAnimationSet& pSet);

		/*
			PlayerAnimator : getSpriteSheet - Get the Sprite Sheet currently in use for the current animation
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pSet - An EAnimationSet value defining the set to retrieve the active sprite sheet of

			return LocalResource<Texture> - Returns a Texture Local Resource object containing the loaded sprite sheet
		*/
		SDL2_Engine::LocalResource<SDL2_Engine::Texture> getSpriteSheet(const EAnimationSet& pSet) const;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Updating----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PlayerAnimator : update - Update the contained animation values
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pDelta - The delta time for current cycle
		*/
		void update(const float& pDelta);

	private:
		//! Define the values needed for a complete animation
		struct Animation {
			//! Store the frames in the animation
			std::vector<AniFrame> frames;

			//! Store the frame rate of the animation
			float fps;

			//! Flag if the animation should loop itself
			bool loop;
		};

		//! Define the values needed to monitor a set of animations
		struct AnimationSet {
			//! Store the spritesheet used for rendering
			SDL2_Engine::LocalResource<SDL2_Engine::Texture> spritesheet;

			//! Store a collection of animations
			std::unordered_map<EAnimation, Animation> animations;

			//! Store the current playing animation
			EAnimation currentAni;

			//! Store the current frame that is being used
			size_t currentFrame;

			//! Store a timer used for updating values
			float timer;
		};

		//! Store two sets of animations for playback
		AnimationSet mAniSets[ANI_SET_TOTAL];
	};
}