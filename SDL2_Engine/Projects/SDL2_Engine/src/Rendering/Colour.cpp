#include "Colour.hpp"

#include "../Utilities/Endian/Endian.hpp"

namespace SDL2_Engine {
	namespace Rendering {
		/*
			Colour : Assignment Operator (ID) - Assign the current object a new Colour ID value
			Created: 09/10/2017
			Modified: 09/10/2017

			param[in] pID - A colourID value defining the colour ID to assign to this object

			return Colour& - Returns a reference to itself
		*/
		Colour& Colour::operator=(const colourID& pID) {
			//Convert the ID value
			ID = Utilities::Endian::convert(pID);

			//Return itself
			return *this;
		}
	}
}
