#pragma once

//! Include SDL2_Engine objects
#include "Interfaces/IUIBase.hpp"
#include "../../Rendering/Colour.hpp"

//! Prototype the SDL_Texture
struct SDL_Texture;

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
			 *		Name: UIPanel
			 *		Author: Mitchell Croft
			 *		Created: 13/10/2017
			 *		Modified: 13/10/2017
			 *
			 *		Purpose:
			 *		A simple display utility for a solid colour or texture
			 *		
			 *		Notes:
			 *		This UI component can be initialised by default through the Canvas.loadCanvasFromObjx function.
			 *		Settable values are:
			 *		
			 *		fillColour - A string value defining the hexadecimal colour value to fill the panel in (Eg. "FF0000FF" for red)
			 *		filterColour - A string value defining the hexadecimal colour value to tint the displayed image (Eg. "00FF00FF" for green)
			 *		borderColour - A string value defining the hexadecimal colour value to outline the Panel (Eg. "0000FFFF" for blue)
			 *		image - A string defining the relative filepath to the texture to load (Eg. "./imgs/example.png")
			**/
			class SDL2_LIB_INC UIPanel : public IUIBase {
			public:
				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-----------------------------------Data Accessors------------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UIPanel : getFillColour - Get the Colour object that is being used to fill in the panel
					Created: 13/10/2017
					Modified: 13/10/2017

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				inline const Rendering::Colour& getFillColour() const { return mFillColour; }

				/*
					UIPanel : setFillColour - Set the Colour that is being used to fill in the panel
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pCol - A Colour object holding the new colour values
				*/
				inline void setFillColour(const Rendering::Colour& pCol) { mFillColour = pCol; }

				/*
					UIPanel : getFilterColour - Get the Colour object that is being used to filter the image
					Created: 13/10/2017
					Modified: 13/10/2017

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				inline const Rendering::Colour& getFilterColour() const { return mFilterColour; }

				/*
					UIPanel : setFilterColour - Set the Colour object that is being used to colour filter the image
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pCol - A Colour object holding the new colour values
				*/
				inline void setFilterColour(const Rendering::Colour& pCol) { mFilterColour = pCol; }
				
				/*
					UIPanel : getBorderColour - Get the Colour object that is being used to outline the Panel
					Created: 13/10/2017
					Modified: 13/10/2017

					return const Colour& - Returns a constant reference to the internal Colour object
				*/
				inline const Rendering::Colour& getBorderColour() const { return mBorderColour; }

				/*
					UIPanel : setBorderColour - Set the Colour object that is being used to outline the Panel
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pCol - A Colour object holding the new colour values
				*/
				inline void setBorderColour(const Rendering::Colour& pCol) { mBorderColour = pCol; }

				/*
					UIPanel : getImage - Get the image the UIPanel is rendering
					Created: 13/10/2017
					Modified: 13/10/2017

					return SDL_Texture* - Returns a pointer to the current SDL_Texture
				*/
				inline SDL_Texture* getImage() const { return mImage; }

				/*
					UIPanel : setImage - Set the image the UIPanel is rendering
					Created: 13/10/2017
					Modified: 13/10/2017

					param[in] pImg - A pointer to the SDL_Texture object to use
				*/
				inline void setImage(SDL_Texture* pImg) { mImage = pImg; }

				/////////////////////////////////////////////////////////////////////////////////////////////////////
				////////-------------------------------Implementable Interface-------------------------------////////
				/////////////////////////////////////////////////////////////////////////////////////////////////////

				/*
					UIPanel : Constructor - Initialise with default values
					Created: 13/10/2017
					Modified: 13/10/2017
				*/
				inline UIPanel() : IUIBase(EUIType::Panel), mFillColour(Rendering::Colour::White), mBorderColour(Rendering::Colour::Black), mImage(nullptr) {}

				/*
					UIPanel : render - Function to facilitate the the rendering of images once per cycle
					Created: 13/10/2017
					Modified: 16/10/2017
				*/
				void render() override;

			private:
				//! Store a colour to fill in with/filter image
				union { Rendering::Colour mFillColour, mFilterColour; };

				//! Store a colour to outline the border in
				Rendering::Colour mBorderColour;

				//! Store a pointer to the (optional) image to display
				SDL_Texture* mImage;
			};
		}
	}
}
