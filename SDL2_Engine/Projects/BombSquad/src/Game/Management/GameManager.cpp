#include "GameManager.hpp"

//! Include time for seeding the random number generator
#include <time.h>

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>

using namespace SDL2_Engine;

namespace BombSquad {
	//! Define the singleton instance
	GameManager* GameManager::mInstance = nullptr;

	/*
		GameManager : create - Create the singleton instance
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns true if the Game Manager was created successfully
	*/
	bool GameManager::create() {
		//Ensure that the instance hasn't been created yet
		assert(!mInstance);

		//Create the singleton instance
		mInstance = new GameManager();

		//Load the initial powerups 
		mInstance->mBlueprints = mInstance->mPowerLoader.loadDirectory("resources/Powerups/");

		//Return success
		return true;
	}

	/*
		GameManager : destroy - Deallocate memory used by Game Manager
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	void GameManager::destroy() {
		//Check the instance is setup
		if (mInstance) {
			//Remove the players 
			for (size_t i = 0; i < mInstance->mPlayers.size(); i++)
				mInstance->mPlayers[i].controls.unbindControlScheme();
			mInstance->mPlayers.clear();
			
			//Clear all active powerups
			mInstance->mPowerups.clear();
			mInstance->mBlueprints.clear();
			mInstance->mPowerLoader.dispose();

			//Clear the bombs
			mInstance->mBombs.clear();
			mInstance->mDeadlyTiles.dispose();

			//Clear the Singleton instance
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/*
		GameManager : setPlayerControls - Set the control schemes the players will use to control their characters
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pSchemes - A standard vector of ControlScheme objects to use

		NOTE:
		This also sets the number of players that will be in the game
	*/
	void GameManager::setPlayerControls(const std::vector<ControlScheme>& pSchemes) {
		//Check if there are players already in the list
		if (mInstance->mPlayers.size()) {
			//Remove the players 
			for (size_t i = 0; i < mInstance->mPlayers.size(); i++)
				mInstance->mPlayers[i].controls.unbindControlScheme();
			mInstance->mPlayers.clear();
		}

		//Resize the players vector to for the incoming values
		mInstance->mPlayers.resize(pSchemes.size());

		//Store the names of the Animations to load
		const char* PLAYER_ANIM_NAMES[] = { "player1", "player2", "player3", "player4" };

		//Create new player objects
		for (size_t i = 0; i < pSchemes.size(); i++) {
			//Get a reference to the current player object
			Player& curr = mInstance->mPlayers[i];

			//Set the player ID
			curr.id = (EPlayerID)i;

			//Setup the player animator
			if (!curr.animator.loadAnimations("resources/Animations/playerAnimations.objx", PLAYER_ANIM_NAMES[i])) {
				Globals::get<Debug::Logger>().logError("The Game Manager failed to load the animations for '%s' with the same name in the file 'resources/Animations/playerAnimations.objx'", PLAYER_ANIM_NAMES[i]);
			}

			//Assign the control scheme
			curr.controls = pSchemes[i];

			//Bind the controls
			if (!curr.controls.bindControlScheme((EPlayerNumber)((i + 1) * -1))) {
				Globals::get<Debug::Logger>().logError("The Game Manager failed to bind the controls for player %zu", i + 1);
			}
		}
	}

	/*
		GameManager : setupNewMap - Reset all values for a new game
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	void GameManager::setupNewGame() {
		//Clear all active powerups
		mInstance->mPowerups.clear();
		mInstance->mBlueprints.clear();
		mInstance->mPowerLoader.dispose();

		//Clear the bombs
		mInstance->mBombs.clear();
		mInstance->mDeadlyTiles.dispose();

		//Generate the new map
		mInstance->mCurrentMap = mInstance->mGenerator.generateMap(mInstance->mMapWidth, mInstance->mMapHeight, mInstance->mPlayers.size());

		//Create the deadly tile map
		mInstance->mDeadlyTiles.setBounds(mInstance->mCurrentMap.width(), mInstance->mCurrentMap.height());

		//Loop through and setup the players
		size_t index = 0;
		for (size_t x = 0; x < mInstance->mCurrentMap.width() && index != mInstance->mPlayers.size(); x++) {
			for (size_t y = 0; y < mInstance->mCurrentMap.height() && index != mInstance->mPlayers.size(); y++) {
				//Check if the tile is spawn point
				if (mInstance->mCurrentMap[x][y] == ETileType::SpawnPoint) {
					//Get a reference to the current player object
					Player& curr = mInstance->mPlayers[index++];

					//Reset the properties of the player
					curr.properties.reset();

					//Reset the animator back to default
					curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Down);
					curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Down);

					//Reset the alive flag
					curr.alive = true;

					//Stash the current position for the player
					curr.position = { x, y };

					//Reset the movement progress values
					curr.moveProg = { 0.f, 0.f };
				}
			}
		}
	}

	/*
		GameManager : update - Update the current game state values
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pDelta - The delta time for the current cycle
	*/
	void GameManager::update(const float& pDelta) {
		//TODO
	}

	/*
		GameManager : Constructor - Initialise with default values
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	GameManager::GameManager() : 
		mGenerator((unsigned int)time(NULL)),
		mMapWidth(0), 
		mMapHeight(0)
	{}
}
