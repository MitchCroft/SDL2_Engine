#include "Colour.hpp"

//! Include SDL2_Engine objects
#include "../Utilities/Endian/Endian.hpp"
#include "../Globals.hpp"
#include "../Math.hpp"

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

		/*
			Colour : lerp - Linearly interpolate the values of the current towards a second
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pSecond - The Colour object that marks the 'end' goal of the interpolation
			param[in] pT - A 0-1 scale indicating the progress of the interpolation

			return Colour - Returns a Colour object with the interpolated values
		*/
		Colour Colour::lerp(const Colour& pSecond, const float& pT) const {
			//Get the math object
			const Math& math = Globals::get<Math>();

			//Interpolate the values
			return Colour(
				math.lerp(r, pSecond.r, pT),
				math.lerp(g, pSecond.g, pT),
				math.lerp(b, pSecond.b, pT),
				math.lerp(a, pSecond.a, pT)
			);
		}
	}
}
