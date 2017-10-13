#pragma once

//! Include SDL2_Engine objects
#include "../../../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UIBounds
			 *		Author: Mitchell Croft
			 *		Created: 13/10/2017
			 *		Modified: 13/10/2017
			 *		
			 *		Purpose:
			 *		Store screen space coordinate values representing positional information
			**/
			struct SDL2_LIB_INC UIBounds {
				//! Store coordinate data
				int x, y, w, h;

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------------Getters-------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UIBounds : xMax - Get the furthest X axis bounds of the object
					Created: 13/10/2017
					Modified: 13/10/2017

					return int - Returns the coordinate as a single int
				*/
				inline int xMax() const { return x + w; }

				/*
					UIBounds : yMax - Get the furthest Y axis bounds of the object
					Created: 13/10/2017
					Modified: 13/10/2017

					return int - Returns the coordinate as a single int
				*/
				inline int yMax() const { return y + h; }

				/*
					UIBounds : contains - Determines if the defined point is within the UIBounds
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pX - The X coordinate value to evaluate
					param[in] pY - The Y coordinate value to evaluate

					return bool - Returns true if the point is within the bounds
				*/
				inline bool contains(const int& pX, const int& pY) const { return (pX >= x && pX <= xMax() && pY >= y && pY <= yMax()); }

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////---------------------------------------Construction----------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				
				/*
					UIBounds : Constructor - Initialise with default values
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pX - The value to initialise the x coordinate with (Default 0)
					param[in] pY - The value to initialise the y coordinate with (Default 0)
					param[in] pW - The value to initialise the w coordinate with (Default 0)
					param[in] pH - The value to initialise the h coordinate with (Default 0)
				*/
				inline UIBounds(const int& pX = 0, const int& pY = 0, const int& pW = 0, const int& pH = 0) : x(pX), y(pY), w(pW), h(pH) {}
			};
		}
	}
}