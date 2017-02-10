#pragma once

namespace SDL2_Engine {
	//! Flag input as either a button or axis input
	enum class EAxisInputType { Button, Axis };

	//! Label the different axis values that can be checked for input
	enum class EControllerAxisCodes {
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

	//! Label the different controller buttons that can be checked
	enum class EControllerKeyCodes : unsigned short {
		//! Null input, key checked
		Null_Input,

		//! D-Pad Axis'
		DPad_Up				= 0x0001,
		DPad_Down			= 0x0002,
		DPad_Left			= 0x0004,
		DPad_Right			= 0x0008,

		//! Option Buttons
		Start				= 0x0010,
		Back				= 0x0020,

		//! Thumbstick buttons
		Left_Thumbstick		= 0x0040,
		Right_Thumbstick	= 0x0080,

		//! Shoulder / Bumpers
		Left_Shoulder		= 0x0100,
		Right_Shoulder		= 0x0200,

		//! Face Buttons 
		A					= 0x1000,
		B					= 0x2000,
		X					= 0x4000,
		Y					= 0x8000
	};

	//! Provide flags for controller's to check for input
	enum class EControllerID { All = -1, One, Two, Three, Four, TOTAL };

	//! Define a function pointer type for scaling vibration values
	typedef float(*VibrationScaleFunc)(const float& pT);
}