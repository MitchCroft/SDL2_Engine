#include "MapGenerator.hpp"

//! Include required standard functionality
#include <cstdlib>
#include <algorithm>

#include <glm/glm.hpp>

//! Define a simple SIGN macro
#define SIGN(X) ((X) < 0 ? -1 : 1)

/*
	random - Get a 'random' float value in the scale of 0 - 1 (inclusive)
	Author: Mitchell Croft
	Created: 09/11/2017
	Modified: 09/11/2017

	return float - Returns a float value in the 0 - 1 range
*/
inline float random() { return ((float)rand() / (float)RAND_MAX); }

namespace BombSquad {
	/*
		manhattanDistance - Get the Manhattan distance between two points
		Author: Mitchell Croft
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pFrom - The starting point of the path
		param[in] pTo - The ending point of the path

		return size_t - Returns the distance as a size_t value
	*/
	inline size_t manhattanDistance(const coord& pFrom, const coord& pTo) {
		//Get the separation vector
		const coord SEP = pTo - pFrom;

		//Return the combined axis' values
		return (abs(SEP.x) + abs(SEP.y));
	}

	/*
		MapGenerator : Constructor - Initialise with default values
		Created: 09/11/2017
		Modified: 12/11/2017
	*/
	MapGenerator::MapGenerator() :
		mBirthRate(4),
		mDeathLimit(3),
		mInitialChance(0.55f),
		mIterations(-1),
		mMaxIterations(25),
		mPassageRadius(1),
		mObstacleChance(0.25f)
	{}

	/*
		MapGenerator : generateMap - Create a gameMap based on the current settings
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pWidth - The Width of the map to create
		param[in] pHeight - The Height of the map to create
		param[in] pPlayers - The number of spawn points to place on the map

		return gameMap - Returns a gameMap object that has been setup for a game
	*/
	gameMap MapGenerator::generateMap(const size_t& pWidth, const size_t& pHeight, const size_t& pPlayers) const {
		//Flag if the generation process was successful
		bool flag = false;

		//Store a gameMap that can be returned
		gameMap map;

		//Start the process
		do {
			//Create the initial map array
			map = createInitialMap(pWidth, pHeight);

			//Apply the rules for the iterations
			if (mIterations >= 0) {
				//Run the iterations count
				for (int i = 0; i < mIterations; i++)
					simulateStep(map);
			}

			//Simulate steps until there are no chances or maximum limit reached
			else {
				//Store the number of cells modified last step
				int lastModCount = UINT_MAX;

				//Store the number of smoothing iterations that have occurred
				size_t iterCount = 0;

				//Loop until limit is reached
				while (iterCount++ < mMaxIterations) {
					//Perform a simulation step
					size_t modCount = simulateStep(map);

					//Calculate the delta value
					int deltaMod = lastModCount - (int)modCount;

					//Check for loop break
					if (deltaMod == 0) break;

					//Store the last modified count
					lastModCount = (int)modCount;
				}
			}

			//Find all 'free' regions on the map
			RegionIdentifier identifier(ETileType::Free, map);

			//Link the identified regions on the map
			linkRegions(identifier, map);

			//Find distanced coordinates to spawn the players at
			flag = findPlayerSpawns(map, identifier, pPlayers);

		//Loop while spawn points couldn't be placed
		} while (!flag);

		//Spawn the obstacles on the map
		placeObstacles(map);
		
		//Return the generated map
		return map;
	}

	/*
		MapGenerator : setSeed - Set the seed that is used to generate the 'random' values
		Created: 12/11/2017
		Modified: 12/11/2017

		param[in] pSeed - An unsigned integer value to be used as the seed
	*/
	void MapGenerator::setSeed(const unsigned int& pSeed) { srand(pSeed); }

	/*
		MapGenerator : createInitialMap - Create the initial map, and populate with initial free tiles
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pWidth - The width of the map to create
		param[in] pHeight - The height of the map to create

		return gameMap - Returns a gameMap object populated with the starting values
	*/
	gameMap MapGenerator::createInitialMap(const size_t& pWidth, const size_t& pHeight) const {
		//Create the initial empty map
		gameMap map(pWidth, pHeight);

		//Loop through the map and set initial set the initial values
		for (size_t x = 0; x < pWidth; x++) {
			for (size_t y = 0; y < pHeight; y++) {
				//Get the random chance
				if (random() <= mInitialChance) map[x][y] = ETileType::Free;
			}
		}

		//Return the initial map
		return map;
	}

	/*
		MapGenerator : countAliveNeighbours - Find the number of neighboring coordinates which can be considered 'alive'
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pCoord - The position to count the neighbours around
		param[in] pMap - The gameMap that the coordinate exists on

		return size_t - Returns the number of 'alive' coordinates
	*/
	size_t MapGenerator::countAliveNeighbours(const coord& pCoord, const gameMap& pMap) const {
		//Store the number of alive coordinates
		size_t alive = 0;

		//Loop through the neighbouring cells
		for (int x = pCoord.x - 1; x <= pCoord.x + 1; x++) {
			for (int y = pCoord.y - 1; y <= pCoord.y + 1; y++) {
				//Ensure the coord is in bounds
				if (x < 0 || y < 0 || x >= (int)pMap.width() || y >= (int)pMap.height()) continue;

				//Skip itself
				else if (x == pCoord.x && y == pCoord.y) continue;

				//Check the cell is alive
				else if (pMap[x][y] == ETileType::Free)
					alive++;
			}
		}

		//Return the number of alive cells
		return alive;
	}

	/*
		MapGenerator : simulateStep - Perform a single application of the cellular automata rules to a gameMap
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pMap - The gameMap object to apply the rules to
		
		return size_t - Returns the number of tiles that where modified during the application
	*/
	size_t MapGenerator::simulateStep(gameMap& pMap) const {
		//Duplicate the original map
		const gameMap REF_MAP = pMap;

		//Store the number of modified cells
		size_t modifyCount = 0;

		//Loop through and apply the rules
		for (size_t x = 0; x < pMap.width(); x++) {
			for (size_t y = 0; y < pMap.height(); y++) {
				//Get the number of alive neighbours 
				size_t aliveNeighbours = countAliveNeighbours({ x, y }, pMap);

				//Check if the cell is alive
				if (REF_MAP[x][y] == ETileType::Free) {
					//Check if the tile should be killed
					if (aliveNeighbours < mDeathLimit) {
						//Flag modification
						modifyCount++;

						//Kill the cell
						pMap[x][y] = ETileType::Restricted;
					}
				}

				//Otherwise if the cell is dead
				else {
					//Check if it should be rezed
					if (aliveNeighbours > mBirthRate) {
						//Flag modification
						modifyCount++;

						//Rez the cell
						pMap[x][y] = ETileType::Free;
					}
				}
			}
		}

		//Return the modified count
		return modifyCount;
	}

	/*
		MapGenerator : linkRegions - Links regions identified by a RegionIdentifier object
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pIdent - A RegionIdentifer containing the regions to link together
		param[in] pMap - The gameMap object the regions exist on
	*/
	void MapGenerator::linkRegions(RegionIdentifier& pIdent, gameMap& pMap) const {
		//Check there are regions to link
		if (!pIdent.regions.size()) return;

		//Flag the first region as the main
		pIdent.regions[0].linkedToMain = true;

		//Loop through until all regions are linked to main
		bool allLinked = false;
		for (size_t iter = 1; !allLinked; iter++) {
			//Flag all regions as linked
			allLinked = true;

			//Loop through all regions in the list
			for (size_t reg = 0; reg < pIdent.regions.size(); reg++) {
				//Skip if already linked to main
				if (pIdent.regions[reg].linkedToMain) continue;

				//Create a list, pointing to the current regions
				std::vector<Region*> proximity;

				//Add the regions to the list
				proximity.resize(pIdent.regions.size());
				for (size_t i = 0; i < pIdent.regions.size(); i++)
					proximity[i] = &pIdent.regions[i];

				//Sort the proximity based on the distance from the current
				std::sort(proximity.begin(), proximity.end(), [&](const Region* pLeft, const Region* pRight) {
					return (glm::length(glm::vec2(pLeft->center - pIdent.regions[reg].center)) > glm::length(glm::vec2(pRight->center - pIdent.regions[reg].center)));
				});

				//Find the closest possible points
				coord* bestA = &pIdent.regions[reg].edges[0];
				coord* bestB = &proximity[iter]->edges[0];
				float bestDistance = glm::length(glm::vec2(*bestA - *bestB));

				//Loop through all edges of the region
				float buffer = 0;
				for (size_t A = 0; A < pIdent.regions[reg].edges.size(); A++) {
					for (size_t B = 0; B < proximity[iter]->edges.size(); B++) {
						//Get the distance between the points
						buffer = glm::length(glm::vec2(proximity[iter]->edges[B] - pIdent.regions[reg].edges[A]));

						//Check if the distance is less
						if (buffer < bestDistance) {
							bestA = &pIdent.regions[reg].edges[A];
							bestB = &proximity[iter]->edges[B];
							bestDistance = buffer;
						}
					}
				}

				//Get the line between the two points
				coords line = getLine(*bestA, *bestB);

				//Create the link between the regions
				for (size_t i = 0; i < line.size(); i++)
					drawCircle(ETileType::Free, line[i], mPassageRadius, pMap);

				//Create the link between the two
				pIdent.regions[reg].links.push_back(proximity[iter]);
				proximity[iter]->links.push_back(&pIdent.regions[reg]);

				//Check if either are linked to main
				if (pIdent.regions[reg].linkedToMain) pIdent.regions[reg].recursiveLinkToMain();
				else if (proximity[iter]->linkedToMain) proximity[iter]->recursiveLinkToMain();

				//Check if the current is linked to main at this point
				if (!pIdent.regions[reg].linkedToMain) allLinked = false;
			}
		}
	}

	/*
		MapGenerator : getLine - Get the coordinates making up a line between two points
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pFrom - The coordinate at the start of the line
		param[in] pT- The coordinate at the end of the line

		return coords - Returns a collection of coordinates that make up the line
	*/
	coords MapGenerator::getLine(const coord& pFrom, const coord& pTo) const {
		//Create a container for the discovered points
		coords line;

		//Store he current position
		coord pos = pFrom;

		//Get the delta values
		coord delta = pTo - pFrom;

		//Flag if the traversal direction is inverted
		bool inverted = false;

		//Get the step direction
		int step = SIGN(delta.x);

		//Get the gradient step direction
		int gradientStep = SIGN(delta.y);

		//Store the longest and shortest axis
		int longest = abs(delta.x);
		int shortest = abs(delta.y);

		//Ensure the values are stored correctly
		if (longest < shortest) {
			//Flag the inversion
			inverted = true;

			//Swap the values
			longest = abs(delta.y);
			shortest = abs(delta.x);
			step = SIGN(delta.y);
			gradientStep = SIGN(delta.x);
		}

		//Store the gradient accumulation
		int gradientAccumulation = longest / 2;

		//Loop through and find the path
		for (int i = 0; i < longest; i++) {
			//Add the new coordinate to the line
			line.push_back(pos);

			//Increment the position by the step
			if (inverted) pos.y += step;
			else pos.x += step;

			//Increase the gradient accumulated
			gradientAccumulation += shortest;
			if (gradientAccumulation >= longest) {
				//Increment the gradient
				if (inverted) pos.x += gradientStep;
				else pos.y += gradientStep;

				//Subtract the used gradient
				gradientAccumulation -= longest;
			}
		}

		//Return the found coords
		return line;
	}

	/*
		MapGenerator : drawCircle - 'Paint' a specific tile across a gameMap in a circle radius
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pBrush - The Tile Type to apply to the coordinates in the circle area
		param[in] pCenter - The coordinate at the center of the circle
		param[in] pRadius - The radius of the circle to paint
		param[in] pMap - The gameMap to paint the Tiles of
	*/
	void MapGenerator::drawCircle(const ETileType& pBrush, const coord& pCenter, const int& pRadius, gameMap& pMap) const {
		//Store the radius squared
		const int RAD_SQ = pRadius * pRadius;

		//Loop through the coords
		for (int x = -pRadius; x <= pRadius; x++) {
			for (int y = -pRadius; y <= pRadius; y++) {
				//Check if the coords are in the radius of the circle
				if (x * x + y * y <= RAD_SQ) {
					//Calculate the paint coordinates
					coord paint = pCenter + coord(x, y);

					//Check the coords are in range
					if (paint.x < 0 || paint.y < 0 ||
						paint.x >= (int)pMap.width() || paint.y >= (int)pMap.height())
						continue;

					//Set the array value
					pMap[paint.x][paint.y] = pBrush;
				}
			}
		}
	}

	/*
		MapGenerator : findPlayerSpawns - Find the 'ideal' position(s) within the regions to place spawn points for the player(s)
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pMap - The map to find spawn positions on
		param[in] pIdent - A RegionIdentifer object defining the available areas
		param[in] pPlayers - The number of spawn points to find and place on the map

		param[in] bool - Returns true if the player spawns were correctly placed
	*/
	bool MapGenerator::findPlayerSpawns(gameMap& pMap, RegionIdentifier& pIdent, const size_t& pPlayers) const {
		//Store a collective depth map for the current
		depthMap totalDepth(pMap.width(), pMap.height());

		//Store the coordinates that are available for selection
		coords available;

		//Track the current index into available
		size_t currIndex = 0;

		//Loop through and setup from identified regions
		for (size_t reg = 0; reg < pIdent.regions.size(); reg++) {
			//Analyse the depth of the region
			pIdent.regions[reg].analyseDepth();

			//Resize the available list to support the incoming coords
			available.resize(available.size() + pIdent.regions[reg].contained.size());

			//Process all coordinate values
			for (size_t i = 0; i < pIdent.regions[reg].contained.size(); i++) {
				//Get the coord to be processed
				const coord& curr = pIdent.regions[reg].contained[i];

				//Store the depth value in the master map
				totalDepth[curr.x][curr.y] = pIdent.regions[reg].distanceFromExternal[curr.x][curr.y];

				//Add the coord to the overall available list
				available[currIndex++] = curr;
			}
		}

		//Remove any regions that have a depth of 0
		available.erase(std::remove_if(available.begin(), available.end(), [&](const coord& pVal) { return (totalDepth[pVal.x][pVal.y] == 0); }));

		//Check there are enough points available to support the requested players
		if (available.size() < pPlayers) return false;

		//Sort the list based on their depth values
		std::sort(available.begin(), available.end(), [&](const coord& pLeft, const coord& pRight) {
			return (totalDepth[pLeft.x][pLeft.y] > totalDepth[pRight.x][pRight.y]);
		});

		//Create a container for the spawn points
		coords spawnPoints;

		//Resize the array to store all points
		spawnPoints.resize(pPlayers);

		//Stash the deepest point on the map as the first spawn
		spawnPoints[0] = available[0];

		//Narrow down the other spawn positions
		for (size_t player = 1; player < pPlayers; player++) {
			//Store the furthest possible distance found
			const coord* furthest = nullptr;
			int furthestAvgDist = 0;

			//Loop through the available positions to find a spawn point
			for (size_t aval = 0; aval < available.size(); aval++) {
				//Skip over used coordinates
				bool found = false;
				for (size_t i = 0; i < player; i++) {
					if (spawnPoints[i] == available[aval]) {
						found = true;
						break;
					}
				}

				//If matching spawn coord found, skip
				if (found) continue;

				//Store the average distance
				int avg = 0;

				//Loop through the previously established positions
				for (size_t i = 0; i < player; i++) {
					avg += findPath(spawnPoints[i], available[aval], pMap, ETileType::Free).size() +
						(totalDepth[available[aval].x][available[aval].y] * 4);
				}

				//Take the average
				avg /= player;

				//Check if the distance is further
				if (avg > furthestAvgDist) {
					//Save the new values
					furthest = &available[aval];
					furthestAvgDist = avg;
				}
			}

			//Set the spawn point
			spawnPoints[player] = *furthest;
		}

		//Mark the spawn positions on the map
		for (size_t i = 0; i < spawnPoints.size(); i++)
			pMap[spawnPoints[i].x][spawnPoints[i].y] = ETileType::SpawnPoint;

		//Mark the offsets for the safe zone
		const coords OFFSETS = { {1, 0}, {-1, 0}, {0, 1}, {0, -01} };

		//Mark the safe zones to prevent obstacle spawning
		for (size_t i = 0; i < spawnPoints.size(); i++) {
			//Loop through the offset values
			for (size_t j = 0; j < OFFSETS.size(); j++) {
				//Get the offset position
				const coord OFFSET = spawnPoints[i] + OFFSETS[j];

				//Check the offset is in the map bounds
				if (OFFSET.x < 0 || OFFSET.y < 0 ||
					OFFSET.x >= (int)pMap.width() || OFFSET.y >= (int)pMap.height())
					continue;

				//Ensure the area to be modified is a valid type
				else if (pMap[OFFSET.x][OFFSET.y] == ETileType::Free ||
						 pMap[OFFSET.x][OFFSET.y] == ETileType::Restricted)
						 pMap[OFFSET.x][OFFSET.y] = ETileType::Safe;
			}
		}

		//Return success
		return true;
	}

	/*
		MapGenerator : findPath - Find a path between two coordinates on a gameMap
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pStart - The coordinate of the starting point for the search
		param[in] pEnd - The coordinate of the ending point for the search
		param[in] pMap - The gameMap object that the coordinates exist on
		param[in] pTraversable - The Tile Type which should be considered traversable by the algorithm

		return coords - Returns a collection of coordinates that make up the path between the two points
	*/
	coords MapGenerator::findPath(const coord& pStart, const coord& pEnd, const gameMap& pMap, const ETileType& pTraversable) const {
		//Store a A* Node Definition
		struct AStarNode {
			//Store the position of a Node
			coord pos;

			//Store a pointer to the Node object that introduced the current
			AStarNode* parent;

			//Store the score values for the Node
			size_t gScore, hScore, fScore;
		};

		//Store a grid of A* Nodes for caluclation purposes
		Map<AStarNode> navMesh(pMap.width(), pMap.height());

		//Setup the grid
		for (size_t x = 0; x < navMesh.width(); x++) {
			for (size_t y = 0; y < navMesh.height(); y++) {
				//Set default values
				navMesh[x][y] = { {x, y}, nullptr, UINT_MAX, UINT_MAX, UINT_MAX };
			}
		}

		//Store a priority queue of unprocessed Nodes
		std::vector<AStarNode*> unsearched;

		//Setup the initial score values for the starting Node
		navMesh[pStart.x][pStart.y].gScore = 0;
		navMesh[pStart.x][pStart.y].hScore =
		navMesh[pStart.x][pStart.y].fScore = manhattanDistance(pStart, pEnd);

		//Add the start coordinate to the list
		unsearched.push_back(&navMesh[pStart.x][pStart.y]);

		//Store the offsets used to identify neighbours
		const coords OFFSETS = { {0, 1}, {0, -1}, {1, 0}, {-1, 0} };

		//Loop while there are coordinates to process
		AStarNode* current = nullptr;
		while (unsearched.size()) {
			//Sort the queue
			std::sort(unsearched.begin(), unsearched.end(), [](const AStarNode* pLeft, const AStarNode* pRight) {
				return (pLeft->fScore > pRight->fScore);
			});

			//Get the next element
			current = unsearched[0];
			unsearched.erase(unsearched.begin());

			//Check if this is the end 
			if (current->pos == pEnd) break;

			//Add neighbours to the list
			for (size_t i = 0; i < OFFSETS.size(); i++) {
				//Get the next coord
				const coord OFFSET = current->pos + OFFSETS[i];

				//Check the coord is in the map bounds
				if (OFFSET.x < 0 || OFFSET.y < 0 ||
					OFFSET.x >= (int)pMap.width() || OFFSET.y >= (int)pMap.height())
					continue;

				//Check if the coordinate is traversable terrain
				else if (pMap[OFFSET.x][OFFSET.y] != pTraversable) continue;

				//Check if the Node hasn't been searched yet
				else if (!navMesh[OFFSET.x][OFFSET.y].parent) unsearched.push_back(&navMesh[OFFSET.x][OFFSET.y]);

				//Check to see if this cell has a better score
				if (navMesh[OFFSET.x][OFFSET.y].gScore > navMesh[current->pos.x][current->pos.y].gScore + 1) {
					//Set the new parent
					navMesh[OFFSET.x][OFFSET.y].parent = current;

					//Save the new score values
					navMesh[OFFSET.x][OFFSET.y].gScore = navMesh[current->pos.x][current->pos.y].gScore + 1;
					navMesh[OFFSET.x][OFFSET.y].hScore = manhattanDistance(OFFSET, pEnd);
					navMesh[OFFSET.x][OFFSET.y].fScore = navMesh[OFFSET.x][OFFSET.y].gScore + navMesh[OFFSET.x][OFFSET.y].hScore;
				}
			}
		}

		//Create the path to travel
		coords path;

		//Store the path in the array
		if (current->pos == pEnd) {
			while (current->parent) {
				//Add the coordinate to the path
				path.push_back(current->pos);

				//Trace the path back
				current = current->parent;
			}

			//Reverse the path
			std::reverse(path.begin(), path.end());
		}

		//Return the path
		return path;
	}

	/*
		MapGenerator : placeObstacles - Place obstacle tiles around a gameMap
		Created: 09/11/2017
		Modified: 09/11/2017

		param[in] pMap - The gameMap to place the obstacles on
	*/
	void MapGenerator::placeObstacles(gameMap& pMap) const {
		//Loop through the map and randomly place obstacles on the free coords
		for (size_t x = 0; x < pMap.width(); x++) {
			for (size_t y = 0; y < pMap.height(); y++) {
				//Check if the coord is free
				if (pMap[x][y] == ETileType::Free &&
					random() <= mObstacleChance)
					pMap[x][y] = ETileType::Obstacle;
			}
		}
	}
}