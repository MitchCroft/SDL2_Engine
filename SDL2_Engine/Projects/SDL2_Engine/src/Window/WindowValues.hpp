#pragma once

namespace SDL2_Engine {
	//! Flag the properties that can be applied to a window
	enum class EWindowProperties {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------Individual Properties-------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Creates a window that takes up the entire screen
		Fullscreen = 1 << 0,

		//! Window is created visible
		Visibile = 1 << 2,

		//! Window is created hidden
		Invisible = 1 << 3,

		//! Window has no surrounding decorative border
		Borderless = 1 << 4,

		//! Window can be resized by user
		Resizeable = 1 << 5,

		//! Window is created minimised
		Minimised = 1 << 6,

		//! Window is created maximised
		Maximised = 1 << 7,

		//! The window has focus 
		Focus	  = 1 << 9,

		//! Window should support high DPI displays
		High_DPI  = 1 << 13,

		//! Window will always appear on top of other windows
		Forced_Top = 1 << 15,

		//! Window won't appear on the taskbar
		Skip_Taskbar = 1 << 16,

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Pre-Defined Styles--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Standard window that you would find on most applications
		Style_Resizeable = Visibile | Resizeable,

		//! Standard window that can't be resized by the user
		Style_Fixed = Visibile,

		//! Window with no bordering elements/decorations
		Style_Borderless = Visibile | Borderless
	};

	/*
	 *		Name: Dimension2D
	 *		Author: Mitchell Croft
	 *		Created: 06/10/2017
	 *		Modified: 06/10/2017
	 *		
	 *		Purpose: 
	 *		Store simple 2D dimensional values
	**/
	struct Dimension2D { int x, y; };
}