#pragma once

//! Include the required game objects
#include "../GameObjects/Player.hpp"
#include "../GameObjects/Bomb.hpp"
#include "../Powerups/PowerupLoader.hpp"
#include "../MapGeneration/MapGenerator.hpp"

namespace BombSquad {
	/*
	 *		Name: GameManager
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 11/11/2017
	 *		
	 *		Purpose:
	 *		Manage the aspects relating to game play and its setup
	**/
	class GameManager {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			GameManager : create - Create the singleton instance
			Created: 10/11/2017
			Modified: 10/11/2017

			return bool - Returns true if the Game Manager was created successfully
		*/
		static bool create();

		/*
			GameManager : destroy - Deallocate memory used by Game Manager
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		static void destroy();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------------Setup---------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			GameManager : setPlayerControls - Set the control schemes the players will use to control their characters
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pSchemes - A standard vector of ControlScheme objects to use

			NOTE:
			This also sets the number of players that will be in the game
		*/
		static void setPlayerControls(const std::vector<ControlScheme>& pSchemes);

		/*
			GameManger : setBlueprints - Set the powerups that will be used as blueprints during a game
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pPowerups - A standard vector of the Powerups objects to use
		*/
		static void setBlueprints(const std::vector<Powerup>& pPowerups) { mInstance->mBlueprints = pPowerups; }

		/*
			GameManager : setMapDimensions - Set the dimensions of the map that should be generated
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pWidth - The width of the map to generate
			param[in] pHeight - The height of the map to generate
		*/
		inline static void setMapDimensions(const size_t& pWidth, const size_t& pHeight) { mInstance->mMapWidth = pWidth; mInstance->mMapHeight = pHeight; }

		/*
			GameManager : setObstacleDensity - Set the chance of an obstacle being spawned on free tiles
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pChance - A 0 - 1 scale float value representing the chance of spawning an obstacle
		*/
		static void setObstacleDensity(const float& pChance) { mInstance->mGenerator.setObstacleChance(pChance); }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Gameplay-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			GameManager : setupNewMap - Reset all values for a new game
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		static void setupNewGame();

		/*
			GameManager : update - Update the current game state values 
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pDelta - The delta time for the current cycle
		*/
		static void update(const float& pDelta);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Getters--------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			GameManager : getPlayers - Get the player objects that are currently in use
			Created: 10/11/2017
			Modified: 10/11/2017

			return const vector<Player>& - Returns a constant reference to the internal value
		*/
		inline static std::vector<Player>& getPlayers() { return mInstance->mPlayers; }

		/*
			GameManager : getBlueprints - Get the current powerups in the internal Blueprint powerup list
			Created: 10/11/2017
			Modified: 10/11/2017

			return const vector<Powerup>& - Returns a constant reference to the internal value
		*/
		inline static const std::vector<Powerup>& getBlueprints() { return mInstance->mBlueprints; }

		/*
			GameManager : getLoader - Get the Powerup Loader object used for the internal blueprints
			Created: 10/11/2017
			Modified: 11/11/2017

			return const PowerupLoader& - Returns a constant reference to the internal value
		*/
		inline static const PowerupLoader& getLoader() { return mInstance->mPowerLoader; }

		/*
			GameManager : getMap - Get the current map object
			Created: 10/11/2017
			Modified: 10/11/2017

			return const gameMap& - Returns a constant reference to the internal value
		*/
		inline static const gameMap& getMap() { return mInstance->mCurrentMap; }

		/*
			GameManager : getBombs - Get the bombs currently on the map
			Created: 10/11/2017
			Modified: 10/11/2017

			return const unordered_map<coord, Bomb>& - Returns a constant reference to the internal value, where the keys
													   for the objects are their location on the current map
		*/
		inline static const std::unordered_map<coord, Bomb>& getBombs() { return mInstance->mBombs; }

		/*
			GameManager : getDeadlyTiles - Get the map of currently deadly tiles
			Created: 10/11/2017
			Modified: 10/11/2017

			return const Map<float>& - Returns a constant reference to the internal value

			NOTE:
			All indexs with values != 0.f are considered deadly, where the values are in a 0 - 1 scale
			The transition from 1 to 0 is the progress through any animation that is desired to be played
		*/
		inline static const Map<float>& getDeadlyTiles() { return mInstance->mDeadlyTiles; }

		/*
			GameManager : getActivePowerups - Get the powerups currently on the map
			Created: 10/11/2017
			Modified: 10/11/2017

			return const unordered_map<coord, const Powerup*>& - Returns a constant reference to the internal value, where the keys
																 for the objects are their location on the current map
		*/
		inline static const std::unordered_map<coord, const Powerup*>& getActivePowerups() { return mInstance->mPowerups; }

	private:
		//! Store singleton values
		static GameManager* mInstance;
		GameManager(); ~GameManager() = default;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Management Objects---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Store an array of the players in the current game
		std::vector<Player> mPlayers;

		//! Store the Powerup Loader used for this session
		PowerupLoader mPowerLoader;

		//! Store the blueprints for powerups
		std::vector<Powerup> mBlueprints;

		//! Store the map generator used
		MapGenerator mGenerator;

		//! Store the desired dimensions of the map
		size_t mMapWidth, mMapHeight;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Gameplay Objects----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Store the map the players are currently on
		gameMap mCurrentMap;

		//! Store the currently active bombs on the map
		std::unordered_map<coord, Bomb> mBombs;

		//! Store a map of the tiles on the map which are deadly
		Map<float> mDeadlyTiles;

		//! Store the available powerups on the map
		std::unordered_map<coord, const Powerup*> mPowerups;
	};

	//! Create a shorthand type define
	typedef GameManager GM;
}