#include "UITextbox.hpp"

//! Include the SDL2_Engine objects
#include "../../Globals.hpp"
#include "../../Rendering/Renderer.hpp"
#include "../../Input/Keyboard/Keyboard.hpp"

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
			*		Name: TextboxInternalData
			*		Author: Mitchell Croft
			*		Created: 15/10/2017
			*		Modified: 15/10/2017
			*
			*		Purpose:
			*		Store UIButton internal data
			**/
			struct UITextbox::TextboxInternalData {
				//! Store the string to display
				std::string text = "";

				//! Store the Font to use
				_TTF_Font* font = nullptr;

				//! Store the maximum length of the text
				int maxLength = -1;

				//! Store the flags used to process input
				Utilities::Bitmask<Input::EKeyboardInputFlags> inputFlags = Input::EKeyboardInputFlags::All;

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
				UITextbox : getText - Get the text that the Textbox is displaying
				Created: 15/10/2017
				Modified: 15/10/2017

				return const char* - Returns a constant pointer to the text
			*/
			const char* UITextbox::getText() const { return mData->text.c_str(); }

			/*
				UITextbox : setText - Set the text that the Textbox is displaying
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pTxt - A C-String to copy the data of
			*/
			void UITextbox::setText(const char* pTxt) { mData->text = (mData->maxLength < 0 ? pTxt : std::string(pTxt).substr(0, mData->maxLength)); }

			/*
				UITextbox : getFont - Get the SDL Font that the Textbox is using to Render
				Created: 15/10/2017
				Modified: 15/10/2017

				return _TTF_Font* - Returns a pointer to the SDL Font
			*/
			_TTF_Font* UITextbox::getFont() const { return mData->font; }

			/*
				UITextbox : setFont - Set the SDL Font that the Textbox is using to Render
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pFont - A pointer to a _TTF_Font object to use
			*/
			void UITextbox::setFont(_TTF_Font* pFont) { mData->font = pFont; }

			/*
				UITextbox : getMaxLength - Get the maximum length of the text allowed
				Created: 15/10/2017
				Modified: 15/10/2017

				return const int& - Returns a constant reference to the internal value (<0 means no limit)
			*/
			const int& UITextbox::getMaxLength() const { return mData->maxLength; }

			/*
				UITextbox : setMaxLength - Set the maximum length of the text allowed
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pLength - The new maximum length value of the Textbox (<0 means no limit)
			*/
			void UITextbox::setMaxLength(const int& pLength) { mData->maxLength = (pLength < 0 ? -1 : pLength); }

			/*
				UITextbox : getInputFlags - Get the flags used to indicate the allowed characters
				Created: 15/10/2017
				Modified: 15/10/2017

				return const Bitmask<EKeyboardInputFlags>& - Returns a constant reference to the internal bitmask
			*/
			const Utilities::Bitmask<Input::EKeyboardInputFlags>& UITextbox::getInputFlags() const { return mData->inputFlags; }

			/*
				UITextbox : setInputFlags - Set the flags used to indicate the allowed characters
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pFlags - A bitmask of EKeyboardInputFlag values indicating the new settings
			*/
			void UITextbox::setInputFlags(const Utilities::Bitmask<Input::EKeyboardInputFlags>& pFlags) { mData->inputFlags = pFlags; }

			/*
				UITextbox : getTextColour - Get the Colour that the Textbox is using to render text per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UITextbox::getTextColour(const EActionState& pState) const { return mData->textColour[(int)pState]; }

			/*
				UITextbox : setTextColour - Set the Colour that the Textbox text should be rendered in per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UITextbox::setTextColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->textColour[(int)pState] = pCol; }

			/*
				UITextbox : getFillColour - Get the Colour that the Textbox is using for a fill colour per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UITextbox::getFillColour(const EActionState& pState) const { return mData->fillColour[(int)pState]; }

			/*
				UITextbox : setFillColour - Set the Colour that the Textbox is using for fill colour per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UITextbox::setFillColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->fillColour[(int)pState] = pCol; }

			/*
				UITextbox : getBorderColour - Get the Colour that the Textbox is using for a border colour per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UITextbox::getBorderColour(const EActionState& pState) const { return mData->borderColour[(int)pState]; }

			/*
				UITextbox : setBorderColour - Set the Colour that the Textbox is using for a border colour per state
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pState - An EActionState defining the state to retrieve the Colour of
				param[in] pCol - A Colour object holding the new colour values
			*/
			void UITextbox::setBorderColour(const EActionState& pState, const Rendering::Colour& pCol) { mData->borderColour[(int)pState] = pCol; }

			/*
				UITextbox : getRenderType - Get the Render Type that the Textbox is using to Render text
				Created: 15/10/2017
				Modified: 15/10/2017

				return const ETextRenderType& Returns a constant reference to the internal Render Type value
			*/
			const Rendering::ETextRenderType& UITextbox::getRenderType() const { return mData->renderType; }

			/*
				UITextbox : setRenderType - Set the Render Type that the Textbox is using to Render text
				Created: 15/10/2017
				Modified: 15/10/2017

				param[in] pType - An ETextRenderType value defining the new Render Type
			*/
			void UITextbox::setRenderType(const Rendering::ETextRenderType& pType) { mData->renderType = pType; }

			/*
				UITextbox : Constructor - Initialise with default values
				Created: 15/10/2017
				Modified: 15/10/2017
			*/
			UITextbox::UITextbox() : IUIBase(EUIType::Textbox), mData(nullptr) {}

			/*
				UITextbox : createUI - Allocate memory for the Textbox
				Created: 15/10/2017
				Modified: 15/10/2017

				return bool - Returns true if setup was successful
			*/
			bool UITextbox::createUI() { mData = new TextboxInternalData(); return true; }

			/*
				UITextbox : destroyUI - Deallocate memory used for the Textbox
				Created: 15/10/2017
				Modified: 15/10/2017
			*/
			void UITextbox::destroyUI() { if (mData) delete mData; mData = nullptr; }

			/*
				UITextbox : update - Update the internal values in response to keyboard input
				Created: 15/10/2017
				Modified: 15/10/2017
			*/
			void UITextbox::update() {
				//Check if the textbox is highlighted
				if (mState != EActionState::Highlighted) return;

				//Update the internal string
				Globals::get<Input::Keyboard>().modifyStringByKeyboard(mData->text, mData->maxLength, mData->inputFlags);
			}

			/*
				UITextbox : render - Function to facilitate the the rendering of images once per cycle
				Created: 15/10/2017
				Modified: 15/10/2017
			*/
			void UITextbox::render() {
				//Get the renderer
				Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

				//Fill in the background of the button
				rend.drawRect(*(SDL_Rect*)&mLocation, mData->fillColour[(int)mState], true, &mData->borderColour[(int)mState]);

				//Ensure there is a font to use
				if (!mData->font) return;

				//Check there if there is text to draw
				if (mData->text.length())
					rend.drawText(mData->text.c_str(), mData->font, { mLocation.x + mLocation.w / 2, mLocation.y + mLocation.h / 2 }, mData->textColour[(int)mState], Rendering::ETextAlignment::Center, mData->renderType);

				//Otherwise draw instructions
				else {
					//Copy the Colour values for the state
					Rendering::Colour tempCol = mData->textColour[(int)mState];

					//Half the opacity
					tempCol.a >>= 1;

					//Output the instructions
					rend.drawText("Type Here...", mData->font, { mLocation.x + mLocation.w / 2, mLocation.y + mLocation.h / 2 }, tempCol, Rendering::ETextAlignment::Center, mData->renderType);
				}
			}
		}
	}
}
