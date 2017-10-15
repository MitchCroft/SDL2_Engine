#pragma once

//! Include SDL2_Engine objects
#include "../../../__LibraryManagement.hpp"

//! Include Vector objects
#include <glm/vec2.hpp>

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UIBounds
			 *		Author: Mitchell Croft
			 *		Created: 13/10/2017
			 *		Modified: 15/10/2017
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

				/*
					UIBounds : intersects - Determines if the current UIBounds intersects another
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pOther - The other UIBounds object to test against

					return bool - Returns true if either UIBounds object intersects or contains the other
				*/
				inline bool intersects(const UIBounds& pOther) const { return !(xMax() < pOther.x || yMax() < pOther.y || pOther.xMax() < x || pOther.yMax() < y); }

				/*
					UIBounds : distance - Determines the distance between two UIBounds objects
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pOther - The other UIBounds object to calculate the distance between

					return glm::ivec2 - Returns the distance difference along the X and Y axis
				*/
				inline glm::ivec2 distance(const UIBounds& pOther) const { return{ (x + w / 2) - (pOther.x + pOther.w / 2), (y + h / 2) - (pOther.y + pOther.h / 2) }; }

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