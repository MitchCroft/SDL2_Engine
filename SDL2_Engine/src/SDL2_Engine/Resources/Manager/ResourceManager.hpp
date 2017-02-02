#pragma once

#include <memory>

#include "../../Types.hpp"

namespace SDL2_Engine {
	namespace Resources {

		

		class Resources {
			//! Singleton values
			static Resources* mInstance;
			Resources();
			~Resources();

			//unordered_map<string, string> Known resource files look up

			//unordered_map<resourceID, SDL2Resource> ID to resource
			//unordered_map<resourceID, string> ID to filepath

		public:
			//! Initialisation and destruction
			static bool init(const DebugCallback& pWarningCB = nullptr, const DebugCallback& pErrorCB = nullptr);
			static void destroy();

			//! Main Function
			static void update();

			//! Getters

		};
	}
}