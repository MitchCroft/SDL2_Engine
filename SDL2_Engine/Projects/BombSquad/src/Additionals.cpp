#include "Additionals.hpp"

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
	std::string deductLocation(const std::string pParent, const std::string pRelative) {
		//Check if the pRelative is actually relative
		if (pRelative.find(':') != pRelative.npos) return pRelative;

		//Store a std::string of the current filepath
		std::string current = "";

		//Optomise the parent std::string
		int index = -1;
		size_t prog = 0;
		while (true) {
			//Grab the next std::string section
			std::string sub = pParent.substr(prog, (index = (int)pParent.find_first_of("/\\", index + 1)) + 1 - prog);

			//Check there was text extracted
			if (index == pParent.npos) break;

			//Check if its the current directory shorthand
			else if (sub == "./" || sub == ".\\") continue;

			//Check if its the directory up shorthand
			else if (sub == "../" || sub == "..\\")
				current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

			//Otherwise concatenate
			else current += sub;

			//Increase the progress
			prog += sub.length();
		}

		//Optomise the relative std::string
		index = -1;
		prog = 0;
		while (true) {
			//Grab the next std::string section
			std::string sub = pRelative.substr(prog, (index = (int)pRelative.find_first_of("/\\", index + 1)) + 1 - prog);

			//Increase the progress
			prog += sub.length();

			//Check if its the current directory shorthand
			if (sub == "./" || sub == ".\\") continue;

			//Check if its the directory up shorthand
			else if (sub == "../" || sub == "..\\")
				current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

			//Otherwise concatenate
			else current += sub;

			//Check there was text extracted
			if (index == pRelative.npos) {
				current += pRelative.substr(prog);
				break;
			}
		}

		//Return the combination
		return current;
	}
}
