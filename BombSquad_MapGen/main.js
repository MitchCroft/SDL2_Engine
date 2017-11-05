//Create the map generator
let gen = new MapGenerator();
 
//Store the map dimensions
let MAP_WIDTH = 25;
let MAP_HEIGHT = 75;

//Create the Input Manager
let input = new Input();

/*
    printMap - Print a map of characters to the console
    Author: Mitchell Croft
    Created: 04/11/2017
    Modified: 104/11/2017 

    param[in] pMap - The 2D array of string values to output
*/
function printMap(pMap) {
    //Store a single string to use as output
    let output = "";

    //Loop through the map of values
    for (let x = 0; x < pMap.length; x++) {
        for (let y = 0; y < pMap[x].length; y++) {
            output += pMap[x][y];
        }

        //Output the newline character
        output += "\n";
    }

    //Output the generated string
    console.log(output);
};

/*
    gameLoop - Update and display generated map
    Author: Mitchell Croft
    Created: 03/11/2017
    Modified: 05/11/2017

    param[in] pTime - A Time object with the values for the cycle
*/
function gameLoop(pTime) {
    //Update the input manager
    input.update(pTime.deltaTime);

    //Check the time has passed
    if (input.inputPressed(Keys.SPACE)) {
        //Clear previous information
        console.clear();

        //Retrieve the generated map
        let map = gen.generate(MAP_WIDTH, MAP_HEIGHT, Math.ceil(Math.randomRange(1, 4)));

        //Output the map
        printMap(map);
    }
}

//Setup the state manager
let stateManager = new StateManager(gameLoop);