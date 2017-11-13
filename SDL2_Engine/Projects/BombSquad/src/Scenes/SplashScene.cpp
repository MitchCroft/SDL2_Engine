#include "SplashScene.hpp"

//! Include the SDL2_Engine objects required
#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include <Math.hpp>
#include <Time.hpp>
#include <Window/Window.hpp>
#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>
#include <UI/Canvas.hpp>
#include <UI/UIElements/UILabel.hpp>
#include <Scenes/SceneManager.hpp>

//! Include the External Menu to transition to
#include "ExternalMenu.hpp"

using namespace SDL2_Engine;

namespace BombSquad {
	/*
		SplashScene : Constructor - Initialise with default values
		Created: 02/11/2017
		Modified: 02/11/2017
	*/
	SplashScene::SplashScene() : mLabels(nullptr), mLabelCount(0), mTimer(0.f) {}

	/*
		SplashScene : createScene - Load and setup the Scene for rendering
		Created: 02/11/2017
		Modified: 02/11/2017

		return bool - Returns true if the Scene was created successfully
	*/
	bool SplashScene::createScene() {
		//Clear all current UI elements on the canvas
		Globals::get<UI::Canvas>().clearCanvas();

		//Store the size of the text to render in
		const size_t FNT_SIZE = 24;

		//Attempt to load the Font resource
		if (!(mOpeningFont = Globals::get<Resources>().loadResource<Font>("resources/Fonts/Ebrima.ttf", FNT_SIZE))) return false;

		//Store the text to display on the screen
		char* lines[] = { "Disclaimer",
						  "",
						  "This is a student project developed by Mitchell Croft (CIT192561) to demonstrate",
						  "the application of the library 'Objx: Simplifying User Generated Content'.",
						  "All assets included within this project (unless otherwise indicated) where taken from",
						  "the internet and assumed to be open domain.",
						  "",
						  "Email: me@mitchcroft.games - Website: mitchcroft.games - Github: MitchCroft"
		};

		//Get the number of message lines
		mLabelCount = sizeof(lines) / sizeof(char*);

		//Create the array to hold the pointers to the labels
		mLabels = new SDL2_Engine::UI::UIElements::UILabel*[mLabelCount];

		//Get the Window dimensions
		auto DIM = Globals::get<Window>().getWindowDimensions();

		//Move the dimensions to the middle on x and top on y
		DIM.x /= 2;
		DIM.y = (DIM.y / 2) - ((int)mLabelCount / 2 * (int)FNT_SIZE);

		//Loop through and create the labels
		for (size_t i = 0; i < mLabelCount; i++) {
			//Create the Label UI element
			if (!(mLabels[i] = Globals::get<UI::Canvas>().createUI<UI::UIElements::UILabel>())) {
				//Output the error message
				Globals::get<Debug::Logger>().logError("Failed to create the Splash Scene Label UI element");
				return false;
			}

			//Set the labels position to center screen
			mLabels[i]->setLocation({ DIM.x, DIM.y + (int)(FNT_SIZE * i) });

			//Set the alignment
			mLabels[i]->setAlignment(Rendering::ETextAlignment::Center);

			//Set the render type
			mLabels[i]->setRenderType(Rendering::ETextRenderType::Blended);

			//Set the text for the label
			mLabels[i]->setText(lines[i]);

			//Assign the font to the Label
			mLabels[i]->setFont(mOpeningFont->font);
		}

		//Return success
		return true;
	}

	/*
		SplashScene : destroyScene - Function to facilitate the deallocation of resources for a Scene
		Created: 02/11/2017
		Modified: 02/11/2017
	*/
	void SplashScene::destroyScene() { if (mLabels) delete[] mLabels; }

	/*
		SplashScene : update - Function to facilitate the updating of values once per cycle
		Created: 02/11/2017
		Modified: 13/11/2017
	*/
	void SplashScene::update() {
		//Increment the timer's value
		mTimer += Globals::get<Time>().getRealDelta();

		//Get the colour to be applied
		auto toAssign = Rendering::Colour(255, 255, 255, 0).lerp(Rendering::Colour(Rendering::Colour::White), Globals::get<Math>().clamp01(mTimer / 2.f));

		//Set the render colour of the labels
		for (size_t i = 0; i < mLabelCount; i++)
			mLabels[i]->setColour(toAssign);

		//Check if the splash screen is finished
		if (mTimer >= 6.f) {
			//Load the main menu 
			Globals::get<Scenes::SceneManager>().addScene<ExternalMenu>("mainMenu");

			//Close the current scene
			shutdown();
		}
	}
}
