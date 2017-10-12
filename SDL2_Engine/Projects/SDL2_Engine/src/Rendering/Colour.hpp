#pragma once

#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace Rendering {
		//! Type defines for 32-bit colour data
		typedef unsigned char colourChannel;
		typedef unsigned int  colourID;


		/*
		 *		Name: Colour
		 *		Author: Mitchell Croft
		 *		Created: 09/10/2017
		 *		Modified: 12/10/2017
		 *		
		 *		Purpose:
		 *		Provide a method for storing simple 32-bit colour values and a selection
		 *		of pre-defined colour values
		**/
		struct SDL2_LIB_INC Colour {
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------Colour Information Values-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//! Keep contained values in sync
			union {
				//! Name individual colour channels
				struct { colourChannel a, b, g, r; };

				//! Store the entire colour value as a single value
				colourID ID;

				//! Provide array access to colour values
				colourChannel array[sizeof(colourID)];
			};

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------Pre-Defined Colours--------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			enum : colourID {
				//! Format:				= 0xRRGGBBAA
				Transparent				= 0x00000000,
				TransparentBlack		= 0x0000007E,
				TransparentWhite		= 0xFFFFFF7E,
				AliceBlue				= 0xF0F8FFFF,
				AntiqueWhite			= 0xFAEBD7FF,
				Aqua					= 0x00FFFFFF,
				Aquamarine				= 0x7FFFD4FF,
				Azure					= 0xF0FFFFFF,
				Beige					= 0xF5F5DCFF,
				Bisque					= 0xFFE4C4FF,
				Black					= 0x000000FF,
				BlanchedAlmond			= 0xFFEBCDFF,
				Blue					= 0x0000ffFF,
				BlueViolet				= 0x8A2BE2FF,
				Brown					= 0xA52A2AFF,
				BurlyWood				= 0xDEB887FF,
				CadetBlue				= 0x5F9EA0FF,
				Chartreuse				= 0x7FFF00FF,
				Chocolate				= 0xD2691EFF,
				Coral					= 0xFF7F50FF,
				CornflowerBlue			= 0x6495EDFF,
				Cornsilk				= 0xFFF8DCFF,
				Crimson					= 0xDC143CFF,
				Cyan					= 0x00FFFFFF,
				DarkBlue				= 0x00008BFF,
				DarkCyan				= 0x008B8BFF,
				DarkGoldenrod			= 0xB8860BFF,
				DarkGray				= 0xA9A9A9FF,
				DarkGreen				= 0x006400FF,
				DarkKhaki				= 0xBDB76BFF,
				DarkMagenta				= 0x8B008BFF,
				DarkOliveGreen			= 0x556B2FFF,
				DarkOrange				= 0xFF8C00FF,
				DarkOrchid				= 0x9932CCFF,
				DarkRed					= 0x8B0000FF,
				DarkSalmon				= 0xE9967AFF,
				DarkSeaGreen			= 0x8FBC8BFF,
				DarkSlateBlue			= 0x483D8BFF,
				DarkSlateGray			= 0x2F4F4FFF,
				DarkTurquoise			= 0x00CED1FF,
				DarkViolet				= 0x9400D3FF,
				DeepPink				= 0xFF1493FF,
				DeepSkyBlue				= 0x00BFFFFF,
				DimGray					= 0x696969FF,
				DodgerBlue				= 0x1E90FFFF,
				Firebrick				= 0xB22222FF,
				FloralWhite				= 0xFFFAF0FF,
				ForestGreen				= 0x228B22FF,
				Fuchsia					= 0xFF00FFFF,
				Gainsboro				= 0xDCDCDCFF,
				GhostWhite				= 0xF8F8FFFF,
				Gold					= 0xFFD700FF,
				Goldenrod				= 0xDAA520FF,
				Gray					= 0x808080FF,
				Green					= 0x008000FF,
				GreenYellow				= 0xADFF2FFF,
				HoneyDew				= 0xF0FFF0FF,
				HotPink					= 0xFF69B4FF,
				IndianRed				= 0xCD5C5CFF,
				Indigo					= 0x4B0082FF,
				Ivory					= 0xFFFFF0FF,
				Khaki					= 0xF0E68CFF,
				Lavender				= 0xE6E6FAFF,
				LavenderBush			= 0xFFF0F5FF,
				LawnGreen				= 0x7CFC00FF,
				LemonChiffon			= 0xFFFACDFF,
				LightBlue				= 0xADD8E6FF,
				LightCoral				= 0xF08080FF,
				LightCyan				= 0xE0FFFFFF,
				LightGoldenrodYellow	= 0xFAFAD2FF,
				LightGreen				= 0x90EE90FF,
				LightGray				= 0xD3D3D3FF,
				LightPink				= 0xFFB6C1FF,
				LightSalmon				= 0xFFA07AFF,
				LightSeaGreen			= 0x20B2AAFF,
				LightSkyBlue			= 0x87CEFAFF,
				LightSlateGray			= 0x778899FF,
				LightSteelBlue			= 0xB0C4DEFF,
				LightYellow				= 0xFFFFE0FF,
				Lime					= 0x00FF00FF,
				LimeGreen				= 0x32CD32FF,
				Linen					= 0xFAF0E6FF,
				Magenta					= 0xFF00FFFF,
				Maroon					= 0x800000FF,
				MediumAquamarine		= 0x66CDAAFF,
				MediumBlue				= 0x0000CDFF,
				MediumOrchid			= 0xBA55D3FF,
				MediumPurple			= 0x9370DBFF,
				MediumSeaGreen			= 0x3CB371FF,
				MediumSlateBlue			= 0x7B68EEFF,
				MediumSpringGreen		= 0x00FA9AFF,
				MediumTurquoise			= 0x48D1CCFF,
				MediumVioletRed			= 0xC71585FF,
				MidnightBlue			= 0x191970FF,
				MintCream				= 0xF5FFFAFF,
				MistyRose				= 0xFFE4E1FF,
				Moccasin				= 0xFFE4B5FF,
				NavajoWhite				= 0xFFDEADFF,
				Navy					= 0x000080FF,
				OldLace					= 0xFDF5E6FF,
				Olive					= 0x808000FF,
				OliveDrab				= 0x6B8E23FF,
				Orange					= 0xFFA500FF,
				OrangeRed				= 0xFF4500FF,
				Orchid					= 0xDA70D6FF,
				PaleGoldenrod			= 0xEEE8AAFF,
				PaleGreen				= 0x98FB98FF,
				PaleTurquoise			= 0xAFEEEEFF,
				PaleVioletRed			= 0xDB7093FF,
				PapayaWhip				= 0xFFEFD5FF,
				PeachPuff				= 0xFFDAB9FF,
				Peru					= 0xCD853FFF,
				Pink					= 0xFFC0CBFF,
				Plum					= 0xDDA0DDFF,
				PowderBlue				= 0xB0E0E6FF,
				Purple					= 0x800080FF,
				Red						= 0xFF0000FF,
				RosyBrown				= 0xBC8F8FFF,
				RoyalBlue				= 0x4169E1FF,
				SaddleBrown				= 0x8B4513FF,
				Salmon					= 0xFA8072FF,
				SandyBrown				= 0xF4A460FF,
				SeaGreen				= 0x2E8B57FF,
				SeaShell				= 0xFFF5EEFF,
				Sienna					= 0xA0522DFF,
				Silver					= 0xC0C0C0FF,
				SkyBlue					= 0x87CEEBFF,
				SlateBlue				= 0x6A5ACDFF,
				SlateGray				= 0x708090FF,
				Snow					= 0xFFFAFAFF,
				SpringGreen				= 0x00FF7FFF,
				SteelBlue				= 0x4682B4FF,
				Tan						= 0xD2B48CFF,
				Teal					= 0x008080FF,
				Thistle					= 0xD8BFD8FF,
				Tomato					= 0xFF6347FF,
				Turquoise				= 0x40E0D0FF,
				Violet					= 0xEE82EEFF,
				Wheat					= 0xF5DEB3FF,
				White					= 0xFFFFFFFF,
				WhiteSmoke				= 0xF5F5F5FF,
				Yellow					= 0xFFFF00FF,
				YellowGreen				= 0x9ACD32FF
			};

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Construction------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Colour : Default Constructor - Initialise with default values
				Created: 09/10/2017
				Modified: 09/10/2017
			*/
			inline Colour() : r(0), g(0), b(0), a(0xFF) {}

			/*
				Colour : Custom Constructor (Individual) - Initialise the Colour object with specified colour channel values
				Created: 09/10/2017
				Modified: 09/10/2017

				Template R - The type of the R channel value
				Template G - The type of the G channel value
				Template B - The type of the B channel value
				Template A - The type of the A channel value

				param[in] pR - The value for the R colour channel
				param[in] pG - The value for the G colour channel
				param[in] pB - The value for the B colour channel
				param[in] pA - The value for the A colour channel
			*/
			template<typename R, typename G, typename B, typename A>
			inline Colour(const R& pR = (R)0, const G& pG = (G)0, const B& pB = (B)0, const A& pA = (A)0xFF) : r((colourChannel)pR), g((colourChannel)pG), b((colourChannel)pB), a((colourChannel)pA) {}

			/*
				Colour : Custom Constructor (ID) - Initialise the Colour object with a specific colour ID value
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pID - A colourID value defining the colour ID to assign to this object
			*/
			inline Colour(const colourID& pID) : ID(0) { *this = pID; }

			/*
				Colour : Copy Constructor - Initialise the Colour object as a copy of another Colour object
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pCopy - The Colour object to copy the values of
			*/
			inline Colour(const Colour& pCopy) : ID(pCopy.ID) {}

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Functions--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Colour : lerp - Linearly interpolate the values of the current towards a second
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pSecond - The Colour object that marks the 'end' goal of the interpolation
				param[in] pT - A 0-1 scale indicating the progress of the interpolation

				return Colour - Returns a Colour object with the interpolated values
			*/
			Colour lerp(const Colour& pSecond, const float& pT) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------------Operators--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Colour : Assignment Operator (ID) - Assign the current object a new Colour ID value
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pID - A colourID value defining the colour ID to assign to this object

				return Colour& - Returns a reference to itself
			*/
			Colour& operator=(const colourID& pID);

			/*
				Colour : Assignment Operator (Copy) - Assign the current object the value of another Colour object
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pCopy - The Colour object to copy the values of

				return Colour& - Returns a reference to itself
			*/
			inline Colour& operator=(const Colour& pCopy) { ID = pCopy.ID; return *this; }

			/*
				Colour : Subscript Operator - Provide subscript operator access to the array values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pIndex - The desired index of the colour channel to retrieve

				return colourChannel& - Returns a reference to the colourChannel value
			*/
			inline colourChannel& operator[](const size_t& pIndex) { return array[pIndex]; }

			/*
				Colour : Subscript Operator (const) - Provide const subscript operator access to the array values
				Created: 09/10/2017
				Modified: 09/10/2017

				param[in] pIndex - The desired index of the colour channel to retrieve

				return const colourChannel& - Returns a constant reference to the colourChannel value
			*/
			inline const colourChannel& operator[](const size_t& pIndex) const { return array[pIndex]; }

			/*
				Colour : Implicit Cast (Bool) - Convert the Colour to a bool indicating if it should be used to render
				Created: 09/10/2017
				Modified: 09/10/2017

				return bool - Returns true if the alpha channel is not 0
			*/
			inline operator bool() const { return a != 0; }
		};

		//! Create a type define for Color for ease
		typedef Colour Color;
	}
}