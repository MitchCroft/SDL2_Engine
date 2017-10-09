#pragma once

//! Include the SDL2_Engine objects
#include "ISceneRenderer.hpp"

namespace SDL2_Engine {
	namespace Rendering {
		/*
		 *		Name: BasicSceneRenderer
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 09/10/2017
		 *		
		 *		Purpose:
		 *		Provide basic rendering functionality
		**/
		class SDL2_LIB_INC BasicSceneRenderer : public ISceneRenderer {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Rendering Functionality------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				BasicSceneRenderer : drawTexture - Render a texture using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
				param[in] pTexture - A pointer to the SDL_Texture object to render
				param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
				param[in] pClip - A pointer to an SDL_Rect defining a section of the texture to render (Default nullptr)
				param[in] pFilter - A Colour object defining the filter to be applied to the rendered image (Default White)
				param[in] pAngle - A double value defining the rotation amount in degrees (Default 0)
				param[in] pPivot - A pointer to an SDL_Point object defining the pivot point for the rotation (Default nullptr)
				param[in] pFlip - The SDL_RendererFlip value defining if the image should be flipped in any way (Default SDL_FLIP_NONE)
			*/
			virtual void drawTexture(SDL_Renderer*& pRenderer,
									 SDL_Texture*& pTexture,
									 const SDL_Rect& pPosition,
									 const SDL_Rect* pClip = nullptr,
									 const Colour& pFilter = Colour::White,
									 const double& pAngle = 0.0,
									 const SDL_Point* pPivot = nullptr,
									 const int& pFlip = 0) override;

			/*
				BasicSceneRenderer : drawLine - Render a line using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
				param[in] pFrom - An SDL_Point defining the starting position of the line
				param[in] pTo - An SDL_Point defining the end position of the line
				param[in] pColour - A Colour object defining the colour to draw the line in (Default Black)
			*/
			virtual void drawLine(SDL_Renderer*& pRenderer,
								  const SDL_Point& pFrom,
								  const SDL_Point& pTo,
								  const Colour& pColour = Colour::Black) override;

			/*
				BasicSceneRenderer : drawRect - Render a uniform area using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
				param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
				param[in] pColour - A Colour object defining the colour to render the outline in or render the fill if pFill is true
				param[in] pFill - A boolean flag indicating if the Rectangle should be filled in (Default false)
				param[in] pBorderColour - A pointer to a Colour object to render the outline of the area if pFill is true (Default nullptr)
			*/
			virtual void drawRect(SDL_Renderer*& pRenderer,
								  const SDL_Rect& pPosition,
								  const Colour& pColour,
								  const bool& pFill = false,
								  const Colour* pBorderColour = nullptr) override;

			/*
				BasicSceneRenderer : drawText - Render text using the passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pRenderer - The SDL Renderer object to be used to draw graphics to the screen
				param[in] pText - A c-string defining the text to render
				param[in] pFont - An SDL_Font to be used to render the Text
				param[in] pPosition - An SDL_Point defining where the text should be rendered
				param[in] pColour - A Colour object defining what colour the text should rendered in (Default Black)
				param[in] pAlignment - An ETextAlignment value defining if the text should be aligned a specific way (Default Null)
				param[in] pRenderType - An ETextRenderType value defining how the text should be rendered (Default Solid)
			*/
			virtual void drawText(SDL_Renderer*& pRenderer,
								  const char*& pText,
								  _TTF_Font*& pFont,
								  const SDL_Point& pPosition,
								  const Colour& pColour = Colour::Black,
								  const ETextAlignment& pAlignment = ETextAlignment::Null,
								  const ETextRenderType& pRenderType = ETextRenderType::Solid) override;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Construction/Destruction------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				BasicSceneRenderer : Default Constructor - Initialise with default values
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			BasicSceneRenderer();

			/*
				BasicSceneRenderer : createRenderer - Setup the Scene Renderer
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pRenderer - The SDL_Renderer object to allow for the creation of default textures

				return bool - Returns true if the Scene Renderer was created successfully
			*/
			virtual bool createRenderer(SDL_Renderer*& pRenderer) override;

			/*
				BasicSceneRenderer : destroyRenderer - Free memory and resources for the Scene Renderer
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			virtual void destroyRenderer() override;

		private:
			//! Store a blank texture to be used to render filled rectangles
			SDL_Texture* mBlank;
		};
	}
}