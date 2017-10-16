#pragma once

//! Include SDL2_Engine objects
#include "Interfaces/IUIBase.hpp"
#include "Interfaces/IUIAction.hpp"
#include "../../Rendering/RenderingValues.hpp"
#include "../../Rendering/Colour.hpp"

//! Prototype the SDL2 font
struct _TTF_Font;

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UIButton
			 *		Author: Mitchell Croft
			 *		Created: 14/10/2017
			 *		Modified: 14/10/2017
			 *		
			 *		Purpose:
			 *		Provide a visual representation of a potential action
			 *		that a user can take
			 *		
			 *		Notes:
			 *		This UI component can be initialised by default through the Canvas.loadCanvasFromObjx function.
			 *		Settable values are:
			 *		
			 *		text - A string value defining the text to display on screen
			 *		font - A string defining the relative filepath to the true type font to load (Eg. "./fonts/arial.ttf")
			 *		fontSize - An integer value defining the size of font to use 
			 *		fontIndex - An integer value defining the index of the font to use
			 *		textColours - An array of string values defining the hexadecimal colour value to colour the text in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		fillColours - An array of string values defining the hexadecimal colour value to fill in the background in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		borderColours - An array of string values defining the hexadecimal colour value to outline the button in for each state [ "FF0000FF", "00FF00FF", "0000FFFF" ]
			 *		renderType - A string defining the method used to render the text ("blended", "solid")
			**/
			class SDL2_LIB_INC UIButton : public IUIBase, public IUIAction {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------Data Accessors------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UIButton : getText - Get the text that the Button is displaying
					Created: 14/10/2017
					Modified: 14/10/2017

					return const char* - Returns a constant pointer to the text
				*/
				const char* getText() const;

				/*
					UIButton : setText - Set the text that the Button is displaying
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pTxt - A C-String to copy the data of
				*/
				void setText(const char* pTxt);

				/*
					UIButton : getFont - Get the SDL Font that the Button is using to Render
					Created: 14/10/2017
					Modified: 14/10/2017

					return _TTF_Font* - Returns a pointer to the SDL Font
				*/
				_TTF_Font* getFont() const;

				/*
					UIButton : setFont - Set the SDL Font that the Button is using to Render
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pFont - A pointer to a _TTF_Font object to use
				*/
				void setFont(_TTF_Font* pFont);

				/*
					UIButton : getTextColour - Get the Colour that the Button is using to render text per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getTextColour(const EActionState& pState) const;

				/*
					UIButton : setTextColour - Set the Colour that the Button text should be rendered in per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/
				void setTextColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UIButton : getFillColour - Get the Colour that the Button is using for a fill colour per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getFillColour(const EActionState& pState) const;

				/*
					UIButton : setFillColour - Set the Colour that the Button is using for fill colour per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/
				void setFillColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UIButton : getBorderColour - Get the Colour that the Button is using for a border colour per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getBorderColour(const EActionState& pState) const;

				/*
					UIButton : setBorderColour - Set the Colour that the Button is using for a border colour per state
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pState - An EActionState defining the state to retrieve the Colour of
					param[in] pCol - A Colour object holding the new colour values
				*/	
				void setBorderColour(const EActionState& pState, const Rendering::Colour& pCol);

				/*
					UIButton : getRenderType - Get the Render Type that the Button is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					return const ETextRenderType& Returns a constant reference to the internal Render Type value
				*/
				const Rendering::ETextRenderType& getRenderType() const;

				/*
					UIButton : setRenderType - Set the Render Type that the Button is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pType - An ETextRenderType value defining the new Render Type
				*/
				void setRenderType(const Rendering::ETextRenderType& pType);

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UIButton : Constructor - Initialise with default values
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				UIButton();

				/*
					UIButton : createUI - Allocate memory for the Button
					Created: 14/10/2017
					Modified: 14/10/2017

					return bool - Returns true if setup was successful
				*/
				bool createUI() override;

				/*
					UIButton : destroyUI - Deallocate memory used for the Button
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				void destroyUI() override;

				/*
					UIButton : render - Function to facilitate the the rendering of images once per cycle
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				void render() override;

			private:
				//! Define the internal protected elements for the UIButton
				struct ButtonInternalData;
				ButtonInternalData* mData;
			};
		}
	}
}