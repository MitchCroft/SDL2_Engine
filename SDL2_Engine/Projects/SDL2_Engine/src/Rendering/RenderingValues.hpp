#pragma once

namespace SDL2_Engine {
	namespace Rendering {
		//! The flags used to indicate how the Renderer should be setup
		enum class ERendererProperties {
			Software_Fallback		= 1 << 0,
			Hardware_Acceleration	= 1 << 1,
			VSync					= 1 << 2,
			Target_Texture			= 1 << 3
		};

		//! Flag rendering position information for the SceneRenderers
		enum class ETextAlignment { Null, Left, Center, Right };

		//! Flag text rendering options 
		enum class ETextRenderType { Solid, Blended };
	}
}