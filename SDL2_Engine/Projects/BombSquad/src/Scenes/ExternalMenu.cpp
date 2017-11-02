#include "ExternalMenu.hpp"

// !Include the SDL2_Engine objects required
#include <Globals.hpp>
#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceFont.hpp>
#include <UI/Canvas.hpp>
#include <UI/UIElements/UILabel.hpp>
#include <Utilities/VersionDescriptor.hpp>

using namespace SDL2_Engine;

namespace BombSquad {
	/*
		ExternalMenu : Constructor - Initialise with default values
		Created: 02/11/2017
		Modified: 02/11/2017

		param[in] pName - The name of the object within 'MenuDesc.objx' file to load UI elements from
	*/
	ExternalMenu::ExternalMenu(const char* pName) : mLoadName(pName) {}

	/*
		ExternalMenu : createScene - Load and setup the Scene for rendering
		Created: 02/11/2017
		Modified: 02/11/2017

		return bool - Returns true if the Scene was created successfully
	*/
	bool ExternalMenu::createScene() {
		//Get the canvas object
		auto& canvas = Globals::get<UI::Canvas>();
		
		//Clear the canvas of all current UI elements
		canvas.clearCanvas();

		//Attempt to load the specified menu
		if (!canvas.loadCanvasFromObjx("resources/UI/MenuDesc.objx", mLoadName.c_str())) return false;

		//Load the font to use for the version output
		mVersionFont = Globals::get<Resources>().loadResource<Font>("resources/Fonts/Ebrima.ttf", 16);

		//Check the font loaded successfully
		if (mVersionFont->status() != EResourceLoadStatus::Loaded) return false;

		//Create a label to display the version information on
		auto label = Globals::get<UI::Canvas>().createUI<UI::UIElements::UILabel>();

		//Get the version identifier for the application
		const auto& VER = Globals::getApplicationVersion();

		//Set the text for the label
		label->setText(("VER: " + std::to_string(VER.major) + "." + std::to_string(VER.minor) + "." + std::to_string(VER.patch) + " (" + std::to_string(VER.versionID) + ")").c_str());

		//Assign the font
		label->setFont(mVersionFont->font);

		//Set the position
		label->setLocation({ 0, 0 });

		//Set the alignment
		label->setAlignment(Rendering::ETextAlignment::Left);

		//Set the colour
		label->setColour(Rendering::Colour::White);

		//Return success
		return true;
	}
}