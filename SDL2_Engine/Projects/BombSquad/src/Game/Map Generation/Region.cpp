#include "Region.hpp"

//! Include the required STL objects
#include <queue>

namespace BombSquad {
	/*
		Region : Constructor - Initialise with default values
		Created: 08/11/2017
		Modified: 08/11/2017

		param[in] pCoords - A collection of coords that are contained within the region
		param[in] pWidth - The width of the map that the coordinates exist on
		param[in] pHeight - The height of the map that the coordinates exist on
	*/
	Region::Region(const coords& pCoords, const size_t& pWidth, const size_t& pHeight) : contained(pCoords) {
		//Create the location map
		locationMap.setBounds(pWidth, pHeight);

		//Loop through and process the contained coordinates
		for (size_t i = 0; i < contained.size(); i++) {
			//Add the coordinate to the center of mass
			center += contained[i];

			//Register the coordinate in the location map
			locationMap[contained[i].x][contained[i].y] = 1;
		}

		//Take the average to get the center of mass
		center /= contained.size();

		//Mark the offsets to apply to search for outer-region areas
		const coord OFFSETS[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
		const size_t OFFSET_SIZE = sizeof(OFFSETS) / sizeof(const coord);

		//Loop through the coordinates
		for (size_t i = 0; i < contained.size(); i++) {
			//Get a reference to the current coordinate value
			const coord& COORD = contained[i];

			//Check if the coord is on the bounds of the map
			if (COORD.x == 0 || COORD.y == 0 ||
				COORD.x == pWidth - 1 || COORD.y == pHeight - 1);

			//Check if the coordinate has (at least) one non-contained coordinate
			else {
				//Loop through the axis offset values
				bool found = false;
				for (size_t j = 0; j < OFFSET_SIZE; j++) {
					//Apply the offset position
					const coord OFFSET = COORD + OFFSETS[j];

					//Check the coord is on map
					if (OFFSET.x < 0 || OFFSET.y < 0 ||
						OFFSET.x >= pWidth || OFFSET.y >= pHeight)
						continue;

					//Check if the coordinate is on location map
					else if (!locationMap[OFFSET.x][OFFSET.y]) {
						found = true;
						break;
					}
				}

				//Check if an external coordinate was found
				if (!found) continue;
			}

			//Add the coordinate to the edges list
			edges.push_back(COORD);
		}
	}

	/*
		Region : analyseDepth - Flood search each coordinate to find the distance from a coordinate not contained in the region
		Created: 08/11/2017
		Modified: 08/11/2017
	*/
	void Region::analyseDepth() {
		//Create a temporary coordinate element that tracks position and parent object data
		struct CoordTrace {
			//! Store the position
			coord pos;

			//! Store the parent of the coordinate
			CoordTrace* parent = nullptr;
		};

		//Setup the depth map
		distanceFromExternal.setBounds(locationMap.width(), locationMap.height());

		//Store the offsets used to find neighbors
		const coord OFFSETS[] = { {-1, 0}, {1, 0}, {0, -1}, {0, 1}, {-1, -1}, {-1, 1}, {1, 1}, {1, -1} };
		const size_t OFFSET_SIZE = sizeof(OFFSETS) / sizeof(const coord);

		//Loop through all coordinates to find their depth
		for (size_t i = 0; i < contained.size(); i++) {
			//Store a reference to the current coordinate being processed
			const coord COORD = contained[i];

			//Create a hit map for searched coordinates
			hitMap searched(locationMap.width(), locationMap.height());

			//Store a queue of coordinates to process
			std::queue<CoordTrace*> unsearched;

			//Create a CoordTrace Grid
			Map<CoordTrace> traceGrid(locationMap.width(), locationMap.height());

			//Set the position of the initial coord
			traceGrid[COORD.x][COORD.y].pos = COORD;

			//Add the current coordinate to the queue
			unsearched.push(&traceGrid[COORD.x][COORD.y]);

			//Flag if the search has gone out of bounds
			bool outOfBounds = false;

			//Loop through and process trace coords
			CoordTrace* current = nullptr;
			while (unsearched.size()) {
				//Get the next item
				current = unsearched.front();
				unsearched.pop();

				//Check to see if this location is outside of the region
				if (!locationMap[current->pos.x][current->pos.y])
					break;

				//Add neighboring cells to the unsearched list
				for (size_t j = 0; j < OFFSET_SIZE; j++) {
					//Get the offset position
					const coord OFFSET = current->pos + OFFSETS[j];

					//Check the coordinate is in bounds
					if (OFFSET.x < 0 || OFFSET.y < 0 ||
						OFFSET.x >= searched.width() || OFFSET.y >= searched.height()) {
						outOfBounds = true;
						break;
					}

					//Check if the coordinate has been searched
					else if (!searched[OFFSET.x][OFFSET.y]) {
						//Setup the Grid Node
						traceGrid[OFFSET.x][OFFSET.y].pos = OFFSET;
						traceGrid[OFFSET.x][OFFSET.y].parent = current;

						//Add the offset into the unsearched queue
						unsearched.push(&traceGrid[OFFSET.x][OFFSET.y]);

						//Flag the offset in the searched map
						searched[OFFSET.x][OFFSET.y] = 1;
					}
				}

				//If search went out of bounds, break
				if (outOfBounds) break;
			}

			//Check to see if an outer-region was found
			if (current) {
				//Reset the depth value
				distanceFromExternal[COORD.x][COORD.y] = -1;

				//Trace the path back
				while (current) {
					//Increase the distance
					distanceFromExternal[COORD.x][COORD.y]++;

					//Move up the chain
					current = current->parent;
				}
			}
		}
	}

	/*
		Region : recursiveLinkToMain - Recursively traverse the links vector and toggle the linkedToMain
									   flag
		Created: 09/11/2017
		Modified: 09/11/2017
	*/
	void Region::recursiveLinkToMain() {
		//Check if the current is already linked to main
		if (linkedToMain) return;

		//Toggle the linked flag
		linkedToMain = true;

		//Recurse through the links
		for (size_t i = 0; i < links.size(); i++)
			links[i]->recursiveLinkToMain();
	}
}