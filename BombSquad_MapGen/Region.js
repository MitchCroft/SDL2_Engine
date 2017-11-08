/*
 *      Name: Region
 *      Author: Mitchell Croft
 *      Created: 04/11/2017
 *      Modified: 05/11/2017
 * 
 *      Purpose:
 *      Monitor a collection of coordinates as a Region on a 2D string map
 *      and at request find the distance of each coord from an edge
 **/

/*
    Region : Constructor - Initialise with a collection of values
    Created: 04/11/2017
    Modified: 04/11/2017

    param[in] pCoords - An array of Vec2 objects containing enclosed
    param[in] pWidth - The width of the map the coordinates are on
    param[in] pHeight - The height of the map the coordinates are on
*/
function Region(pCoords, pWidth, pHeight) {
    //! Store the dimenions of the map
    this.mapWidth = pWidth;
    this.mapHeight = pHeight;

    //! Store the coordinates in the region
    this.coordinates = [];

    //! Store the 'center of mass' for the region
    this.center = new Vec2();

    //! Store the regions that are linked together
    this.links = [];

    //! Store a hitmap of the different coordinates
    this.hitMap = [];

    //! Store a map of the distances of each coord from an edge
    this.depthMap = [];

    //! Loop through and read from the parameter
    for (let i = 0; i < pCoords.length; i++) {
        //! Get the coordinate value to use
        let coord = pCoords[i];

        //! Store the cooridinate in the internal map
        this.coordinates[i] = coord;

        //! Add to the center of mass
        this.center.addSet(coord);

        //! Check the index exists in the info maps
        if (!(this.hitMap[coord.x] instanceof Array)) {
            this.hitMap[coord.x] = [];
            this.depthMap[coord.x] = [];
        }

        //! Register the point in the hitMap
        this.hitMap[coord.x][coord.y] = "X";

        //! Assign the default depth value
        this.depthMap[coord.x][coord.y] = 0;
    }

    //! Average out the center of mass
    this.center.divSet(this.coordinates.length);

    //! Store a list of coordinates on the edge of the region
    this.edges = [];

    //! Store a list of the axis offsets to check for edges
    let edgeCheckOffsets = [new Vec2(-1, 0), new Vec2(1, 0), new Vec2(0, -1), new Vec2(0, 1)];

    //! Loop through the coordinates and check if its on the edge
    for (let i = 0; i < this.coordinates.length; i++) {
        //! Get the coordinate
        let coord = this.coordinates[i];

        //! Check if coordinate is on the bounds of the map
        if (coord.x === 0 || coord.x === pWidth - 1 ||
            coord.y === 0 || coord.y === pHeight - 1);

        //! Check if the coordinate has (at least) one non-contained coordinate
        else {
            //! Loop through the axis offset values
            let found = false;
            for (let j = 0; j < edgeCheckOffsets.length; j++) {
                //! The offset position to check the hitmap at
                let offset = coord.add(edgeCheckOffsets[j]);

                //! Check the offset coords are on the map
                if (offset.x < 0 || offset.y < 0 || offset.x >= pWidth || offset.y >= pHeight)
                    continue;

                //! Ensure the check bounds on the 'x' axis are valid arrays
                else if (!(this.hitMap[offset.x] instanceof Array))
                    this.hitMap[offset.x] = [];

                //! Check if the offset coordinate is not on the hit map
                if (this.hitMap[offset.x][offset.y] !== "X") {
                    found = true;
                    break;
                }
            }

            //! Check if an edge was found
            if (!found) continue;
        }

        //! Add the coordinate to the edges list
        this.edges.push(coord);
    }
};

/*
    Region : analyseDepth - Flood search each coordinate to find the distance from an edge coordinate
    Created: 04/11/2017
    Modified: 05/11/2017
*/
Region.prototype.analyseDepth = function() {
    //Create a temporary coordinate element that tracks position data and the object which added it to the search list
    function CoordTrace(pVec, pParent) {
        //Store the coordinates
        this.pos = Validate.instance(pVec, Vec2, null, true);

        //Store the parent of the coord
        this.parent = Validate.instance(pParent, CoordTrace, null);
    };

    //Store the offsets used to find neighbours
    let offsets = [new Vec2(-1, 0), new Vec2(1, 0), new Vec2(0, -1), new Vec2(0, 1), new Vec2(-1, -1), new Vec2(-1, 1), new Vec2(1, 1), new Vec2(1, -1)];

    //Loop through the coordinates to find their depth
    for (let i = 0; i < this.coordinates.length; i++) {
        //Store the coordinate to retrieve the value of
        let coord = this.coordinates[i];

        //Store a list of the Coord Traces to investigate
        let unsearched = [];

        //Store a map of the searched coordinates
        let searchMap = [];

        //Add the current coordinate to the list
        unsearched.push(new CoordTrace(coord));

        //Register the first coordiante
        searchMap[coord.x] = [];
        searchMap[coord.x][coord.y] = 1;

        //Flag if the search has gone out of bounds
        let outOfBounds = false;

        //Loop through and process trace coords
        let current = null;
        while (current = unsearched.shift()) {
            //Check to see if this point is not on the hitmap
            if (this.hitMap[current.pos.x][current.pos.y] !== "X")
                break;

            //Add neighbouring cells to the unsearched list
            for (let j = 0; j < offsets.length; j++) {
                //Get the offset position
                let offset = current.pos.add(offsets[j]);

                //Check the coordinate is in bounds
                if (offset.x < 0 || offset.y < 0 ||
                    offset.x >= this.mapWidth || offset.y >= this.mapHeight) {
                    outOfBounds = true;
                    break;
                }

                //Check the position is valid on the searched map
                if (!(searchMap[offset.x] instanceof Array))
                    searchMap[offset.x] = [];

                //Check if offset coordinate has been searched
                if (searchMap[offset.x][offset.y] !== 1) {
                    //Add the coordinate to the search list
                    unsearched.push(new CoordTrace(offset, current));

                    //Flag coordinate on searched map
                    searchMap[offset.x][offset.y] = 1;
                }
            }

            //If search has gone out fo bounds break
            if (outOfBounds) break;
        }

        //Check to see if the edge was found
        if (current) {
            //Reset the depth value
            this.depthMap[coord.x][coord.y] = -1;

            //Trace back path
            while (current !== null) {
                this.depthMap[coord.x][coord.y]++;
                current = current.parent;
            }
        }
    }
};