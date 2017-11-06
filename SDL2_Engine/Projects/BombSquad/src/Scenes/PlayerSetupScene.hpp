#pragma once

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

//! Include the Control Scheme object for monitoring available options
#include "../Game/ControlScheme.hpp"

//! Include the required STL objects
#include <vector>

namespace BombSquad {
	/*
	 *		Name: PlayerSetupScene
	 *		Author: Mitchell Croft
	 *		Created: 06/11/2017
	 *		Modified: 06/11/2017
	 *		
	 *		Purpose:
	 *		Allow for the user(s) to enter the game and dictate their
	 *		desired input methodology
	**/
	class PlayerSetupScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			PlayerSetupScene : Constructor - Initialise with default values
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		PlayerSetupScene();

		/*
			PlayerSetupScene : createScene - Load the values required for the Scene to operate
			Created: 06/11/2017
			Modified: 06/11/2017

			return bool - Returns true if the Scene was initialised successfully
		*/
		bool createScene() override;

		/*
			PlayerSetupScene : destroyScene - Destroy the values used to setup the Scene
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		inline void destroyScene() override {}

		/*
			PlayerSetupScene : update - Update the internal values corresponding to player actions
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		void update() override;

		/*
			PlayerSetupScene : render - Render the Scene to the Window 
			Created: 06/11/2017
			Modified: 06/11/2017
		*/
		void render() override;

	private:
		//! Track a list of the unclaimed control schemes available
		std::vector<ControlScheme> mUnclaimedSchemes;

		//! Track the claimed control schemes available
		std::vector<ControlScheme> mClaimedSchemes;
	};
}