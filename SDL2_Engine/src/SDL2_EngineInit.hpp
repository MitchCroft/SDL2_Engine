#pragma once

//! Prototype the Time object
namespace SDL2_Engine { namespace Time { class Time; } }

//! Define the functions to use for the State Manager
extern bool init();
extern void updateLoop(SDL2_Engine::Time::Time& pTime);
extern void drawLoop(SDL2_Engine::Time::Time& pTime);
extern void destroy();