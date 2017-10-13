#pragma once

//! Include the SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Input/AxisInputValues.hpp"
#include "CanvasValues.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
		 *		Name: CanvasInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 13/10/2017
		 *		Modified: 13/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Canvas object
		**/
		struct SDL2_LIB_INC CanvasInitialiser {
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------Navigation Axis------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			union {
				struct {
					//! The name of the Virtual Axis to use for horizontal navigation (Default "horizontal")
					Input::vAxisName horizontalAxis;

					//! The name of the Virtual Axis to use for vertical navigation (Default "vertical")
					Input::vAxisName verticalAxis;

					//! The name of the Virtual Axis to use for UI element confirmation (Default "confirm")
					Input::vAxisName confirmAxis;
				};

				//! Provide access to the axis names as an array
				Input::vAxisName axisArray[3];
			};

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------UI Setup Callbacks-----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Store a callback that is used to load non-default (user defined) UI elements from an Objx file (Default nullptr)
			UI::CustomUILoadCB customLoader;

			//! Store a callback that is used to setup intractable UI elements (Default nullptr)
			UI::ActionUISetupCB actionSetup;

			/*
				CanvasInitialiser : Constructor - Initialise with default values
				Created: 13/10/2017
				Modified: 13/10/2017
			*/
			CanvasInitialiser();
		};
	}
}