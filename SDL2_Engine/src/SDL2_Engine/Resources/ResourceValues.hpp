#pragma once

namespace SDL2_Engine {
	namespace Resources {
		//! Store enumeration values for tracking the status of a SDL2Resource 
		enum class EResourceLoadStatus {
			Error = 1,
			Unloaded = 2,
			Loaded = 4,
			Freed = 8,
		};

		//! Store enumeration values for tracking the type of resource that is currently in use
		enum class EResourceType {
			Null = -1,
			Texture,
			SFX,
			Music,
			Cursor,
			Font,
			FontString,
			Generic
		};

		//! Define a type used for storing the resource ID
		typedef unsigned int resourceID;
	}
}