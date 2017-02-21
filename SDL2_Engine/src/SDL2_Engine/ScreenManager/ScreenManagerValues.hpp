#pragma once

namespace SDL2_Engine {
	namespace ScreenManager {
		//! Track the different values that a scene can have
		enum EScreenState {
			Active,
			Priority = 1 << 1,
			Hidden = 1 << 2,
			Shutdown = 1 << 3,
		};
	}
}