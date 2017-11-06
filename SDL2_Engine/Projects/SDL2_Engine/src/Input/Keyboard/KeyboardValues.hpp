#pragma once

namespace SDL2_Engine {
	namespace Input {

		//! List the flags that can be used when reading in string information from a keyboard
		enum class EKeyboardInputFlags : char {
			//! Standard classifications
			Alphabetical	= 1 << 0,
			Numerical		= 1 << 1,
			Special			= 1 << 2,
			Space			= 1 << 3,

			//! Shorthand combinations
			AlphaNumerical	= Alphabetical | Numerical,
			All				= Alphabetical | Numerical | Special | Space
		};

		//! List the different keyboard key codes that can be read from
		enum class EKeyboardKeyCode : unsigned int {
			//! Default
			Null,

			//! Alpha keys
			Space = 0x20,
			A = 0x41,
			B = 0x42,
			C = 0x43,
			D = 0x44,
			E = 0x45,
			F = 0x46,
			G = 0x47,
			H = 0x48,
			I = 0x49,
			J = 0x4A,
			K = 0x4B,
			L = 0x4C,
			M = 0x4D,
			N = 0x4E,
			O = 0x4F,
			P = 0x50,
			Q = 0x51,
			R = 0x52,
			S = 0x53,
			T = 0x54,
			U = 0x55,
			V = 0x56,
			W = 0x57,
			X = 0x58,
			Y = 0x59,
			Z = 0x5A,

			//! Numerical keys
			Num0 = 0x30,
			Num1 = 0x31,
			Num2 = 0x32,
			Num3 = 0x33,
			Num4 = 0x34,
			Num5 = 0x35,
			Num6 = 0x36,
			Num7 = 0x37,
			Num8 = 0x38,
			Num9 = 0x39,
			Num_Pad0 = 0x60,
			Num_Pad1 = 0x61,
			Num_Pad2 = 0x62,
			Num_Pad3 = 0x63,
			Num_Pad4 = 0x64,
			Num_Pad5 = 0x65,
			Num_Pad6 = 0x66,
			Num_Pad7 = 0x67,
			Num_Pad8 = 0x68,
			Num_Pad9 = 0x69,

			//! Math Operators
			Num_Pad_Multiply = 0x6A,
			Num_Pad_Subtract = 0x6D,
			Num_Pad_Decimal = 0x6E,
			Num_Pad_Divide = 0x6F,

			//! Misc. characters
			Semi_Colon = 0xBA,
			Plus = 0xBB,
			Comma = 0xBC,
			Minus = 0xBD,
			Period = 0xBE,
			Slash_Forward = 0xBF,
			Slash_Backward = 0xDC,
			Tilde = 0xC0,
			Square_Left = 0xDB,
			Square_Right = 0xDD,
			Quote = 0xDE,

			//! Function keys
			F1 = 0x70,
			F2 = 0x71,
			F3 = 0x72,
			F4 = 0x73,
			F5 = 0x74,
			F6 = 0x75,
			F7 = 0x76,
			F8 = 0x77,
			F9 = 0x78,
			F10 = 0x79,
			F11 = 0x7A,
			F12 = 0x7B,
			F13 = 0x7C,
			F14 = 0x7D,
			F15 = 0x7E,
			F16 = 0x7F,
			F17 = 0x80,
			F18 = 0x81,
			F19 = 0x82,
			F20 = 0x83,
			F21 = 0x84,
			F22 = 0x85,
			F23 = 0x86,
			F24 = 0x87,

			//! Arrow keys
			Arrow_Left = 0x25,
			Arrow_Up = 0x26,
			Arrow_Right = 0x27,
			Arrow_Down = 0x28,

			//! Command keys
			Escape = 0x1B,
			Backspace = 0x08,
			TAB = 0x09,
			Return = 0x0D,
			Shift = 0x10,
			Shift_Left = 0xA0,
			Shift_Right = 0xA1,
			Control = 0x11,
			Control_Left = 0xA2,
			Control_Right = 0xA3,
			Alt = 0x12,
			Alt_Left = 0xA4,
			Alt_Right = 0xA5,
			Windows_Left = 0x5B,
			Windows_Right = 0x5C,
			Apps = 0x5D,
			Page_Up = 0x21,
			Page_Down = 0x22,
			End = 0x23,
			Home = 0x24,
			Cancel = 0x03,
			Clear = 0x0C,
			Select = 0x29,
			Print = 0x2A,
			Execute = 0x2B,
			Print_Screen = 0x2C,
			Insert = 0x2D,
			Delete = 0x2E,
			Help = 0x2F,
			Sleep = 0x5F,

			//! Toggle keys
			Caps_Lock = 0x14,
			Num_Lock = 0x90,
			Scroll_Lock = 0x91,
		};
	}
}