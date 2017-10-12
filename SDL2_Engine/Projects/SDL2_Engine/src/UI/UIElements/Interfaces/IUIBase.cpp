#include "IUIBase.hpp"

//! Include standard c string functionality
#include <cstring>

namespace SDL2_Engine {
	namespace UI {
		namespace UIElements {
			/*
				IUIBase : Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 12/10/2017
			*/
			IUIBase::IUIBase() : mTag("\0") {}

			/*
				IUIBase : setTag - Set the tag information associated with the UI element
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pTag - A pointer to a string of characters to be used as the new tag information
			*/
			void IUIBase::setTag(const char* pTag) { strcpy_s(mTag, pTag); }
		}
	}
}