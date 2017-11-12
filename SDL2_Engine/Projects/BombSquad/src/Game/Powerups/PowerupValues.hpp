#pragma once

namespace BombSquad {
	//! Store bitmask flags for the properties the powerup effects
	enum class EPowerupEffectFlags {
		Null				= 0,
		Fuse_Duration		= 1 << 0,
		Delta_Blast_Radius	= 1 << 1,
		Detonate_All		= 1 << 2,
		Delta_Bomb_Count	= 1 << 3,
		Reverse_Controls	= 1 << 4,
		Teleport			= 1 << 5,
		Has_Mines			= 1 << 6,
		Speed				= 1 << 8,
	};

	//! Store flags for the Actor the powerup(s) act on
	enum class EPowerupActor { Null, Self, Enemy, Random };
}