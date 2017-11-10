#include "RegionIdentifier.hpp"

//! Include the required STL objects
#include <queue>
#include <algorithm>

namespace BombSquad {
	/*
		RegionIdentifier : Constructor - Initialise with default values
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pIdent - An ETileType value identifying the specifier for the region
		param[in] pMap - A gameMap object containing the values to process
	*/
	RegionIdentifier::RegionIdentifier(const ETileType& pIdent, const gameMap& pMap) {
		//Create a hitmap to determine searched coordinates
		hitMap searched(pMap.width(), pMap.height());

		//Store the offset values to apply when looking for neighbors
		const coord OFFSETS[] = { { -1, 0 },{ 1, 0 },{ 0, -1 },{ 0, 1 } };
		const size_t OFFSET_SIZE = sizeof(OFFSETS) / sizeof(const coord);

		//Loop through the hitmap and finds areas that haven't been processed
		for (size_t x = 0; x < searched.width(); x++) {
			for (size_t y = 0; y < searched.height(); y++) {
				//Check to see if the coord has been processed
				if (!searched[x][y]) {
					//Flag as searched
					searched[x][y] = 1;

					//See if the identifier is a match
					if (pMap[x][y] == pIdent) {
						//Store a list of the coordinates found in the region
						coords enclosed;

						//Store a list of the unsearched coordinates
						std::queue<coord> unsearched;

						//Store the current coord in the unsearched list
						unsearched.push({ x, y });

						//Process the unsearched coords to find all connected coordinates
						coord current;
						while (unsearched.size()) {
							//Get the next coordinate
							current = unsearched.front();
							unsearched.pop();

							//Add the coordinate to the enclosed list
							enclosed.push_back(current);

							//Loop through the neighboring coordinates
							for (size_t i = 0; i < OFFSET_SIZE; i++) {
								//Get the offset value
								const coord OFFSET = current + OFFSETS[i];

								//Check the offset is in the bounds of the map
								if (OFFSET.x < 0 || OFFSET.y < 0 ||
									OFFSET.x >= (int)searched.width() || OFFSET.y >= (int)searched.height())
									continue;

								//Determine if the point has already been searched
								else if (searched[OFFSET.x][OFFSET.y]) continue;

								//Flag the coordinate as searched
								searched[OFFSET.x][OFFSET.y] = 1;

								//Check if the coordinate is the same as the identifier
								if (pMap[OFFSET.x][OFFSET.y] == pIdent)
									unsearched.push(OFFSET);
							}
						}

						//Add the new region to the list
						regions.push_back({ enclosed, searched.width(), searched.height() });
					}
				}
			}
		}

		//Sort the regions based on area
		std::sort(regions.begin(), regions.end(), [](const Region& pLeft, const Region& pRight) {
			return pLeft.contained.size() > pRight.contained.size();
		});
	}
}
