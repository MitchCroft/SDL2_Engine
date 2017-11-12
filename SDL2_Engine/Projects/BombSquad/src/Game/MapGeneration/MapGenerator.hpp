#pragma once

//! Include the RegionIdentifier object
#include "RegionIdentifier.hpp"

namespace BombSquad {
	/*
	 *		Name: MapGenerator 
	 *		Author: Mitchell Croft
	 *		Created: 09/11/2017
	 *		Modified: 12/11/2017
	 *		
	 *		Purpose:
	 *		Generate a gameMap object representing a possible area of play for
	 *		a bomberman-esque game
	**/
	class MapGenerator {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Construction---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			MapGenerator : Constructor - Initialise with default values
			Created: 09/11/2017
			Modified: 12/11/2017
		*/
		MapGenerator();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------------Generation----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			MapGenerator : generateMap - Create a gameMap based on the current settings
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pWidth - The Width of the map to create
			param[in] pHeight - The Height of the map to create
			param[in] pPlayers - The number of spawn points to place on the map

			return gameMap - Returns a gameMap object that has been setup for a game
		*/
		gameMap generateMap(const size_t& pWidth, const size_t& pHeight, const size_t& pPlayers) const;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------------Getters/Setters-------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			MapGenerator : getBirthRate - Get the current birth rate for the cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			return const size_t& - Returns a constant reference to the internal value
		*/
		inline const size_t& getBirthRate() const { return mBirthRate; }

		/*
			MapGenerator : setBirthRate - Set the birth rate used for the cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pVal - The new birth rate value
		*/
		inline void setBirthRate(const size_t& pVal) { mBirthRate = pVal; }

		/*
			MapGenerator : getDeathLimit - Get the current death limit for the cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			return const size_t& - Returns a constant reference to the internal value
		*/
		inline const size_t& getDeathLimit() const { return mDeathLimit; }

		/*
			MapGenerator : setDeathLimit - Set the death limit used for the cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pVal - The new death limit value
		*/
		inline void setDeathLimit(const size_t& pVal) { mDeathLimit = pVal; }

		/*
			MapGenerator : getInitialChance - Get the initial chance of spawning an alive cell for the 
											  cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			return const float& - Returns a constant reference to the internal value
		*/
		inline const float& getInitialChance() const { return mInitialChance; }

		/*
			MapGenerator : setInitialChance - Set the initial chance of spawning an alive cell for the 
											  cellular automata process
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pScale - A 0-1 scale float value that defines the new chance of initial spawn
		*/
		inline void setInitialChance(const float& pScale) { mInitialChance = pScale; }

		/*
			MapGenerator : getIterations - Get the number of iterations to apply the cellular automata rules
			Created: 09/11/2017
			Modified: 09/11/2017

			return const int& - Returns a constance reference to the internal value
		*/
		inline const int& getIterations() const { return mIterations; }

		/*
			MapGenerator : setIterations - Set the number of iterations to apply the cellular automata rules
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pIter - The new number of iterations (where < 0 is until smoothed)
		*/
		inline void setIterations(const int& pIter) { mIterations = pIter; }

		/*
			MapGenerator : getMaxIterations - Get the maximum number of iterations to apply cellular automata
											  rules when mIterations is < 0
			Created: 09/11/2017
			Modified: 09/11/2017

			return const size_t& - Returns a constant reference to the internal value
		*/
		inline const size_t& getMaxIterations() const { return mMaxIterations; }

		/*
			MapGenerator : setMaxIterations - Set the maximum number of iterations to apply cellular automata
											  rules when mIterations is < 0
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pVal - The new maximum number of iterations that can be applied
		*/
		inline void setMaxIterations(const size_t& pVal) { mMaxIterations = pVal; }

		/*
			MapGenerator : getPassageRadius - Get the radius used when creating passages between regions
			Created: 09/11/2017
			Modified: 09/11/2017

			return const int& - Returns a constant reference to the internal value
		*/
		inline const int& getPassageRadius() const { return mPassageRadius; }

		/*
			MapGenerator : setPassageRadius - Set the radius used when creating passages between regions
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pRad - The new radius of the created passageways 
		*/
		inline void setPassageRadius(const int& pRad) { mPassageRadius = pRad; }

		/*
			MapGenerator : getObstacleChance - Get the chance of an obstacle tile being placed on a free tile
			Created: 09/11/2017
			Modified: 09/11/2017

			return const float& - Returns a constant reference to the internal value
		*/
		inline const float& getObstacleChance() const { return mObstacleChance; }

		/*
			MapGenerator : setObstacleChance - Set the chance of an obstacle tile being placed on a free tile
			Created: 09/11/2017
			Modified: 09/11/2017

			param[in] pChance - A 0-1 scale float value that defines the new chance of spawning an obstacle
		*/
		inline void setObstacleChance(const float& pChance) { mObstacleChance = pChance; }

		/*
			MapGenerator : setSeed - Set the seed that is used to generate the 'random' values
			Created: 12/11/2017
			Modified: 12/11/2017

			param[in] pSeed - An unsigned integer value to be used as the seed
		*/
		void setSeed(const unsigned int& pSeed);

	private:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------------Values------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//! Store the values used to influence the cellular automata generation
		size_t mBirthRate, mDeathLimit;
		float mInitialChance;

		//! Store the number of simulation steps that should be applied to the map (<0 is until smoothed)
		int mIterations;

		//! Store the maximum number of iterations to be applied when mIterations is < 0
		size_t mMaxIterations;

		//! Store the radius of the passages to create when linking regions
		int mPassageRadius;

		//! Store the chance of a free position having an obstacle spawned to obstruct a path
		float mObstacleChance;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Generation Delegates----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		//! Create the initial map, and populate with initial free tiles
		gameMap createInitialMap(const size_t& pWidth, const size_t& pHeight) const;

		//! Find the number of neighboring coordinates which can be considered 'alive'
		size_t countAliveNeighbours(const coord& pCoord, const gameMap& pMap) const;

		//! Perform a single application of the cellular automata rules to a gameMap
		size_t simulateStep(gameMap& pMap) const;

		//! Links regions identified by a RegionIdentifier object
		void linkRegions(RegionIdentifier& pIdent, gameMap& pMap) const;

		//! Get the coordinates making up a line between two points
		coords getLine(const coord& pFrom, const coord& pTo) const;

		//! 'Paint' a specific tile across a gameMap in a circle radius
		void drawCircle(const ETileType& pBrush, const coord& pCenter, const int& pRadius, gameMap& pMap) const;

		//! Find the 'ideal' position(s) within the regions to place spawn points for the player(s)
		bool findPlayerSpawns(gameMap& pMap, RegionIdentifier& pIdent, const size_t& pPlayers) const;

		//! Find a path between two coordinates on a gameMap
		coords findPath(const coord& pStart, const coord& pEnd, const gameMap& pMap, const ETileType& pTraversable) const;

		//! Place obstacle tiles around a gameMap
		void placeObstacles(gameMap& pMap) const;
	};
}