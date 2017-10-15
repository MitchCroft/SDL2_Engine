#include "UILabel.hpp"

//! Include the SDL2_Engine objects
#include "../../Globals.hpp"
#include "../../Rendering/Renderer.hpp"

//! Include the required STL objects
#include <string>

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: LabelInternalData
			 *		Author: Mitchell Croft
			 *		Created: 14/10/2017
			 *		Modified: 14/10/2017
			 *		
			 *		Purpose:
			 *		Store internal UI Label data
			**/
			struct UILabel::LabelInternalData {
				//! Store the string to display
				std::string text = "Default";

				//! Store the Font to use
				_TTF_Font* font = nullptr;

				//! Store the colour to display it in
				Rendering::Colour colour = Rendering::Colour::Black;

				//! Store the alignment setting to use
				Rendering::ETextAlignment alignment = Rendering::ETextAlignment::Center;

				//! Store the render type
				Rendering::ETextRenderType renderType = Rendering::ETextRenderType::Blended;
			};

			/*
				UILabel : getText - Get the text that the Label is displaying
				Created: 14/10/2017
				Modified: 14/10/2017

				return const char* - Returns a constant pointer to the text
			*/
			const char* UILabel::getText() const { return mData->text.c_str(); }

			/*
				UILabel : setText - Set the text that the Label is displaying
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pTxt - A C-String to copy the data of
			*/
			void UILabel::setText(const char* pTxt) { mData->text = pTxt; }

			/*
				UILabel : getFont - Get the SDL Font that the Label is using to Render
				Created: 14/10/2017
				Modified: 14/10/2017

				return _TTF_Font* - Returns a pointer to the SDL Font
			*/
			_TTF_Font* UILabel::getFont() const { return mData->font; }

			/*
				UILabel : setFont - Set the SDL Font that the Label is using to Render
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pFont - A pointer to a _TTF_Font object to use
			*/
			void UILabel::setFont(_TTF_Font* pFont) { mData->font = pFont; }

			/*
				UIPanel : getColour - Get the Colour that the Label is using to render text
				Created: 14/10/2017
				Modified: 14/10/2017

				return const Colour& - Returns a constant reference to the internal Colour object
			*/
			const Rendering::Colour& UILabel::getColour() const { return mData->colour; }

			/*
				UILabel : setColour - Set the Colour that the Label text should be rendered in
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pCol - A Colour object holding the new colour values
			*/
			void UILabel::setColour(const Rendering::Colour& pCol) { mData->colour = pCol; }

			/*
				UILabel : getAlignment - Get the Alignment that the Label is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				return const ETextAlignment& - Returns a constant reference to the internal alignment value
			*/
			const Rendering::ETextAlignment& UILabel::getAlignment() const { return mData->alignment; }

			/*
				UILabel : setAlignment - Set the Alignment that the Label is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pAlign - An ETextAlignment value defining the new alignment
			*/
			void UILabel::setAlignment(const Rendering::ETextAlignment& pAlign) { mData->alignment = pAlign; }

			/*
				UILabel : getRenderType - Get the Render Type that the Label is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				return const ETextRenderType& Returns a constant reference to the internal Render Type value
			*/
			const Rendering::ETextRenderType& UILabel::getRenderType() const { return mData->renderType; }

			/*
				UILabel : setRenderType - Set the Render Type that the Label is using to Render text
				Created: 14/10/2017
				Modified: 14/10/2017

				param[in] pType - An ETextRenderType value defining the new Render Type
			*/
			void UILabel::setRenderType(const Rendering::ETextRenderType& pType) { mData->renderType = pType; }

			/*
				UILabel : Constructor - Initialise with default values
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			UILabel::UILabel() : IUIBase(EUIType::Label), mData(nullptr) {}

			/*
				UILabel : createUI - Allocate memory for the Label
				Created: 14/10/2017
				Modified: 14/10/2017

				return bool - Returns true if setup was successful
			*/
			bool UILabel::createUI() { mData = new LabelInternalData();	return true; }

			/*
				UILabel : destroyUI - Deallocate memory used for the Label
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			void UILabel::destroyUI() { if (mData) delete mData; mData = nullptr; }

			/*
				UILabel : render - Function to facilitate the the rendering of images once per cycle
				Created: 14/10/2017
				Modified: 14/10/2017
			*/
			void UILabel::render() {
				//Check there is a font and text to render
				if (mData->text.length() && mData->font) {
					//Get the renderer
					Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

					//Render the text
					rend.drawText(mData->text.c_str(), mData->font, *(SDL_Point*)&mLocation, mData->colour, mData->alignment, mData->renderType);
				}
			}
		}
	}
}
