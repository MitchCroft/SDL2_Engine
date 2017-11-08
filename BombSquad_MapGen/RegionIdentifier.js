/*
 *      Name: RegionIdentifier
 *      Author: Mitchell Croft
 *      Created: 04/11/2017
 *      Modified: 04/11/2017
 * 
 *      Purpose:
 *      Identify specific areas of coordiantes on a 2D array of strings
 **/

/*
    RegionIdentifier : Constructor - Initialise with default values
    Created: 04/11/2017
    Modified: 04/11/2017

    param[in] pIdent - A string value identifying the specifier for the region
    param[in] pMap - The 2D array of string values representing the map
*/
function RegionIdentifier(pIdent, pMap) {
    //! Store the width of the map
    this.mapWidth = pMap.length;

    //! Check there is a width
    if (!this.mapWidth) throw new Error("Can not identify regions on a map with no width");

    //! Store the height of the map
    this.mapHeight = pMap[this.mapWidth - 1].length;

    //Check there is a height
    if (!this.mapHeight) throw new Error("Can not identify regions on a map with no height");

    //! Store a list of regions that where found on the map
    this.regions = [];

    //! Store a hitmap of processed cells
    let hitMap = [];

    //! Setup intial hitmap
    for (let i = 0; i < this.mapWidth; i++) {
        //! Set initial values
        hitMap[i] = [];
        hitMap[i].length = this.mapHeight;
        hitMap[i].fill(0);
    }

    //! Store the offset values to apply when looking for enighbours
    let offsets = [new Vec2(-1, 0), new Vec2(1, 0), new Vec2(0, -1), new Vec2(0, 1)];

    //! Loop through the hitmap and find areas that haven't been processed
    for (let x = 0; x < this.mapWidth; x++) {
        for (let y = 0; y < this.mapHeight; y++) {
            //! Check to see if the cell has been processed
            if (!hitMap[x][y]) {
                //! Flag as searched
                hitMap[x][y] = 1;

                //! See if the identifier is a match
                if (pMap[x][y] === pIdent) {
                    //! Store a list of all the coords found in the region
                    let enclosedCoords = [];

                    //! Store a list of the unsearched coordinates
                    let unsearched = [new Vec2(x, y)];

                    //! Process the unsearched coords to find all connected cells
                    let current = null;
                    while (current = unsearched.shift()) {
                        //! Add the coordinate to the enclosed list
                        enclosedCoords.push(current);

                        //! Loop through the neighbouring cells
                        for (let i = 0; i < offsets.length; i++) {
                            //! Get the offset value
                            let offset = current.add(offsets[i]);

                            //! Check offset is in bounds of map
                            if (offset.x < 0 || offset.y < 0 ||
                                offset.x >= this.mapWidth || offset.y >= this.mapHeight)
                                continue;

                            //! Determine if the point has already been searched
                            else if (hitMap[offset.x][offset.y]) continue;

                            //! Flag the point as searched
                            hitMap[offset.x][offset.y] = 1;

                            //! Check point is of the same identifier type
                            if (pMap[offset.x][offset.y] !== pIdent) continue;

                            //! Add the point to the unsearched list
                            unsearched.push(offset);
                        }
                    }

                    //! Add the new region to the internal list
                    this.regions.push(new Region(enclosedCoords, this.mapWidth, this.mapHeight));
                }
            }
        }
    }

    //! Sort the regions based on area
    this.regions.sort(function(pFirst, pSecond) {
        return pSecond.coordinates.length - pFirst.coordinates.length; });
};