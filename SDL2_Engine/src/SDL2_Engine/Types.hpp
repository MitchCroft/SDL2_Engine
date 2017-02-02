#pragma once

/*
 *		Name: Types
 *		Author: Mitchell Croft
 *		Created: 23/01/2017
 *		Modified: 02/02/2017
 *		
 *		Purpose:
 *		Type define a number of recurring, useful types that can be used
**/
namespace SDL2_Engine {
	//! Type define for the value used as a debug callback
	typedef void (*DebugCallback)(const char* pFormatString, ...);

	//! Type defines for byte information
	typedef char byte;
	typedef unsigned char ubyte;

	//! Type define for short handing unsigned int
	typedef unsigned int uint;
}