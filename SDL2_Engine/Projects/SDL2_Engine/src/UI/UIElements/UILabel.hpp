#pragma once

//! Include SDL2_Engine objects
#include "Interfaces/IUIBase.hpp"
#include "../../Rendering/RenderingValues.hpp"
#include "../../Rendering/Colour.hpp"

//! Prototype the SDL2 font
struct _TTF_Font;

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UILabel
			 *		Author: Mitchell Croft
			 *		Created: 14/10/2017
			 *		Modified: 14/10/2017
			 *		
			 *		Purpose:
			 *		A simple object used to position and display text
			**/
			class SDL2_LIB_INC UILabel : public IUIBase {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------Data Accessors------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				
				/*
					UILabel : getText - Get the text that the Label is displaying
					Created: 14/10/2017
					Modified: 14/10/2017

					return const char* - Returns a constant pointer to the text
				*/
				const char* getText() const;

				/*
					UILabel : setText - Set the text that the Label is displaying
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pTxt - A C-String to copy the data of
				*/
				void setText(const char* pTxt);

				/*
					UILabel : getFont - Get the SDL Font that the Label is using to Render
					Created: 14/10/2017
					Modified: 14/10/2017

					return _TTF_Font* - Returns a pointer to the SDL Font
				*/
				_TTF_Font* getFont() const;

				/*
					UILabel : setFont - Set the SDL Font that the Label is using to Render
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pFont - A pointer to a _TTF_Font object to use
				*/
				void setFont(_TTF_Font* pFont);

				/*
					UIPanel : getColour - Get the Colour that the Label is using to render text
					Created: 14/10/2017
					Modified: 14/10/2017

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				const Rendering::Colour& getColour() const;

				/*
					UILabel : setColour - Set the Colour that the Label text should be rendered in
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pCol - A Colour object holding the new colour values
				*/
				void setColour(const Rendering::Colour& pCol);

				/*
					UILabel : getAlignment - Get the Alignment that the Label is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					return const ETextAlignment& - Returns a constant reference to the internal alignment value
				*/
				const Rendering::ETextAlignment& getAlignment() const;

				/*
					UILabel : setAlignment - Set the Alignment that the Label is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pAlign - An ETextAlignment value defining the new alignment
				*/
				void setAlignment(const Rendering::ETextAlignment& pAlign);

				/*
					UILabel : getRenderType - Get the Render Type that the Label is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					return const ETextRenderType& Returns a constant reference to the internal Render Type value
				*/
				const Rendering::ETextRenderType& getRenderType() const;

				/*
					UILabel : setRenderType - Set the Render Type that the Label is using to Render text
					Created: 14/10/2017
					Modified: 14/10/2017

					param[in] pType - An ETextRenderType value defining the new Render Type
				*/
				void setRenderType(const Rendering::ETextRenderType& pType);

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UILabel : Constructor - Initialise with default values
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				UILabel();

				/*
					UILabel : createUI - Allocate memory for the Label
					Created: 14/10/2017
					Modified: 14/10/2017

					return bool - Returns true if setup was successful
				*/
				bool createUI() override;

				/*
					UILabel : destroyUI - Deallocate memory used for the Label
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				void destroyUI() override;

				/*
					UILabel : render - Function to facilitate the the rendering of images once per cycle
					Created: 14/10/2017
					Modified: 14/10/2017
				*/
				void render() override;

			private:
				//! Define the internal protected elements for the UILabel
				struct LabelInternalData;
				LabelInternalData* mData;
			};
		}
	}
}