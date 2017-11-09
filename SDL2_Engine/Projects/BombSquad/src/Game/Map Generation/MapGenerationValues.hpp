#pragma once

//! Include the required STL objects
#include <vector>

//! Include the GLM iVec2 objects
#include <glm/vec2.hpp>

//! Include the Map object
#include "Map.hpp"

namespace BombSquad {
	//! Store an enumeration of the different tile types
	enum class ETileType {
		//! Tile that can be used as a spawn point for a player
		SpawnPoint = -3,

		//! Free tile that can be passed over
		Free,

		//! Safe tile that can't have obstacles placed on its location
		Safe,

		//! Flags an impassable tile
		Restricted,

		//! Temporarily blocked tile that must be destroyed with a bomb
		Obstacle,
	};

	//! Create a type define for the coordinate value
	typedef glm::ivec2 coord;
	
	//! Create a type define for a collection of coordinates
	typedef std::vector<coord> coords;

	//! Create a type define for the commonly used map types
	typedef Map<char>		hitMap;
	typedef Map<short>		depthMap;
	typedef Map<ETileType>	gameMap;
}