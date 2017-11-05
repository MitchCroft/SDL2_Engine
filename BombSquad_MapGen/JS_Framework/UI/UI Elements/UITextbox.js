/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                   State Defines                                            ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: TextboxStates
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Purpose:
 *		Name the different states a Textbox exist in
 **/
let TextboxStates = { DEFAULT: 0, SELECTED: 1 };

/*
 *		Name: ValidCharacters
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Purpose:
 *		Store bit flag values for the types of characters that can
 *		be entered into a Textbox
 **/
let ValidCharacters = { ALPHABETICAL: 1, NUMERICAL: 2, SPACE: 4, SPECIAL: 8, ALPHANUM: 3, ALL: 15 };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UITextbox
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Color.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		A simple implementation of a Textbox, primarily visible however will
 *		track key presses when active
 **/

/*
	UITextbox : Constructor - Initialise with default values
	20/05/2017

	param[in] pSetup - An object with values used to setup the Textbox
*/
function UITextbox(pSetup) {
	//Call the UIBase setup
	UIBase.call(this, pSetup);

	//Set default values
	this.__Internal__Dont__Modify__.state = TextboxStates.DEFAULT;
	this.__Internal__Dont__Modify__.textBuffer = "";

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////--------------------------------------------Extract Values--------------------------------------------////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Pull simple values
	this.selected = Validate.type(pSetup["selected"], "boolean", false);
	this.__Internal__Dont__Modify__.validCharacters = Math.clamp(Validate.type(pSetup["validCharacters"], "number", ValidCharacters.ALL), 0, ValidCharacters.ALL);
	this.__Internal__Dont__Modify__.maximumLength = Validate.type(pSetup["maximumLength"], "number", -1);
	this.__Internal__Dont__Modify__.text = Validate.type(pSetup["text"], "string", "");
	this.__Internal__Dont__Modify__.font = Validate.type(pSetup["font"], "string", "12px Arial");

	this.__Internal__Dont__Modify__.textColor = (typeof pSetup["textColor"] === "string" ? new Color(pSetup["textColor"]) : Validate.instance(pSetup["textColor"], Color));
	this.__Internal__Dont__Modify__.outlineColor = (typeof pSetup["outlineColor"] === "string" ? new Color(pSetup["outlineColor"]) : Validate.instance(pSetup["outlineColor"], Color));

	this.__Internal__Dont__Modify__.align = Validate.type(pSetup["align"], "number", TextAlign.CENTER);
	this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pSetup["outlineDistance"], "number", 1);

	this.__Internal__Dont__Modify__.borderWidth = Validate.type(pSetup["borderWidth"], "number", 1);

	//Setup as empty arrays
	this.__Internal__Dont__Modify__.fillColors = [];
	this.__Internal__Dont__Modify__.borderColors = [];

	//Retrieve the array values in the setup
	let fillArray = Validate.instance(pSetup["fillColors"], Array);
	let borderArray = Validate.instance(pSetup["borderColors"], Array);

	//Loop through the states and test values
	for (var i = 0; i <= TextboxStates.SELECTED; i++) {
		//Check the fill array
		if (!(fillArray[i] instanceof Color) && (typeof fillArray[i] !== "string")) {
			//Get the inverse of progress
			let invProg = 1 - (i / (TextboxStates.SELECTED + 1));

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

	//Save the value of this
	let self = this;

	//Setup the event listener for detecting and adding characters
	window.addEventListener("keypress", function(pEvt) {
		//Check to see if the textbox is active
		if (!self.__Internal__Dont__Modify__.state) return;

		//Conver the keyCode to a character
		let char = String.fromCharCode(pEvt.keyCode);

		if (/[\x00-\x1F]/.test(char)) return;

		//Check to see if the value can be used
		if ((String.isAlpha(char) && self.__Internal__Dont__Modify__.validCharacters & ValidCharacters.ALPHABETICAL) ||
			(String.isDigit(char) && self.__Internal__Dont__Modify__.validCharacters & ValidCharacters.NUMERICAL) ||
			(char === " " && self.__Internal__Dont__Modify__.validCharacters & ValidCharacters.SPACE) ||
			self.__Internal__Dont__Modify__.validCharacters & ValidCharacters.SPECIAL)
			self.__Internal__Dont__Modify__.textBuffer += char;
	}, false);

	//Setup te event listener for detecting backspace
	window.addEventListener("keydown", function(pEvt) {
		//Check to see if the textbox is active
		if (!self.__Internal__Dont__Modify__.state) return;

		//Check if the key pressed is the backspace
		if (pEvt.keyCode === 8) {
			//Determine if there are keys to subtract from the buffer
			if (self.__Internal__Dont__Modify__.textBuffer.length)
				self.__Internal__Dont__Modify__.textBuffer = self.__Internal__Dont__Modify__.textBuffer.substr(0, self.__Internal__Dont__Modify__.textBuffer.length - 1);

			//Otherwise subtract from the actual text
			else if (self.__Internal__Dont__Modify__.text.length)
				self.__Internal__Dont__Modify__.text = self.__Internal__Dont__Modify__.text.substr(0, self.__Internal__Dont__Modify__.text.length - 1);
		}
	});
};

ExtendProperties(UITextbox, UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UITextbox : selected - Get the current selection state of the Textbox
		20/05/2017

		return bool - Returns true if the Textbox is currently selected
	*/
	get selected() {
		return (this.__Internal__Dont__Modify__.state === TextboxStates.SELECTED);
	},

	/*
		UITextbox : selected - Set the selection state of the Textbox
		20/05/2017

		param[in] pState - A boolean value representing the new state
	*/
	set selected(pState) {
		this.__Internal__Dont__Modify__.state = (Validate.type(pState, "boolean", 0, true) ? TextboxStates.SELECTED : TextboxStates.DEFAULT);
	},

	/*
		UITextbox : validCharacters - Get the bitmask of the valid characters that can be entered into the Textbox
		20/05/2017

		return number - Returns a bitmask numerical value, corresponding to the ValidCharacters object
	*/
	get validCharacters() {
		return this.__Internal__Dont__Modify__.validCharacters;
	},

	/*
		UITextbox : validCharacters - Set the bitmask of the valid characters that can be entered into the Textbox
		20/05/2017

		param[in] pVal - A bitmask numerical value value, corresponding to the ValidCharacters object
	*/
	set validCharacters(pVal) {
		this.__Internal__Dont__Modify__.validCharacters = Math.clamp(Validate.type(pVal, "number", 0, true), 0, ValidCharacters.ALL).toFixed(0);
	},

	/*
		UITextbox : maximumLength - Get the maximum number of characters allowed in the textbox
		20/05/2017

		Note: < 0 values indicate no limit

		return number - Returns a number holding the maximum charcter count
	*/
	get maximumLength() {
		return this.__Internal__Dont__Modify__.maximumLength;
	},

	/*
		UITextbox : maximumLength - Set the maximum number of characters allowed in the textbox
		20/05/2017

		Note: < 0 values indicate no limit

		param[in] pVal - A number indicating the maximum number of characters
	*/
	set maximumLength(pVal) {
		this.__Internal__Dont__Modify__.maximumLength = Validate.type(pVal, "number", -1, true).toFixed(0);
	},

	/*
		UITextbox : text - Get the text of the Textbox
		20/05/2017

		return string - Returns the text as string
	*/
	get text() {
		return this.__Internal__Dont__Modify__.text;
	},

	/*
		UITextbox : text - Set the text of the Textbox
		20/05/2017

		param[in] pText - A string value holding the text to be displayed
	*/
	set text(pText) {
		this.__Internal__Dont__Modify__.text = Validate.type(pText, "string", "ERROR", true);
	},

	/*
		UITextbox : font - Get the font of the Textbox
		20/05/2017

		return string - Returns the font as string
	*/
	get font() {
		return this.__Internal__Dont__Modify__.font;
	},

	/*
		UITextbox : font - Set the font for the Textbox
		20/05/2017

		param[in] pFont - A string value golding the font to use
	*/
	set font(pFont) {
		this.__Internal__Dont__Modify__.font = Validate.type(pFont, "string", "ERROR", true);
	},

	/*
		UITextbox : textColor - Get the color being used to draw the text
		20/05/2017

		return Color - Returns a color object
	*/
	get textColor() {
		return new Color(this.__Internal__Dont__Modify__.textColor);
	},

	/*
		UITextbox : textColor - Set the color being used to draw the text
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
		UITextbox : outlineColor - Get the color being used to outline the text
		20/05/2017

		return Color - Returns a color object
	*/
	get outlineColor() {
		return new Color(this.__Internal__Dont__Modify__.outlineColor);
	},

	/*
		UITextbox : outlineColor - Set the color being used to outline the text
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
		UITextbox : align - Get the text alignment of the Textbox
		20/05/2017

		return number - Returns the alignment as a number
	*/
	get align() {
		return this.__Internal__Dont__Modify__.align;
	},

	/*
		UITextbox : align - Set the text alignment of the Textbox
		20/05/2017

		param[in] pAlign - A number from the TextAlign define object
	*/
	set align(pAlign) {
		this.__Internal__Dont__Modify__.align = Validate.type(pAlign, "number", 0, true);
	},

	/*
		UITextbox : outlineDistance - Get the distance of the outline around the text
		20/05/2017

		return number - Returns the distance as a number
	*/
	get outlineDistance() {
		return this.__Internal__Dont__Modify__.outlineDistance;
	},

	/*
		UITextbox : outlineDistance - Set the distance of the outline around the text
		20/05/2017

		param[in] pDist - A number defining the distance to outline the text
	*/
	set outlineDistance(pDist) {
		this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pDist, "number", 0, true);
	},

	/*
		UITextbox : borderWidth - Get the border width for the outline color
		20/05/2017

		return number - Returns a number defining the border width
	*/
	get borderWidth() {
		return this.__Internal__Dont__Modify__.borderWidth;
	},

	/*
		UITextbox : borderWidth - Set the border width for the outline color
		20/05/2017

		param[in] pVal - A number representing the new border width
	*/
	set borderWidth(pVal) {
		this.__Internal__Dont__Modify__.borderWidth = Validate.type(pVal, "number", 0, true);
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               State Functions                                              ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UITextbox : setStateFill - Set the fill color for a specific state
		20/05/2017

		param[in] pState - A number indicating the state to modify
		param[in] pCol - A color object to use for the state
	*/
	setStateFill: function(pState, pCol) {
		//Clean the parameters
		pState = Math.clamp(Validate.type(pState, "number", 0, true), 0, TextboxStates.PRESSED).toFixed(0);
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the state color
		this.__Internal__Dont__Modify__.fillColors[pState] = new Color(pCol);
	},

	/*
		UITextbox : setStateBorder - Set the border color for a specific state
		20/05/2017

		param[in] pState - A number indicating the state to modify
		param[in] pCol - A color object to use for the state
	*/
	setStateBorder: function(pState, pCol) {
		//Clean the parameters
		pState = Math.clamp(Validate.type(pState, "number", 0, true), 0, TextboxStates.PRESSED).toFixed(0);
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the state color
		this.__Internal__Dont__Modify__.borderColors[pState] = new Color(pCol);
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                  Main Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UITextbox : update - Update the text with the textbuffer values
		20/05/2017
	*/
	update: function() {
		//Check if there is anything in the buffer to be added
		if (this.__Internal__Dont__Modify__.textBuffer.length) {
			//Add the new characters onto the string
			this.__Internal__Dont__Modify__.text += this.__Internal__Dont__Modify__.textBuffer;

			//Remove excess characters
			if (this.__Internal__Dont__Modify__.maximumLength >= 0 && this.__Internal__Dont__Modify__.text.length > this.__Internal__Dont__Modify__.maximumLength)
				this.__Internal__Dont__Modify__.text = this.__Internal__Dont__Modify__.text.substr(0, this.__Internal__Dont__Modify__.maximumLength);

			//Empty the text buffer 
			this.__Internal__Dont__Modify__.textBuffer = "";
		}
	},

	/*
		UITextbox : draw - Draw the button to the screen
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
	}
});