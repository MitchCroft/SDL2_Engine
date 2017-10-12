#pragma once

namespace SDL2_Engine {
	namespace Input {
		//! Store the ID values of the mouse buttons that can be checked
		enum class EMouseButton { Left, Middle, Right, Extra1, Extra2 };

		//! Flag the different directional axis available on the mouse
		enum class EMouseAxis { X, Y };
	}
}