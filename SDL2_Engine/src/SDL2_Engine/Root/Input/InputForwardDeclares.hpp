#pragma once

/*
 *		Name: InputForwardDeclares.h
 *		Author: Mitchell Croft
 *		Created: 31/01/2017
 *		Modified: 10/02/2017
 *		
 *		Purpose:
 *		Forward declare all values related the controller input
 *		section of the namespace
**/
namespace SDL2_Engine {
	///////////////////////////////////////////////////////////////////////////////////////////////
	////                                   ENUMERATION VALUES                                  ////
	///////////////////////////////////////////////////////////////////////////////////////////////
	enum class EAxisInputType;
	enum class EControllerAxisCodes;
	enum class EControllerKeyCodes : unsigned short;
	enum class EControllerID;

	///////////////////////////////////////////////////////////////////////////////////////////////
	////                                     CLASS OBJECTS                                     ////
	///////////////////////////////////////////////////////////////////////////////////////////////
	class Input;

	///////////////////////////////////////////////////////////////////////////////////////////////
	////                                     STRUCT OBJECTS                                    ////
	///////////////////////////////////////////////////////////////////////////////////////////////
	struct VirtualAxis;
	struct VibrationSetting;
}