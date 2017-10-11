#include "Mouse.hpp"

//! Include the SDL2_Engine objects
#include "../../Globals.hpp"
#include "../../Time.hpp"
#include "../../Resources/ResourceTypes/LocalResourceCursor.hpp"
#include "../../Window/Window.hpp"

//! Include the SDL objects
#include <SDL.h>

//! Label the different states to be handled
enum EStates : unsigned char { STATE_CUR, STATE_PRE, STATE_TOTAL };

//! Store axis labels for mouse positions
enum EAxis : unsigned char { AXIS_X, AXIS_Y, AXIS_TOTAL };

//! Store the total count of mouse buttons
enum EButtonTotal : unsigned char { BTN_TOTAL = (int)SDL2_Engine::Input::EMouseButton::Extra2 + 1 };

namespace SDL2_Engine {
	namespace Input {
		/*
		 *		Name: MouseInternalData
		 *		Author: Mitchell Croft
		 *		Created: 06/10/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Store internal mouse state data
		**/
		struct Mouse::MouseInternalData {
			//! Store positional data for the mouse
			int mousePoints[STATE_TOTAL][AXIS_TOTAL];

			//! Store mouse state data
			bool mouseStates[STATE_TOTAL][BTN_TOTAL];

			//! Store the Cursor Resource to use
			LocalResource<Cursor> cursor;

			//! Store a timer for the cursor
			float timer = 0;

			//! Store the current animation frame
			size_t currentFrame = 0;
		};

		/*
			Mouse : hideCursor - Change the visibility state of the Cursor
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pState - A bool indicating the new cursor visibility state
		*/
		void Mouse::hideCursor(const bool& pState) { SDL_ShowCursor(pState ? SDL_ENABLE : SDL_DISABLE); }

		/*
			Mouse : useCursor - Use a loaded LocalResource Cursor
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pCursor - A LocalResource<Cursor> shared pointer to the cursor resource to use

			return bool - Returns true if the cursor was successfully assigned
		*/
		bool Mouse::useCursor(const LocalResource<Cursor>& pCursor) {
			//Check if the resource is null
			if (!pCursor) mData->cursor = nullptr;

			//Check Cursor resource is loaded
			else if (pCursor->status() == EResourceLoadStatus::Loaded) {
				//Reset the animation values
				mData->timer = 0.f;
				mData->currentFrame = 0;

				//Assign the resource
				mData->cursor = pCursor;
			}

			//Failed resource
			else return false;

			//Default success
			return true;
		}

		/*
			Mouse : x - Get the X position of the mouse
			Created: 06/10/2017
			Modified: 06/10/2017

			return const int& - Returns a constant reference to the mouse X position
		*/
		const int& Mouse::x() const { return mData->mousePoints[STATE_CUR][AXIS_X]; }

		/*
			Mouse : y - Get the Y position of the mouse
			Created: 06/10/2017
			Modified: 06/10/2017

			return const int& - Returns a constant reference to the mouse Y position
		*/
		const int& Mouse::y() const { return mData->mousePoints[STATE_CUR][AXIS_Y]; }

		/*
			Mouse : deltaX - Get the change in X position from the last cycle
			Created: 06/10/2017
			Modified: 06/10/2017

			return int - Returns the difference as an int
		*/
		int Mouse::deltaX() const { return (mData->mousePoints[STATE_PRE][AXIS_X] - mData->mousePoints[STATE_CUR][AXIS_X]); }

		/*
			Mouse : deltaY - Get the change in Y position from the last cycle
			Created: 06/10/2017
			Modified: 06/10/2017

			return int - Returns the difference
		*/
		int Mouse::deltaY() const { return (mData->mousePoints[STATE_PRE][AXIS_Y] - mData->mousePoints[STATE_CUR][AXIS_Y]); }

		/*
			Mouse : buttonDown - Checks to see if the specified mouse button is currently down
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pBtn - An EMouseButton value defining the mouse button to check

			return bool - Returns true if the mouse button is down
		*/
		bool Mouse::buttonDown(const EMouseButton& pBtn) const { return mData->mouseStates[STATE_CUR][(int)pBtn]; }

		/*
			Mouse : buttonPressed - Checks to see if the specified mouse button has been pressed this cycle
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pBtn - An EMouseButton value defining the mouse button to check

			return bool - Returns true if the mouse button was pressed this cycle
		*/
		bool Mouse::buttonPressed(const EMouseButton& pBtn) const { return (mData->mouseStates[STATE_CUR][(int)pBtn] && !mData->mouseStates[STATE_PRE][(int)pBtn]); }

		/*
			Mouse : buttonReleased - Checks to see if the specified mouse button has been released this cycle
			Created: 06/10/2017
			Modified: 06/10/2017

			param[in] pBtn - An EMouseButton value defining the mouse button to check

			return bool - Returns true if the mouse button was released this cycle
		*/
		bool Mouse::buttonReleased(const EMouseButton& pBtn) const { return (!mData->mouseStates[STATE_CUR][(int)pBtn] && mData->mouseStates[STATE_PRE][(int)pBtn]); }

		/*
			Mouse : Constructor - Initialise with default values
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		Mouse::Mouse() : mData(nullptr) {}

		/*
			Mouse : createInterface - Empty function
			Created: 06/10/2017
			Modified: 11/10/2017

			return bool - Always returns true
		*/
		bool Mouse::createInterface() {
			//Create the data object
			mData = new MouseInternalData();

			//Return success
			return true;
		}

		/*
			Window : destroyInterface - Deallocate the resources used by the Mouse object
			Created: 06/10/2017
			Modified: 06/10/2017
		*/
		void Mouse::destroyInterface() { if (mData) delete mData; mData = nullptr; }

		/*
			Window : update - Update the position values and cursor values of the mouse
			Created: 06/10/2017
			Modified: 11/10/2017
		*/
		void Mouse::update() {
			//Check that the Window has focus
			if (!Globals::get<Window>().hasProperty(EWindowProperties::Focus)) return;

			//Save the old mouse values
			memcpy_s(mData->mousePoints[STATE_PRE], sizeof(int) * AXIS_TOTAL, mData->mousePoints[STATE_CUR], sizeof(int) * AXIS_TOTAL);
			memcpy_s(mData->mouseStates[STATE_PRE], sizeof(bool) * BTN_TOTAL, mData->mouseStates[STATE_CUR], sizeof(bool) * BTN_TOTAL);

			//Update the mouse coordinates
			int btnMask = SDL_GetMouseState(&mData->mousePoints[STATE_CUR][AXIS_X], &mData->mousePoints[STATE_CUR][AXIS_Y]);

			//Update the current key states
			for (size_t i = 1; i < BTN_TOTAL; i++) 
				mData->mouseStates[STATE_CUR][i] = (btnMask & (1 << i) ? true : false);

			//Check if there is a cursor assigned
			if (mData->cursor) {
				//Add to the timer
				mData->timer += Globals::get<Time>().getRealDelta();

				//Check if the timer is over fps limit
				if (mData->timer >= mData->cursor->fps()) {
					//Spend the built up time
					float integral;
					mData->timer = modf(mData->timer / mData->cursor->fps(), &integral) * mData->cursor->fps();

					//Increment the current frame count
					mData->currentFrame = (mData->currentFrame + (size_t)integral) % mData->cursor->frameCount();

					//Set the cursor
					SDL_SetCursor(mData->cursor->getFrame(mData->currentFrame));
				}
			}
		}
	}
}