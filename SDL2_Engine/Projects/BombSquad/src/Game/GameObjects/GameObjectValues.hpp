#pragma once

//! Include the GLM values
#include <glm/vec2.hpp>

namespace BombSquad {
	//! Simple enum for describing the different players
	enum class EPlayerID { One, Two, Three, Four };

	//! Create a type define for the coordinate value
	typedef glm::ivec2 coord;
}