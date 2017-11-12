#include "PlayerAnimator.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceGeneric.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
using namespace SDL2_Engine;

//! Include the Objx Parser to read control scheme data
#include <Objx_node.hpp>
#include <Objx_document.hpp>
#include <Objx_reader.hpp>

//! Simple/basic clamp macro
#define CLAMP(VAL, MIN, MAX) ((VAL) < (MIN) ? (VAL) : ((VAL) > (MAX) ? (MAX) : (VAL)))

#include "../../Additionals.hpp"

namespace BombSquad {
	/*
		PlayerAnimator : Constructor - Initialise with default values
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pLocation - The filepath location of the Objx file containing the animation descriptions (Default nullptr)
		param[in] pName - The name of the contained Objx object to load animation values from (Default nullptr)
	*/
	PlayerAnimator::PlayerAnimator(const char* pLocation /*= nullptr*/, const char* pName /*= nullptr*/) { loadAnimations(pLocation, pName); }

	/*
		PlayerAnimator : loadAnimations - Load the animations from an external Objx file
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pLocation - The filepath location of the Objx file containing the animation descriptions
		param[in] pName - The name of the contained Objx object to load animation values from

		return bool - Returns true if the animations where loaded
	*/
	bool PlayerAnimator::loadAnimations(const char* pLocation, const char* pName) {
		//Check the parameter values are valid
		if (!pLocation || !pName) return false;

		//Get the resource object
		Resources& res = Globals::get<Resources>();

		//Attempt to load the Objx file
		LocalResource<Generic> objxData = res.loadResource<Generic>(pLocation);

		//Check the file was loaded
		if (objxData->status() != EResourceLoadStatus::Loaded) return false;

		//Retrieve the logging object
		Debug::Logger& logger = Globals::get<Debug::Logger>();

		//Create the Objx Reader
		bool flag = false;
		Objx::Reader reader([&](const Objx::EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
			//Output error header
			if (!flag) logger.logError("The PlayerAnimator.loadAnimations function encountered an error/errors when attempting to parse the animation descriptor located at '%s'", pLocation);

			//Output the error information
			logger.logError("Line: %zu\tColumn: %zu\t\t%s\t(%s, %i)", pLine, pColumn, pMsg, Objx::statusCodeToString(pCode), (int)pCode);

			//Toggle the flag
			flag = true;
		}, 4);

		//Attempt to parse the loaded data
		const Objx::Document DOC = reader.cparse(objxData->data());

		//Check if an error was encountered
		if (flag) return false;

		//Check the document has the specified name
		if (!DOC.hasKey(pName)) {
			//Output error information
			logger.logError("The PlayerAnimator.loadAnimations function encountered an error loading data from the Object '%s' in the animation descriptor file '%s'. The object does not exist", pName, pLocation);
			return false;
		}

		//Store the names of the objects to load
		const char* SET_NAMES[] = { "head", "body" };
		const EAnimationSet SETS[] = { EAnimationSet::Head, EAnimationSet::Body };
		const char* ANI_NAMES[] = { "down", "up", "left", "right", "cheer", "death" };
		const EAnimation ANIMS[] = { EAnimation::Down, EAnimation::Up, EAnimation::Left, EAnimation::Right, EAnimation::Cheer, EAnimation::Death };

		//Store the number of actions to process
		const size_t ANI_COUNT = sizeof(ANI_NAMES) / sizeof(const char*);
		const size_t SET_COUNT = sizeof(SET_NAMES) / sizeof(const char*);

		//Get the Object
		const Objx::Node CONTAINER = DOC[pName];

		//Loop through the different animation sets
		for (size_t set = 0; set < SET_COUNT; set++) {
			//Get the current animation set container
			const Objx::Node SET_CONTAINER = CONTAINER[SET_NAMES[set]];

			//Deduce the location of the spritesheet
			const std::string DED_LOC = deductLocation(pLocation, SET_CONTAINER["spritesheet"].readVal<Objx::xstring>());

			//Load the spritesheet for the animation set
			mAniSets[set].spritesheet = res.loadResource<Texture>(DED_LOC.c_str());

			//Check the spritesheet was loaded
			if (mAniSets[set].spritesheet->status() != EResourceLoadStatus::Loaded) {
				//Output error
				logger.logError("The PlayerAnimator.loadAnimations function failed to load the spritesheet at deduced location '%s' for the '%s' set of animations.", DED_LOC.c_str(), SET_NAMES[set]);
				return false;
			}

			//Load the animations
			for (size_t ani = 0; ani < ANI_COUNT; ani++) {
				//Get the current animation container
				const Objx::Node ANI_CONTAINER = SET_CONTAINER[ANI_NAMES[ani]];

				//Get a reference to the current animation
				Animation& curr = mAniSets[set].animations[ANIMS[ani]];

				//Read the simple flags
				curr.loop	= ANI_CONTAINER["loop"].readVal<Objx::xbool>(true);
				curr.fps	= (float)ANI_CONTAINER["fps"].readVal<Objx::xdec>(2.0);

				//Convert the fps flag to incremental 
				curr.fps = (curr.fps > 0.f ? 1.f / curr.fps : FLT_MAX);

				//Get the list of animation frames
				const Objx::Node FRAMES = ANI_CONTAINER["frames"];

				//Resize the frames count to store all
				curr.frames.resize(FRAMES.size());

				//Loop through and read all frame specifications
				for (size_t i = 0; i < FRAMES.size(); i++) {
					curr.frames[i].x = FRAMES.readArray<Objx::xobjx>(i).readProp("x").readVal<Objx::xint>();
					curr.frames[i].y = FRAMES.readArray<Objx::xobjx>(i).readProp("y").readVal<Objx::xint>();
					curr.frames[i].w = FRAMES.readArray<Objx::xobjx>(i).readProp("width").readVal<Objx::xint>();
					curr.frames[i].h = FRAMES.readArray<Objx::xobjx>(i).readProp("height").readVal<Objx::xint>();
				}

				//Check if this animation is the default
				if (!strcmp(SET_CONTAINER["default"].readVal<Objx::xstring>().c_str(), ANI_NAMES[ani]))
					setAnimation(SETS[set], ANIMS[ani]);
			}
		}

		//Return successful
		return true;
	}

	/*
		PlayerAnimator : setAnimation - Set the current animation that is playing for an animation set
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pSet - An EAnimationSet value defining the set to modify the current animation of
		param[in] pAnimation - An EAnimation value defining the animation to start playing
	*/
	void PlayerAnimator::setAnimation(const EAnimationSet& pSet, const EAnimation& pAnimation) {
		//Get a reference to the specified set
		AnimationSet& set = mAniSets[(int)pSet];

		//Check to see if that animation is already playing
		if (set.currentAni == pAnimation) return;

		//Reset the progress values
		set.currentFrame = 0;
		set.timer = 0.f;

		//Set the new current animation
		set.currentAni = pAnimation;
	}

	/*
		PlayerAnimator : getFrame - Get the current frame for a specific animation set
		Created: 08/11/2017
		Modified: 12/11/2017

		param[in] pSet - An EAnimationSet value defining the set to retrieve the active frame of

		return AniFrame - Return an AniFrame object holding the animation frame location
	*/
	AniFrame PlayerAnimator::getFrame(const EAnimationSet& pSet) const {
		//Get a reference to the specified set
		const AnimationSet& set = mAniSets[(int)pSet];

		//Attempt to find the animation
		auto it = set.animations.find(set.currentAni);

		//Return the active frame
		return (it != set.animations.end() ? it->second.frames[set.currentFrame] : AniFrame());
	}

	/*
		PlayerAnimator : getSpriteSheet - Get the Sprite Sheet currently in use for the current animation
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pSet - An EAnimationSet value defining the set to retrieve the active sprite sheet of

		return LocalResource<Texture> - Returns a Texture Local Resource object containing the loaded sprite sheet
	*/
	SDL2_Engine::LocalResource<SDL2_Engine::Texture> PlayerAnimator::getSpriteSheet(const EAnimationSet& pSet) const { return mAniSets[(int)pSet].spritesheet; }

	/*
		PlayerAnimator : update - Update the contained animation values
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pDelta - The delta time for current cycle
	*/
	void PlayerAnimator::update(const float& pDelta) {
		//Loop through the animation sets
		for (size_t i = 0; i < ANI_SET_TOTAL; i++) {
			//Get a reference to the current animation values
			AnimationSet& set = mAniSets[i];
			Animation& ani = set.animations[set.currentAni];

			//Increment the timer
			mAniSets[i].timer += pDelta;

			//Check if the timer is over the fps limit
			if (set.timer >= ani.fps) {
				//Spend the built up time
				float integral;
				set.timer = modf(set.timer / ani.fps, &integral) * ani.fps;

				//Check if the animation loops
				if (ani.loop) set.currentFrame = (set.currentFrame + (size_t)integral) % ani.frames.size();
				else set.currentFrame = CLAMP((int)(set.currentFrame + (size_t)integral), 0, (int)ani.frames.size() - 1);
			}
		}
	}
}