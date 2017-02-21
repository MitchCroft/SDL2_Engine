#include "BaseScreen.hpp"

#include "../Time/Time.hpp"

namespace SDL2_Engine {
	namespace ScreenManager {
		/*
			BaseScreen : update - Empty update block that can be overridden
			Author: Mitchell Croft
			Created: 21/02/2017
			Modified: 21/02/2017

			param[in/out] The time object for the thread this update is executing in
		*/
		void BaseScreen::update(Time::Time pTime) {}

		/*
			BaseScreen : handleInput - Empty handle input block that can be overridden
			Author: Mitchell Croft
			Created: 21/02/2017
			Modified: 21/02/2017

			param[in/out] The time object for the thread this handle input is executing in
		*/
		void BaseScreen::handleInput(Time::Time pTime) {}

		/*
			BaseScreen : threadedUpdate - Empty threaded update block that can be overridden
			Author: Mitchell Croft
			Created: 21/02/2017
			Modified: 21/02/2017

			param[in/out] The time object for the thread this threaded update is executing on
		*/
		void BaseScreen::threadedUpdate(Time::Time pTime) {}
	}
}