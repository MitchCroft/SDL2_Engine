#include "UIButton.hpp"

//! Include the SDL2_Engine objects
#include "../../Globals.hpp"
#include "../../Rendering/Renderer.hpp"

//! Include the required STL objects
#include <string>

//! Get the SDL_Point definition
#include <SDL_rect.h>

//! Store how many different actionable states there are
#define STATE_TOTAL 3

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: ButtonInternalData
			 *		Author: Mitchell Croft
			 *		Created: 14/10/2017
			 *		Modified: 14/10/2017
			 *		
			 *		Purpose:
			 *		Store UIButton internal data
			**/
			struct UIButton::ButtonInternalData {
				//! Store the string to display
				std::string text = "Default";

				//! Store the Font to use
				_TTF_Font* font = nullptr;

				//! Store the colour to display text in
				Rendering::Colour textColour[STATE_TOTAL]{ 0x0000007F, Rendering::Colour::Black , Rendering::Colour::Black };

				//! Store the colour to fill the button in
				Rendering::Colour fillColour[STATE_TOTAL]{ 0xD3D3D37F, 0xD3D3D3FF, 0x708090FF };

				//! Store the colour to fill the border in
				Rendering::Colour borderColour[STATE_TOTAL]{ 0x0000007F, Rendering::Colour::Black , Rendering::Colour::Black };

				//! Store the render type
				Rendering::ETextRenderType renderType = Rendering::ETextRenderType::Blended;
			};

			/*
				UIButton : getText - Get the text that the Button is displaying
				Created: 14/10/2017
				Modified: 14/10/2017

				return const char* - Returns a constant pointer to the text
			*/
			const char* UIButton::getText() const { return mData->text.c_str(); }

			/*
				UIButton : setText - Set the text that the Button is displaying
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pTxt - A C-String to copy the data of
			*/
			void UIButton::setText(const char* pTxt) { mData->text = pTxt; }

			/*
				UIButton : getFont - Get the SDL Font that the Button is using to Render
				Created: 14/10/2017
				Modified: 14/10/2017

				return _TTF_Font* - Returns a pointer to the SDL Font
			*/
			_TTF_Font* UIButton::getFont() const { return mData->font; }

			/*
				UIButton : setFont - Set the SDL Font that the Button is using to Render
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pFont - A pointer to a _TTF_Font object to use
			*/
			void UIButton::setFont(_TTF_Font* pFont) { mData->font = pFont; }

			/*
				UIButton : getTextColour - Get the Colour that the Button is using to render text per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UIButton::getTextColour(const EActionState& pState) const { return mData->textColour[(int)pState]; }

			/*
				UIButton : setTextColour - Set the Colour that the Button text should be rendered in per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UIButton::setTextColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->textColour[(int)pState] = pCol; }

			/*
				UIButton : getFillColour - Get the Colour that the Button is using for a fill colour per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UIButton::getFillColour(const EActionState& pState) const { return mData->fillColour[(int)pState]; }

			/*
				UIButton : setFillColour - Set the Colour that the Button is using for fill colour per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UIButton::setFillColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->fillColour[(int)pState] = pCol; }

			/*
				UIButton : getBorderColour - Get the Colour that the Button is using for a border colour per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UIButton::getBorderColour(const EActionState& pState) const { return mData->borderColour[(int)pState]; }

			/*
				UIButton : setBorderColour - Set the Colour that the Button is using for a border colour per state
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UIButton::setBorderColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->borderColour[(int)pState] = pCol; }

			/*
				UIButton : getRenderType - Get the Render Type that the Button is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				return const ETextRenderType& Returns a constant reference to the internal Render Type value
			*/
			const Rendering::ETextRenderType& UIButton::getRenderType() const { return mData->renderType; }

			/*
				UIButton : setRenderType - Set the Render Type that the Button is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pType - An ETextRenderType value defining the new Render Type
			*/
			void UIButton::setRenderType(const Rendering::ETextRenderType& pType) { mData->renderType = pType; }

			/*
				UIButton : Constructor - Initialise with default values
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			UIButton::UIButton() : IUIBase(EUIType::Button), mData(nullptr) {}

			/*
				UIButton : createUI - Allocate memory for the Button
				Created: 14/10/2017
				Modified: 14/10/2017

				return bool - Returns true if setup was successful
			*/
			bool UIButton::createUI() { mData = new ButtonInternalData(); return true; }

			/*
				UIButton : destroyUI - Deallocate memory used for the Button
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			void UIButton::destroyUI() { if (mData) delete mData; mData = nullptr; }

			/*
				UIButton : render - Function to facilitate the the rendering of images once per cycle
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			void UIButton::render() {
				//Get the renderer
				Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

				//Fill in the background of the button
				rend.drawRect(*(SDL_Rect*)&mLocation, mData->fillColour[(int)mState], true, &mData->borderColour[(int)mState]);

				//Check there is text to render
				if (!(mData->text.length() && mData->font)) return;

				//Render the text
				rend.drawText(mData->text.c_str(), mData->font, {mLocation.x + mLocation.w / 2, mLocation.y + mLocation.h / 2}, mData->textColour[(int)mState], Rendering::ETextAlignment::Center, mData->renderType);
			}
		}
	}
}
