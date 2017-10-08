#pragma once

namespace SDL2_Engine {
	/*
	 *		Name: ResourceTypePrototypes
	 *		Author: Mitchell Croft
	 *		Created: 03/10/2017
	 *		Modified: 03/10/2017
	 *
	 *		Purpose:
	 *		Provide struct prototypes that can be fed to the Resource Manager
	 *		to describe the type of resource to be loaded
	**/
	struct Texture;
	struct SFX;
	struct Music;
	struct Cursor;
	struct Font;
	struct Generic;

	//! Define a type used for storing the resource ID
	typedef char resourceID;

	//! Store enumeration values for tracking the type of resource that is currently in use
	enum class EResourceType : resourceID {
		Null = -1,
		Texture,
		SFX,
		Music,
		Cursor,
		Font,
		Generic
	};

	//! Store enumeration values for tracking the status of a SDL2Resource object
	enum class EResourceLoadStatus : char {
		Error		= 1 << 0,
		Unloaded	= 1 << 1,
		Loaded		= 1 << 2,
		Freed		= 1 << 3
	};
}