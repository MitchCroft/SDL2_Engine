#pragma once

#include "../../Utilities/IGlobal.hpp"
#include <memory>

namespace SDL2_Engine {
	//! Prototype the Cursor identifying struct
	struct Cursor;

	//! Prototype the LocalResource 
	namespace ResourceTypes { template<typename T> class __LocalResource; }

	//! Define the alias for the Local Resources
	template<typename T>
	using LocalResource = std::shared_ptr<ResourceTypes::__LocalResource<T>>;

	namespace Input {
		//! Store the ID values of the mouse buttons that can be checked
		enum class EMouseButton { Left, Middle, Right, Extra1, Extra2 };

		/*
		 *		Name: Mouse
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Handle changes in mouse movement and presses
		**/
		class SDL2_LIB_INC Mouse : public Utilities::IGlobal {
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
				Mouse : x - Get the X position of the mouse
				Created: 06/10/2017
				Modified: 06/10/2017

				return const int& - Returns a constant reference to the mouse X position
			*/
			const int& x() const;

			/*
				Mouse : y - Get the Y position of the mouse
				Created: 06/10/2017
				Modified: 06/10/2017

				return const int& - Returns a constant reference to the mouse Y position
			*/
			const int& y() const;

			/*
				Mouse : deltaX - Get the change in X position from the last cycle
				Created: 06/10/2017
				Modified: 06/10/2017

				return int - Returns the difference as an int
			*/	
			int deltaX() const;

			/*
				Mouse : deltaY - Get the change in Y position from the last cycle
				Created: 06/10/2017
				Modified: 06/10/2017

				return int - Returns the difference 
			*/
			int deltaY() const;

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
				Modified: 06/10/2017

				return bool - Always returns true
			*/
			inline bool createInterface() override { return true; }

			/*
				Window : destroyInterface - Deallocate the resources used by the Mouse object
				Created: 06/10/2017
				Modified: 06/10/2017
			*/
			void destroyInterface() override;

			/*
				Window : update - Update the position values and cursor values of the mouse
				Created: 06/10/2017
				Modified: 06/10/2017
			*/
			void update() override;

			private:
				//! Define the internal protected elements of the Mouse
				struct MouseInternalData;
				MouseInternalData* mData;
		};
	}
}