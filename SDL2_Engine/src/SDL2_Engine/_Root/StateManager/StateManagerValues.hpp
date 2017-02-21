#pragma once

namespace SDL2_Engine {
	//! Prototype the Time object
	namespace Time { class Time; }

	//! Define types for the function pointers
	typedef bool(*StateManagerInitFunc)();
	typedef void(*StateManagerUpdateFunc)(Time::Time&);
	typedef void(*StateManagerDestroyFunc)();
}