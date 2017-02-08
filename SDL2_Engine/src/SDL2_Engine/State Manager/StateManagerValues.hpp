#pragma once

namespace SDL2_Engine {
	//! Prototype the Time object
	class Time;

	namespace StateManager {
		//! Define types for the function pointers
		typedef bool(*StateManagerInitFunc)();
		typedef void(*StateManagerUpdateFunc)(Time&);
		typedef void(*StateManagerDestroyFunc)();
	}
}