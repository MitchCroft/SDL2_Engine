#include "VirtualAxis.hpp"

//! Include memory manipulation functionality
#include <string.h>

namespace SDL2_Engine {
	namespace Input {
		/*
			VirtualAxis : Constructor - Initialise with default values
			Created: 12/10/2017
			Modified: 12/10/2017
		*/
		VirtualAxis::VirtualAxis() { memset(this, 0, sizeof(VirtualAxis)); }

		/*
			VirtualAxis : Copy Constructor - Initialise with the values of another VirtualAxis
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pCopy - The Virtual Axis object to copy
		*/
		VirtualAxis::VirtualAxis(const VirtualAxis& pCopy) { memcpy_s(this, sizeof(VirtualAxis), &pCopy, sizeof(VirtualAxis)); }
	}
}
