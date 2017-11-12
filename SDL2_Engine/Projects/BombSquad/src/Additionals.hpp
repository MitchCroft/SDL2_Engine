#pragma once

#include <string>

namespace BombSquad {
	/*
		deductLocation - Deduct the location of relative resources, stripping superfluous characters
		Author: Mitchell Croft
		Created: 13/10/2017
		Modified: 11/11/2017

		param[in] pParent - The original string location of a file
		param[in] pRelative - The file path of the second file, relative to pParent

		return std::string - Returns the clean filepath as a new std::string object
	*/
	extern std::string deductLocation(const std::string pParent, const std::string pRelative);
}