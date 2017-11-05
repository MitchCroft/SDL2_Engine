/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: Canvas
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Graphics.js, Mat3.js
 *
 *		Version: 2.0
 *		Collections of UI objects as well as loading based on objects
 *		
 *		Purpose:
 *		Store and display UI elements for rendering to a
 *		HTML 5 canvas object
 **/

/*
	Canvas : Constructor - Initialise the Canvas with default values
	20/05/2017

	param[in] pDisplayX - A number defining the screen X units position to start displaying UI in
	param[in] pDisplayY - A number defining the screen Y units position to start displaying UI in
	param[in] pDisplayWidth - A number defining the screen width units to display UI in
	param[in] pDisplayHeight - A number defining the screen height units to display UI in
*/
function Canvas(pDisplayX, pDisplayY, pDisplayWidth, pDisplayHeight) {
	/*  WARNING:
        Don't modify this internal object from the outside of the Canvas object.
        Instead use Canvas object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Store the display offset positions
		dispX: pDisplayX,
		dispY: pDisplayY,

		//Store the display dimensions
		dispWidth: pDisplayWidth,
		dispHeight: pDisplayHeight,

		//Store the UI elements to display
		uiElements: [],

		//Keep track of the different groups of UI elements
		uiGroups: [],

		//Store callback information for UI management
		groupCallbacks: []
	};
};

ExtendProperties(Canvas, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : displayX - Get the current screen X display position
		20/05/2017

		return number - Returns the position as a number
    */
	get displayX() {
		return this.__Internal__Dont__Modify__.dispX;
	},

	/*
		Canvas : displayX - Set the current screen X display position
		20/05/2017

		param[in] pVal - A number defining the new canvas screen X position 
	*/
	set displayX(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set Canvas displayX to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.dispX = pVal;
	},

	/*
		Canvas : displayY - Get the current screen Y display position
		20/05/2017

		return number - Returns the position as a number
	*/
	get displayY() {
		return this.__Internal__Dont__Modify__.dispY;
	},

	/*
		Canvas : displayY - Set the current screen Y display position
		20/05/2017

		param[in] pVal - A number defining the new canvas screen Y position 
	*/
	set displayY(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set Canvas displayY to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.dispY = pVal;
	},

	/*
		Canvas : displayWidth - Get the current screen display width
		20/05/2017

		return number - Returns the width as a number
	*/
	get displayWidth() {
		return this.__Internal__Dont__Modify__.dispWidth;
	},

	/*
		Canvas : displayWidth - Set the current screen display width
		20/05/2017

		param[in] pVal - A number defining the new canvas screen width
	*/
	set displayWidth(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set Canvas displayWidth to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.dispWidth = pVal;
	},

	/*
		Canvas : displayHeight - Get the current screen display height
		20/05/2017

		return number - Returns the height as a number
	*/
	get displayHeight() {
		return this.__Internal__Dont__Modify__.dispHeight;
	},

	/*
		Canvas : displayHeight - Set the current screen display height
		20/05/2017

		param[in] pVal - A number defining the new canvas screen height
	*/
	set displayHeight(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set Canvas displayHeight to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.dispHeight = pVal;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               General Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : clear - Clear all information stored on the Canvas
		21/05/2017
	*/
	clear: function() {
		this.clearUI();
		this.clearGroups();
		this.clearCallbacks();
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                            UI Management Functions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : createElement - Create a new UI element, adding it to the Canvas 
		20/05/2017

		param[in] pIdent - A string identifying the type of object to attempt to create
		param[in] pSetup - An object that can be filled with values to be used in the initialisation
						   of the UI component
		param[in] pGroup - Optional string identifier for the group to place the element in (Default null)

		return UI/bool - If the function succeeds it returns a UI object of the identified type,
						 otherwise it returns false
	*/
	createElement: function(pIdent, pSetup, pGroup) {
		//Clean the group flag
		pGroup = Validate.type(pGroup, "string", null);

		//Evaluate the type of object to create
		let evaledType = eval(pIdent);

		//Check the type is correct
		if (typeof evaledType !== "function") return false;

		//Store a map of values to multiply position values by
		let posOut = {
			x: this.__Internal__Dont__Modify__.dispWidth / 100,
			y: this.__Internal__Dont__Modify__.dispHeight / 100,
			width: this.__Internal__Dont__Modify__.dispWidth / 100,
			height: this.__Internal__Dont__Modify__.dispHeight / 100
		};

		//Adjust position values in setup
		if (pSetup) {
			//Adjust those values
			for (var pos in posOut) {
				//Skip non-owned properties
				if (!posOut.hasOwnProperty(pos)) continue;

				//Check to see if the settings contains the position value
				if (pos in pSetup)
					pSetup[pos] *= posOut[pos];
			}
		}

		//Create the new object
		let obj = new evaledType((typeof pSetup === "object" ? pSetup : {}));

		//Test to ensure it is of UIBase
		if (!obj instanceof UIBase) return false;

		//Insert the UI elemnt into the array
		this.__Internal__Dont__Modify__.uiElements.push(obj);

		//Determine if the element should be added to a group
		if (pGroup) {
			//Ensure the group exists
			this.createGroup(pGroup);

			//Add the UI element to the group
			this.__Internal__Dont__Modify__.uiGroups[pGroup].push(obj);

			//Store the group name on the UI element
			obj.__Group__ = pGroup;
		}

		//Return the new UI element
		return obj;
	},

	/*
		Canvas : loadFromJSON - Load UI elements from a JSON object
		21/05/2017

		param[in] pJSON - An object storing the values required to setup the 
		param[in] pClearFlags - An object with bool flags for "ui", "groups" and/or "callbacks"
								indicating what should removed (Default null)
	*/
	loadFromJSON: function(pJSON, pClearFlags) {
		//Validate the parameters
		pJSON = Validate.type(pJSON, "object", null, true);
		pClearFlags = Validate.type(pClearFlags, "object", {});

		//Check the clear flags
		if (pClearFlags["ui"]) this.clearUI();
		if (pClearFlags["groups"]) this.clearGroups();
		if (pClearFlags["callbacks"]) this.clearCallbacks();

		//Loop through the different properties of the JSON object
		for (var name in pJSON) {
			//Skip non-owned properties
			if (!pJSON.hasOwnProperty(name)) continue;

			//Create the UI element
			let ui = this.createElement(pJSON[name]["type"], pJSON[name]["settings"], pJSON[name]["group"]);

			//Tag the UI element with its name
			ui.tag = name;
		}
	},

	/*
		Canvas : findWithTag - Find a UI element with a specific tag
		21/05/2017

		param[in] pTag - The tag of the UI element to look for

		return UI Element - Returns the first UI element found with a matching tag, or null if not found
	*/
	findWithTag: function(pTag) {
		//Validate the parameter
		pTag = Validate.type(pTag, "string", "", true);

		//Loop through all UI elements
		for (var i = 0; i < this.__Internal__Dont__Modify__.uiElements.length; i++) {
			if (this.__Internal__Dont__Modify__.uiElements[i].tag === pTag)
				return this.__Internal__Dont__Modify__.uiElements[i];
		}

		//Default not found
		return null;
	},

	/*
		Canvas : findAllWithTag - Find all UI elements with a specific tag
		21/05/2017

		param[in] pTag - The tag of the UI element to look for

		return UI ELement Array - Returns an array of the UI elements found
	*/
	findAllWithTag: function(pTag) {
		//Validate the parameter
		pTag = Validate.type(pTag, "string", "", true);

		//Create an array to store the found elements
		let found = [];

		//Loop through all UI elements
		for (var i = 0; i < this.__Internal__Dont__Modify__.uiElements.length; i++) {
			if (this.__Internal__Dont__Modify__.uiElements[i].tag === pTag)
				found.push(this.__Internal__Dont__Modify__.uiElements[i]);
		}

		//Return the found elements
		return found;
	},

	/*
		Canvas : removeElement - Remove a specific UI element from the Canvas
		20/05/2017

		param[in] pUI - The UI object to be removed from the Canvas

		return bool - Returns true if the UI element was successfully removed 
	*/
	removeElement: function(pUI) {
		//Validate the parameter
		pUI = Validate.instance(pUI, UIBase, null, true);

		//Loop through the existing UI elements
		for (var i = this.__Internal__Dont__Modify__.uiElements.length - 1; i >= 0; i--) {
			//Check to see if the element matches the parameter
			if (this.__Internal__Dont__Modify__.uiElements[i] === pUI) {
				//Check to see if the UI element belongs to a group
				if ("__Group__" in pUI) {
					//Check to see if the group still exists
					if (pUI.__Group__ in this.__Internal__Dont__Modify__.uiGroups) {
						//Loop through the group to try and remove the UI element
						for (var k = 0; k < this.__Internal__Dont__Modify__.uiGroups[pUI.__Group__].length; k++) {
							//Check for match
							if (this.__Internal__Dont__Modify__.uiGroups[pUI.__Group__][k] === pUI) {
								//Splice the element out
								this.__Internal__Dont__Modify__.uiGroups[pUI.__Group__].splice(k, 1);

								//Exit the search
								break;
							}
						}
					}
				}

				//Splice the element from the array
				this.__Internal__Dont__Modify__.uiElements.splice(i, 1);

				//Return success
				return true;
			}
		}

		//Default return failure
		return false;
	},

	/*
		Canvas : clearUI - Remove all UI elements from the Canvas
		20/05/2017
	*/
	clearUI: function() {
		for (var i = this.__Internal__Dont__Modify__.uiElements.length - 1; i >= 0; i--) {
			this.removeElement(this.__Internal__Dont__Modify__.uiElements[i]);
		}
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                          UI Group Management Functions                                     ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : createGroup - Create a named UI group
		21/05/2017

		param[in] pName - A string defining the name of the group

		return bool - Returns true if the group was created
	*/
	createGroup: function(pName) {
		//Validate the parameter
		pName = Validate.type(pName, "string", "", true);

		//Check to see if the group already exists
		if (pName in this.__Internal__Dont__Modify__.uiGroups)
			return false;

		//Create the new group
		this.__Internal__Dont__Modify__.uiGroups[pName] = [];

		//Return success
		return true;
	},

	/*
		Canvas : removeGroup - Remove a named UI group
		21/05/2017

		param[in] pName - A string defining the name of the group

		return bool - Returns true if the group was removed
	*/
	removeGroup: function(pName) {
		//Validate the parameter
		pName = Validate.type(pName, "string", "", true);

		//Check to see if the group already exists
		if (!(pName in this.__Internal__Dont__Modify__.uiGroups))
			return false;

		//Remove the group
		delete this.__Internal__Dont__Modify__.uiGroups[pName];

		//Return success
		return true;
	},

	/*
		Canvas : clearGroups - Remove all UI groups from the Canvas
		21/05/2017
	*/
	clearGroups: function() {
		this.__Internal__Dont__Modify__.uiGroups = [];
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                       Group Callback Management Functions                                  ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : addGroupCallback - Add a callback for a group of UI elements
		21/05/2017

		param[in] pGroup - The name of the group to apply the callback to
		param[in] pCB - A function that takes a single UI element as a parameter
	*/
	addGroupCallback: function(pGroup, pCB) {
		//Validate the parameters
		pGroup = Validate.type(pGroup, "string", "", true);
		pCB = Validate.type(pCB, "function", null, true);

		//Add the callback
		this.__Internal__Dont__Modify__.groupCallbacks[pGroup] = pCB;
	},

	/*
		Canvas : removeGroupCallback - Remove a callback for a group of UI elements
		21/05/2017

		param[in] pGroup - The name of the group to remove the callback for
	*/
	removeGroupCallback: function(pGroup) {
		//Validate the parameters
		pGroup = Validate.type(pGroup, "string", "", true);

		//Remove the callback
		if (pGroup in this.__Internal__Dont__Modify__.groupCallbacks)
			delete this.__Internal__Dont__Modify__.groupCallbacks[pGroup];
	},

	/*
		Canvas : clearCallbacks - Remove all callbacks from the Canvas
		21/05/2017
	*/
	clearCallbacks: function() {
		this.__Internal__Dont__Modify__.groupCallbacks = [];
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                 Main Functions                                             ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : update - Update the current UI elements
		20/05/2017

		param[in] pDelta - The delta time for the current cycle
	*/
	update: function(pDelta) {
		//Loop through the available callbacks
		for (var cb in this.__Internal__Dont__Modify__.groupCallbacks) {
			//Skip non-properties
			if (!this.__Internal__Dont__Modify__.groupCallbacks.hasOwnProperty(cb))
				continue;

			//Check to see if there is a group with the name
			if (cb in this.__Internal__Dont__Modify__.uiGroups) {
				//Loop through the elements in the group and raise the callback
				for (var i = 0; i < this.__Internal__Dont__Modify__.uiGroups[cb].length; i++)
					this.__Internal__Dont__Modify__.groupCallbacks[cb](this.__Internal__Dont__Modify__.uiGroups[cb][i]);

			}
		}

		//Loop through UI elements
		for (var i = 0; i < this.__Internal__Dont__Modify__.uiElements.length; i++) {
			if (this.__Internal__Dont__Modify__.uiElements[i].update !== null)
				this.__Internal__Dont__Modify__.uiElements[i].update(pDelta);
		}
	},

	/*
		Canvas : draw - Draw all current UI elements
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Setup the rendering matrix
		let uiTransform = createTranslationMat(this.__Internal__Dont__Modify__.dispX, this.__Internal__Dont__Modify__.dispY);

		//Add onto the render stack
		if (pGraphics.pushTransform(uiTransform)) {
			//Loop through the UI elements
			for (var i = 0; i < this.__Internal__Dont__Modify__.uiElements.length; i++) {
				//Check if the UI element is visible
				if (!this.__Internal__Dont__Modify__.uiElements[i].visible) continue;

				//Ensure that there is a draw function to call
				if (this.__Internal__Dont__Modify__.uiElements[i].draw !== null)
					this.__Internal__Dont__Modify__.uiElements[i].draw(pGraphics);
			}

			//End the rendering
			pGraphics.endRender();
		}
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                             Conversion Functions                                           ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Canvas : convertPoint - Convert a point to a position on the Canvas
		20/05/2017

		param[in] pPos - A Vec2 object defining the position to convert
		param[in] pTransform - An optional Mat3 object to be applied to the conversion (Default null)

		return Vec2 - Returns a Vec2 object with the converted position
	*/
	convertPoint: function(pPos, pTransform) {
		//Clean the parameters
		pPos = Validate.instance(pPos, Vec2, null, true);
		pTransform = Validate.instance(pTransform, Mat3);

		//Return the conversion
		return pTransform.multi(createTranslationMat(this.__Internal__Dont__Modify__.dispX, this.__Internal__Dont__Modify__.dispY)).inverse().multiVec(pPos);
	},
});