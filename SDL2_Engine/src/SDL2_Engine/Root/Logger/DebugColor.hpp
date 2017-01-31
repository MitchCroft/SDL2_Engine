#pragma once

#include "../../Types.hpp"

namespace SDL2_Engine {
	/*
	*		Name: DebugColor
	*		Author: Mitchell Croft
	*		Created: 23/01/2017
	*		Modified: 23/01/2017
	*
	*		Purpose:
	*		Group and manage colors used for rendering Debug text
	*		to the output
	**/
	class DebugColor {
		//! Store the internal byte code for the color
		ubyte mCode;

	public:
		enum : ubyte {
			BLACK,
			DARK_BLUE,
			DARK_GREEN,
			DARK_CYAN,
			DARK_RED,
			DARK_MAGENTA,
			DARK_YELLOW,
			LIGHT_GRAY,
			GRAY,
			BLUE,
			GREEN,
			CYAN,
			RED,
			MAGENTA,
			YELLOW,
			WHITE,
			BLACK_FILL = 0,
			DARK_BLUE_FILL = 16,
			DARK_GREEN_FILL = 32,
			DARK_CYAN_FILL = 48,
			DARK_RED_FILL = 64,
			DARK_MAGENTA_FILL = 80,
			DARK_YELLOW_FILL = 96,
			LIGHT_GRAY_FILL = 112,
			GRAY_FILL = 128,
			BLUE_FILL = 144,
			GREEN_FILL = 160,
			CYAN_FILL = 176,
			RED_FILL = 192,
			MAGENTA_FILL = 208,
			YELLOW_FILL = 224,
			WHITE_FILL = 240
		};

		//! Constructor
		inline DebugColor(ubyte pCode = 0) : mCode(pCode) {}

		//! Explicit value retriever
		inline const ubyte& value() const { return mCode; }

		//! Assignment operator
		inline DebugColor& operator=(const DebugColor& pCopy) { mCode = pCopy.mCode; }

		//! Implicit conversion to ubyte
		inline const ubyte&	operator()() { return mCode; }
	};
}