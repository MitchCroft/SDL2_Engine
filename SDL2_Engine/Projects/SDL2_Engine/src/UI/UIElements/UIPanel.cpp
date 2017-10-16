#include "UIPanel.hpp"

//! Include the SDL2_Engine objects
#include "../../Globals.hpp"
#include "../../Rendering/Renderer.hpp"

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
				UIPanel : render - Function to facilitate the the rendering of images once per cycle
				Created: 13/10/2017
				Modified: 16/10/2017
			*/
			void UIPanel::render() {
				//Get the renderer
				Rendering::Renderer& rend = Globals::get<Rendering::Renderer>();

				//Check if there is an image to draw
				if (mImage) {
					//Draw the texture
					rend.drawTexture(mImage, *(SDL_Rect*)&mLocation, nullptr, mFilterColour);

					//Outline the image
					rend.drawRect(*(SDL_Rect*)&mLocation, mBorderColour);
				}

				//Draw the panel
				else rend.drawRect(*(SDL_Rect*)&mLocation, mFillColour, true, &mBorderColour);
			}
		}
	}
}
