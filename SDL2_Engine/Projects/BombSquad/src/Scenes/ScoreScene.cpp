#include "ScoreScene.hpp"

//! Include the Game Manager interface
#include "../Game/Management/GameManager.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Time.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>

#include <Window/Window.hpp>

#include <Rendering/Renderer.hpp>

#include <UI/Canvas.hpp>
#include <UI/UIElements/UIButton.hpp>

#include <Scenes/SceneManager.hpp>
#include "ExternalMenu.hpp"
#include "GameLoadingScene.hpp"

using namespace SDL2_Engine;
using namespace SDL2_Engine::UI::UIElements;

#include <SDL.h>
#include <algorithm>

namespace BombSquad {
	/*
		ScoreScene : Constructor - Initialise with default values
		Created: 13/11/2017
		Modified: 13/11/2017
	*/
	ScoreScene::ScoreScene() :
		mDestroyGM(true)
	{}

	/*
		ScoreScene : createScene - Load the values required for the Scene to operate
		Created: 13/11/2017
		Modified: 13/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool ScoreScene::createScene() {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Load Resources----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//Get the resources manager
		Resources& res = Globals::get<Resources>();
		
		//Load the background image
		mBackground = res.loadResource<Texture>("resources/Textures/GameBackground.png");

		//Check the texture was loaded correctly
		if (mBackground->status() != EResourceLoadStatus::Loaded) return false;

		//Load the score font
		mScoreFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 26);

		//Check the font was loaded correctly
		if (mScoreFont->status() != EResourceLoadStatus::Loaded) return false;

		//Load the action font
		mActionFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 18);

		//Check the font loaded correctly
		if (mActionFont->status() != EResourceLoadStatus::Loaded) return false;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Organise Player Objects------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Get the player objects
		mPlayers = GM::getPlayers();

		//Organise the players base on who is winning
		std::sort(mPlayers.begin(), mPlayers.end(), [](const Player& pLeft, const Player& pRight) {
			//Get the sum scores
			int sumLeft = (int)pLeft.wins - (int)pLeft.losses, sumRight = (int)pRight.wins - (int)pRight.losses;

			//Check if the sums are the same
			if (sumLeft != sumRight) return (sumLeft > sumRight);

			//Check if they are both still alive
			else if (pLeft.alive || pRight.alive) return (pLeft.alive && !pRight.alive);

			//Otherwise sort based on the their ID
			else return ((int)pLeft.id < (int)pRight.id);
		});

		//Set the animations based on their alive status
		for (size_t i = 0; i < mPlayers.size(); i++) {
			//Check state
			if (mPlayers[i].alive) {
				mPlayers[i].animator.setAnimation(EAnimationSet::Head, EAnimation::Cheer);
				mPlayers[i].animator.setAnimation(EAnimationSet::Body, EAnimation::Cheer);
			} else {
				mPlayers[i].animator.setAnimation(EAnimationSet::Head, EAnimation::Down);
				mPlayers[i].animator.setAnimation(EAnimationSet::Body, EAnimation::Down);
			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Create UI Elements--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//Get the Window dimensions
		const auto& DIM = Globals::get<Window>().getWindowDimensions();

		//Get the canvas object
		UI::Canvas& canvas = Globals::get<UI::Canvas>();

		//Create the play again button
		UIButton* playAgainBTN = canvas.createUI<UIButton>();

		//Position the button
		playAgainBTN->setLocation({ DIM.x / 2 + 50, DIM.y - 60, 200, 50 });

		//Set the text
		playAgainBTN->setText("Play Again");

		//Set the font
		playAgainBTN->setFont(mActionFont->font);

		//Set the action
		playAgainBTN->setAction([&](IUIAction* pObj, void* pData) {
			//Shutdown the current scene
			shutdown();

			//Prevent the Game Manager from being destroyed
			mDestroyGM = false;

			//Load the loading scene
			Globals::get<Scenes::SceneManager>().addScene<GameLoadingScene>();
		});

		//Create the main menu button
		UIButton* mainMenuBTN = canvas.createUI<UIButton>();

		//Position the button
		mainMenuBTN->setLocation({ DIM.x / 2 - 250, DIM.y - 60, 200, 50 });

		//Set the text
		mainMenuBTN->setText("Main Menu");

		//Set the font
		mainMenuBTN->setFont(mActionFont->font);

		//Set the action
		mainMenuBTN->setAction([&](IUIAction* pObj, void* pData) {
			//Shutdown the current scene
			shutdown();

			//Load the loading scene
			Globals::get<Scenes::SceneManager>().addScene<ExternalMenu>("mainMenu");
		});

		//Rebuild the interaction map
		canvas.rebuildInteractionMap();

		//Return success
		return true;
	}

	/*
		ScoreScene : destroyScene - Destroy the values used to setup the Scene
		Created: 13/11/2017
		Modified: 13/11/2017
	*/
	void ScoreScene::destroyScene() { if (mDestroyGM) GM::destroy(); }

	/*
		ScoreScene : update - Update the internal values corresponding to player actions
		Created: 13/11/2017
		Modified: 13/11/2017
	*/
	void ScoreScene::update() {
		//Get the time object
		const Time& TIME = Globals::get<Time>();

		//Update the player animators
		for (size_t i = 0; i < mPlayers.size(); i++)
			mPlayers[i].animator.update(TIME.getRealDelta());
	}

	/*
		ScoreScene : render - Render the Scene to the Window
		Created: 13/11/2017
		Modified: 13/11/2017
	*/
	void ScoreScene::render() {
		//Get the Window dimensions
		const auto& DIM = Globals::get<Window>().getWindowDimensions();

		//Get the Renderer object
		Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

		//Draw the background
		rend.drawTexture(mBackground->texture, { 0, 0, DIM.x, DIM.y });

		//Get the background panel
		const SDL_Rect PANEL = { DIM.x / 4, DIM.y / 4, DIM.x / 2, DIM.y / 2 };

		//Create a simple border colour
		const Rendering::Colour BORD_COL = Rendering::Colour::Black;

		//Fill in the panel
		rend.drawRect(PANEL, Rendering::Colour::White, true, &BORD_COL);

		//Get the segment size for each player
		const int& SEG_SIZE = PANEL.h / (int)mPlayers.size();

		//Loop through and render the player values
		for (size_t i = 0; i < mPlayers.size(); i++) {
			//Create a rectangle to store where the player animation is being rendered
			const SDL_Rect PLAY_ICO = { PANEL.x, PANEL.y + SEG_SIZE * (int)i, SEG_SIZE, SEG_SIZE };

			//Get the player object
			const Player& CURR = mPlayers[i];

			//Get the first frame
			SDL_Rect source = *(SDL_Rect*)&CURR.animator.getFrame(EAnimationSet::Body);

			//Render the frame
			rend.drawTexture(CURR.animator.getSpriteSheet(EAnimationSet::Body)->texture, PLAY_ICO, &source);

			//Get the second frame
			source = *(SDL_Rect*)&CURR.animator.getFrame(EAnimationSet::Head);

			//Render the frame
			rend.drawTexture(CURR.animator.getSpriteSheet(EAnimationSet::Head)->texture, PLAY_ICO, &source);

			//Compile a score string
			const std::string SCORE_STR = "Wins: " + std::to_string(CURR.wins) + " | Losses: " + std::to_string(CURR.losses);

			//Get the text draw position
			const SDL_Point TXT_POS = { PANEL.x + (PANEL.w / 3) * 2, PLAY_ICO.y + SEG_SIZE / 2 };

			//Render the score string
			rend.drawText(SCORE_STR.c_str(), mScoreFont->font, TXT_POS, Rendering::Colour::Black, Rendering::ETextAlignment::Center);
		}
	}
}
