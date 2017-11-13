#include "GameSetupScene.hpp"

#include <string>

//! Include the Game Manager interface
#include "../Game/Management/GameManager.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include <Math.hpp>

#include <Input/Controllers/Controllers.hpp>
#include <Input/Keyboard/Keyboard.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceTexture.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>

#include <Window/Window.hpp>
#include <UI/Canvas.hpp>
#include <UI/UIElements/Interfaces/UIBounds.hpp>

#include <Scenes/SceneManager.hpp>
#include "PlayerSetupScene.hpp"
#include "GameLoadingScene.hpp"

using namespace SDL2_Engine;
using namespace SDL2_Engine::UI;
using namespace SDL2_Engine::UI::UIElements;

namespace BombSquad {
	/*
		roundToClosestMultiple - Round a value to the closest multiple
		Author: Mitchell Croft
		Created: 11/11/2017
		Modified: 11/11/2017

		param[in] pVal - The value to be round off
		param[in] pMultiple - The multiple to round to

		return int - Returns the rounded value as an int
	*/
	inline int roundToClosestMultiple(const int& pVal, const int& pMultiple) { return ((pVal + pMultiple / 2) / pMultiple) * pMultiple; }

	/*
		GameSetupScene : Constructor - Initialise with default values
		Created: 11/11/2017
		Modified: 11/11/2017

		param[in] pControls - A vector of Control Schemes to be given to the player
	*/
	GameSetupScene::GameSetupScene(const std::vector<ControlScheme>& pControls) : mDestroyGM(true), mPowerupFlags(nullptr), mScrollIndex(0) {
		//Create the Game Manager
		GM::create();

		//Stash the player control schemes
		GM::setPlayerControls(pControls);
	}

	/*
		GameSetupScene : createScene - Load the values required for the Scene to operate
		Created: 11/11/2017
		Modified: 13/11/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool GameSetupScene::createScene() {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Load Resources----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Get the resources object
		Resources& res = Globals::get<Resources>();

		//Load the background panel texture
		mBackgroundTex = res.loadResource<Texture>("resources/UI/Textures/MenuBackground.png");

		//Check the texture loaded correctly
		if (mBackgroundTex->status() != EResourceLoadStatus::Loaded) return false;

		//Load a larger title font
		mTitleFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 50);

		//Check the font loaded correctly
		if (mTitleFont->status() != EResourceLoadStatus::Loaded) return false;

		//Load the minor label font
		mMinorLabelFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 25);

		//Check the font was loaded correctly
		if (mMinorLabelFont->status() != EResourceLoadStatus::Loaded) return false;

		//Load the information supplying font
		mInfoFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 20);

		//Check the font loaded correctly
		if (mInfoFont->status() != EResourceLoadStatus::Loaded) return false;

		//Load the action indicating font
		mActionFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 17);

		//Check the font loaded correctly
		if (mActionFont->status() != EResourceLoadStatus::Loaded) return false;

		//Load the description font
		mDescFont = res.loadResource<Font>("resources/Fonts/Ebrima.ttf", 12);

		//Check the font loaded correctly
		if (mDescFont->status() != EResourceLoadStatus::Loaded) return false;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Setup Game Manager--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Get the available powerups 
		mAvailablePowerups = GM::getBlueprints();

		//Create the powerup flag array
		mPowerupFlags = new char[mAvailablePowerups.size()];

		//Set the initial value as enabled
		memset(mPowerupFlags, 1, sizeof(char) * mAvailablePowerups.size());

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Create UI Elements--------------------------------////////
		////////                                                                                     ////////
		////////                        Featuring lots of horrible magic numbers                     ////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//Get the window dimensions
		const auto DIM = Globals::get<Window>().getWindowDimensions();
		
		//Get the canvas 
		Canvas& canvas = Globals::get<Canvas>();

		//Clear previously loaded UI elements
		canvas.clearCanvas();

		//Create the background object
		UIPanel* backgroundPanel = canvas.createUI<UIPanel>();

		//Assign the panel to the entire window
		backgroundPanel->setLocation({ 0, 0, DIM.x, DIM.y });

		//Give the background image
		backgroundPanel->setImage(mBackgroundTex->texture);

		#pragma region Scene Title
		//Create a label for the width
		UILabel* sceneTitle = canvas.createUI<UILabel>();

		//Set the label position
		sceneTitle->setLocation({ DIM.x / 2, 60 });

		//Set alignment
		sceneTitle->setAlignment(Rendering::ETextAlignment::Center);

		//Set text
		sceneTitle->setText("Game Setup");

		//Set font
		sceneTitle->setFont(mTitleFont->font);

		//Set the colour
		sceneTitle->setColour(Rendering::Colour::Wheat);
		#pragma endregion

		#pragma region Play Button
		//Create the play button
		UIButton* playBtn = canvas.createUI<UIButton>();

		//Position the button near the bottom of the screen
		playBtn->setLocation({ DIM.x / 2 - 100, DIM.y - 150, 200, 100 });

		//Set the play action
		playBtn->setAction([&](IUIAction* pObj, void* pData) {
			//Remove flagged powerups
			for (int i = (int)mAvailablePowerups.size() - 1; i >= 0; i--) {
				//Check if the flag is off
				if (!mPowerupFlags[i])
					mAvailablePowerups.erase(mAvailablePowerups.begin() + i);
			}
			
			//Assign the stored values to the Game Manager
			GM::setBlueprints(mAvailablePowerups);

			//Convert the map dimensions to integers
			int width = std::stoi(mWidthTB->getText());
			int height = std::stoi(mHeightTB->getText());

			//Assign the dimensions
			GM::setMapDimensions(width, height);

			//Convert the chance to a 0 - 1 scale float
			float chance = (float)std::stoi(mObstacleTB->getText()) / 100.f;

			//Assign the chance
			GM::setObstacleDensity(chance);

			//Convert the powerup chance to a 0 - 1 scale float
			chance = (float)std::stoi(mPowerupChanceTB->getText()) / 100.f;

			//Assign the chance
			GM::setPowerupChance(chance);

			//Shutdown the game setup scene
			shutdown();

			//Load the Game Load Screen
			Globals::get<Scenes::SceneManager>().addScene<GameLoadingScene>();

			//Prevent the manager being destroyed with the scene
			mDestroyGM = false;
		});

		//Set the text on the play button
		playBtn->setText("Play");

		//Set the font
		playBtn->setFont(mInfoFont->font);
		#pragma endregion
		
		#pragma region Map Width
		//Create a label for the width
		UILabel* mapWidth = canvas.createUI<UILabel>();

		//Set the label position
		mapWidth->setLocation({ 130, 120 });

		//Set alignment
		mapWidth->setAlignment(Rendering::ETextAlignment::Center);

		//Set text
		mapWidth->setText("Width");

		//Set font
		mapWidth->setFont(mMinorLabelFont->font);

		//Set the colour
		mapWidth->setColour(Rendering::Colour::Wheat);

		//Create the width textbox
		mWidthTB = canvas.createUI<UITextbox>();

		//Position the width textbox
		mWidthTB->setLocation({ 80, 180, 100, 100 });

		//Set the text conditions
		mWidthTB->setInputFlags(Input::EKeyboardInputFlags::Numerical);

		//Set the maximum length
		mWidthTB->setMaxLength(2);

		//Set the starting text
		mWidthTB->setText("25");

		//Set the font
		mWidthTB->setFont(mInfoFont->font);

		//Create the width increase button
		UIButton* widthIncrease = canvas.createUI<UIButton>();

		//Set the location
		widthIncrease->setLocation({ 80, 140, 100, 40 });

		//Set the text
		widthIncrease->setText("/\\");

		//Set the font
		widthIncrease->setFont(mActionFont->font);

		//Set the action
		widthIncrease->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mWidthTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) + 5;

			//Set the text
			mWidthTB->setText(std::to_string(val).c_str());
		});

		//Create the width decrease button
		UIButton* widthDecrease = canvas.createUI<UIButton>();

		//Set the location
		widthDecrease->setLocation({ 80, 280, 100, 40 });

		//Set the text
		widthDecrease->setText("\\/");

		//Set the font
		widthDecrease->setFont(mActionFont->font);

		//Set the action
		widthDecrease->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mWidthTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) - 5;

			//Set the text
			mWidthTB->setText(std::to_string(val).c_str());
		});
		#pragma endregion

		#pragma region Map Height
		//Create a label for the width
		UILabel* mapHeight = canvas.createUI<UILabel>();

		//Set the label position
		mapHeight->setLocation({ 230, 120 });

		//Set alignment
		mapHeight->setAlignment(Rendering::ETextAlignment::Center);

		//Set text
		mapHeight->setText("Height");

		//Set font
		mapHeight->setFont(mMinorLabelFont->font);

		//Set the colour
		mapHeight->setColour(Rendering::Colour::Wheat);

		//Create the height textbox
		mHeightTB = canvas.createUI<UITextbox>();

		//Position the height textbox
		mHeightTB->setLocation({ 180, 180, 100, 100 });

		//Set the text conditions
		mHeightTB->setInputFlags(Input::EKeyboardInputFlags::Numerical);

		//Set the maximum length
		mHeightTB->setMaxLength(2);

		//Set the starting text
		mHeightTB->setText("15");

		//Set the font
		mHeightTB->setFont(mInfoFont->font);

		//Create the height increase button
		UIButton* heightIncrease = canvas.createUI<UIButton>();

		//Set the location
		heightIncrease->setLocation({ 180, 140, 100, 40 });

		//Set the text
		heightIncrease->setText("/\\");

		//Set the font
		heightIncrease->setFont(mActionFont->font);

		//Set the action
		heightIncrease->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mHeightTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) + 5;

			//Set the text
			mHeightTB->setText(std::to_string(val).c_str());
		});

		//Create the width decrease button
		UIButton* heightDecrease = canvas.createUI<UIButton>();

		//Set the location
		heightDecrease->setLocation({ 180, 280, 100, 40 });

		//Set the text
		heightDecrease->setText("\\/");

		//Set the font
		heightDecrease->setFont(mActionFont->font);

		//Set the action
		heightDecrease->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mHeightTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) - 5;

			//Set the text
			mHeightTB->setText(std::to_string(val).c_str());
		});
		#pragma endregion

		#pragma region Obstacle Chance
		//Create a label for the width
		UILabel* obstacleChance = canvas.createUI<UILabel>();

		//Set the label position
		obstacleChance->setLocation({ 640, 172 });

		//Set alignment
		obstacleChance->setAlignment(Rendering::ETextAlignment::Center);

		//Set text
		obstacleChance->setText("Obstacle Spawn Chance");

		//Set font
		obstacleChance->setFont(mMinorLabelFont->font);

		//Set the colour
		obstacleChance->setColour(Rendering::Colour::Wheat);

		//Create the obstacle chance textbox
		mObstacleTB = canvas.createUI<UITextbox>();

		//Position the obstacle chance textbox
		mObstacleTB->setLocation({ 565, 192, 150, 75 });

		//Set the text conditions
		mObstacleTB->setInputFlags(Input::EKeyboardInputFlags::Numerical);

		//Set the maximum length
		mObstacleTB->setMaxLength(3);

		//Set the starting text
		mObstacleTB->setText("35");

		//Set the font
		mObstacleTB->setFont(mInfoFont->font);

		//Create the chance increase button
		UIButton* increaseChance = canvas.createUI<UIButton>();

		//Position the button
		increaseChance->setLocation({ 715, 192, 25, 75 });

		//Set the text
		increaseChance->setText(">");

		//Set the font
		increaseChance->setFont(mActionFont->font);

		//Set the action
		increaseChance->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mObstacleTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) + 5;

			//Set the text
			mObstacleTB->setText(std::to_string(val).c_str());
		});

		//Create the decrease chance button
		UIButton* decreaseChance = canvas.createUI<UIButton>();

		//Position the button
		decreaseChance->setLocation({ 540, 192, 25, 75 });

		//Set the text
		decreaseChance->setText("<");

		//Set the font
		decreaseChance->setFont(mActionFont->font);

		//Set the action
		decreaseChance->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mObstacleTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) - 5;

			//Set the text
			mObstacleTB->setText(std::to_string(val).c_str());
		});
		#pragma endregion

		#pragma region Powerup Chance
		//Create a label for the width
		UILabel* powerupChance = canvas.createUI<UILabel>();

		//Set the label position
		powerupChance->setLocation({ 640, 280 });

		//Set alignment
		powerupChance->setAlignment(Rendering::ETextAlignment::Center);

		//Set text
		powerupChance->setText("Powerup Spawn Chance");

		//Set font
		powerupChance->setFont(mMinorLabelFont->font);

		//Set the colour
		powerupChance->setColour(Rendering::Colour::Wheat);

		//Create the obstacle chance textbox
		mPowerupChanceTB = canvas.createUI<UITextbox>();

		//Position the obstacle chance textbox
		mPowerupChanceTB->setLocation({ 565, 300, 150, 75 });

		//Set the text conditions
		mPowerupChanceTB->setInputFlags(Input::EKeyboardInputFlags::Numerical);

		//Set the maximum length
		mPowerupChanceTB->setMaxLength(3);

		//Set the starting text
		mPowerupChanceTB->setText("35");

		//Set the font
		mPowerupChanceTB->setFont(mInfoFont->font);

		//Create the chance increase button
		UIButton* increasePUChance = canvas.createUI<UIButton>();

		//Position the button
		increasePUChance->setLocation({ 715, 300, 25, 75 });

		//Set the text
		increasePUChance->setText(">");

		//Set the font
		increasePUChance->setFont(mActionFont->font);

		//Set the action
		increasePUChance->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mPowerupChanceTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) + 5;

			//Set the text
			mPowerupChanceTB->setText(std::to_string(val).c_str());
		});

		//Create the decrease chance button
		UIButton* decreasePUChance = canvas.createUI<UIButton>();

		//Position the button
		decreasePUChance->setLocation({ 540, 300, 25, 75 });

		//Set the text
		decreasePUChance->setText("<");

		//Set the font
		decreasePUChance->setFont(mActionFont->font);

		//Set the action
		decreasePUChance->setAction([&](IUIAction* pObj, void* pData) {
			//Get the text as an integer value
			int val = std::stoi(mPowerupChanceTB->getText());

			//Increase the value by 5
			val = roundToClosestMultiple(val, 5) - 5;

			//Set the text
			mPowerupChanceTB->setText(std::to_string(val).c_str());
		});
		#pragma endregion

		#pragma region Powerup List
		//Define the size of the powerup list levels
		UIBounds listDims = { 840, 280, 300, 50 };

		//Set the number of powerup list levels to create
		mButtonPointers.resize(5);

		//Create the range of powerup list levels
		for (size_t i = 0; i < mButtonPointers.size(); i++) {
			//Create the button UI element
			mButtonPointers[i] = canvas.createUI<UIButton>();

			//Position the button
			mButtonPointers[i]->setLocation({ listDims.x, listDims.y + (int)i * listDims.h, listDims.w, listDims.h });

			//Tag the button with the index
			mButtonPointers[i]->setTag(std::to_string(i).c_str());

			//Set the font
			mButtonPointers[i]->setFont(mInfoFont->font);

			//Set empty default text
			mButtonPointers[i]->setText("");

			//Apply the action
			mButtonPointers[i]->setAction([&](IUIAction* pObj, void* pData) {
				//Cast the object to a button
				UIButton* btn = dynamic_cast<UIButton*>(pObj);

				//Check the button was received
				if (!btn) return;

				//Convert the tag to a powerup index
				int powerupIndex = mScrollIndex + std::stoi(btn->getTag().c_str());

				//Check the index is in range
				if (powerupIndex >= (int)mAvailablePowerups.size()) return;

				//Toggle the flag
				mPowerupFlags[powerupIndex] = !mPowerupFlags[powerupIndex];

				//Set the state fill colours based on the toggle
				if (mPowerupFlags[powerupIndex]) {
					btn->setFillColour(EActionState::Default, Rendering::Colour::DarkGreen);
					btn->setFillColour(EActionState::Highlighted, Rendering::Colour::Green);
				} else {
					btn->setFillColour(EActionState::Default, Rendering::Colour::DarkRed);
					btn->setFillColour(EActionState::Highlighted, Rendering::Colour::Red);
				}
			});
		}

		//Force the powerup list to update
		updatePowerupList();

		//Create the up list button
		UIButton* upList = canvas.createUI<UIButton>();

		//Set the location
		upList->setLocation({ listDims.x, listDims.y - listDims.h, listDims.w, listDims.h });

		//Set the text
		upList->setText("/\\");

		//Set the font
		upList->setFont(mActionFont->font);

		//Set the action
		if (mAvailablePowerups.size() > mButtonPointers.size()) {
			upList->setAction([&](IUIAction* pObj, void* pData) {
				//Check if the scroll index can be reduce further
				if (mScrollIndex > 0) {
					//Reduce the scroll index
					mScrollIndex--;

					//Reload the powerup list
					updatePowerupList();
				}
			});
		}

		//Create the down list button
		UIButton* downList = canvas.createUI<UIButton>();

		//Set the location
		downList->setLocation({ listDims.x, listDims.y + listDims.h * (int)mButtonPointers.size(), listDims.w, listDims.h });

		//Set the text
		downList->setText("\\/");

		//Set the font
		downList->setFont(mActionFont->font);

		//Set the action
		if (mAvailablePowerups.size() > mButtonPointers.size()) {
			downList->setAction([&](IUIAction* pObj, void* pData) {
				//Check the scroll index should be increased further
				if (mScrollIndex < (int)mAvailablePowerups.size() - (int)mButtonPointers.size()) {
					//Increase the scroll index
					mScrollIndex++;

					//Reload the powerup list
					updatePowerupList();
				}
			});
		}
		#pragma endregion

		#pragma region Powerup Display
		//Create a panel to cover the back
		UIPanel* descPanel = canvas.createUI<UIPanel>();

		//Set the position
		descPanel->setLocation({ 80, 400, 600, 150 });

		//Create a panel to display 
		mSpritePanel = canvas.createUI<UIPanel>();

		//Set the location
		mSpritePanel->setLocation({ 90, 410, 130, 130 });

		//Create the name label
		mPowerupName = canvas.createUI<UILabel>();

		//Set the location
		mPowerupName->setLocation({ 250, 410 });

		//Set the font
		mPowerupName->setFont(mActionFont->font);

		//Set alignment
		mPowerupName->setAlignment(Rendering::ETextAlignment::Left);

		//Create the description label
		mPowerupDescription = canvas.createUI<UILabel>();

		//Set the location
		mPowerupDescription->setLocation({ 250, 480 });

		//Set the font
		mPowerupDescription->setFont(mDescFont->font);

		//Set alignment
		mPowerupDescription->setAlignment(Rendering::ETextAlignment::Left);
		#pragma endregion

		//Build the interactive map
		canvas.rebuildInteractionMap();

		//Return success
		return true;
	}

	/*
		GameSetupScene : destroyScene - Destroy the values used to setup the Scene
		Created: 11/11/2017
		Modified: 11/11/2017
	*/
	void GameSetupScene::destroyScene() {
		//Destroy the powerup flags
		if (mPowerupFlags) {
			delete[] mPowerupFlags;
			mPowerupFlags = nullptr;
		}

		//Check for GM destroy flag
		if (mDestroyGM) GM::destroy();
	}

	/*
		GameSetupScene : update - Monitor input values to check for return to previous Scene
		Created: 11/11/2017
		Modified: 13/11/2017
	*/
	void GameSetupScene::update() {
		//Check for return 
		if (Globals::get<Input::Controllers>().btnPressed(Input::EGamePadBtnCodes::B) ||
			Globals::get<Input::Keyboard>().keyPressed(Input::EKeyboardKeyCode::Escape)) {
			//Flag this scene for shutdown
			shutdown();

			//Add the Player Setup Scene to active rotation
			Globals::get<Scenes::SceneManager>().addScene<PlayerSetupScene>();
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Clamp Usable Values-------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//Get the Math object
		const Math& MATH = Globals::get<Math>();

		//Create buffer to store numerical data in
		int buffer;

		//Check the width textbox is not in use
		if (mWidthTB->getState() != EActionState::Highlighted) {
			//Check there is text
			if (!strlen(mWidthTB->getText())) mWidthTB->setText("25"); 

			//Otherwise clamp values
			else {
				//Get the width textbox value as an int
				buffer = std::stoi(mWidthTB->getText());

				//Bind the value in the 10 - 50 range
				buffer = MATH.clamp(buffer, 10, 50);

				//Set the textbox value
				mWidthTB->setText(std::to_string(buffer).c_str());
			}
		}

		//Check the height textbox is not in use
		if (mHeightTB->getState() != EActionState::Highlighted) {
			//Check there is text
			if (!strlen(mHeightTB->getText())) mHeightTB->setText("15");

			//Otherwise clamp values
			else {
				//Get the height textbox value as an int
				buffer = std::stoi(mHeightTB->getText());

				//Bind the value in the 10 - 50 range
				buffer = MATH.clamp(buffer, 10, 50);

				//Set the textbox value
				mHeightTB->setText(std::to_string(buffer).c_str());
			}
		}

		//Check the obstacle chance textbox is not in use
		if (mObstacleTB->getState() != EActionState::Highlighted) {
			//Check there is text
			if (!strlen(mObstacleTB->getText())) mObstacleTB->setText("35");

			//Otherwise clamp values
			else {
				//Get the obstacle chance as an int
				buffer = std::stoi(mObstacleTB->getText());

				//Bind the value in the 0 - 100 range
				buffer = MATH.clamp(buffer, 0, 100);

				//Set the textbox value
				mObstacleTB->setText(std::to_string(buffer).c_str());
			}
		}

		//Check the powerup chance textbox is not in use
		if (mPowerupChanceTB->getState() != EActionState::Highlighted) {
			//Check there is text
			if (!strlen(mPowerupChanceTB->getText())) mPowerupChanceTB->setText("35");

			//Otherwise clamp values
			else {
				//Get the powerup chance as an int
				buffer = std::stoi(mPowerupChanceTB->getText());

				//Bind the value in the 0 - 100 range
				buffer = MATH.clamp(buffer, 0, 100);

				//Set the textbox value
				mPowerupChanceTB->setText(std::to_string(buffer).c_str());
			}
		}

		//Get the active UI object
		IUIBase* active = Globals::get<Canvas>().getActionObject();

		//Check there is an active object and that it has a tag
		if (active && active->getTag().length()) {
			//Get the powerup index
			int pu = mScrollIndex + std::stoi(active->getTag().c_str());

			//Double check its in bounds
			if (pu < (int)mAvailablePowerups.size()) {
				//Set the values
				mSpritePanel->setImage(GM::getLoader().getTexture(mAvailablePowerups[pu].getTextureID()));
				mPowerupName->setText(mAvailablePowerups[pu].getName().c_str());
				mPowerupDescription->setText(mAvailablePowerups[pu].getDescription().c_str());
			}
		}

		//Otherwise nullify the powerup display information
		else {
			mSpritePanel->setImage(nullptr);
			mPowerupName->setText("Powerup Name");
			mPowerupDescription->setText("Powerup Description");
		}
	}

	/*
		GameSetupScene : updatePowerupList - Update the UI Button elements to list information corresponding to the current scroll index
		Created: 11/11/2017
		Modified: 11/11/2017
	*/
	void GameSetupScene::updatePowerupList() {
		//Loop through the available buttons
		for (int i = 0; i < (int)mButtonPointers.size(); i++) {
			//Get the powerup index
			int pu = mScrollIndex + i;

			//Check if the powerup index is in bounds
			if (pu >= (int)mAvailablePowerups.size()) return;

			//Set the button text based on the powerup name
			mButtonPointers[i]->setText(mAvailablePowerups[pu].getName().c_str());

			//Set the state fill colours based on the toggle
			if (mPowerupFlags[pu]) {
				mButtonPointers[i]->setFillColour(EActionState::Default, Rendering::Colour::DarkGreen);
				mButtonPointers[i]->setFillColour(EActionState::Highlighted, Rendering::Colour::Green);
			} else {
				mButtonPointers[i]->setFillColour(EActionState::Default, Rendering::Colour::DarkRed);
				mButtonPointers[i]->setFillColour(EActionState::Highlighted, Rendering::Colour::Red);
			}
		}
	}
}