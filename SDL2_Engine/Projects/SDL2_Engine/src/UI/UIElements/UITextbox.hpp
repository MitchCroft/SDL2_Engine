#pragma once

//! Include SDL2_Engine objects
#include "Interfaces/IUIBase.hpp"
#include "Interfaces/IUIAction.hpp"
#include "../../Rendering/RenderingValues.hpp"
#include "../../Rendering/Colour.hpp"
#include "../../Input/Keyboard/KeyboardValues.hpp"
#include "../../Utilities/Bitmask.hpp"

//! Prototype the SDL2 font
struct _TTF_Font;

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UITextbox
			 *		Author: Mitchell Croft
			 *		Created: 15/10/2017
			 *		Modified: 15/10/2017
			 *		
			 *		Purpose:
			 *		Provide a visual element that the user can use to enter custom
			 *		text information
			 *		
			 *		Notes:
			 *		This UI component can be initialised by default through the Canvas.loadCanvasFromObjx function.
			 *		Settable values are:
			 *		
			 *		text - A string value defining the text to display on screen
			 *		font - A string defining the relative filepath to the true type font to load (Eg. "./fonts/arial.ttf")
			 *		fontSize - An integer value defining the size of font to use 
			 *		fontIndex - An integer value defining the index of the font to use
			 *		maxLength - An integer value defining the maximum length of the text that can be entered into the textbox. Values < 0 remove the limit
			 *		inputFlags - An array of string values defining the character inclusion flags ("alphabetical", "numerical", "special", "space", "alphanumerical", "all")
			 *		textColours - An array of string values defining the hexadecimal colour value to colour the text in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		fillColours - An array of string values defining the hexadecimal colour value to fill in the background in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		borderColours - An array of string values defining the hexadecimal colour value to outline the button in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		renderType - A string defining the method used to render the text ("blended", "solid")
			**/
			class SDL2_LIB_INC UITextbox : public IUIBase, public IUIAction {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------Data Accessors------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UITextbox : getText - Get the text that the Textbox is displaying
					Created: 15/10/2017
					Modified: 15/10/2017

					return const char* - Returns a constant pointer to the text
				*/
				const char* getText() const;

				/*
					UITextbox : setText - Set the text that the Textbox is displaying
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pTxt - A C-String to copy the data of
				*/
				void setText(const char* pTxt);

				/*
					UITextbox : getFont - Get the SDL Font that the Textbox is using to Render
					Created: 15/10/2017
					Modified: 15/10/2017

					return _TTF_Font* - Returns a pointer to the SDL Font
				*/
				_TTF_Font* getFont() const;

				/*
					UITextbox : setFont - Set the SDL Font that the Textbox is using to Render
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pFont - A pointer to a _TTF_Font object to use
				*/
				void setFont(_TTF_Font* pFont);

				/*
					UITextbox : getMaxLength - Get the maximum length of the text allowed
					Created: 15/10/2017
					Modified: 15/10/2017

					return const int& - Returns a constant reference to the internal value (<0 means no limit)
				*/
				const int& getMaxLength() const;

				/*
					UITextbox : setMaxLength - Set the maximum length of the text allowed
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pLength - The new maximum length value of the Textbox (<0 means no limit)
				*/
				void setMaxLength(const int& pLength);

				/*
					UITextbox : getInputFlags - Get the flags used to indicate the allowed characters
					Created: 15/10/2017
					Modified: 15/10/2017

					return const Bitmask<EKeyboardInputFlags>& - Returns a constant reference to the internal bitmask
				*/
				const Utilities::Bitmask<Input::EKeyboardInputFlags>& getInputFlags() const;

				/*
					UITextbox : setInputFlags - Set the flags used to indicate the allowed characters
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pFlags - A bitmask of EKeyboardInputFlag values indicating the new settings
				*/
				void setInputFlags(const Utilities::Bitmask<Input::EKeyboardInputFlags>& pFlags);

				/*
					UITextbox : getTextColour - Get the Colour that the Textbox is using to render text per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getTextColour(const EActionState& pState) const;

				/*
					UITextbox : setTextColour - Set the Colour that the Textbox text should be rendered in per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/
				void setTextColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UITextbox : getFillColour - Get the Colour that the Textbox is using for a fill colour per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getFillColour(const EActionState& pState) const;

				/*
					UITextbox : setFillColour - Set the Colour that the Textbox is using for fill colour per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/
				void setFillColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UITextbox : getBorderColour - Get the Colour that the Textbox is using for a border colour per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getBorderColour(const EActionState& pState) const;

				/*
					UITextbox : setBorderColour - Set the Colour that the Textbox is using for a border colour per state
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/
				void setBorderColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UITextbox : getRenderType - Get the Render Type that the Textbox is using to Render text
					Created: 15/10/2017
					Modified: 15/10/2017

					return const ETextRenderType& Returns a constant reference to the internal Render Type value
				*/
				const Rendering::ETextRenderType& getRenderType() const;

				/*
					UITextbox : setRenderType - Set the Render Type that the Textbox is using to Render text
					Created: 15/10/2017
					Modified: 15/10/2017

					param[in] pType - An ETextRenderType value defining the new Render Type
				*/
				void setRenderType(const Rendering::ETextRenderType& pType);

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UITextbox : Constructor - Initialise with default values
					Created: 15/10/2017
					Modified: 15/10/2017
				*/
				UITextbox();

				/*
					UITextbox : createUI - Allocate memory for the Textbox
					Created: 15/10/2017
					Modified: 15/10/2017

					return bool - Returns true if setup was successful
				*/
				bool createUI() override;

				/*
					UITextbox : destroyUI - Deallocate memory used for the Textbox
					Created: 15/10/2017
					Modified: 15/10/2017
				*/
				void destroyUI() override;

				/*
					UITextbox : update - Update the internal values in response to keyboard input
					Created: 15/10/2017
					Modified: 15/10/2017
				*/
				void update() override;

				/*
					UITextbox : render - Function to facilitate the the rendering of images once per cycle
					Created: 15/10/2017
					Modified: 15/10/2017
				*/
				void render() override;

			private:
				//! Define the internal protected elements for the UITextbox
				struct TextboxInternalData;
				TextboxInternalData* mData;
			};
		}
	}
}