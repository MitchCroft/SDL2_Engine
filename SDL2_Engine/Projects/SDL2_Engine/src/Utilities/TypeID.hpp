#pragma once

//! Get information on types used
#include <typeinfo>

namespace SDL2_Engine {
	namespace Utilities {
		//! Define the type used to identify the separate interfaces
		typedef size_t typeID;

		/*
			typeToID - Convert a template type value into a unique hash code
			Created: 19/07/2017
			Modified: 11/10/2017

			Template T - A generic, non-void type

			return typeID - Returns the ID as typeID value
		*/
		template<typename T>
		inline typeID typeToID() {
			//Get the type information
			const std::type_info& type = typeid(T);

			//Return the hash code
			return type.hash_code();
		}
	}
}