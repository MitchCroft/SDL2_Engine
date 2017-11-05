/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                   State Defines                                            ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: ButtonStates
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Purpose:
 *		Name the different states a Button exist in
 **/
let ButtonStates = { DEFAULT: 0, HOVER: 1, PRESSED: 2 };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UIButton
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Color.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		A simple solely visual representation of a 'button' that
 *		must be managed from an external point
 **/

/*
	UIButton : Constructor - Initialise with default values
	20/05/2017

	param[in] pSetup - An object with values used to setup the Button
*/
function UIButton(pSetup) {
	//Call the UIBase setup
	UIBase.call(this, pSetup);

	//Set default state
	this.__Internal__Dont__Modify__.state = ButtonStates.DEFAULT;

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////--------------------------------------------Extract Values--------------------------------------------////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Pull simple values
	this.__Internal__Dont__Modify__.text = Validate.type(pSetup["text"], "string", "");
	this.__Internal__Dont__Modify__.font = Validate.type(pSetup["font"], "string", "12px Arial");

	this.__Internal__Dont__Modify__.textColor = (typeof pSetup["textColor"] === "string" ? new Color(pSetup["textColor"]) : Validate.instance(pSetup["textColor"], Color));
	this.__Internal__Dont__Modify__.outlineColor = (typeof pSetup["outlineColor"] === "string" ? new Color(pSetup["outlineColor"]) : Validate.instance(pSetup["outlineColor"], Color));

	this.__Internal__Dont__Modify__.align = Validate.type(pSetup["align"], "number", TextAlign.CENTER);
	this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pSetup["outlineDistance"], "number", 1);

	this.__Internal__Dont__Modify__.borderWidth = Validate.type(pSetup["borderWidth"], "number", 1);

	this.__Internal__Dont__Modify__.clickEvent = Validate.type(pSetup["clickEvent"], "function", null);

	//Setup as empty arrays
	this.__Internal__Dont__Modify__.fillColors = [];
	this.__Internal__Dont__Modify__.borderColors = [];

	//Retrieve the array values in the setup
	let fillArray = Validate.instance(pSetup["fillColors"], Array);
	let borderArray = Validate.instance(pSetup["borderColors"], Array);

	//Loop through the states and test values
	for (var i = 0; i <= ButtonStates.PRESSED; i++) {
		//Check the fill array
		if (!(fillArray[i] instanceof Color) && (typeof fillArray[i] !== "string")) {
			//Get the inverse of progress
			let invProg = 1 - (i / (ButtonStates.PRESSED + 1));

			//Set the Color value
			fillArray[i] = new Color(255 * invProg, 255 * invProg, 255 * invProg);
		}

		//Clone the color object
		else fillArray[i] = new Color(fillArray[i]);

		//Check the border array
		if (!(borderArray[i] instanceof Color) && (typeof borderArray[i] !== "string"))
			borderArray[i] = new Color();

		//Clone the color object
		else borderArray[i] = new Color(borderArray[i]);
	};

	//Assign the arrays to the internal
	this.__Internal__Dont__Modify__.fillColors = fillArray;
	this.__Internal__Dont__Modify__.borderColors = borderArray;
};

ExtendProperties(UIButton, UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIButton : state - Get the current state of the Button
		20/05/2017

		return number - Returns a number representing the state, as defined in the ButtonStates object
	*/
	get state() {
		return this.__Internal__Dont__Modify__.state;
	},

	/*
		UIButton : state - Set the current state of the button
		20/05/2017

		param[in] pState - A number representing the new state, as defined in the ButtonStates object
	*/
	set state(pState) {
		this.__Internal__Dont__Modify__.state = Math.clamp(Validate.type(pState, "number", 0, true), 0, ButtonStates.PRESSED).toFixed(0);
	},

	/*
		UIButton : text - Get the text of the Button
		20/05/2017

		return string - Returns the text as string
	*/
	get text() {
		return this.__Internal__Dont__Modify__.text;
	},

	/*
		UIButton : text - Set the text of the Button
		20/05/2017

		param[in] pText - A string value holding the text to be displayed
	*/
	set text(pText) {
		this.__Internal__Dont__Modify__.text = Validate.type(pText, "string", "ERROR", true);
	},

	/*
		UIButton : font - Get the font of the Button
		20/05/2017

		return string - Returns the font as string
	*/
	get font() {
		return this.__Internal__Dont__Modify__.font;
	},

	/*
		UIButton : font - Set the font for the Button
		20/05/2017

		param[in] pFont - A string value golding the font to use
	*/
	set font(pFont) {
		this.__Internal__Dont__Modify__.font = Validate.type(pFont, "string", "ERROR", true);
	},

	/*
		UIButton : textColor - Get the color being used to draw the text
		20/05/2017

		return Color - Returns a color object
	*/
	get textColor() {
		return new Color(this.__Internal__Dont__Modify__.textColor);
	},

	/*
		UIButton : textColor - Set the color being used to draw the text
		20/05/2017

		param[in] pCol - A color object holding the values to draw the text in
	*/
	set textColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.textColor = new Color(pCol);
	},

	/*
		UIButton : outlineColor - Get the color being used to outline the text
		20/05/2017

		return Color - Returns a color object
	*/
	get outlineColor() {
		return new Color(this.__Internal__Dont__Modify__.outlineColor);
	},

	/*
		UIButton : outlineColor - Set the color being used to outline the text
		20/05/2017

		param[in] pCol - A color object holding the values to outline the text in
	*/
	set outlineColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.outlineColor = new Color(pCol);
	},

	/*
		UIButton : align - Get the text alignment of the Button
		20/05/2017

		return number - Returns the alignment as a number
	*/
	get align() {
		return this.__Internal__Dont__Modify__.align;
	},

	/*
		UIButton : align - Set the text alignment of the Button
		20/05/2017

		param[in] pAlign - A number from the TextAlign define object
	*/
	set align(pAlign) {
		this.__Internal__Dont__Modify__.align = Validate.type(pAlign, "number", 0, true);
	},

	/*
		UIButton : outlineDistance - Get the distance of the outline around the text
		20/05/2017

		return number - Returns the distance as a number
	*/
	get outlineDistance() {
		return this.__Internal__Dont__Modify__.outlineDistance;
	},

	/*
		UIButton : outlineDistance - Set the distance of the outline around the text
		20/05/2017

		param[in] pDist - A number defining the distance to outline the text
	*/
	set outlineDistance(pDist) {
		this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pDist, "number", 0, true);
	},

	/*
		UIButton : borderWidth - Get the border width for the outline color
		20/05/2017

		return number - Returns a number defining the border width
	*/
	get borderWidth() {
		return this.__Internal__Dont__Modify__.borderWidth;
	},

	/*
		UIButton : borderWidth - Set the border width for the outline color
		20/05/2017

		param[in] pVal - A number representing the new border width
	*/
	set borderWidth(pVal) {
		this.__Internal__Dont__Modify__.borderWidth = Validate.type(pVal, "number", 0, true);
	},

	/*
		UIButton : clickEvent - Set the function assigned to this buttons click event
		20/05/2017

		param[in] pFunc - The function to be called for this buttons clicked event
	*/
	set clickEvent(pFunc) {
		//Check the type is a function or null
		if (pFunc !== null && !Validate.type(pFunc, "function", null, true)) return;

		//Assign the callback
		this.__Internal__Dont__Modify__.clickEvent = pFunc;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               State Functions                                              ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIButton : setStateFill - Set the fill color for a specific state
		20/05/2017

		param[in] pState - A number indicating the state to modify
		param[in] pCol - A color object to use for the state
	*/
	setStateFill: function(pState, pCol) {
		//Clean the parameters
		pState = Math.clamp(Validate.type(pState, "number", 0, true), 0, ButtonStates.PRESSED).toFixed(0);
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the state color
		this.__Internal__Dont__Modify__.fillColors[pState] = new Color(pCol);
	},

	/*
		UIButton : setStateBorder - Set the border color for a specific state
		20/05/2017

		param[in] pState - A number indicating the state to modify
		param[in] pCol - A color object to use for the state
	*/
	setStateBorder: function(pState, pCol) {
		//Clean the parameters
		pState = Math.clamp(Validate.type(pState, "number", 0, true), 0, ButtonStates.PRESSED).toFixed(0);
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the state color
		this.__Internal__Dont__Modify__.borderColors[pState] = new Color(pCol);
	},

	/*
		UIButton : click - Trigger the internal click event callbakc
		20/05/2017
	*/
	click: function() {
		if (this.__Internal__Dont__Modify__.clickEvent !== null)
			this.__Internal__Dont__Modify__.clickEvent();
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                  Main Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIButton : draw - Draw the button to the screen
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Set the color styles
		pGraphics.draw.fillStyle = this.__Internal__Dont__Modify__.fillColors[this.__Internal__Dont__Modify__.state].rgba;
		pGraphics.draw.strokeStyle = this.__Internal__Dont__Modify__.borderColors[this.__Internal__Dont__Modify__.state].rgba;

		//Set the border line width
		pGraphics.draw.lineWidth = this.__Internal__Dont__Modify__.borderWidth;

		//Draw the rectangles
		pGraphics.draw.fillRect(this.__Internal__Dont__Modify__.x,
			this.__Internal__Dont__Modify__.y, this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);
		pGraphics.draw.strokeRect(this.__Internal__Dont__Modify__.x,
			this.__Internal__Dont__Modify__.y, this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);

		//Check there is text to render
		if (this.__Internal__Dont__Modify__.text === "") return;

		//Set the font
		pGraphics.draw.font = this.__Internal__Dont__Modify__.font;

		//Calculate the text offset values
		let txtXOff = 0;
		let txtYOff = this.__Internal__Dont__Modify__.height / 2;

		//Switch on the text alignment type
		switch (this.__Internal__Dont__Modify__.align) {
			case TextAlign.CENTER:
				txtXOff += this.__Internal__Dont__Modify__.width / 2;
				break;
			case TextAlign.RIGHT:
				txtXOff += this.__Internal__Dont__Modify__.width;
				break;
		}

		//Render the text
		pGraphics.outlineText(this.__Internal__Dont__Modify__.text, this.__Internal__Dont__Modify__.x + txtXOff, this.__Internal__Dont__Modify__.y + txtYOff,
			this.__Internal__Dont__Modify__.textColor, this.__Internal__Dont__Modify__.outlineColor, this.__Internal__Dont__Modify__.align,
			this.__Internal__Dont__Modify__.outlineDistance);
	},
});