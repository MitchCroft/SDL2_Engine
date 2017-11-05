/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                            Object Definition                                               ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: Input
 *		Author: Mitchell Croft
 *		Date: 06/07/2017
 *
 *		Requires:
 *		Vec2.js, ExtendProperties.js
 *
 *		Version: 3.0
 *		Cleaned up object definition and useage to match
 *		other elements in the framework
 *
 *		Purpose:
 *		Manage changes in input states and provide an interface
 *		for using that information within a game project. To use, 
 *		call update function once per cycle (i.e. game loop)
 **/

/*
	Input : Constructor - Initialise with default values
	06/07/2017

	param[in] pCanvas - The canvas object to use for correcting mouse positions
*/
function Input(pCanvas) {
    //Ensure that the canvas is valid
    switch (typeof pCanvas) {
        case "object":
            if (pCanvas.nodeName === "CANVAS") break;
        default:
            pCanvas = null;
    }

    /*  WARNING:
        Don't modify this internal object from the outside of the Input object.
        Instead use Input object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
    this.__Internal__Dont__Modify__ = {
        //Track changes in key states per cycle
        prevKeyStates: [],
        curKeyStates: [],

        //Store changes of input inbetween cycles
        bufferStates: [],

        //Store a map of the different Virtual Axis objects
        axisObjects: {},

        //Store a map of the different Virtual Axis values
        axisValues: {},

        //Store the position of the mouse cursor
        mousePos: new Vec2(),

        //Store a reference to the canvas in use for mouse coordinate correction
        screenCanvas: pCanvas,
    };

    //Setup the event callbacks
    window.addEventListener("keydown", this.onKeyDown.bind(this), false);
    window.addEventListener("keyup", this.onKeyUp.bind(this), false);
    window.addEventListener("mousedown", this.onMouseDown.bind(this), false);
    window.addEventListener("mouseup", this.onMouseUp.bind(this), false);
    window.addEventListener("mousemove", this.onMouseMove.bind(this), false);
};

ExtendProperties(Input, {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                               Property Definitions                                         ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
    	Input : mousePos - Get the current position of the mouse on the window
    	06/07/2017

    	return Vec2 - Returns a Vec2 object holding the position data
    */
    get mousePos() {
        //Create a vector object to hold the values
        let pos = new Vec2(this.__Internal__Dont__Modify__.mousePos);

        //Correct position based on canvas
        if (this.__Internal__Dont__Modify__.screenCanvas !== null) {
            pos.x -= this.__Internal__Dont__Modify__.screenCanvas.offsetLeft;
            pos.y -= this.__Internal__Dont__Modify__.screenCanvas.offsetTop;
        }

        //Return the position
        return pos;
    },

    /*
    	Input : canvas - Set the canvas that is being used for input correction
    	06/07/2017

    	param[in] pCnv - A CanvasRenderingContext2D object to be used for input correction
    */
    set canvas(pCnv) {
        this.__Internal__Dont__Modify__.screenCanvas = Validate.instance(pCnv, CanvasRenderingContext2D, null, true);
    },

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                                 Input Functions                                            ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
		Input : inputDown - Check if the passed in Input value is currently down
		06/07/2017

		param[in] pVal - An integral number reflecting the input to check as described in
						 the Keys and Buttons objects

		return bool - Returns true if the input is down
    */
    inputDown: function(pVal) {
        return this.__Internal__Dont__Modify__.curKeyStates[pVal];
    },

    /*
		Input : inputUp - Check if the passed in Input value is currently up
		06/07/2017

		param[in] pVal - An integral number reflecting the input to check as described in
						 the Keys and Buttons objects

		return bool - Returns true if the input is up
    */
    inputUp: function(pVal) {
        return !this.__Internal__Dont__Modify__.curKeyStates[pVal];
    },

    /*
		Input : inputPressed - Check to see if the Input value has been pressed this cycle
		06/07/2017

		param[in] pVal - An integral number reflecting the input to check as described in
						 the Keys and Buttons objects

		return bool - Returns true if the input was pressed this cycle
    */
    inputPressed: function(pVal) {
        return (this.__Internal__Dont__Modify__.curKeyStates[pVal] && !this.__Internal__Dont__Modify__.prevKeyStates[pVal]);
    },

    /*
		Input : inputReleased - Check to see if the Input value was released this cycle
		06/07/2017

		param[in] pVal - An integral number reflecting the input to check as described in
						 the Keys and Buttons objects

		return bool - Returns true if the input was released this cycle
    */
    inputReleased: function(pVal) {
        return (!this.__Internal__Dont__Modify__.curKeyStates[pVal] && this.__Internal__Dont__Modify__.prevKeyStates[pVal]);
    },

    /*
        Input : getAxis - Get the axis vlaue of a specified key
        07/07/2017

        param[in] pKey - A string value that represents the axis vlaue to retrieve

        return number - Returns a number in the range of -1 to 1
    */
    getAxis: function(pKey) {
        return (pKey in this.__Internal__Dont__Modify__.axisValues ? this.__Internal__Dont__Modify__.axisValues[pKey] : 0);
    },

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                                 Axis Functions                                             ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
		Input : addAxis - Add a new INput Axis object to be used by the Input Manager
		06/07/2017

		param[in] pAxis - The new Input Axis object to add to the Input Manager

		return bool - Returns true if the Axis was successfully added to the manager
    */
    addAxis: function(pAxis) {
        //Verify input value
        pAxis = Validate.instance(pAxis, InputAxis, null, true);

        //Check if the axis already exists
        if (!(pAxis.name in this.__Internal__Dont__Modify__.axisObjects)) {
            //Create the axis value
            this.__Internal__Dont__Modify__.axisValues[pAxis.name] = 0;

            //Create the array for Input Axis objects with this name
            this.__Internal__Dont__Modify__.axisObjects[pAxis.name] = [];
        }

        //Loop through all existing Virtual Axis objects for copy
        for (var i = this.__Internal__Dont__Modify__.axisObjects[pAxis.name].length - 1; i >= 0; i--) {
            //Check if Virtual Axis object already exists
            if (this.__Internal__Dont__Modify__.axisObjects[pAxis.name][i] === pAxis)
                return false;
        }

        //Add the Axis object to the map
        this.__Internal__Dont__Modify__.axisObjects[pAxis.name].push(pAxis);

        //Return successful
        return true;
    },

    /*
		Input : removeAxis - Remove a Input Axis object from the Input Manager
		06/07/2017

		param[in] pAxis - The Input Axis object to remove

		return bool - Returns true if the object was successfully removed
    */
    removeAxis: function(pAxis) {
        //Verify input value
        pAxis = Validate.instance(pAxis, InputAxis, null, true);

        //Check if the name exists in the map
        if (!(pAxis.name in this.__Internal__Dont__Modify__.axisObjects)) return false;

        //Look through to find matching Input Axis
        for (var i = this.__Internal__Dont__Modify__.axisObjects[pAxis.name].length - 1; i >= 0; i--) {
            if (this.__Internal__Dont__Modify__.axisObjects[pAxis.name][i] === pAxis) {
                //Remove the object from the list
                this.__Internal__Dont__Modify__.axisObjects[pAxis.name].splice(i, 1);

                //Check to see if there are other Axis objects
                if (!this.__Internal__Dont__Modify__.axisObjects[pAxis.name].length) {
                    //Remove the Axis values and objects arrays
                    delete this.__Internal__Dont__Modify__.axisValues[pAxis.name];
                    delete this.__Internal__Dont__Modify__.axisObjects[pAxis.name];
                }

                //Return success
                return true;
            }
        }

        //Default return failure
        return false;
    },

    /*
		Input : clearAxis - Completly remove all Input Axis objects with a specific name
		06/07/2017

		param[in] pName - A string containing the name of the axis to clear

		return bool - Returns true if the axis was cleared
    */
    clearAxis: function(pName) {
        //Clean parameter
        pName = Validate.type(pName, "string", "", true);

        //Check if the name exists in the map
        if (!(pName in this.__Internal__Dont__Modify__.axisObjects)) return false;

        //Remove the Axis values and objects arrays
        delete this.__Internal__Dont__Modify__.axisValues[pName];
        delete this.__Internal__Dont__Modify__.axisObjects[pName];

        //Return successful
        return true;
    },

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                               Main Function                                                ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
		Input : update - Update the Input states based on the changes since the last call
		06/07/2017

		Requires:
		This functionality requires the function to be called once per cycle. Call this at the start of each

		param[in] pDelta - The delta time for the cycle
    */
    update: function(pDelta) {
        //Loop thorugh the key states and copy values over
        for (var key in this.__Internal__Dont__Modify__.bufferStates) {
            //Ensure own property
            if (!this.__Internal__Dont__Modify__.bufferStates.hasOwnProperty(key)) continue;

            //Copy the current state to the previous register
            this.__Internal__Dont__Modify__.prevKeyStates[key] = this.__Internal__Dont__Modify__.curKeyStates[key];

            //Copy the current state from the buffer
            this.__Internal__Dont__Modify__.curKeyStates[key] = this.__Internal__Dont__Modify__.bufferStates[key];
        }

        //Update the axis values
        for (var axisName in this.__Internal__Dont__Modify__.axisObjects) {
            //Ensure own property
            if (!this.__Internal__Dont__Modify__.axisObjects.hasOwnProperty(axisName)) continue;

            //Track the strongest input for the axis
            let strengthVal = 0;

            //Store the average gravity of all InputAxis objects
            let gravAvg = 0;

            //Loop through the axis objects
            for (var i = this.__Internal__Dont__Modify__.axisObjects[axisName].length - 1; i >= 0; i--) {
                //Store a reference to the axis object
                let obj = this.__Internal__Dont__Modify__.axisObjects[axisName][i];

                //Add the gravity to the running sum
                gravAvg += obj.gravity;

                //Track the total strength being applied
                let objStrength = 0;

                //Check for the positive keys
                if (this.__Internal__Dont__Modify__.curKeyStates[obj.positiveKey] ||
                    this.__Internal__Dont__Modify__.curKeyStates[obj.altPositiveKey])
                    objStrength += obj.strength;

                //Check for the negative keys
                if (this.__Internal__Dont__Modify__.curKeyStates[obj.negativeKey] ||
                    this.__Internal__Dont__Modify__.curKeyStates[obj.altNegativeKey])
                    objStrength -= obj.strength;

                //Check if this axis is stronger then the previous
                if (Math.abs(objStrength) > Math.abs(strengthVal))
                    strengthVal = objStrength;
            }

            //Test if there is any strength to apply
            if (strengthVal) {
                //Add the strength value
                this.__Internal__Dont__Modify__.axisValues[axisName] += strengthVal * pDelta;

                //Clamp the axis values from -1 to 1
                this.__Internal__Dont__Modify__.axisValues[axisName] = Math.clamp(this.__Internal__Dont__Modify__.axisValues[axisName], -1, 1);
            }

            //Otherwise apply gravity
            else {
                //Get the direction to apply gravity
                let dir = Math.sign(this.__Internal__Dont__Modify__.axisValues[axisName]) * -1;

                //Average out the gravity value
                gravAvg /= this.__Internal__Dont__Modify__.axisObjects[axisName].length;

                //Get the gravity value applied to the current value
                let appliedVal = this.__Internal__Dont__Modify__.axisValues[axisName] + gravAvg * dir * pDelta;

                //Assign the axis value
                this.__Internal__Dont__Modify__.axisValues[axisName] = (Math.sign(appliedVal) === dir ? 0 : appliedVal);
            }
        }
    },

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                              Callback Functions                                            ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
		Input : onKeyDown - Event callback for keyboard presses while on web page
		06/07/2017

		param[in] pEvt - Information about the event that occured
    */
    onKeyDown: function(pEvt) {
        this.__Internal__Dont__Modify__.bufferStates[pEvt.keyCode] = true;
    },

    /*
		Input : onKeyUp - Event callback for keyboard releases while on web page
		06/07/2017

		param[in] pEvt - Information about the event that occured
    */
    onKeyUp: function(pEvt) {
        this.__Internal__Dont__Modify__.bufferStates[pEvt.keyCode] = false;
    },

    /*
		Input : onMouseDown - Event callback for mouse presses while on web page
		06/07/2017

		param[in] pEvt - Information about the event that occured
    */
    onMouseDown: function(pEvt) {
        //Support IE (For some reason)
        if (!pEvt.which && pEvt.button) {
            if (pEvt.button & Buttons.LEFT_CLICK) pEvt.which = Buttons.LEFT_CLICK;
            else if (pEvt.button & Buttons.MIDDLE_CLICK) pEvt.which = Buttons.MIDDLE_CLICK;
            else if (pEvt.button & Buttons.RIGHT_CLICK) pEvt.which = Buttons.RIGHT_CLICK;
        }

        //Set the mouse down value
        this.__Internal__Dont__Modify__.bufferStates[pEvt.which] = true;
    },

    /*
		Input : onMouseUp - Event callback for mouse releases while on web page
		06/07/2017

		param[in] pEvt - Information about the event that occured
    */
    onMouseUp: function(pEvt) {
        //Support IE (For some reason)
        if (!pEvt.which && pEvt.button) {
            if (pEvt.button & Buttons.LEFT_CLICK) pEvt.which = Buttons.LEFT_CLICK;
            else if (pEvt.button & Buttons.MIDDLE_CLICK) pEvt.which = Buttons.MIDDLE_CLICK;
            else if (pEvt.button & Buttons.RIGHT_CLICK) pEvt.which = Buttons.RIGHT_CLICK;
        }

        //Set the mouse up value
        this.__Internal__Dont__Modify__.bufferStates[pEvt.which] = false;
    },

    /*
		Input : onMouseMove - Event callback for mouse movement while on web page
		06/07/2017

		param[in] pEvt - Information about the event that occured
    */
    onMouseMove: function(pEvt) {
        //Set the position
        this.__Internal__Dont__Modify__.mousePos.x = pEvt.pageX;
        this.__Internal__Dont__Modify__.mousePos.y = pEvt.pageY;
    },
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                            Object Definition                                               ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: InputAxis
 *      Author: Mitchell Croft
 *      Date: 6/07/2017
 *
 *		Requires:
 *		ExtendProperties.js
 *
 *		Version: 2.0
 *		Added property accessors
 *
 *      Purpose:
 *      Allow for scaleable input return based on a combination
 *      of input values (Key or button)
 **/

/*
    InputAxis : Constructor - Initialise with default values
    06/07/2017

    param[in] pSetup - An object with values used to setup the panel
*/
function InputAxis(pSetup) {
    //Ensure that pSetup is an object
    pSetup = Validate.type(pSetup, "object", null, true);

    /*  WARNING:
        Don't modify this internal object from the outside of the InputAxis object.
        Instead use InputAxis object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
    this.__Internal__Dont__Modify__ = {
        //Store the name of the axis that this object effects
        name: Validate.type(pSetup["name"], "string", "", true),

        //Store key values that effect the axis value
        positiveKey: Validate.type(pSetup["positiveKey"], "number", 0),
        negativeKey: Validate.type(pSetup["negativeKey"], "number", 0),

        //Store the alternative key values
        altPositiveKey: Validate.type(pSetup["altPositiveKey"], "number", 0),
        altNegativeKey: Validate.type(pSetup["altNegativeKey"], "number", 0),

        //Store the strength of the input axis 
        strength: Validate.type(pSetup["strength"], "number", 1),

        //Store the gravity of the input axis
        gravity: Validate.type(pSetup["gravity"], "number", 1),
    };
};

ExtendProperties(InputAxis, {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                               Property Definitions                                         ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
    	InputAxis : name - Get the name of the specified virtual axis
    	06/07/2017

    	return string - Returns the name as a string
    */
    get name() {
        return this.__Internal__Dont__Modify__.name;
    },

    /*
    	InputAxis : positiveKey - Get the key being used as positive input
    	06/07/2017

    	return number - Returns the key as an integral number defined in the Keys and Buttons objects
    */
    get positiveKey() {
        return this.__Internal__Dont__Modify__.positiveKey;
    },

    /*
    	InputAxis : positiveKey - Set the key being used as positive input
    	06/07/2017

    	param[in] pKey - An integral number storing a value defined in the Keys and Buttons objects
    */
    set positiveKey(pKey) {
        this.__Internal__Dont__Modify__.positiveKey = Math.abs(Math.round(Validate.type(pKey, "number", 0, true)));
    },

    /*
    	InputAxis : negativeKey - Get the key being used as negative input
    	06/07/2017

    	return number - Returns the key as an integral number defined in the Keys and Buttons objects
    */
    get negativeKey() {
        return this.__Internal__Dont__Modify__.negativeKey;
    },

    /*
    	InputAxis : negativeKey - Set the key being used as negative input
    	06/07/2017

    	param[in] pKey - An integral number storing a value defined in the Keys and Buttons objects
    */
    set negativeKey(pKey) {
        this.__Internal__Dont__Modify__.negativeKey = Math.abs(Math.round(Validate.type(pKey, "number", 0, true)));
    },

    /*
    	InputAxis : altPositiveKey - Get the alternate key being used as positive input
    	06/07/2017

    	return number - Returns the key as an integral number defined in the Keys and Buttons objects
    */
    get altPositiveKey() {
        return this.__Internal__Dont__Modify__.altPositiveKey;
    },

    /*
    	InputAxis : altPositiveKey - Set the alternate key being used as positive input
    	06/07/2017

    	param[in] pKey - An integral number storing a value defined in the Keys and Buttons objects
    */
    set altPositiveKey(pKey) {
        this.__Internal__Dont__Modify__.altPositiveKey = Math.abs(Math.round(Validate.type(pKey, "number", 0, true)));
    },

    /*
    	InputAxis : altNegativeKey - Get the alternate key being used as negative input
    	06/07/2017

    	return number - Returns the key as an integral number defined in the Keys and Buttons objects
    */
    get altNegativeKey() {
        return this.__Internal__Dont__Modify__.altNegativeKey;
    },

    /*
    	InputAxis : altNegativeKey - Set the alternate key being used as negative input
    	06/07/2017

    	param[in] pKey - An integral number storing a value defined in the Keys and Buttons objects
    */
    set altNegativeKey(pKey) {
        this.__Internal__Dont__Modify__.altNegativeKey = Math.abs(Math.round(Validate.type(pKey, "number", 0, true)));
    },

    /*
    	InputAxis : strength - Get the strength value of the Virtual Axis
    	06/07/2017

    	return number - Returns the strength as a positive number
    */
    get strength() {
        return this.__Internal__Dont__Modify__.strength;
    },

    /*
    	InputAxis : strength - Set the strength value of the Virtual Axis
    	06/07/2017

    	param[in] pVal - A number value containing the new strength
    */
    set strength(pVal) {
        this.__Internal__Dont__Modify__.strength = Math.abs(Validate.type(pVal, "number", 0, true));
    },

    /*
    	InputAxis : gravity - Get the gravity value of the Virtual Axis
    	06/07/2017

    	return number - Returns the gravity as a positive number
    */
    get gravity() {
        return this.__Internal__Dont__Modify__.gravity;
    },

    /*
    	InputAxis : gravity - Set the gravity value of the Virtual Axis
    	06/07/2017

    	param[in] pVal - A number value containing the new gravity
    */
    set gravity(pVal) {
        this.__Internal__Dont__Modify__.gravity = Math.abs(Validate.type(pVal, "number", 0, true));
    },
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                               Key Defines                                                  ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Keys
 *      Author: Mitchell Croft
 *      Date: 15/06/2016
 *
 *      Purpose:
 *      Name the numerical values with the key's that they correspond with
 *      to enable input coding in a readable fashion. These values should
 *      be altered 
 *
 *      Key codes taken from http://www.cambiaresearch.com/articles/15/javascript-key-codes
 **/

let Keys = {
    //NUMBERS
    NUM0: 48,
    NUM1: 49,
    NUM2: 50,
    NUM3: 51,
    NUM4: 52,
    NUM5: 53,
    NUM6: 54,
    NUM7: 55,
    NUM8: 56,
    NUM9: 57,
    PAD0: 96,
    PAD1: 97,
    PAD2: 98,
    PAD3: 99,
    PAD4: 100,
    PAD5: 101,
    PAD6: 102,
    PAD7: 103,
    PAD8: 104,
    PAD9: 105,

    //LETTERS
    A: 65,
    B: 66,
    C: 67,
    D: 68,
    E: 69,
    F: 70,
    G: 71,
    H: 72,
    I: 73,
    J: 74,
    K: 75,
    L: 76,
    M: 77,
    N: 78,
    O: 79,
    P: 80,
    Q: 81,
    R: 82,
    S: 83,
    T: 84,
    U: 85,
    V: 86,
    W: 87,
    X: 88,
    Y: 89,
    Z: 90,

    //SPECIAL CHARACTERS
    BACKSPACE: 8,
    TAB: 9,
    ENTER: 13,
    SHIFT: 16,
    CTRL: 17,
    ALT: 18,
    PAUSE: 19,
    CAPS: 20,
    ESCAPE: 27,
    SPACE: 32,
    PAGE_UP: 33,
    PAGE_DOWN: 34,
    END: 35,
    HOME: 36,
    LEFT: 37,
    UP: 38,
    RIGHT: 39,
    DOWN: 40,
    INSERT: 45,
    DELETE: 46,
    LEFT_WIN: 91,
    RIGHT_WIN: 92,
    SELECT: 93,
    MULTIPLY: 106,
    ADD: 107,
    SUBTRACT: 109,
    DECIMAL_POINT: 110,
    DIVIDE: 111,
    NUM_LOCK: 144,
    SCOLL_LOCK: 145,
    SEMI_COLON: 186,
    EQUAL_SIGN: 187,
    COMMA: 188,
    DASH: 189,
    PERIOD: 190,
    FORWARD_SLASH: 191,
    BACK_SLASH: 220,
    TILDE: 192,
    OPEN_BRACKET: 219,
    CLOSE_BRACKET: 221,
    SINGLE_QUOTE: 222,

    //FUNCTION KEYS
    F1: 112,
    F2: 113,
    F3: 114,
    F4: 115,
    F5: 116,
    F6: 117,
    F7: 118,
    F8: 119,
    F9: 120,
    F10: 121,
    F11: 122,
    F12: 123
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                           Mouse Button Defines                                             ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Buttons
 *      Author: Mitchell Croft
 *      Date: 15/06/2016
 *
 *      Purpose:
 *      Name the numerical values that correspond to the different mouse 
 *      buttons to enable input coding in a readable fashion. These values
 *      should not be altered.
 **/

let Buttons = { LEFT_CLICK: 1, MIDDLE_CLICK: 2, RIGHT_CLICK: 3 };