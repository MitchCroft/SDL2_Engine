#pragma once

//! Include the SDL2_Engine objects
#include "../../Utilities/IGlobal.hpp"
#include "../../Resources/Resources.hpp"
#include "MouseValues.hpp"

namespace SDL2_Engine {
	//! Define a simple Point structure
	struct Point { int x, y; };

	namespace Input {
		/*
		 *		Name: Mouse
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 12/10/2017
		 *		
		 *		Purpose:
		 *		Handle changes in mouse movement and presses
		**/
		class SDL2_LIB_INC Mouse : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------------Modifiers------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Mouse : hideCursor - Change the visibility state of the Cursor
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pState - A bool indicating the new cursor visibility state
			*/
			void hideCursor(const bool& pState);

			/*
				Mouse : useCursor - Use a loaded LocalResource Cursor
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pCursor - A LocalResource<Cursor> shared pointer to the cursor resource to use

				return bool - Returns true if the cursor was successfully assigned
			*/
			bool useCursor(const LocalResource<Cursor>& pCursor);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------------Data--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Mouse : getPos - Get the position of the Mouse, relative to the Window
				Created: 12/10/2017
				Modified: 12/10/2017

				return Point - Return the X and Y position values as a Point object
			*/
			Point getPos() const;

			/*
				Mouse : getPos - Get a specific Mouse axis position, relative to the Window
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pAxis - An EMouseAxis value defining the axis to retrieve

				return const int& - Returns a constant int reference to the position value
			*/
			const int& getPos(const EMouseAxis& pAxis) const;

			/*
				Mouse : getDelta - Get the delta position of the Mouse
				Created: 12/10/2017
				Modified: 12/10/2017

				return Point - Returns the X and Y delta position values as a Point object
			*/
			Point getDelta() const;

			/*
				Mouse : getDelta - Get a specific Mouse axis' delta position
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pAxis - AN EMouseAxis value defining the axis to retrieve

				return int - Returns the delta position as an int
			*/
			int getDelta(const EMouseAxis& pAxis) const;

			/*
				Mouse : buttonDown - Checks to see if the specified mouse button is currently down
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pBtn - An EMouseButton value defining the mouse button to check

				return bool - Returns true if the mouse button is down
			*/
			bool buttonDown(const EMouseButton& pBtn) const;

			/*
				Mouse : buttonPressed - Checks to see if the specified mouse button has been pressed this cycle
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pBtn - An EMouseButton value defining the mouse button to check

				return bool - Returns true if the mouse button was pressed this cycle
			*/
			bool buttonPressed(const EMouseButton& pBtn) const;

			/*
				Mouse : buttonReleased - Checks to see if the specified mouse button has been released this cycle
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pBtn - An EMouseButton value defining the mouse button to check

				return bool - Returns true if the mouse button was released this cycle
			*/
			bool buttonReleased(const EMouseButton& pBtn) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Mouse : Constructor - Initialise with default values
				Created: 06/10/2017
				Modified: 06/10/2017

				param[in] pSetup - Defines how the Window should be setup
			*/
			Mouse();

			/*
				Mouse : createInterface - Empty function
				Created: 06/10/2017
				Modified: 11/10/2017

				return bool - Always returns true
			*/
			bool createInterface() override;

			/*
				Window : destroyInterface - Deallocate the resources used by the Mouse object
				Created: 06/10/2017
				Modified: 06/10/2017
			*/
			void destroyInterface() override;

			/*
				Window : update - Update the position values and cursor values of the mouse
				Created: 06/10/2017
				Modified: 11/10/2017
			*/
			void update() override;

			private:
				//! Define the internal protected elements of the Mouse
				struct MouseInternalData;
				MouseInternalData* mData;
		};
	}
}