/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: StateManager
 *		Author: Mitchell Croft
 *		Date: 07/07/2017
 *
 *		Version: 3.0
 *		Modified to align closer with the rest of the 
 *		framework
 *
 *		Requires:
 *		ExtendProperties.js
 *
 *		Purpose:
 *		Manage the active game loop function and supply
 *		timeing information to user state functions
 **/

/*
	StateManager : Constructor - Initialise with default values
	07/07/2017

	param[in] pCB - The function that will be called every frame, optionally taking 
					the State Managers Time object as a parameter
*/
function StateManager(pCB) {
	/*  WARNING:
        Don't modify this internal object from the outside of the StateManager object.
        Instead use StateManager object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Maintain a time object 
		time: new Time(true),

		//Track the callback function
		callback: Validate.type(pCB, "function", null),
	};

	//Bind the animation loop to this object
	this.animationLoop = this.animationLoop.bind(this);

	//Start the animation loop
	this.animationLoop();
};

ExtendProperties(StateManager, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		StateManager : callback - Set the function that will be called every animation frame
		07/07/2017

		param[in] pCB - The function that will be called every frame, optionally taking 
						the State Managers Time object as a parameterparam[in]
	*/
	set callback(pCB) {
		this.__Internal__Dont__Modify__.callback = Validate.type(pCB, "function", null, true);
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Main Function                                                ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		StateManager : animationLoop - The function that will be recalled by the window action. 
									   Called automatically, does not require user intervention
		07/07/2017
	*/
	animationLoop: function() {
		//Update the time values
		this.__Internal__Dont__Modify__.time.update();

		//Run the callback function
		if (this.__Internal__Dont__Modify__.callback)
			this.__Internal__Dont__Modify__.callback(this.__Internal__Dont__Modify__.time);

		//Request the window recalls the loop
		window.requestAnimationFrame(this.animationLoop);
	},
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: Time
 *		Author: Mitchell Croft
 *		Date: 07/07/2017
 *
 *		Version: 1.0
 *
 *		Requires:
 *		ExtendProperties.js
 *
 *		Purpose:
 *		Manage timing information in a centralised 
 *		point
 **/

/*
	StateManager : Constructor - Initialise with default values
	07/07/2017

	param[in] pRestrict - A flag that indicates delta time information should be
						  clamped to a maximum value of 1 (Default false)
*/
function Time(pRestrict) {
	/*  WARNING:
        Don't modify this internal object from the outside of the Time object.
        Instead use Time object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Store the restriction flag
		restricted: (pRestrict === true),

		//Store time information
		preTime: Date.now(),
		curTime: Date.now(),

		//Store the active time scale
		timeScale: 1,

		//Store delta time values
		deltaTime: 0,
		realDeltaTime: 0,

		//Store runtime values
		elapsedTime: 0,
		realElapsedTime: 0,
	};
};

ExtendProperties(Time, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Time : restricted - Get the restricted flag of the Time object
		07/07/2017

		return bool - Returns true if the delta times are restricted to a maximum of 1
	*/
	get restricted() {
		return this.__Internal__Dont__Modify__.restricted;
	},

	/*
		Time : restricted - Set the restricted flag of the Time object
		07/07/2017

		param[in] pState - A boolean value that defines the new restrictions
	*/
	set restricted(pState) {
		this.__Internal__Dont__Modify__.restricted = Validate.type(pState, "boolean", false, true);
	},

	/*
		Time : timeScale - Get the currently applied time scale value
		07/07/2017

		return number - Returns the time scale as a number that is >= 0
	*/
	get timeScale() {
		return this.__Internal__Dont__Modify__.timeScale;
	},

	/*
		Time : timeScale - Set the time scale to be applied to time values
		07/07/2017

		param[in] pScl - A number that is >= 0 to be applied to scale time values
	*/
	set timeScale(pScl) {
		this.__Internal__Dont__Modify__.timeScale = Math.max(Validate.type(pScl, "number", 0, true), 0);
	},

	/*
		Time : deltaTime - Get the scaled delta time 
		07/07/2017

		return number - Returns the delta time as a number (in seconds)
	*/
	get deltaTime() {
		return this.__Internal__Dont__Modify__.deltaTime;
	},

	/*
		Time : realDeltaTime - Get the unscaled delta time
		07/07/2017

		return number - Returns the real delta time as a number (in seconds)
	*/
	get realDeltaTime() {
		return this.__Internal__Dont__Modify__.realDeltaTime;
	},

	/*
		Time : elapsedTime - Get the scaled elapsed time
		07/07/2017

		return number - Returns the elapsed time as a number (in seconds)
	*/
	get elapsedTime() {
		return this.__Internal__Dont__Modify__.elapsedTime;
	},

	/*
		Time : realElapsedTime - Get the unscaled elapsed time
		07/07/2017

		return number - Returns the real elapsed time as a number (in seconds)
	*/
	get realElapsedTime() {
		return this.__Internal__Dont__Modify__.realElapsedTime;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Main Function                                                ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		Time : update - Update the time values based on time elapsed between update function calls
		07/07/2017
    */
	update: function() {
		//Update the time values
		this.__Internal__Dont__Modify__.preTime = this.__Internal__Dont__Modify__.curTime;
		this.__Internal__Dont__Modify__.curTime = Date.now();

		//Calculate the delta time
		this.__Internal__Dont__Modify__.realDeltaTime = (this.__Internal__Dont__Modify__.curTime - this.__Internal__Dont__Modify__.preTime) * 0.001;

		//Check if delta time needs to be restricted
		if (this.__Internal__Dont__Modify__.realDeltaTime > 1 && this.__Internal__Dont__Modify__.restricted)
			this.__Internal__Dont__Modify__.realDeltaTime = 1;

		//Calculate the scaled delta time
		this.__Internal__Dont__Modify__.deltaTime = this.__Internal__Dont__Modify__.realDeltaTime * this.__Internal__Dont__Modify__.timeScale;

		//Increase the runtime values
		this.__Internal__Dont__Modify__.elapsedTime += this.__Internal__Dont__Modify__.deltaTime;
		this.__Internal__Dont__Modify__.realElapsedTime += this.__Internal__Dont__Modify__.realDeltaTime;
	},
});