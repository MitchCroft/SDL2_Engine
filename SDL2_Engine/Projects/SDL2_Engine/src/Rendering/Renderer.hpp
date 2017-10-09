#pragma once

//! Include the SDL2_Engine objects
#include "../Utilities/IGlobal.hpp"
#include "ISceneRenderer.hpp"

//! Prototype the SDL objects
struct SDL_Window;

namespace SDL2_Engine {
	//! Prototype the initialiser
	namespace Initialisation { struct RendererInitialiser; }

	namespace Rendering {
		/*
		 *		Name: Renderer
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 09/10/2017
		 *		
		 *		Purpose:
		 *		Provide a uniform interface for rendering images to the screen 
		 *		while allowing customisable rendering options
		**/
		class SDL2_LIB_INC Renderer : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Rendering Functionality------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Renderer : drawTexture - Render a texture using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pTexture - A pointer to the SDL_Texture object to render
				param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
				param[in] pClip - A pointer to an SDL_Rect defining a section of the texture to render (Default nullptr)
				param[in] pFilter - A Colour object defining the filter to be applied to the rendered image (Default White)
				param[in] pAngle - A double value defining the rotation amount in degrees (Default 0)
				param[in] pPivot - A pointer to an SDL_Point object defining the pivot point for the rotation (Default nullptr)
				param[in] pFlip - The SDL_RendererFlip value defining if the image should be flipped in any way (Default SDL_FLIP_NONE)
			*/
			void drawTexture(SDL_Texture* pTexture,
							 const SDL_Rect& pPosition,
							 const SDL_Rect* pClip = nullptr,
							 const Colour& pFilter = Colour::White,
							 const double& pAngle = 0.0,
							 const SDL_Point* pPivot = nullptr,
							 const int& pFlip = 0);

			/*
				Renderer : drawLine - Render a line using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pFrom - An SDL_Point defining the starting position of the line
				param[in] pTo - An SDL_Point defining the end position of the line
				param[in] pColour - A Colour object defining the colour to draw the line in (Default Black)
			*/
			void drawLine(const SDL_Point& pFrom,
						  const SDL_Point& pTo,
						  const Colour& pColour = Colour::Black);

			/*
				Renderer : drawRect - Render a uniform area using passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pPosition - An SDL_Rect object defining where the image is to be rendered
				param[in] pColour - A Colour object defining the colour to render the outline in or render the fill if pFill is true
				param[in] pFill - A boolean flag indicating if the Rectangle should be filled in (Default false)
				param[in] pBorderColour - A pointer to a Colour object to render the outline of the area if pFill is true (Default nullptr)
			*/
			void drawRect(const SDL_Rect& pPosition,
						  const Colour& pColour,
						  const bool& pFill = false,
						  const Colour* pBorderColour = nullptr);

			/*
				Renderer : drawText - Render text using the passed in values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pText - A c-string defining the text to render
				param[in] pFont - An SDL_Font to be used to render the Text
				param[in] pPosition - An SDL_Point defining where the text should be rendered
				param[in] pColour - A Colour object defining what colour the text should rendered in (Default Black)
				param[in] pAlignment - An ETextAlignment value defining if the text should be aligned a specific way (Default Null)
				param[in] pRenderType - An ETextRenderType value defining how the text should be rendered (Default Solid)
			*/
			void drawText(const char* pText,
						  _TTF_Font* pFont,
						  const SDL_Rect& pPosition,
						  const Colour& pColour = Colour::Black,
						  const ETextAlignment& pAlignment = ETextAlignment::Null,
						  const ETextRenderType& pRenderType = ETextRenderType::Solid);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------Rendering Accessors--------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Renderer : getRenderer - Retrieve the SDL_Renderer object
				Created: 09/10/2017
				Modified: 09/10/2017

				return SDL_Renderer* - Returns a pointer to the SDL_Renderer object
			*/
			SDL_Renderer* getRenderer();

			/*
				Renderer : getSceneRenderer - Retrieve a pointer to the current Scene Renderer, cast to a specific type
				Created: 09/10/2017
				Modified: 09/10/2017

				Template T - The ISceneRenderer subclass to cast to

				return T* - Returns a T pointer to the Scene Renderer

				NOTE:
				The internal Scene Renderer object is not guaranteed to be of type 'T'. An invalid pointer could be returned.
				Be certain of the current Scene Renderers type before using this function
			*/
			template<typename T>
			inline T* getSceneRenderer() {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneRenderer, T>::value, "Can not convert the Renderer's Scene Renderer to an object that is not a subclass of ISceneRenderer");

				//Cast the SceneRenderer
				return (T*)returnSceneRenderer();
			}

			/*
				Renderer : setSceneRenderer - Set the SceneRenderer object that the Renderer will use 
				Created: 09/10/2017
				Modified: 09/10/2017

				Template T - The ISceneRenderer subclass object to use
				Template TArgs - A parameter pack of types use to setup the new SceneRenderer

				param[in] pArgs - A parameter pack of the values needed to setup the Scene Renderer

				return bool - Returns true if the ISceneRenderer object was successfully set
			*/
			template<typename T, typename ... TArgs>
			inline bool setSceneRenderer(const TArgs ... pArgs) {
				//Ensure the template is of the correct type
				static_assert(std::is_base_of<ISceneRenderer, T>::value, "Can not use a type that is not a subclass of ISceneRenderer as the Scene Renderer");

				//Create the new object
				return setSceneRenderer(new T(pArgs...));
			}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Renderer : Constructor - Initialise with default values
				Created: 05/10/2017
				Modified: 05/10/2017

				param[in] pWindow - A pointer to the SDL_Window object
				param[in] pSetup - Defines how the Renderer should be setup
			*/
			Renderer(SDL_Window* pWindow, const Initialisation::RendererInitialiser& pSetup);

			/*
				Renderer : createInterface - Verify and setup starting information
				Created: 05/10/2017
				Modified: 05/10/2017

				return bool - Returns true if the Resources Manager was setup correctly
			*/
			bool createInterface() override;

			/*
				Renderer : destroyInterface - Deallocate internal memory allocated
				Created: 05/10/2017
				Modified: 05/10/2017
			*/
			void destroyInterface() override;

			/*
				Renderer : update - Empty function
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			inline void update() override {}

		private:
			//! Define the internal protected elements for the Renderer
			struct RendererInternalData;
			RendererInternalData* mData;

			//! Functionality to retrieve and change SceneRenderers throughout execution
			ISceneRenderer* returnSceneRenderer();
			bool setSceneRenderer(ISceneRenderer* pRenderer);
		};
	}
}