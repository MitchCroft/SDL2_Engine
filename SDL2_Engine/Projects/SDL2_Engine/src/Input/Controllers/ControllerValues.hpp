#pragma once

namespace SDL2_Engine {
	namespace Input {
		//! Define the ID's for the different GamePads
		enum class EGamePadID { 
			One		= 1 << 0, 
			Two		= 1 << 1, 
			Three	= 1 << 2, 
			Four	= 1 << 3, 
			All		= One | Two | Three | Four
		};

		//! List the different axis that can be read from a GamePad
		enum class EGamePadAxisCodes {
			//! Null input, axis not checked
			Null_Axis = -1,

			//! Trigger Axis
			Left_Trigger,
			Right_Trigger,

			//! Thumbstick Axis'
			Left_X,
			Left_Y,
			Right_X,
			Right_Y
		};

		//!  List the different button keycodes that can be read from a GamePad
		enum class EGamePadBtnCodes : unsigned short {
			//! Null input, key checked
			Null_Input		 = 0x0000,

			//! D-Pad Axis'
			DPad_Up			 = 0x0001,
			DPad_Down		 = 0x0002,
			DPad_Left		 = 0x0004,
			DPad_Right		 = 0x0008,

			//! Option Buttons
			Start			 = 0x0010,
			Back			 = 0x0020,

			//! Thumbstick buttons
			Left_Thumbstick  = 0x0040,
			Right_Thumbstick = 0x0080,

			//! Shoulder / Bumpers
			Left_Shoulder	 = 0x0100,
			Right_Shoulder	 = 0x0200,

			//! Face Buttons 
			A				 = 0x1000,
			B				 = 0x2000,
			X				 = 0x4000,
			Y				 = 0x8000,

			//! Any button
			Any				 = 0xFFFF
		};

		//! Prototype the GamePad object for the const typedef
		class _GamePad;

		//! Type define a simple accesser type for the user to use
		typedef const _GamePad* GamePad;
	}
}