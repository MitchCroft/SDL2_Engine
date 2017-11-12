#include "PlayerSetupScene.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Time.hpp>
#include <Math.hpp>
#include <Debug/Logger.hpp>

#include <Window/Window.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceGeneric.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>

#include <Rendering/Renderer.hpp>

#include <UI/Canvas.hpp>

#include <Input/Controllers/Controllers.hpp>
#include <Input/Keyboard/Keyboard.hpp>
#include <Input/StringToInputValues.hpp>
using namespace SDL2_Engine;
using namespace SDL2_Engine::Input;

//! Include the Objx Parser to read control scheme data
#include <Objx_node.hpp>
#include <Objx_document.hpp>
#include <Objx_reader.hpp>

//! Allow for the return to the main menu
#include <Scenes/SceneManager.hpp>
#include "ExternalMenu.hpp"
#include "GameSetupScene.hpp"

//! Include the SDL objects
#include <SDL.h>

namespace BombSquad {
	/*
		PlayerSetupScene : Constructor - Initialise with default values
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	PlayerSetupScene::PlayerSetupScene() {}

	/*
		PlayerSetupScene : createScene - Load the values required for the Scene to operate
		Created: 06/11/2017
		Modified: 08/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool PlayerSetupScene::createScene() {
		//Clear the Canvas
		Globals::get<UI::Canvas>().clearCanvas();

		//Retrieve the logging object
		Debug::Logger& logger = Globals::get<Debug::Logger>();

		//Get the resources interface
		Resources& res = Globals::get<Resources>();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Load Animators-----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//Store the names of the Animations to load
		const char* PLAYER_ANIM_NAMES[MAX_PLAYER_COUNT] = { "player1", "player2", "player3", "player4" };

		//Load the animations
		for (size_t i = 0; i < MAX_PLAYER_COUNT; i++) {
			//Check the load was successful
			if (!mAnimators[i].loadAnimations("resources/Animations/playerAnimations.objx", PLAYER_ANIM_NAMES[i])) {
				//Output error
				logger.logError("Player Setup Scene failed to load the animations for '%s' from the file 'resources/Animations/playerAnimations.objx' (Index: %zu)", PLAYER_ANIM_NAMES[i], i);
				return false;
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Load UI Elements----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Load the background image
		mBackground = res.loadResource<Texture>("resources/UI/Textures/MenuBackground.png");

		//Check image was loaded correctly
		if (mBackground->status() != EResourceLoadStatus::Loaded) return false;

		//Load the foreground image
		mForeground = res.loadResource<Texture>("resources/UI/Textures/PlayerSetupForeground.png");

		//Check the image was loaded correctly
		if (mForeground->status() != EResourceLoadStatus::Loaded) return false;

		//Load the text font
		mTextFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 30);

		//Check the font was loaded correctly
		if (mTextFont->status() != EResourceLoadStatus::Loaded) return false;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------Load Control Schemes--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Load the control scheme data
		LocalResource<Generic> controlSchemesData = res.loadResource<Generic>("resources/controls.objx");

		//Check the file was loaded properly
		if (controlSchemesData->status() != EResourceLoadStatus::Loaded) return false;
		
		//Create the Objx Reader
		bool flag = false;
		Objx::Reader reader([&](const Objx::EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
			//Output error header
			if (!flag) logger.logError("The Player Setup Scene encountered an error/errors when attempting to parse the control scheme list located at 'resources/controls.objx'");

			//Output the error information
			logger.logError("Line: %zu\tColumn: %zu\t\t%s\t(%s, %i)", pLine, pColumn, pMsg, Objx::statusCodeToString(pCode), (int)pCode);

			//Toggle the flag
			flag = true;
		}, 2);

		//Attempt to parse the loaded data
		const Objx::Document DOC = reader.cparse(controlSchemesData->data());

		//Check if an error was encountered
		if (flag) return false;

		//Check there are control schemes loaded
		else if (!DOC.size()) {
			//Output error message
			logger.logError("The Player Setup Scene loaded no Control Schemes from the file 'resources/controls.objx'. There needs to be a minimum of four to proceed");

			//Return failure
			return false;
		}

		//Store the names of the objects to load
		const char* AXIS_NAMES[] = { "verticalMove", "horizontalMove", "place", "detonate" };
		const EPlayerAction ACTIONS[] = { EPlayerAction::Move_Vertical, EPlayerAction::Move_Horizontal, EPlayerAction::Place_Bomb, EPlayerAction::Detonate_Mine };

		//Store the number of actions to process
		const size_t ACTION_COUNT = sizeof(ACTIONS) / sizeof(EPlayerAction);

		//Process all loaded control Schemes
		for (auto iter = DOC.cbegin(); iter != DOC.cend(); ++iter) {
			//Switch on the physical device being used
			switch (stringToInputType(iter->readProp("inputDevice").readVal<Objx::xstring>().c_str())) {
			case EInputDevice::GamePad: {
				//Get the ID of the GamePad to setup for
			 	Utilities::Bitmask<EGamePadID> gamePadID = stringToGamePadID(iter->readProp("id").readVal<Objx::xstring>().c_str());

				//Loop through and find the Axis' that need setting up
				for (int player = 0; player < 4; player++) {
					//Check if this Game Pad is in the mask
					if (gamePadID & (1 << player)) {
						//Create a new Control Scheme object
						ControlScheme newScheme;

						//Assign the GamePad ID to the Control Scheme
						newScheme.setGamePadID((EGamePadID)(1 << player));

						//Loop through the actions to process
						for (size_t i = 0; i < ACTION_COUNT; i++) {
							//Setup for the action object
							const Objx::Node actionObj = iter->readProp(AXIS_NAMES[i]);
							VirtualAxis actionAxis; actionAxis.inputType = EInputDevice::GamePad; actionAxis.gamePads = (1 << player);

							//Read the action object values
							actionAxis.sensitivity		= (float)actionObj["sensitivity"].readVal<Objx::xdec>(1000.0);
							actionAxis.gravity			= (float)actionObj["gravity"].readVal<Objx::xdec>(1000.0);
							actionAxis.invert			= actionObj["invert"].readVal<Objx::xbool>(false);
							actionAxis.gamePadInputType	= stringToGamePadInputType(actionObj["inputType"].readVal<Objx::xstring>().c_str());
							actionAxis.bPosBtn			= stringToGamePadBtn(actionObj["posBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bAltPosBtn		= stringToGamePadBtn(actionObj["altPosBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bNegBtn			= stringToGamePadBtn(actionObj["negBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.bAltNegBtn		= stringToGamePadBtn(actionObj["altNegBtn"].readVal<Objx::xstring>().c_str());
							actionAxis.aAxis			= stringToGamePadAxis(actionObj["axis"].readVal<Objx::xstring>().c_str());
							actionAxis.aDeadZone		= (float)actionObj["deadZone"].readVal<Objx::xdec>(0.5);

							//Add action movement object to the scheme
							newScheme.addActionControl(ACTIONS[i], actionAxis);
						}

						//Add the new scheme to the unclaimed list
						mUnclaimedSchemes.push_back(newScheme);
					}
				}
				break; }
			case EInputDevice::Keyboard: {
				//Create a new Control Scheme object
				ControlScheme newScheme;

				//Loop through the actions to process
				for (size_t i = 0; i < ACTION_COUNT; i++) {
					//Setup for the action object
					const Objx::Node actionObj = iter->readProp(AXIS_NAMES[i]);
					VirtualAxis actionAxis; actionAxis.inputType = EInputDevice::Keyboard;

					//Read the action object values
					actionAxis.sensitivity				= (float)actionObj["sensitivity"].readVal<Objx::xdec>(1000.0);
					actionAxis.gravity					= (float)actionObj["gravity"].readVal<Objx::xdec>(1000.0);
					actionAxis.invert					= actionObj["invert"].readVal<Objx::xbool>(false);
					actionAxis.kPosBtn					= stringToKeyboardKey(actionObj["posBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kAltPosBtn				= stringToKeyboardKey(actionObj["altPosBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kNegBtn					= stringToKeyboardKey(actionObj["negBtn"].readVal<Objx::xstring>().c_str());
					actionAxis.kAltNegBtn				= stringToKeyboardKey(actionObj["altNegBtn"].readVal<Objx::xstring>().c_str());

					//Add action movement object to the scheme
					newScheme.addActionControl(ACTIONS[i], actionAxis);
				}

				//Add the new scheme to the unclaimed list
				mUnclaimedSchemes.push_back(newScheme);
				break; }
			default:
				//Log the error
				logger.logError("Player Setup Scene encountered an unrecognised 'inputDevice' value '%s'. Please use either 'GamePad' or 'Keyboard'", iter->readProp("inputDevice").readVal<Objx::xstring>().c_str());
			}
		}

		//Check there at least four control schemes loaded
		if (mUnclaimedSchemes.size() < 4) {
			//Output error message
			logger.logError("The Player Setup Scene loaded %zu Control Schemes from the file 'resources/controls.objx'. There needs to be a minimum of four to proceed", mUnclaimedSchemes.size());

			//Return failure
			return false;
		}

		//Bind all control schemes to demonstration setups
		for (size_t i = 0; i < mUnclaimedSchemes.size(); i++)
			mUnclaimedSchemes[i].bindControlScheme((EPlayerNumber)i);

		//Return success
		return true;
	}

	/*
		PlayerSetupScene : destroyScene - Destroy the values used to setup the Scene
		Created: 06/11/2017
		Modified: 08/11/2017
	*/
	void PlayerSetupScene::destroyScene() {
		//Release all bound demonstration control schemes
		for (size_t i = 0; i < mUnclaimedSchemes.size(); i++)
			mUnclaimedSchemes[i].unbindControlScheme();
		for (size_t i = 0; i < mClaimedSchemes.size(); i++)
			mClaimedSchemes[i].unbindControlScheme();
	}

	/*
		PlayerSetupScene : update - Update the internal values corresponding to player actions
		Created: 06/11/2017
		Modified: 08/11/2017
	*/
	void PlayerSetupScene::update() {
		//Check for exit back to main menu
		if (Globals::get<Controllers>().btnPressed(EGamePadBtnCodes::B) ||		//Any controller backs out
			Globals::get<Keyboard>().keyPressed(EKeyboardKeyCode::Escape)) {	//Keyboard hits escape
			//Load the main menu 
			Globals::get<Scenes::SceneManager>().addScene<ExternalMenu>("mainMenu");

			//Close the current scene
			shutdown();

			//Exit the function
			return;
		}

		//Check if there is a need to check for more unclaimed options
		if (mClaimedSchemes.size() < MAX_PLAYER_COUNT && mUnclaimedSchemes.size()) {
			//Loop through the unclaimed schemes to see if any have been activated
			for (int i = (int)mUnclaimedSchemes.size() - 1; i >= 0 && mClaimedSchemes.size() < MAX_PLAYER_COUNT; i--) {
				//Check if the place bomb action button was pressed
				if (mUnclaimedSchemes[i].actionPressed(EPlayerAction::Place_Bomb)) {
					//Move the unclaimed scheme to the claimed list
					mClaimedSchemes.push_back(mUnclaimedSchemes[i]);

					//Play a short vibration if it's a game pad
					mUnclaimedSchemes[i].deathVibration();

					//Remove the scheme from the unclaimed list
					mUnclaimedSchemes.erase(mUnclaimedSchemes.begin() + i);
				}
			}
		}

		//Check for progression to powerup selections
		if (mClaimedSchemes.size() >= 2) {
			//Check for continuation to next section
			for (size_t i = 0; i < mClaimedSchemes.size(); i++) {
				//Check for detonation button press
				if (mClaimedSchemes[i].actionPressed(EPlayerAction::Detonate_Mine)) {
					//Flag the current scene for shutdown
					shutdown();

					//Unbind all claimed control schemes
					for (size_t i = 0; i < mClaimedSchemes.size(); i++)
						mClaimedSchemes[i].unbindControlScheme();
					
					//Progress the scene to the game setup 
					Globals::get<Scenes::SceneManager>().addScene<GameSetupScene>(mClaimedSchemes);

					//Exit the function
					return;
				}
			}
		}

		//Get the Time Object
		const Time& TIME = Globals::get<Time>();

		//Check for changing the animation of claimed players
		for (size_t i = 0; i < mClaimedSchemes.size(); i++) {
			//Get the axis values
			float horizontal = mClaimedSchemes[i].actionAxis(EPlayerAction::Move_Horizontal);
			float vertical = mClaimedSchemes[i].actionAxis(EPlayerAction::Move_Vertical);

			//Check direction of movement
			if		(horizontal < 0) { mAnimators[i].setAnimation(EAnimationSet::Head, EAnimation::Left);	mAnimators[i].setAnimation(EAnimationSet::Body, EAnimation::Left); }
			else if (horizontal > 0) { mAnimators[i].setAnimation(EAnimationSet::Head, EAnimation::Right);	mAnimators[i].setAnimation(EAnimationSet::Body, EAnimation::Right); }
			else if (vertical < 0)	 { mAnimators[i].setAnimation(EAnimationSet::Head, EAnimation::Up);		mAnimators[i].setAnimation(EAnimationSet::Body, EAnimation::Up); }
			else if (vertical > 0)	 { mAnimators[i].setAnimation(EAnimationSet::Head, EAnimation::Down);	mAnimators[i].setAnimation(EAnimationSet::Body, EAnimation::Down); }

			//Update the Player Animator
			mAnimators[i].update(TIME.getDelta());
		}
	}

	/*
		PlayerSetupScene : render - Render the Scene to the Window
		Created: 06/11/2017
		Modified: 06/11/2017
	*/
	void PlayerSetupScene::render() {
		//Get the Renderer object
		Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

		//Get the window bounds
		auto bounds = Globals::get<Window>().getWindowDimensions();

		//Draw the background object
		rend.drawTexture(mBackground->texture, { 0, 0, bounds.x, bounds.y });

		//Draw the foreground
		rend.drawTexture(mForeground->texture, { 0, 0, bounds.x, bounds.y });

		//Dodgy hard offset values for the PlayerSetupForeground.png image
		const SDL_Rect AREAS[MAX_PLAYER_COUNT] = { {354, 73, 251, 240 }, {674, 73, 251, 240 }, {354, 388, 251, 240 }, {674, 388, 251, 240 } };

		//Get the Math object
		const Math& math = Globals::get<Math>();

		//Store an array of the text to display under inactive player slots
		const char* INACT_MSG[] = { "Press", "'Place Bomb'", "To Join" };
		const size_t MSG_COUNT = sizeof(INACT_MSG) / sizeof(const char*);

		//Loop through and render the player animators
		for (size_t i = 0; i < MAX_PLAYER_COUNT; i++) {
			//Get the sprite sheet's for the current animator
			LocalResource<Texture> spriteSheetHead = mAnimators[i].getSpriteSheet(EAnimationSet::Head);
			LocalResource<Texture> spriteSheetBody = mAnimators[i].getSpriteSheet(EAnimationSet::Body);

			//Get the animation frames for the current animation
			AniFrame frameHead = mAnimators[i].getFrame(EAnimationSet::Head);
			AniFrame frameBody = mAnimators[i].getFrame(EAnimationSet::Body);

			//Check if the current is an active control scheme
			if (i < mClaimedSchemes.size()) {
				//Get the smallest dimension of the area
				const int SMALLEST_DIM = math.smallest(AREAS[i].w, AREAS[i].h);

				//Create an SDL_rect in the center of the area
				SDL_Rect area = { AREAS[i].x, AREAS[i].y, SMALLEST_DIM, SMALLEST_DIM };

				//Move the X and Y to center area
				area.x += (AREAS[i].w / 2) - (area.w / 2);
				area.y += (AREAS[i].h / 2) - (area.h / 2);

				//Draw the animator frames
				rend.drawTexture(spriteSheetBody->texture, area, (SDL_Rect*)&frameBody);
				rend.drawTexture(spriteSheetHead->texture, area, (SDL_Rect*)&frameHead);
			}

			//Display inactive control scheme 
			else {
				//Create a semi-transparent colour filter for the animators
				Rendering::Colour filter = Rendering::Colour::White;
				filter.a = 128;

				//Get the smallest dimension of the area
				const int SMALLEST_DIM = math.smallest(AREAS[i].w / 3, AREAS[i].h / 3);

				//Create an SDL_rect in the center of the area
				SDL_Rect area = { AREAS[i].x, AREAS[i].y, SMALLEST_DIM, SMALLEST_DIM };

				//Move the X and Y to center area
				area.x += (AREAS[i].w / 2) - (area.w / 2);
				area.y += (int)(AREAS[i].h / 10);

				//Draw the animator frames
				rend.drawTexture(spriteSheetBody->texture, area, (SDL_Rect*)&frameBody, filter);
				rend.drawTexture(spriteSheetHead->texture, area, (SDL_Rect*)&frameHead, filter);

				//Render the text
				for (size_t j = 0; j < MSG_COUNT; j++) {
					//Get the coordinate data 
					SDL_Point textPos = { AREAS[i].x + AREAS[i].w / 2, AREAS[i].y + AREAS[i].h / 2 + (int)mTextFont->fontSize * (int)j };

					//Draw the text line
					rend.drawText(INACT_MSG[j], mTextFont->font, textPos, Rendering::Colour::Black, Rendering::ETextAlignment::Center, Rendering::ETextRenderType::Blended);
				}
			}
		}

		//Check if there are enough players to start a game
		if (mClaimedSchemes.size() >= 2) {
			//Get the center of the screen
			SDL_Point center = { bounds.x / 2, bounds.y / 2 };

			//Render start message
			rend.drawText("Press 'Detonate' to Continue", mTextFont->font, center, Rendering::Colour::Black, Rendering::ETextAlignment::Center, Rendering::ETextRenderType::Blended);
		}
	}
}