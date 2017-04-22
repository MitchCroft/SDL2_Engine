#pragma once

//! Include the standard objects
#include <memory>
#include <string>

//! Include the SDL objects
#include <SDL_rect.h>
#include <SDL_render.h>

//! Include the Color object
#include "Color.hpp"

namespace SDL2_Engine {
	//! Prototype the required Resources objects
	namespace Resources { 
		//! Prototype the Resource Types
		struct Texture; struct Font; 

		//! Prototype the SDL2Resource object
		namespace ResourceTypes { template<typename T> class __SDL2Resource; }

		//! Create an alias for the Resources
		template<class T> using SDL2Resource = std::shared_ptr<ResourceTypes::__SDL2Resource<T>>;
	}

	namespace Rendering {
		//! Create an enum to classify the types of RenderSets
		enum class ERenderSetType { Texture, String };

		/*
		 *		Name: RenderSet
		 *		Author: Mitchell Croft
		 *		Created: 08/03/2017
		 *		Modified: 15/03/2017
		 *		
		 *		Purpose:
		 *		Store common rendering information common to both
		 *		Textures and Strings
		**/
		struct RenderSet {
			//! Store the type of the RenderSet
			const ERenderSetType type;

			//! Store the source and destination rectangles
			SDL_Rect destRect;
			SDL_Rect srcRect;

			//! Store the color filter to apply to the image
			Color filter;

			//! Store the angle with which to rotate the image (Degrees)
			double angle;			

			//! Store the center point of the image (Used for rotation)
			SDL_Point center;

			//! Store any image flipping instructions
			SDL_RendererFlip flip;

			//! Store a simple string tag for the Set
			std::string tag;

			//! Construction / Destruction
			inline RenderSet(ERenderSetType pType, const SDL_Rect& pDest, const SDL_Rect& pSrcRect,
				const double& pAngle, const Color& pFilter, const SDL_Point& pCenter, 
				const SDL_RendererFlip& pFlip, const char* pTag) :
				type(pType), destRect(pDest), srcRect(pSrcRect), angle(pAngle), filter(pFilter), center(pCenter), flip(pFlip), tag(pTag) {}
			inline virtual ~RenderSet() = default;
		};

		

		/*
		 *		Name: RenderSetTexture 
		 *		Author: Mitchell Croft
		 *		Created: 08/03/2017
		 *		Modified: 15/03/2017
		 *		
		 *		Purpose:
		 *		Store the SDL2Resource<Texture> to be rendered to
		 *		the render target
		**/
		struct RenderSetTexture : public RenderSet {
			//! Store the texture to be rendered
			Resources::SDL2Resource<Resources::Texture> texture;

			//! Construction / Destruction
			inline RenderSetTexture(const Resources::SDL2Resource<Resources::Texture>& pTex, const SDL_Rect& pDest, const SDL_Rect& pSrcRect,
				const double& pAngle, const Color& pFilter, const SDL_Point& pCenter, const SDL_RendererFlip& pFlip, const char* pTag) : 
				texture(pTex), RenderSet(ERenderSetType::Texture, pDest, pSrcRect, pAngle, pFilter, pCenter, pFlip, pTag) 
			{}
			inline ~RenderSetTexture() override = default;
		};

		/*
		 *		Name: RenderSetString 
		 *		Author: Mitchell Croft
		 *		Created: 08/03/2017
		 *		Modified: 15/03/2017
		 *		
		 *		Purpose:
		 *		Store the string and font to be used to render to
		 *		the render target
		**/
		struct RenderSetString : public RenderSet {
			//! Store the Font to render the string with
			Resources::SDL2Resource<Resources::Font> font;

			//! Store the string to render
			std::string string;

			//! Construction / Destruction
			inline RenderSetString(const char* pText, const Resources::SDL2Resource<Resources::Font>& pString, const SDL_Rect& pDest, const SDL_Rect& pSrcRect,
				const double& pAngle, const Color& pFilter, const SDL_Point& pCenter, const SDL_RendererFlip& pFlip, const char* pTag) :
				font(pString), RenderSet(ERenderSetType::String, pDest, pSrcRect, pAngle, pFilter, pCenter, pFlip, pTag)
			{}
			inline ~RenderSetString() override = default;
		};
	}
}