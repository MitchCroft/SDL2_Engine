#pragma once

//! Include the SDL2_Engine objects
#include "UIElements/UIValues.hpp"
#include "../Utilities/Action.hpp"

//! Prototype the Objx Node object
namespace Objx { class Node; }

namespace SDL2_Engine {
	namespace UI {
		//! Prototype the UI interfaces
		namespace UIElements { class IUIBase; class IUIAction; }

		//! Define a function typedef that can be used to load non standard UI elements from an Objx file
		typedef Utilities::Action<UIElements::IUIBase*(const Objx::Node& pObj)>::signature CustomUILoadCB;

		//! Define a function typedef that can be used to setup actions for UI elements
		typedef Utilities::Action<void(UIElements::IUIAction* pObject, const uiTag& pTag)>::signature ActionUISetupCB;
	}
}