#pragma once

//! Include the GLM values
#include <glm/vec2.hpp>

namespace std {
	template<>
	struct hash<glm::ivec2> {
		size_t operator()(const glm::ivec2& pVal) const {
			return (hash<int>()(pVal.x) ^ (hash<int>()(pVal.y) << 1));
		}
	};
}

namespace BombSquad {
	//! Simple enum for describing the different players
	enum class EPlayerID { One, Two, Three, Four };

	//! Create a type define for the coordinate value
	typedef glm::ivec2 coord;
}