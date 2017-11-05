/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: UIList
 *		Author: Mitchell Croft
 *		Date: 29/05/2017
 *
 *		Requires:
 *		ExtendProperties.js, UIBase.js, Color.js, Graphics.js
 *
 *		Version: 1.0
 *
 *		Purpose:
 *		Display various items stored in an list
 **/

/*
	UIList : Constructor - Initialise with default values
	29/05/2017

	param[in] pSetup - An object with values used to setup the List
*/
function UIList(pSetup) {
	//Call the UIBase setup
	UIBase.call(this, pSetup);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////--------------------------------------------Extract Values--------------------------------------------////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//General
	this.__Internal__Dont__Modify__.fillColor = (typeof pSetup["fillColor"] === "string" ? new Color(pSetup["fillColor"]) : Validate.instance(pSetup["fillColor"], Color));
	this.__Internal__Dont__Modify__.borderColor = (typeof pSetup["borderColor"] === "string" ? new Color(pSetup["borderColor"]) : Validate.instance(pSetup["borderColor"], Color));
	this.__Internal__Dont__Modify__.borderWidth = Validate.type(pSetup["borderWidth"], "number", 1);

	//Label
	this.__Internal__Dont__Modify__.labelText = Validate.type(pSetup["labelText"], "string", "");
	this.__Internal__Dont__Modify__.labelFont = Validate.type(pSetup["labelFont"], "string", "12px Arial");
	this.__Internal__Dont__Modify__.labelColor = (typeof pSetup["labelColor"] === "string" ? new Color(pSetup["labelColor"]) : Validate.instance(pSetup["labelColor"], Color));
	this.__Internal__Dont__Modify__.labelOutlineColor = (typeof pSetup["labelOutlineColor"] === "string" ? new Color(pSetup["labelOutlineColor"]) : Validate.instance(pSetup["labelOutlineColor"], Color));
	this.__Internal__Dont__Modify__.labelOutlineDistance = Validate.type(pSetup["labelOutlineDistance"], "number", 1);
	this.__Internal__Dont__Modify__.labelAlign = Validate.type(pSetup["labelAlign"], "number", TextAlign.CENTER);

	this.__Internal__Dont__Modify__.labelRenderOffset = Validate.type(pSetup["labelRenderOffset"], "number", 15);

	//List
	this.__Internal__Dont__Modify__.list = Validate.instance(pSetup["list"], Array);
	this.__Internal__Dont__Modify__.listFont = Validate.type(pSetup["listFont"], "string", "12px Arial");
	this.__Internal__Dont__Modify__.listColor = (typeof pSetup["listColor"] === "string" ? new Color(pSetup["listColor"]) : Validate.instance(pSetup["listColor"], Color));
	this.__Internal__Dont__Modify__.listOutlineColor = (typeof pSetup["listOutlineColor"] === "string" ? new Color(pSetup["listOutlineColor"]) : Validate.instance(pSetup["listOutlineColor"], Color));
	this.__Internal__Dont__Modify__.listOutlineDistance = Validate.type(pSetup["listOutlineDistance"], "number", 1);
	this.__Internal__Dont__Modify__.listAlign = Validate.type(pSetup["listAlign"], "number", TextAlign.CENTER);

	this.__Internal__Dont__Modify__.listRenderOffset = Validate.type(pSetup["listRenderOffset"], "number", 15);

	//Scrolling
	this.scrollIndex = Validate.type(pSetup["scrollIndex"], "number", 0);
};

ExtendProperties(UIList, UIBase, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIList : fillColor - Get the fill color being used for this List
		20/05/2017

		return Color - Returns a color object
	*/
	get fillColor() {
		return new Color(this.__Internal__Dont__Modify__.fillColor);
	},

	/*
		UIList : fillColor - Set the new fill color for this List
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
		UIList : borderColor - Get the border color being used for this List
		20/05/2017

		return Color - Returns a color object
	*/
	get borderColor() {
		return new Color(this.__Internal__Dont__Modify__.borderColor);
	},

	/*
		UIList : borderColor - Set the border color being used for this List
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
		UIList : borderWidth - Get the border width for the outline color
		20/05/2017

		return number - Returns a number defining the border width
	*/
	get borderWidth() {
		return this.__Internal__Dont__Modify__.borderWidth;
	},

	/*
		UIList : borderWidth - Set the border width for the outline color
		20/05/2017

		param[in] pVal - A number representing the new border width
	*/
	set borderWidth(pVal) {
		this.__Internal__Dont__Modify__.borderWidth = Validate.type(pVal, "number", 0, true);
	},

	/*
		UIList : labelText - Get the text being displayed as the label
		29/05/2017

		return String - Returns the label as a string
	*/
	get labelText() {
		return this.__Internal__Dont__Modify__.labelText;
	},

	/*
		UIList : labelText - Set the text being displayed as the label
		29/05/2017

		param[in] pTxt - A string defining the text to be displayed as the label for the List
	*/
	set labelText(pTxt) {
		this.__Internal__Dont__Modify__.labelText = Validate.type(pTxt, "string", "", true);
	},

	/*
		UIList : labelFont - Get the font being used for displaying the label
		29/05/2017

		return String - Returns the font as a string
	*/
	get labelFont() {
		return this.__Internal__Dont__Modify__.labelFont;
	},

	/*
		UIList : labelFont - Set the font being used for displaying the label
		29/05/2017

		param[in] pFont - A string containing the font to use for the label
	*/
	set labelFont(pFont) {
		this.__Internal__Dont__Modify__.labelFont(pFont, "string", "", true);
	},

	/*
		UIList : labelColor - Get the color being used to display the label
		29/05/2017

		return Color - Returns a Color object with the values in use
	*/
	get labelColor() {
		return new Color(this.__Internal__Dont__Modify__.labelColor);
	},

	/*
		UIList : labelColor - Set the color being used to display the label
		29/05/2017

		param[in] pCol - A Color object storing the values to use
	*/
	set labelColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.labelColor = new Color(pCol);
	},

	/*
		UIList : labelOutlineColor - Get the color being used to outline the label
		29/05/2017

		return Color - Returns a Color object with the values in use
	*/
	get labelOutlineColor() {
		return new Color(this.__Internal__Dont__Modify__.labelOutlineColor);
	},

	/*
		UIList : labelOutlineColor - Set the color being used to outline the label
		29/05/2017

		param[in] pCol - A Color object storing the values to use
	*/
	set labelOutlineColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.labelOutlineColor = new Color(pCol);
	},

	/*
		UIList : labelOutlineDistance - Get the distance of the outline around the label
		29/05/2017

		return Number - Returns the distance as Number
	*/
	get labelOutlineDistance() {
		return this.__Internal__Dont__Modify__.labelOutlineDistance;
	},

	/*
		UIList : labelOutlineDistance - Set the distance of the outline around the label
		29/05/2017

		param[in] pDist - A number containing the new distance
	*/
	set labelOutlineDistance(pDist) {
		this.__Internal__Dont__Modify__.labelOutlineDistance = Validate.type(pDist, "number", 0, true);
	},

	/*
		UIList : labelAlign - Get the alignment being used for the label
		29/05/2017

		return Number - Returns the alignment as a number
	*/
	get labelAlign() {
		return this.__Internal__Dont__Modify__.labelAlign;
	},

	/*
		UIList : labelAlign - Set the text alignemnt of the label
		29/05/2017

		param[in] pAlign - A number from the TextAlign define object
	*/
	set labelAlign(pAlign) {
		this.__Internal__Dont__Modify__.labelAlign = Validate.type(pAlign, "number", 0, true);
	},

	/*
		UIList : labelRenderOffset - Get the offset used when rendering the label
		29/05/2017

		return Number - Returns the render offset as a number
	*/
	get labelRenderOffset() {
		return this.__Internal__Dont__Modify__.labelRenderOffset;
	},

	/*
		UIList : labelRenderOffset - Set the offset used when rendering the label
		29/05/2017

		param[in] pVal - A number representing the new render offset
	*/
	set labelRenderOffset(pVal) {
		this.__Internal__Dont__Modify__.labelRenderOffset = Validate.type(pVal, "number", 0, true);
	},

	/*
		UIList : listFont - Get the font being used to display the list items
		29/05/2017

		return String - Returns the font in use as a String
	*/
	get listFont() {
		return this.__Internal__Dont__Modify__.listFont;
	},

	/*
		UIList : listFont - Set the font being used to display the list items
		29/05/2017

		param[in] pFont - A string containing the font to use for the list items
	*/
	set listFont(pFont) {
		this.__Internal__Dont__Modify__.listFont = Validate.type(pFont, "string", "", true);
	},

	/*
		UIList : listColor - Get the color being used to display the list items
		29/05/2017

		return Color - Returns a Color object containing the values
	*/
	get listColor() {
		return new Color(this.__Internal__Dont__Modify__.listColor);
	},

	/*
		UIList : listColor - Set the color being used to display the list items
		29/05/2017

		param[in] pCol - A Color obejct holding the new color values
	*/
	set listColor(pCol) {
		//Clean value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set value
		this.__Internal__Dont__Modify__.listColor = new Color(pCol);
	},

	/*
		UIList : listOutlineColor - Get the color being used to outline the list items
		29/05/2017

		return Color - Returns a Color object holding the values
	*/
	get listOutlineColor() {
		return new Color(this.__Internal__Dont__Modify__.listOutlineColor);
	},

	/*
		UIList : listOutlineColor - Set the color being used to outline the list items
		29/05/2017

		param[in] pCol - A Color object holding the new values
	*/
	set listOutlineColor(pCol) {
		//Clean the value
		pCol = Validate.instance(pCol, Color, null, true);

		//Set the value
		this.__Internal__Dont__Modify__.listOutlineColor = new Color(pCol);
	},

	/*
		UIList : listOutlineDistance - Get the distance of the outline around the list items
		29/05/2017

		return Number - Returns the outline distance as a number
	*/
	get listOutlineDistance() {
		return this.__Internal__Dont__Modify__.listOutlineDistance;
	},

	/*
		UIList : listOutlineDistance - Set the distance of the outline around the list items
		29/05/2017

		param[in] pDist - A number holding the new distance for the outline
	*/
	set listOutlineDistance(pDist) {
		this.__Internal__Dont__Modify__.listOutlineDistance = Validate.type(pDist, "number", 0, true);
	},

	/*
		UIList : listAlign - Get the alignment of the list items
		29/05/2017

		return Number - Returns the alignment as a number
	*/
	get listAlign() {
		return this.__Internal__Dont__Modify__.listAlign;
	},

	/*
		UIList : listAlign - Set the alignment of the list items
		29/05/2017

		param[in] pAlign - A number from the TextAlign define object
	*/
	set listAlign(pAlign) {
		this.__Internal__Dont__Modify__.listAlign = Validate.type(pAlign, "number", 0, true);
	},

	/*
		UIList : listRenderOffset - Get the offset used when rendering list items
		29/05/2017

		return Number - Returns the render offset as a number
	*/
	get listRenderOffset() {
		return this.__Internal__Dont__Modify__.listRenderOffset;
	},

	/*
		UIList : listRenderOffset - Set the offset used when rendering list items
		29/05/2017

		param[in] pVal - A number holding the offset values for rendering
	*/
	set listRenderOffset(pVal) {
		this.__Internal__Dont__Modify__.listRenderOffset = Validate.type(pVal, "number", 0, true);
	},

	/*
		UIList : scrollIndex - Get the current scroll index of the list
		29/05/2017

		return Number - Returns a number defining the index
	*/
	get scrollIndex() {
		return this.__Internal__Dont__Modify__.scrollIndex;
	},

	/*
		UIList : scrollIndex - Set the current scroll index of the list
		29/05/2017

		param[in] pInd - A number defining the index to 'scroll' to
	*/
	set scrollIndex(pInd) {
		//Clean the value
		pInd = Validate.type(pInd, "number", 0, true);

		//Determine if there are items in the list
		if (!this.__Internal__Dont__Modify__.list.length) this.__Internal__Dont__Modify__.scrollIndex = 0;

		//Set based on passed in index
		else {
			//If negative wrap around
			if (pInd < 0) this.__Internal__Dont__Modify__.scrollIndex = this.__Internal__Dont__Modify__.list.length - 1;
			else this.__Internal__Dont__Modify__.scrollIndex = pInd % this.__Internal__Dont__Modify__.list.length;
		}
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                List Functions                                              ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIList : addItem - Add an item into the list
		29/05/2017

		param[in] pItem - An item of indescriminate type that can be displayed as a string
		param[in] pUnique - Flags if the items should be unique, will remove all other matches 
							(Default false)
	*/
	addItem: function(pItem, pUnique) {
		//Check for removal of values
		if (pUnique === true)
			this.removeItems(pItem);

		//Add the item
		this.__Internal__Dont__Modify__.list.push(pItem);
	},

	/*
		UIList : removeItem - Remove the first item of matching value
		29/05/2017

		param[in] pItem - The item to compare the internal list against

		return bool - Returns true if an item was removed
	*/
	removeItem: function(pItem) {
		//Loop thorugh all items
		for (var i = 0; i < this.__Internal__Dont__Modify__.list.length; i++) {
			if (this.__Internal__Dont__Modify__.list[i] === pItem) {
				//Remove the item from the list
				this.__Internal__Dont__Modify__.list.splice(i, 1);

				//Reset the scroll index
				this.scrollIndex = this.scrollIndex;

				//Return success
				return true;
			}
		}

		//Default failure
		return false;
	},

	/*
		UIList : removeItems - Remove all items matching a value from the list
		29/05/2017

		param[in] pItem - The item to compare the internal list against

		return bool - Returns true if an item was removed
	*/
	removeItems: function(pItem) {
		//Store a removeal flag
		let flag = false;

		//Loop through all internal items
		for (var i = this.__Internal__Dont__Modify__.list.length - 1; i >= 0; i--) {
			if (this.__Internal__Dont__Modify__.list[i] === pItem) {
				//Remove the item from the list
				this.__Internal__Dont__Modify__.list.splice(i, 1);

				//Toggle the flag
				flag = true;
			}
		}

		//Reset the scroll index
		this.scrollIndex = this.scrollIndex;

		//Return the flag
		return flag;
	},

	/*
		UIList : clear - Remove all items from the list
		30/05/2017
	*/
	clear: function() {
		this.__Internal__Dont__Modify__.list = [];
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                  Main Functions                                            ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		UIList : draw - Draw the List to the screen
		20/05/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Set the color styles
		pGraphics.draw.fillStyle = this.__Internal__Dont__Modify__.fillColor.rgba;
		pGraphics.draw.strokeStyle = this.__Internal__Dont__Modify__.borderColor.rgba;

		//Set the border line width
		pGraphics.draw.lineWidth = this.__Internal__Dont__Modify__.borderWidth;

		//Draw the rectangles
		pGraphics.draw.fillRect(this.__Internal__Dont__Modify__.x, this.__Internal__Dont__Modify__.y,
			this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);
		pGraphics.draw.strokeRect(this.__Internal__Dont__Modify__.x, this.__Internal__Dont__Modify__.y,
			this.__Internal__Dont__Modify__.width, this.__Internal__Dont__Modify__.height);

		//Draw the label
		if (this.__Internal__Dont__Modify__.labelText !== "") {
			//Set the font
			pGraphics.draw.font = this.__Internal__Dont__Modify__.labelFont;

			//Calculate the offset values
			let txtXOffset = 0;
			let txtYOffset = this.__Internal__Dont__Modify__.labelRenderOffset;

			//Switch based on the text alignment
			switch (this.__Internal__Dont__Modify__.labelAlign) {
				case TextAlign.CENTER:
					txtXOffset += this.__Internal__Dont__Modify__.width / 2;
					break;
				case TextAlign.RIGHT:
					txtXOffset += this.__Internal__Dont__Modify__.width;
					break;
			}

			//Draw the label
			pGraphics.outlineText(this.__Internal__Dont__Modify__.labelText, this.__Internal__Dont__Modify__.x + txtXOffset, this.__Internal__Dont__Modify__.y + txtYOffset,
				this.__Internal__Dont__Modify__.labelColor, this.__Internal__Dont__Modify__.labelOutlineColor,
				this.__Internal__Dont__Modify__.labelAlign, this.__Internal__Dont__Modify__.labelOutlineDistance);
		}

		//Draw the list items
		if (this.__Internal__Dont__Modify__.list.length) {
			//Set the font
			pGraphics.draw.font = this.__Internal__Dont__Modify__.listFont;

			//Calculate the horizontal offset values
			let txtXOffset = 0;

			//Switch based on the text alignment
			switch (this.__Internal__Dont__Modify__.listAlign) {
				case TextAlign.CENTER:
					txtXOffset += this.__Internal__Dont__Modify__.width / 2;
					break;
				case TextAlign.RIGHT:
					txtXOffset += this.__Internal__Dont__Modify__.width;
					break;
			}

			//Loop through the elements
			for (var ind = this.__Internal__Dont__Modify__.scrollIndex, txtYOffset = this.__Internal__Dont__Modify__.y + this.__Internal__Dont__Modify__.labelRenderOffset + this.__Internal__Dont__Modify__.listRenderOffset; ind < this.__Internal__Dont__Modify__.list.length && txtYOffset < this.__Internal__Dont__Modify__.y + this.__Internal__Dont__Modify__.height; ind++, txtYOffset += this.__Internal__Dont__Modify__.listRenderOffset) {
				//Draw the elements
				pGraphics.outlineText(this.__Internal__Dont__Modify__.list[ind], this.__Internal__Dont__Modify__.x + txtXOffset, txtYOffset,
					this.__Internal__Dont__Modify__.listColor, this.__Internal__Dont__Modify__.listOutlineColor,
					this.__Internal__Dont__Modify__.listAlign, this.__Internal__Dont__Modify__.listOutlineDistance);
			}
		}
	},
});