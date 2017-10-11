#pragma once

namespace SDL2_Engine {
	namespace Scenes {
		//! Track the different the properties of a scene
		enum class ESceneProperties : char {
			//! Active flag, lack of which means the scene should be destroyed
			Active		= 1 << 1,

			//! Visible flag, lack of which means the scene wont have the render function called
			Visible		= 1 << 2,

			//! Flags if the scene should be given priority treatment
			Priority	= 1 << 3,
		};
	}
}