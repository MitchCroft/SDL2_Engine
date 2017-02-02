#pragma once

namespace SDL2_Engine {
	namespace Resources {
		//! Store enumeration values for tracking the status of a SDL2Resource 
		enum class EResourceLoadStatus {
			ERROR = -1,
			Unloaded,
			Loaded,
			Freed,
		};

		//! Store enumeration values for tracking the type of resource that is currently in use
		enum class EResourceType {
			Null = -1,
			Texture,
			SFX,
			Music,
			Cursor,
			Font,
			Generic
		};

		//! Define a type used for storing the resource ID
		typedef unsigned int resourceID;
	}
}