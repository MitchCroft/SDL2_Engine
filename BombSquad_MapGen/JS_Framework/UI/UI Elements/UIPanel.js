/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UIPanel
 *		Author: Mitchell Croft
 *		Date: 20/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Color.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		A simple display for an image or solid rectangle
 **/

/*
	UIPanel : Constructor - Initialise with default values
	20/05/2017

	param[in] pSetup - An object with values used to setup the panel
*/
function UIPanel(pSetup) {
	//Call the UIBase setup
	UIBase.call(this, pSetup);

	//Extract the setup values
	this.__Internal__Dont__Modify__.fillColor = (typeof pSetup["fillColor"] === "string" ? new Color(pSetup["fillColor"]) : Validate.instance(pSetup["fillColor"], Color));
	this.__Internal__Dont__Modify__.borderColor = Validate.instance(pSetup["borderColor"], Color);
	this.__Internal__Dont__Modify__.borderWidth = Validate.type(pSetup["borderWidth"], "number", 1);
	this.__Internal__Dont__Modify__.image = ("image" in pSetup ? Validate.instance(pSetup["image"], Image, null) : null);
};

ExtendProperties(UIPanel, UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIPanel : fillColor - Get the fill color being used for this panel
		20/05/2017

		return Color - Returns a color object
	*/
	get fillColor() {
		return new Color(this.__Internal__Dont__Modify__.fillColor);
	},

	/*
		UIPanel : fillColor - Set the new fill color for this panel
		20/05/2017

		param[in] pCol - A color object holding the new fill values
	*/
	set fillColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.fillColor = new Color(pCol);
	},

	/*
		UIPanel : borderColor - Get the border color being used for this panel
		20/05/2017

		return Color - Returns a color object
	*/
	get borderColor() {
		return new Color(this.__Internal__Dont__Modify__.borderColor);
	},

	/*
		UIPanel : borderColor - Set the border color being used for this panel
		20/05/2017

		param[in] pCol - A color object holding the new border values
	*/
	set borderColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.borderColor = new Color(pCol);
	},

	/*
		UIPanel : borderWidth - Get the border width for the outline color
		20/05/2017

		return number - Returns a number defining the border width
	*/
	get borderWidth() {
		return this.__Internal__Dont__Modify__.borderWidth;
	},

	/*
		UIPanel : borderWidth - Set the border width for the outline color
		20/05/2017

		param[in] pVal - A number representing the new border width
	*/
	set borderWidth(pVal) {
		this.__Internal__Dont__Modify__.borderWidth = Validate.type(pVal, "number", 0, true);
	},

	/*
		UIPanel : image - Set the image being used as a display for this panel
		20/05/2017

		param[in] pImg - An image object to be used to display on the panel
	*/
	set image(pImg) {
		//Check the type
		if (!pImg instanceof Image && pImg !== null)
			throw new Error("Can not set UIPanel image to " + pImg + " (Type: " + typeof pImg + ") Please use an Image object");

		//Set the value
		this.__Internal__Dont__Modify__.image = pImg;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                  Main Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIPanel : draw - Draw the panel to the screen
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Check if there is an image to draw
		if (this.__Internal__Dont__Modify__.image !== null)
			pGraphics.draw.drawImage(this.__Internal__Dont__Modify__.image, this.__Internal__Dont__Modify__.x,
				this.__Internal__Dont__Modify__.y, this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);

		//Draw the panel
		else {
			//Set the styles
			pGraphics.draw.fillStyle = this.__Internal__Dont__Modify__.fillColor.rgba;
			pGraphics.draw.strokeStyle = this.__Internal__Dont__Modify__.borderColor.rgba;

			//Set the border line width
			pGraphics.draw.lineWidth = this.__Internal__Dont__Modify__.borderWidth;

			//Draw the rectangles
			pGraphics.draw.fillRect(this.__Internal__Dont__Modify__.x,
				this.__Internal__Dont__Modify__.y, this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);
			pGraphics.draw.strokeRect(this.__Internal__Dont__Modify__.x,
				this.__Internal__Dont__Modify__.y, this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);
		}
	},
});