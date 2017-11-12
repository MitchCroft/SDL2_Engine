#include "GameScene.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Time.hpp>
#include <Math.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>

#include <Rendering/Renderer.hpp>

using namespace SDL2_Engine;
using namespace SDL2_Engine::Rendering;

//! Include the basic SDL objects
#include <SDL.h>

//! Include the required BombSquad objects
#include "../Game/Management/GameManager.hpp"

//! Define the time to wait before progressing to the score scene
#define PROGRESS_DELAY_AMOUNT 5.f

//! Define the number of frames in the animated explosion
#define EXPLOSION_FRAMES 6U

//! Define the number of frames in the animated bomb
#define BOMB_FRAMES 6U

namespace BombSquad {
	/*
		GameScene : Constructor - Initialise with default values
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	GameScene::GameScene() :
		mDestroyGM(true),
		mEndTimer(0.f)
	{}

	/*
		GameScene : createScene - Load the values required for the Scene to operate
		Created: 12/11/2017
		Modified: 12/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool GameScene::createScene() {
		//Get the Resources object
		Resources& res = Globals::get<Resources>();
		
		//Load the background texture
		mBackground = res.loadResource<Texture>("resources/Textures/GameBackground.png");

		//Check the texture loaded correctly
		if (mBackground->status() != EResourceLoadStatus::Loaded) return false;

		//Load the spritesheet
		mSpritesheet = res.loadResource<Texture>("resources/Textures/Spritesheet.png");

		//Check the texture loaded correctly
		if (mSpritesheet->status() != EResourceLoadStatus::Loaded) return false;

		//Return success
		return true;
	}

	/*
		GameScene : destroyScene - Destroy the values used to setup the Scene
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	void GameScene::destroyScene() {
		//Check if the Game Manager should be destroyed
		if (mDestroyGM) GM::destroy();
	}

	/*
		GameScene : update - Check for game completion
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	void GameScene::update() {
		//Get the time object
		const Time& TIME = Globals::get<Time>();

		//Update the Game Manager
		GM::update(TIME.getDelta());

		//Get the players from the Game Manager
		const auto& PLAYERS = GM::getPlayers();

		//Count how many are left alive
		char alivePlayers = 0;
		for (size_t i = 0; i < PLAYERS.size(); i++) {
			if (PLAYERS[i].alive) alivePlayers++;
		}

		//If there are 1 or fewer remaining players start the clock
		if (alivePlayers < 2) {
			//Add the delta time to the timer
			mEndTimer += TIME.getRealDelta();

			//Check if the timer has surpassed the allowed amount
			if (mEndTimer >= PROGRESS_DELAY_AMOUNT) {
				//Shutdown the current scene
				shutdown();

				//Load the score scene
				//TODO
				
				//Prevent the Game Manager being destroyed
				mDestroyGM = false;
			}
		}
	}

	/*
		GameScene : render - Render the Scene to the Window
		Created: 12/11/2017
		Modified: 12/11/2017
	*/
	void GameScene::render() {
		//Get the renderer object
		Renderer& rend = Globals::get<Renderer>();

		//Render the background
		rend.drawTexture(mBackground->texture, { 0, 0, 1280, 720 });

		//Define the area in which the game should be rendered
		const SDL_Rect GAME_AREA = { 46, 58, 1196, 612 };

		//Get the current map state
		const gameMap& MAP = GM::getMap();

		//Get the explosive map
		const Map<float>& DEADLY = GM::getDeadlyTiles();

		//Calculate the tile sizes 
		const coord DIM = { GAME_AREA.w / MAP.width(), GAME_AREA.h / MAP.height() };

		//Iterate through the map
		for (size_t x = 0; x < MAP.width(); x++) {
			for (size_t y = 0; y < MAP.height(); y++) {
				//Get the tile value
				const int TILE = (int)MAP[x][y];

				//Check if it is render-able
				if (TILE >= 0) {
					//Create the horribly hard coded source rectangle
					SDL_Rect src = { TILE * 32, 32, 32, 32 };

					//Render the tile
					rend.drawTexture(mSpritesheet->texture, { GAME_AREA.x + DIM.x * (int)x, GAME_AREA.y + DIM.y * (int)y, DIM.x, DIM.y }, &src);
				}

				//Check if the tile is deadly
				else if (DEADLY[x][y]) {
					//Get the frame number
					int frame = (int)floor((1.f - DEADLY[x][y]) / (1.f / EXPLOSION_FRAMES));

					//Create another horribly hard coded source rectangle
					SDL_Rect src = { 64 + frame * 32, 32, 32, 32 };

					//Render the explosion
					rend.drawTexture(mSpritesheet->texture, { GAME_AREA.x + DIM.x * (int)x, GAME_AREA.y + DIM.y * (int)y, DIM.x, DIM.y }, &src);
				}
			}
		}

		//Get the currently ticking bombs
		const auto& BOMBS = GM::getBombs();

		//Loop through and render the bombs
		for (auto it = BOMBS.cbegin(); it != BOMBS.cend(); ++it) {
			//Store the source area to render
			SDL_Rect src;

			//Check if bomb is a mine
			if (it->second.isMine) src = { 192, 0, 32, 32 };

			//Calculate bomb frames
			else {
				//Get the frame number
				int frame = (int)floor((it->second.timer / it->second.fuseLength) / (1.f / BOMB_FRAMES));

				//Get another horribly hard coded area
				src = { frame * 32, 0, 32, 32 };
			}

			//Render the bomb
			rend.drawTexture(mSpritesheet->texture, { GAME_AREA.x + DIM.x * it->first.x, GAME_AREA.y + DIM.y * it->first.y, DIM.x, DIM.y }, &src);
		}

		//Get the Powerup Loader
		const PowerupLoader& PUL = GM::getLoader();

		//Get the currently available powerups
		const auto& POWERUPS = GM::getActivePowerups();

		//Loop through and render the powerups
		for (auto it = POWERUPS.cbegin(); it != POWERUPS.cend(); ++it) 
			rend.drawTexture(PUL.getTexture(it->second->getTextureID()), { GAME_AREA.x + DIM.x * it->first.x, GAME_AREA.y + DIM.y * it->first.y, DIM.x, DIM.y });

		//Get the current players
		const auto& PLAYERS = GM::getPlayers();

		//Loop through and render the players
		for (size_t i = 0; i < PLAYERS.size(); i++) {
			//Get the animator
			const auto& ANI = PLAYERS[i].animator;

			//Get the render position
			SDL_Rect drawPos = { GAME_AREA.x + DIM.x * PLAYERS[i].position.x + (int)(DIM.x * PLAYERS[i].moveProg.x),
								 GAME_AREA.y + DIM.y * PLAYERS[i].position.y + (int)(DIM.y * PLAYERS[i].moveProg.y),
								 DIM.x, DIM.y 
			};

			//Get the frame
			SDL_Rect src = *(SDL_Rect*)&ANI.getFrame(EAnimationSet::Body);

			//Render the frames
			rend.drawTexture(ANI.getSpriteSheet(EAnimationSet::Body)->texture, drawPos, &src);

			//Get the other frame
			src = *(SDL_Rect*)&ANI.getFrame(EAnimationSet::Head);

			//Render the frames
			rend.drawTexture(ANI.getSpriteSheet(EAnimationSet::Head)->texture, drawPos, &src);
		}
	}
}
