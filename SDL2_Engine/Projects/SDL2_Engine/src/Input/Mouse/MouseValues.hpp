#pragma once

namespace SDL2_Engine {
	namespace Input {
		//! Flag the different input types available to the Mouse
		enum class EMouseInputType { Null = -1, Button, Axis };

		//! Store the ID values of the mouse buttons that can be checked
		enum class EMouseButton { Null = -1, Left, Middle, Right, Extra1, Extra2 };

		//! Flag the different directional axis available on the mouse
		enum class EMouseAxis { Null = -1, X, Y };
	}
}