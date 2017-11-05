/*
 *      Name: SymbolRegistry
 *      Author: Mitchell Croft
 *      Created: 04/11/2017
 *      Modified: 04/11/2017
 * 
 *      Purpose:
 *      Store the symbolic values used to represent map elements with specific keys
**/
let SymbolRegistry = {
    //! Standard Cell Values
    "Restricted":   "#",
    "Free"      :   " ",
    "Blocked"   :   "X",
    "Safe"      :   "S",
};

/*
 *      Name: MapGenerator
 *      Author: Mitchell Croft
 *      Created: 03/11/2017
 *      Modified: 05/11/2017
 * 
 *      Purpose:
 *      Generate a 2D array of data representing a possible area of play
 *      for a bomberman-esque game
**/

/*
    MapGenerator : Constructor - Initialise with default values
    Created: 03/11/2017
    Modified: 05/11/2017

    param[in] pSetup - An object with starting initial values for the generation controls
*/
function MapGenerator(pSetup) {
    //! Ensure that the setup is an object
    pSetup = Validate.type(pSetup, "object", {});

    //! Wrip the starting values from the setup
    this.birthRate = Validate.type(pSetup.birthRate, "number", 4);
    this.deathLimit = Validate.type(pSetup.deathLimit, "number", 3);
    this.initialChance = Validate.type(pSetup.initialChance, "number", 0.55);

    //! Store the number of iterations to perform (<0 is until smoothed)
    this.iterations = Validate.type(pSetup.iterations, "number", -1);

    //! Store the maximum numbe rof iterations to allow for when iterations is < 0
    this.maxSmoothing = Validate.type(pSetup.maxSmoothing, "number", 25);

    //! Flag the chance of obstacles being spawned
    this.obstacleChance = Validate.type(pSetup.obstacleChance, "number", 0.25);

    //! Store the size of the passages to carve
    this.passageSize = Validate.type(pSetup.passageSize, "number", 1);
}

/*
    MapGenerator : generate - Create a 2D map array with the specified values
    Created: 03/11/2017
    Modified: 05/11/2017

    param[in] pWidth - The width of the map to create
    param[in] pHeight - The height of the map to create
    param[in] pPlayers - The number of player spawn points to place on the map

    return 2D String Array - Returns a two dimensional array of single character strings representing the map cells
*/
MapGenerator.prototype.generate = function(pWidth, pHeight, pPlayers) {
    //Clean the parameter values
    pWidth = Validate.type(pWidth, "number", 0, true);
    pHeight = Validate.type(pHeight, "number", 0, true);

    //Flag if the map was loaded correctly
    let loadedFlag = false;

    //Store a reference to the activly modified map values
    let map = null;

    //Start the process
    do {
        //Output Process
        console.log("Creating Initial Layout of Map");

        //Create the initial 2D array of values
        map = this.createInitialMap(pWidth, pHeight);

        //Output process
        console.log("Applying Rules for Specified Iterations");

        //Apply the rules for the iterations
        if (this.iterations >= 0) {
            //Run the iteration counts over the map
            for (let i = 0; i < this.iterations; i++)
                this.simulateStep(map);
        }

        //Simulate steps until delta reaches a point
        else {
            //Store the number of cells modified last step
            let lastModCount = Number.MAX_VALUE;

            //Store the number of smoothing iterations have occurred
            let iterCount = 0;

            //Loop until further notice
            while (iterCount++ < this.maxSmoothing) {
                //Perform a simulation step
                let modCount = this.simulateStep(map);

                //Calculate the delta value
                let deltaMod = lastModCount - modCount;

                //Check for loop break
                if (deltaMod === 0) break;

                //Store the last modified count
                lastModCount = modCount;
            }
        }

        //Output Process
        console.log("Identifying Regions");

        //Find all 'free' regions on the map
        let regionIdentifier = new RegionIdentifier(SymbolRegistry["Free"], map);

        //Output process
        console.log("Linking Seperated Regions");

        //Link the identified regions
        this.linkRegions(regionIdentifier, map, pWidth, pHeight);

        //Output process
        console.log("Placing Player Spawn Points: (" + pPlayers + " Players)");

        //Find equidistant(ish) coordiantes for the player spawn points
        flag = this.findPlayerSpawns(map, pWidth, pHeight,regionIdentifier, pPlayers);

        //Output process
        if (!flag) console.log("Failed to place all Player Spawn Points. Restarting process...");
    } while (!flag);

    //Output Process
    console.log("Placeing Obstacles");

    //Spawn obstacles on the map
    this.spawnObstacles(map, pWidth, pHeight);

    //Return the generated map
    return map;
};

/*
    MapGenerator : createInitialMap - Setup the initial array with the starting values
    Created: 03/11/2017
    Modified: 03/11/2017

    param[in] pWidth - The width of the map to create
    param[in] pHeight - The height of the map to create

    return 2D String Array - Returns a two dimensional array of single character strings representing the initial map cells
*/
MapGenerator.prototype.createInitialMap = function(pWidth, pHeight) {
    //Create the initial array
    let initial = [];

    //Loop through the width to create the initial array elements
    for (let i = 0; i < pWidth; i++) {
        //Create the array at the index
        initial[i] = [];

        //Set the length of the array
        initial[i].length = pHeight;

        //Fill with the empty values
        initial[i].fill(SymbolRegistry["Restricted"]);
    }

    //Calculate the inverse initial spawn chance
    let invChance = 1 - this.initialChance;

    //Loop through the array elements and set initial values
    for (let x = 0; x < pWidth; x++) {
        for (let y = 0; y < pHeight; y++) {
            //Get a random chance
            if (Math.random() < invChance)
                initial[x][y] = SymbolRegistry["Free"];
        }
    }

    //Return the initial map
    return initial;
};

/*
    MapGenerator : countAliveNeighbours - Count the number of cells that are alive around a coordinate
    Created: 03/11/2017
    Modified: 03/11/2017

    param[in] pX - The X coordinate of the cell to check
    param[in] pY - The Y coordinate of the cell to check
    param[in] pMap - The 2D map to use as information

    return number - Returns the number of alive cells surounding the point
*/
MapGenerator.prototype.countAliveNeighbours = function(pX, pY, pMap) {
    //Get the width of the map
    let width = pMap.length;

    //Check there is width
    if (!width) return 0;

    //Get the height of the map
    let height = pMap[width - 1].length;

    //Check there is height
    if (!height) return 0;

    //Store the number of cells alive
    let alive = 0;

    //Loop through neighbouring cells
    for (let x = pX - 1; x <= pX + 1; x++) {
        for (let y = pY - 1; y<= pY + 1; y++) {
            //Ensure the coords are in bounds
            if (x < 0 || y < 0 || x >= width || y >= height) continue;

            //Skip itself
            else if (x == pX && y == pY) continue;

            //Check the cell is alive
            else if (pMap[x][y] == SymbolRegistry["Free"])
                alive++;
        }
    }

    //Return the number of alive cells
    return alive;
};

/*
    MapGenerator : simulateStep - Perform an application step of the rules to a map
    Created: 03/11/2017
    Modified: 03/11/2017

    param[in] pMap - The map to perform the application step to

    return number - Returns the number of cells modified during the step
*/
MapGenerator.prototype.simulateStep = function(pMap) {
    //Store the width of the map to process
    let width = pMap.length;

    //Check there is a width
    if (!width) return 0;

    //Store the height of the map to process
    let height = pMap[width - 1].length;

    //Check there is a heightRestricFreeted
    if (!height) return 0;
    
    //Duplicate the original map
    let refMap = [];

    //Duplicate the array elements
    for (let x = 0; x < width; x++) {
        //Create array element
        refMap[x] = [];

        //Duplicate individual elements
        for (let y = 0; y < height; y++) {
            refMap[x][y] = pMap[x][y];
        }
    }

    //Store the number of modified cells
    let modifyCount = 0;

    //Loop through and apply the rules
    for (let x = 0; x < width; x++) {
        for (let y = 0; y < height; y++) {
            //Get the number of alive neighbours
            let aliveNeighbours = this.countAliveNeighbours(x, y, refMap);

            //Check if the cell is alive
            if (refMap[x][y] == SymbolRegistry["Free"]) {
                //Check if it should be killed
                if (aliveNeighbours < this.deathLimit) {
                    //Flag a modification
                    modifyCount++;

                    //Kill the cell
                    pMap[x][y] = SymbolRegistry["Restricted"];
                }
            }

            //Otherwise if cell is dead
            else {
                //Check if it should be resurected
                if (aliveNeighbours > this.birthRate) {
                    //Flag a modification
                    modifyCount++;

                    //Rez the cell
                    pMap[x][y] = SymbolRegistry["Free"];
                }
            }
        }
    }

    //Return the modified count
    return modifyCount;
};

/*
    MapGenerator : linkRegions - Link regions identified by a Region Identifier object
    Created: 04/11/2017
    Modified: 04/11/2017

    param[in] pIdent - A RegionIdentifier object defining the objects to link
    param[in] pMap - The 2D string array representing the map of values
    param[in] pWidth - The width of the pMap array
    param[in] pHeight - The height of the pMap array
*/
MapGenerator.prototype.linkRegions = function(pIdent, pMap, pWidth, pHeight) {
    //Check there are regiosn to process
    if (!pIdent.regions.length) return;

    //Flag the largest room as the main
    pIdent.regions[0].linkedMain = true;

    //Have a simple recursive function to retrace linked rooms back to main
    let recurseLink = function(pRegion) {
        //Check if the region is already flagged
        if (pRegion.linkedMain) return;

        //Set the linked flag
        pRegion.linkedMain = true;

        //Recurse through sub regions
        for (let i = 0; i < pRegion.links.length; i++)
            recurseLink(pRegion.links[i]);
    };

    //Loop through until all regions are linked to main
    for (let iter = 1, allLinked = false; !allLinked; iter++) {
        //Flag all regions as linked
        allLinked = true;

        //Loop through all regions in the list
        for (let i = 0; i < pIdent.regions.length; i++) {
            //Check to see if the region is linked to main
            if (pIdent.regions[i].linkedMain) continue;
            
            //Create a copy of the array of elements
            let proximity = [];
            for (let j = 0; j < pIdent.regions.length; j++)
                proximity[j] = pIdent.regions[j];

            //Sort the list based on proximity to the current
            proximity.sort(function(pFirst, pSecond) {
                return (pFirst.center.subtract(pIdent.regions[i].center).mag - pSecond.center.subtract(pIdent.regions[i].center).mag);
            });

            //Find the closest possible points
            let bestA = pIdent.regions[i].edges[0];
            let bestB = proximity[iter].edges[0];
            let bestDist = bestB.subtract(bestA).mag;

            //Loop through all edges of the region
            let buffer = 0;
            for (let A = 0; A < pIdent.regions[i].edges.length; A++) {
                for (let B = 0; B < proximity[iter].edges.length; B++) {
                    //Get the distance between the points
                    buffer = proximity[iter].edges[B].subtract(pIdent.regions[i].edges[A]).mag;

                    //Check if the distance is less
                    if (buffer < bestDist) {
                        bestA = pIdent.regions[i].edges[A];
                        bestB = proximity[iter].edges[B];
                        bestDist = buffer;
                    }
                }
            }

            //Get the line between the points
            let line = this.getLine(bestA, bestB);

            //Create the link between the regions
            for (let j = 0; j < line.length; j++)
                this.drawCircle(SymbolRegistry["Free"], line[j], this.passageSize, pMap, pWidth, pHeight);

            //Create the link between the two
            pIdent.regions[i].links.push(proximity[iter]);
            proximity[iter].links.push(pIdent.regions[i]);

            //Check if either are linked to main
            if (pIdent.regions[i].linkedMain)    recurseLink(proximity[iter]);
            else if (proximity[iter].linkedMain) recurseLink(pIdent.regions[i]);

            //Check if the current is linked to main at this point
            if (!pIdent.regions[i].linkedMain) allLinked = false;
        }
    }
};

/*
    MapGenerator : getLine - Get the coordinates making up a line between two points
    Created: 04/11/2017
    Modified: 04/11/2017

    param[in] pFrom - A Vec2 object containing the coordinate to reach
    param[in] pTo - A Vec2 object containing the coordinate to start from

    return Vec2 Array - Returns an array of Vec2 objects holding position data
*/
MapGenerator.prototype.getLine = function(pFrom, pTo) {
    //Create an array to store the data in
    let line = [];

    //Store the current position
    let x = pFrom.x;
    let y = pFrom.y;

    //Get the delta values
    let delta = pTo.subtract(pFrom);

    //Flag if the traversal direction is inverted
    let inverted = false;

    //Calculate the step direction
    let step = Math.sign(delta.x);

    //Calculate the gradient step direction
    let gradientStep = Math.sign(delta.y);

    //Store the longest and shortest axis
    let longest = Math.abs(delta.x);
    let shortest = Math.abs(delta.y);

    //Ensure the values are stored correctly
    if (longest < shortest) {
        //Flag the inversion
        inverted = true;

        //Swap the values
        longest = Math.abs(delta.y);
        shortest = Math.abs(delta.x);
        step = Math.sign(delta.y);
        gradientStep = Math.sign(delta.x);
    }

    //Store the gradient accumulation
    let gradientAccumulation = longest / 2;

    //Loop through and find the path
    for (let i = 0; i < longest; i++) {
        //Add the new coordinate to the line
        line.push(new Vec2(x, y));

        //Increment the position by the step
        if (inverted) y += step;
        else x += step;

        //Increase the gradient accumulated
        gradientAccumulation += shortest;
        if (gradientAccumulation >= longest) {
            //Increment by the gradient
            if (inverted) x += gradientStep;
            else y += gradientStep;

            //Subtract the used gradient
            gradientAccumulation -= longest;
        }
    }

    //Return the found coords
    return line;
};

/*
    MapGenerator : drawCircle - Draw a sequence of characters on the 'map' in a circle radius
    Created: 04/11/2017
    Modified: 04/11/2017
 
    param[in] pSequance - The string sequance to paint into the array
    param[in] pCenter - The center point of the circle
    param[in] pRadius - The radius of the circle to draw
    param[in] pMap - The 2D string array of characters representing the map to modify
    param[in] pWidth - The width of the pMap array
    param[in] pHeight - The height of the pMap array
*/
MapGenerator.prototype.drawCircle = function(pSequance, pCenter, pRadius, pMap, pWidth, pHeight) {
    //Loop through the coords
    for (let x = -pRadius; x <= pRadius; x++) {
        for (let y = -pRadius; y <= pRadius; y++) {
            //Check if the coords are in the radius of the circle
            if (x * x + y * y <= pRadius * pRadius) {
                //Calculate the coordinates
                let drawX = pCenter.x + x;
                let drawY = pCenter.y + y;

                //Check the coords are in range of the map
                if (drawX < 0 || drawY < 0 ||
                    drawX >= pWidth || drawY >= pHeight)
                    continue;

                //Set the array value
                pMap[drawX][drawY] = pSequance;
            }
        }
    }
};

/*
    MapGenerator : findPlayerSpawns - Find the ideal position(s) within regions to place spawn points for players
    Created: 05/11/2017
    Modified: 05/11/2017

    param[in] pMap - The 2D array of string values which represent the map
    param[in] pWidth - The width of the pMap array
    param[in] pHeight - The height of th pMap array
    param[in] pIdent - The RegionIdentifier which has identified the valid coordinates on pMap for spawns
    param[in] pPlayers - The number of spawn points to place on the map

    return bool - Returns true if player spawns were correctly setup
*/
MapGenerator.prototype.findPlayerSpawns = function(pMap, pWidth, pHeight, pIdent, pPlayers) {
    //Store a depth map for the current map
    let depthMap = [];
    for (let i = 0; i < pWidth; i++) {
        depthMap[i] = [];
        depthMap[i].length = pHeight;
        depthMap[i].fill(0);
    }
        
    //Store the coordinates that are available for selection
    let available = [];

    //Loop through and setup from identified regions
    for (let i = 0 ; i < pIdent.regions.length; i++) {
        //Analyse the depth for the region
        pIdent.regions[i].analyseDepth();

        //Process all coordiante values
        for (let j = 0; j < pIdent.regions[i].coordinates.length; j++) {
            //Get the coord to be processed
            let coord = pIdent.regions[i].coordinates[j];

            //Store the depth value in the master map
            depthMap[coord.x][coord.y] = pIdent.regions[i].depthMap[coord.x][coord.y];

            //Add the coord to the overall available list
            available.push(coord);
        }
    }

    //Remove any regions that have a depth of 0
    available = available.filter(function(pVal) { return (depthMap[pVal.x][pVal.y] !== 0); });

    //Check there are enough points available to support this many players
    if (available.length < pPlayers) return false;

    //Sort the list based on their depth values
    available.sort(function(pFirst, pSecond) { return depthMap[pSecond.x][pSecond.y] - depthMap[pFirst.x][pFirst.y]; });

    //Store an array of the spawn positions for the players (Start with the deepest for player 1)
    let spawnPoints = [ available[0] ];

    //Narrow down the furthest available positions
    for (let player = 1; player < pPlayers; player++) {
        //Store the furthest possible distance found
        let furthest = null;
        let furthestAvgDist = 0;

        //Loop through the available positions to spawn the player
        for (let aval = 0; aval < available.length; aval++) {
            //Skip over any used coordinates
            let found = false;
            for (let current = 0; current < player; current++) {
                if (available[aval].equal(spawnPoints[current])) {
                    found = true;
                    break;
                }
            }

            //If duplicate coord was found continue
            if (found) continue;

            //Store the average distance
            let avg = 0;

            //Loop through the previously established positions
            for (let pos = 0; pos < player; pos++) {
                avg += this.findPath(spawnPoints[pos], available[aval], pMap, pWidth, pHeight, SymbolRegistry["Free"]).length
                    + (depthMap[available[aval].x][available[aval].y] * 4);
            }

            //Take the average
            avg /= player;

            //Check if the distance is further
            if (avg > furthestAvgDist) {
                //Save the new values
                furthest = available[aval];
                furthestAvgDist = avg;
            }
        }

        //Set the spawn point
        spawnPoints[player] = furthest;
    }

    //Mark the spawn positions on the map
    for (let i = 0; i < spawnPoints.length; i++) 
        pMap[spawnPoints[i].x][spawnPoints[i].y] = "" + (i + 1);

    //Mark the offsets for the safe zones
    let offsets = [ new Vec2(1, 0), new Vec2(-1, 0), new Vec2(0, 1), new Vec2(0, -1) ];

    //Mark the safe zones (Prevent obstacle spawns)
    for (let i = 0; i < spawnPoints.length; i++) {
        //Loop through the offset values
        for (let j = 0; j < offsets.length; j++) {
            //Get the offset position
            let offset = spawnPoints[i].add(offsets[j]);

            //Check the offset is in the map bounds
            if (offset.x < 0 || offset.y < 0 ||
                offset.x >= pWidth || offset.y >= pHeight)
                continue;

            //Ensure area to be modified is a valid type
            if (pMap[offset.x][offset.y] === SymbolRegistry["Free"] ||
                pMap[offset.x][offset.y] === SymbolRegistry["Restricted"])
                pMap[offset.x][offset.y] = SymbolRegistry["Safe"];
        }
    }

    //Return success
    return true;
};

/*
    MapGenerator : findPath - Find a path between two coordinates on a map
    Created: 05/11/2017
    Modified: 05/11/2017

    param[in] pStart - A Vec2 object defining the starting coordinates
    param[in] pEnd - A Vec2 object defining the ending coordinates
    param[in] pMap - The 2D array of strings representing the map to process
    param[in] pWidth - The width of the pMap array
    param[in] pHeight - The height of the pMap array
    param[in] pTraversal - The chracter sequance to use as a valid path in the pMap

    return Vec2 Array - Returns a Vec2 array defining the path to take between the two points
*/
MapGenerator.prototype.findPath = function(pStart, pEnd, pMap, pWidth, pHeight, pTraversal) {
    //Store a A* Node definition
    function AStarNode(pPos) {
        //Store the position of the Node on the map
        this.pos = Validate.instance(pPos, Vec2, null, true);

        //Store a reference to the parent object that introduced the Node
        this.parent = null;

        //Store the score values for the Node
        this.gScore = Number.MAX_VALUE;
        this.hScore = Number.MAX_VALUE;
        this.fScore = Number.MAX_VALUE;
    };

    //Define a simple heuristic function
    function heuristic(pTo, pFrom) { 
        //Get the seperation vector
        let sep = pTo.subtract(pFrom);

        //Return the manhatten distance
        return Math.abs(sep.x) + Math.abs(sep.y);
    };

    //Store a grid of the Nodes for calculation purposes
    let navMesh = [];

    //Establish the grid
    for (let x = 0; x < pWidth; x++) {
        //Create the new array
        navMesh[x] = [];

        //Create the individual elements
        for (let y = 0; y < pHeight; y++)
            navMesh[x][y] = new AStarNode(new Vec2(x, y));
    }

    //Store a priority queue of coordinates values to process
    let unsearched = [];

    //Add the start coordinate to the list
    unsearched.push(pStart);

    //Setup the score values for the Node
    navMesh[pStart.x][pStart.y].gScore = 0;
    navMesh[pStart.x][pStart.y].hScore =
    navMesh[pStart.x][pStart.y].fScore = heuristic(pEnd, pStart);

    //Store the offset values used to identify neighbours
    let offsets = [ new Vec2(0, 1), new Vec2(0, -1), new Vec2(1, 0), new Vec2(-1, 0) ];

    //Loop while there are coordinates to process
    let current = null;
    while (unsearched.length) {
        //Sort the unsearched list for priority
        unsearched.sort(function(pLeft, pRight) { return navMesh[pLeft.x][pLeft.y].fScore - navMesh[pRight.x][pRight.y].fScore; });

        //Get the next element to process
        current = unsearched.shift();

        //Check if the current is the end of the road
        if (current.equal(pEnd)) break;

        //Add neighbours to the list
        for (let i = 0; i < offsets.length; i++) {
            //Get the offset coordinate
            let coord = current.add(offsets[i]);

            //Check coordinate is in map bounds
            if (coord.x < 0 || coord.y < 0 ||
                coord.x >= pWidth || coord.y >= pHeight)
                continue;

            //Check coordinate is a traversible type
            if (pMap[coord.x][coord.y] !== pTraversal) continue;

            //Check if the Node has been searched yet
            if (!navMesh[coord.x][coord.y].parent) unsearched.push(coord);

            //Check to see if this cell has a better Score
            if (navMesh[coord.x][coord.y].gScore > navMesh[current.x][current.y].gScore + 1) {
                //Set the new parent
                navMesh[coord.x][coord.y].parent = navMesh[current.x][current.y];

                //Save the score values
                navMesh[coord.x][coord.y].gScore = navMesh[current.x][current.y].gScore + 1;
                navMesh[coord.x][coord.y].hScore = heuristic(pEnd, coord);
                navMesh[coord.x][coord.y].fScore = navMesh[coord.x][coord.y].gScore + navMesh[coord.x][coord.y].hScore;
            }
        }
    }

    //Create the array to travel
    let path = [];

    //Store the path
    if (current) {
        //Store the path in the array
        while(navMesh[current.x][current.y].parent) {
            //Add the coordinate to the path
            path.push(navMesh[current.x][current.y].pos);

            //Trace back the path
            current = navMesh[current.x][current.y].parent.pos;
        }

        //Reverse the stored coordinates
        path = path.reverse();
    }

    //Return the path
    return path;
};

/*
    MapGenerator : spawnObstacles - Spawn the obstacle objects on the map
    Created: 05/11/2017
    Modified: 05/11/2017

    param[in] pMap - The 2D array of string values which represent the map
    param[in] pWidth - The width of the pMap array
    param[in] pHeight - The height of the pMap array
*/
MapGenerator.prototype.spawnObstacles = function(pMap, pWidth, pHeight) {
    //Loop through the map and randomly place obstacles on the free cells
    for (let x = 0; x < pWidth; x++) {
        for (let y = 0; y < pHeight; y++) {
            //Check if this cell is free
            if (pMap[x][y] === SymbolRegistry["Free"] && 
                Math.random() <= this.obstacleChance)
                pMap[x][y] = SymbolRegistry["Blocked"];
        }
    }
};