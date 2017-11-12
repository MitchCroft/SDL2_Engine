#pragma once

//! Include the STD elements
#include <vector>

//! Include the SDL2_Engine objects required to run the scene
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

#include <UI/UIElements/UIButton.hpp>
#include <UI/UIElements/UIPanel.hpp>
#include <UI/UIElements/UILabel.hpp>
#include <UI/UIElements/UITextbox.hpp>

//! Include the required Game Objects
#include "../Game/Powerups/Powerup.hpp"
#include "../Game/Input/ControlScheme.hpp"

namespace BombSquad {
	/*
	 *		Name: GameSetupScene
	 *		Author: Mitchell Croft
	 *		Created: 11/11/2017
	 *		Modified: 11/11/2017
	 *		
	 *		Purpose:
	 *		Allow for modification of the settings involved in the game,
	 *		from map dimensions and obstacles spawning chances to which 
	 *		powerups should be available in the game
	**/
	class GameSetupScene : public SDL2_Engine::Scenes::ISceneBase {
	public:
		/*
			GameSetupScene : Constructor - Initialise with default values
			Created: 11/11/2017
			Modified: 11/11/2017

			param[in] pControls - A vector of Control Schemes to be given to the player
		*/
		GameSetupScene(const std::vector<ControlScheme>& pControls);

		/*
			GameSetupScene : createScene - Load the values required for the Scene to operate
			Created: 11/11/2017
			Modified: 11/11/2017

			return bool - Returns true if the Scene was initialised successfully
		*/
		bool createScene() override;

		/*
			GameSetupScene : destroyScene - Destroy the values used to setup the Scene
			Created: 11/11/2017
			Modified: 11/11/2017
		*/
		void destroyScene() override;

		/*
			GameSetupScene : update - Monitor input values to check for return to previous Scene
			Created: 11/11/2017
			Modified: 11/11/2017
		*/
		void update() override;

	private:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------------Game Values-----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Flag if the Game Manager should be destroyed on Scene shutdown
		bool mDestroyGM;

		//! Store Values to be modified for use in game
		std::vector<Powerup> mAvailablePowerups;

		//! Store an array of flags for active powerups
		char* mPowerupFlags;

		//! Track the scroll index of the powerups menu
		int mScrollIndex;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Resources------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Store the texture used for the background
		SDL2_Engine::LocalResource<SDL2_Engine::Texture> mBackgroundTex;

		//! Store the fonts used for displaying text information
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mTitleFont;
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mMinorLabelFont;
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mInfoFont;
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mActionFont;
		SDL2_Engine::LocalResource<SDL2_Engine::Font> mDescFont;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------------UI Elements-----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Store pointers to the powerup UI buttons
		std::vector<SDL2_Engine::UI::UIElements::UIButton*> mButtonPointers;

		//! Store pointers to the width, height and obstacle textbox's
		SDL2_Engine::UI::UIElements::UITextbox* mWidthTB;
		SDL2_Engine::UI::UIElements::UITextbox* mHeightTB;
		SDL2_Engine::UI::UIElements::UITextbox* mObstacleTB;

		//! Store the UI elements that are used to show descriptions of the powerups
		SDL2_Engine::UI::UIElements::UIPanel* mSpritePanel;
		SDL2_Engine::UI::UIElements::UILabel* mPowerupName;
		SDL2_Engine::UI::UIElements::UILabel* mPowerupDescription;

		//Functions for modifying active powerups
		void updatePowerupList();
	};
}