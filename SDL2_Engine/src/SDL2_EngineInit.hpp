#pragma once

#include "SDL2_Engine/Root/Time/TimeForwardDeclares.hpp"

//! Define the functions to use for the State Manager
extern bool init();
extern void updateLoop(SDL2_Engine::Time& pTime);
extern void drawLoop(SDL2_Engine::Time& pTime);
extern void destroy();