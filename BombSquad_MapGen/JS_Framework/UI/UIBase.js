/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UIBase
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		Base point for UI elements to inherit from in order
 *		to be added to the Canvas object
 **/

/*
	UIBase : Abstract Constructor - Initialise with default values
	20/05/2017

	param[in] pSetup - An object with values used to setup the panel
*/
function UIBase(pSetup) {
	//Ensure abstract nature of the object
	if (this.constructor === UIBase) throw new Error("Can not instantiate the abstract UIBase object. Create an object that inherits from UIBase");

	/*  WARNING:
        Don't modify this internal object from the outside of the UIBase object.
        Instead use UIBase object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Tag the UI for retrieving functionality
		tag: Validate.type(pSetup["tag"], "string", ""),

		//Flag if the UI element is visible
		visible: Validate.type(pSetup["visible"], "boolean", true),

		//Store percentage dimensions for the UI elements
		x: Validate.type(pSetup["x"], "number", 0),
		y: Validate.type(pSetup["y"], "number", 0),
		width: Validate.type(pSetup["width"], "number", 0),
		height: Validate.type(pSetup["height"], "number", 0)
	};
};

ExtendProperties(UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIBase : tag - Get the tag of the UI object
		21/05/2017

		return string - Returns the tag as a string
	*/
	get tag() {
		return this.__Internal__Dont__Modify__.tag;
	},

	/*
		UIBase : tag - Set the tag of the UI object
		21/05/2017

		param[in] pTag - A string object containing the new tag
	*/
	set tag(pTag) {
		this.__Internal__Dont__Modify__.tag = Validate.type(pTag, "string", 0, true);
	},

	/*
		UIBase : visible - Get the visible flag for the UI object
		20/05/2017

		return bool - Returns the state as a boolean
	*/
	get visible() {
		return this.__Internal__Dont__Modify__.visible;
	},

	/*
		UIBase : visible - Set the visible flag for the UI object
		20/05/2017

		param[in] pState - A boolean value indicating the new visible state
	*/
	set visible(pState) {
		//Check the type
		if (typeof pState !== "boolean")
			throw new Error("Can not set UIBase visible to " + pState + " (Type: " + typeof pState + ") Please use a boolean");

		//Set the value
		this.__Internal__Dont__Modify__.visible = pState;
	},

	/*
		UIBase : x - Get the x for the UI object
		20/05/2017

		return number - Returns the offset as a number
	*/
	get x() {
		return this.__Internal__Dont__Modify__.x;
	},

	/*
		UIBase : x - Set the x for the UI object
		20/05/2017

		param[in] pVal - A number defining the percentage of screen offset to have the UI element
	*/
	set x(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set UIBase x to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.x = pVal;
	},

	/*
		UIBase : y - Get the y for the UI object
		20/05/2017

		return number - Returns the offset as a number
	*/
	get y() {
		return this.__Internal__Dont__Modify__.y;
	},

	/*
		UIBase : y - Set the y for the UI object
		20/05/2017

		param[in] pVal - A number defining the percentage of screen offset to have the UI element
	*/
	set y(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set UIBase y to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.y = pVal;
	},

	/*
		UIBase : width - Get the width for the UI object
		20/05/2017

		return number - Returns the width as a number
	*/
	get width() {
		return this.__Internal__Dont__Modify__.width;
	},

	/*
		UIBase : width - Set the width for the UI object
		20/05/2017

		param[in] pVal - A number defining the percentage of screen width to have the UI element
	*/
	set width(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set UIBase width to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.width = pVal;
	},

	/*
		UIBase : height - Get the height for the UI object
		20/05/2017

		return number - Returns the height as a number
	*/
	get height() {
		return this.__Internal__Dont__Modify__.height;
	},

	/*
		UIBase : height - Set the height for the UI object
		20/05/2017

		param[in] pVal - A number defining the percentage of screen height to have the UI element
	*/
	set height(pVal) {
		//Check the type
		if (typeof pVal !== "number")
			throw new Error("Can not set UIBase height to " + pVal + " (Type: " + typeof pVal + ") Please use a number");

		//Set the value
		this.__Internal__Dont__Modify__.height = pVal;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                 Input Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIBase : contains - Determines if the UI element contains a 2D point
		20/05/2017

		param[in] pPos - A Vec2 object containing the position to test against

		return bool - Returns true if the UI element contains the point
	*/
	contains: function(pPos) {
		//Clean the parameter
		pPos = Validate.instance(pPos, Vec2, null, true);

		//Check the bounds
		return (pPos.x >= this.__Internal__Dont__Modify__.x && pPos.y >= this.__Internal__Dont__Modify__.y &&
			pPos.x <= this.__Internal__Dont__Modify__.x + this.__Internal__Dont__Modify__.width &&
			pPos.y <= this.__Internal__Dont__Modify__.y + this.__Internal__Dont__Modify__.height);
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                Virtual Functions                                           ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIBase : update - Virtual update function to be overriden as needed by child classes
		20/05/2017

		param[in] pDelta - The delta time for the current cycle
	*/
	update: null,

	/*
		UIBase : draw - Virtual draw function to be overriden as needed by child classes
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: null,
});