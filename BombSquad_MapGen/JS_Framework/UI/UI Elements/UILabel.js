/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UILabel
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Color.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		A simple display of text
 **/

/*
	UILabel : Constructor - Initialise with default values
	20/05/2017

	param[in] pSetup - An object with values used to setup the label
*/
function UILabel(pSetup) {
	//Call the UIBase setup
	UIBase.call(this, pSetup);

	//Extract the setup values
	this.__Internal__Dont__Modify__.text = Validate.type(pSetup["text"], "string", "");
	this.__Internal__Dont__Modify__.font = Validate.type(pSetup["font"], "string", "12px Arial");

	this.__Internal__Dont__Modify__.textColor = (typeof pSetup["textColor"] === "string" ? new Color(pSetup["textColor"]) : Validate.instance(pSetup["textColor"], Color));
	this.__Internal__Dont__Modify__.outlineColor = (typeof pSetup["outlineColor"] === "string" ? new Color(pSetup["outlineColor"]) : Validate.instance(pSetup["outlineColor"], Color));

	this.__Internal__Dont__Modify__.align = Validate.type(pSetup["align"], "number", TextAlign.CENTER);
	this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pSetup["outlineDistance"], "number", 1);
};

ExtendProperties(UILabel, UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UILabel : text - Get the text of the label
		20/05/2017

		return string - Returns the text as string
	*/
	get text() {
		return this.__Internal__Dont__Modify__.text;
	},

	/*
		UILabel : text - Set the text of the label
		20/05/2017

		param[in] pText - A string value holding the text to be displayed
	*/
	set text(pText) {
		this.__Internal__Dont__Modify__.text = Validate.type(pText, "string", "ERROR", true);
	},

	/*
		UILabel : font - Get the font of the label
		20/05/2017

		return string - Returns the font as string
	*/
	get font() {
		return this.__Internal__Dont__Modify__.font;
	},

	/*
		UILabel : font - Set the font for the label
		20/05/2017

		param[in] pFont - A string value golding the font to use
	*/
	set font(pFont) {
		this.__Internal__Dont__Modify__.font = Validate.type(pFont, "string", "ERROR", true);
	},

	/*
		UILabel : textColor - Get the color being used to draw the text
		20/05/2017

		return Color - Returns a color object
	*/
	get textColor() {
		return new Color(this.__Internal__Dont__Modify__.textColor);
	},

	/*
		UILabel : textColor - Set the color being used to draw the text
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
		UILabel : outlineColor - Get the color being used to outline the text
		20/05/2017

		return Color - Returns a color object
	*/
	get outlineColor() {
		return new Color(this.__Internal__Dont__Modify__.outlineColor);
	},

	/*
		UILabel : outlineColor - Set the color being used to outline the text
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
		UILabel : align - Get the text alignment of the label
		20/05/2017

		return number - Returns the alignment as a number
	*/
	get align() {
		return this.__Internal__Dont__Modify__.align;
	},

	/*
		UILabel : align - Set the text alignment of the label
		20/05/2017

		param[in] pAlign - A number from the TextAlign define object
	*/
	set align(pAlign) {
		this.__Internal__Dont__Modify__.align = Validate.type(pAlign, "number", 0, true);
	},

	/*
		UILabel : outlineDistance - Get the distance of the outline around the text
		20/05/2017

		return number - Returns the distance as a number
	*/
	get outlineDistance() {
		return this.__Internal__Dont__Modify__.outlineDistance;
	},

	/*
		UILabel : outlineDistance - Set the distance of the outline around the text
		20/05/2017

		param[in] pDist - A number defining the distance to outline the text
	*/
	set outlineDistance(pDist) {
		this.__Internal__Dont__Modify__.outlineDistance = Validate.type(pDist, "number", 0, true);
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                  Main Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UILabel : draw - Draw the label to the screen
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Check there is text to render
		if (this.__Internal__Dont__Modify__.text === "") return;

		//Set the font
		pGraphics.draw.font = this.__Internal__Dont__Modify__.font;

		//Render the text
		pGraphics.outlineText(this.__Internal__Dont__Modify__.text, this.__Internal__Dont__Modify__.x, this.__Internal__Dont__Modify__.y,
			this.__Internal__Dont__Modify__.textColor, this.__Internal__Dont__Modify__.outlineColor, this.__Internal__Dont__Modify__.align,
			this.__Internal__Dont__Modify__.outlineDistance);
	},
});