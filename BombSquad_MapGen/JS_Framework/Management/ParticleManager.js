/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: ParticleManager
 *		Author: Mitchell Croft
 *		Date: 04/07/2017
 *
 *		Version: 2.0
 *		Introduced properties
 *
 *		Requires:
 *		Graphics.js, ExtendProperties.js
 *
 *		Purpose:
 *		Manage a collection of particle emitter objects that are in use
 **/

/*
	ParticleManager : Constructor - Initialise with default values
	04/07/2017
*/
function ParticleManager() {
	/*  WARNING:
        Don't modify this internal object from the outside of the ParticleManager object.
        Instead use ParticleManager object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Store a list of the particle emitters in the manager
		emitters: []
	};
};

ExtendProperties(ParticleManager, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleManager : count - Get the number of emitter's being managed by the manager
		04/07/2017

		return number - Returns the emitter count as a number
	*/
	get count() {
		return this.__Internal__Dont__Modify__.emitters.length;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////																											 ////
	/////										         Management Functions										 ////
	/////																											 ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleManager : createEmitter - Create a new Particle Emitter object and add it to the managed list
		04/07/2017

		param[in] pSetup - An object with values used to setup the panel

		return ParticleEmitter - Returns the new Particle Emitter object that was added to the manager
	*/
	createEmitter: function(pSetup) {
		//Create the new particle emitter
		this.__Internal__Dont__Modify__.emitters[this.__Internal__Dont__Modify__.emitters.length] = new ParticleEmitter(pSetup);

		//Return the new emitter
		return this.__Internal__Dont__Modify__.emitters[this.__Internal__Dont__Modify__.emitters.length - 1];
	},

	/*
		ParticleManager : addEmitter - Add a new Particle Emitter object to the managed list
		04/07/2017

		param[in] pEmitter - A Particle Emitter object to add to the manager

		return bool - Returns true if the Emitter was added to the Manager
	*/
	addEmitter: function(pEmitter) {
		//Validate the emitter object
		Validate.instance(pEmitter, ParticleEmitter, true);

		//Loop through emitters and check for duplicates
		for (var i = this.__Internal__Dont__Modify__.emitters.length - 1; i >= 0; i--) {
			if (this.__Internal__Dont__Modify__.emitters[i] === pEmitter)
				return false;
		}

		//Add the emitter to the managed to the list
		this.__Internal__Dont__Modify__.emitters.push(pEmitter);

		//Return success
		return true;
	},

	/*
		ParticleManager : removeEmitter - Remove a Particle Emitter object from the Manager
		04/07/2017

		param[in] pEmitter - The Particle EMitter object to remove from the manager

		return bool - Returns true if the Emitter was removed
	*/
	removeEmitter: function(pEmitter) {
		//Validate the emitter object
		Validate.instance(pEmitter, ParticleEmitter, true);

		//Loop through contained emitters
		for (var i = this.__Internal__Dont__Modify__.emitters.length - 1; i >= 0; i--) {
			if (this.__Internal__Dont__Modify__.emitters[i] === pEmitter) {
				//Remove the emitter
				this.__Internal__Dont__Modify__.emitters.splice(i, 1);

				//Return success
				return true;
			}
		}

		//Return failure
		return false;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////																											 ////
	/////													Main Functions											 ////
	/////																											 ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleManager : update - Update all of the containined particle emitter objects
		04/07/2017

		param[in] pDelta - The delta time for the cycle
	*/
	update: function(pDelta) {
		//Iterate through all emitters
		for (var i = this.__Internal__Dont__Modify__.emitters.length - 1; i >= 0; i--) {
			if (!this.__Internal__Dont__Modify__.emitters[i].update(pDelta))
				this.__Internal__Dont__Modify__.emitters.splice(i, 1);
		}
	},

	/*
		ParticleManager : draw - render all of the ParticleEmitter objects contained in the manager
		04/07/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Loop through all emitters
		for (var i = 0; i < this.__Internal__Dont__Modify__.emitters.length; i++)
			this.__Internal__Dont__Modify__.emitters[i].draw(pGraphics);
	},
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                   Emitter Types                                            ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: EmitterType
 *      Author: Mitchell Croft
 *      Date: 25/10/2016
 *
 *      Purpose:
 *      Label the different types of emitter that can be created
 **/
var EmitterType = { POINT: 0, DIRECTION: 1, LINE: 2 };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                   Emitter Space                                            ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: EmitterSpace
 *      Author: Mitchell Croft
 *      Date: 04/07/2016
 *
 *      Purpose:
 *      Label the different types of space that particles can be emitted in
 **/
var EmitterSpace = { WORLD: 0, LOCAL: 1 };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *		Name: ParticleEmitter
 *		Author: Mitchell Croft
 *		Date: 04/07/2017
 *
 *		Version: 2.0
 *		Introduced properties and utilised the object property setup of emitters
 *
 *		Requires:
 *		Graphics.js, Vec2.js, Mat3.js, Color.js, ExtendProperties.js
 *
 *		Purpose:
 *		Manage a collection of particles that share properties
 **/

/*
	ParticleEmitter : Constructor - Initialise with default values
	04/07/2017

	param[in] pSetup - An object with values used to setup the panel
*/
function ParticleEmitter(pSetup) {
	//Clean the setup object
	pSetup = Validate.type(pSetup, "object", {});

	/*  WARNING:
        Don't modify this internal object from the outside of the ParticleEmitter object.
        Instead use ParticleEmitter object properties and functions to modify these values
        as this allows for the internal information to update itself and keep it
        correct.
    */
	this.__Internal__Dont__Modify__ = {
		//Store an array of the particles
		particles: [],

		//Flag if the emitter is currently running
		running: false,

		//Store the position of the emitter
		position: Validate.instance(pSetup["position"], Vec2),

		//Extract the emitter values
		type: Math.clamp(Math.round(Validate.type(pSetup["type"], "number", EmitterType.POINT)), EmitterType.POINT, EmitterType.LINE),
		emitterSpace: Math.clamp(Math.round(Validate.type(pSetup["emitterSpace"], "number", EmitterSpace.WORLD)), EmitterSpace.WORLD, EmitterSpace.LOCAL),

		maximum: Validate.type(pSetup["maximum"], "number", 25),
		activeParticles: 0,

		emitTimer: 0,
		emitRate: 1 / Math.max(Validate.type(pSetup["emitRate"], "number", 1), 0.001),

		direction: Validate.instance(pSetup["direction"], Vec2, new Vec2(0, -1)).normalize(),
		lineLength: Validate.type(pSetup["lineLength"], "number", 100),

		runTime: Validate.type(pSetup["runTime"], "number", -1),
		progress: 0,
	};

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////---------------------------------------Extract Particle Values----------------------------------------////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//Store the particle settings to be randomised
	this.__Internal__Dont__Modify__.minLife = Math.max(Validate.type(pSetup["minLife"], "number", 0), 0);
	this.__Internal__Dont__Modify__.maxLife = Math.max(Validate.type(pSetup["maxLife"], "number", 5), this.__Internal__Dont__Modify__.minLife);

	this.__Internal__Dont__Modify__.minVelocity = Math.max(Validate.type(pSetup["minVelocity"], "number", 100), 0);
	this.__Internal__Dont__Modify__.maxVelocity = Math.max(Validate.type(pSetup["maxVelocity"], "number", 500), this.__Internal__Dont__Modify__.minVelocity);

	this.__Internal__Dont__Modify__.negRotation = Math.abs(Validate.type(pSetup["negRotation"], "number", Math.PI * 2)) * -1;
	this.__Internal__Dont__Modify__.posRotation = Math.abs(Validate.type(pSetup["posRotation"], "number", Math.PI * 2));

	//Store the particle settings to lerp over time
	this.__Internal__Dont__Modify__.startSize = Validate.type(pSetup["startSize"], "number", 50);
	this.__Internal__Dont__Modify__.endSize = Validate.type(pSetup["endSize"], "number", 0);

	this.__Internal__Dont__Modify__.startColor = (typeof pSetup["startColor"] === "string" ? new Color(pSetup["startColor"]) : Validate.instance(pSetup["startColor"], Color, new Color(255, 255, 255, 1)));
	this.__Internal__Dont__Modify__.endColor = (typeof pSetup["endColor"] === "string" ? new Color(pSetup["endColor"]) : Validate.instance(pSetup["endColor"], Color, new Color(0, 0, 0, 0)));
	this.__Internal__Dont__Modify__.image = Validate.instance(pSetup["image"], Image, null);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////--------------------------------------------Test For Start--------------------------------------------////
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////
	if (Validate.type(pSetup["running"], "boolean", true)) this.start();
};

ExtendProperties(ParticleEmitter, {
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                               Property Definitions                                         ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleEmitter : running - Gets the running flag of the emitter
		04/07/2017

		return bool - Returns true if the Emitter is currently emitting
	*/
	get running() {
		return this.__Internal__Dont__Modify__.running;
	},

	/*
		ParticleEmitter : running - Set the running state of the Emitter
		04/07/2017

		param[in] pState - A bool value indicating the new running state
	*/
	set running(pState) {
		if (pState) this.start();
		else this.stop();
	},

	/*
		ParticleEmitter : position - Get the position of the Particle Emitter
		04/07/2017

		return Vec2 - Returns a Vec2 object holding the position
	*/
	get position() {
		return new Vec2(this.__Internal__Dont__Modify__.position);
	},

	/*
		ParticleEmitter : position - Set the position of the Particle Emitter
		04/07/2017

		param[in] pPos - A Vec2 object holding the new position values
	*/
	set position(pPos) {
		this.__Internal__Dont__Modify__.position = new Vec2(Validate.instance(pPos, Vec2, null, true));
	},

	/*
		ParticleEmitter : activeParticles - Get the current number of active particles
		04/07/2017

		return number - Returns a number containing the number of currently active particles
	*/
	get activeParticles() {
		return this.__Internal__Dont__Modify__.activeParticles;
	},

	/*
		ParticleEmitter : type - Get the type of the Particle Emitter
		04/07/2017

		return number - Returns a number representing an Emitter type as defined in the EmitterType object
	*/
	get type() {
		return this.__Internal__Dont__Modify__.type;
	},

	/*
		ParticleEmitter : type - Set the type of emission this emitter uses
		04/07/2017

		param[in] pType - A value representing one of the defined types in the EmitterType object
	*/
	set type(pType) {
		this.__Internal__Dont__Modify__.type = Math.clamp(Math.round(Validate.type(pType, "number", 0, true)), EmitterType.POINT, EmitterType.LINE);
	},

	/*
		ParticleEmitter : emitterSpace - Get the space that particles are being emitted into
		04/07/2017

		return number - Returns a number representing the Emitter Space type as defined in the EmitterSpace object
	*/
	get emitterSpace() {
		return this.__Internal__Dont__Modify__.emitterSpace;
	},

	/*
		ParticleEmitter : emitterSpace - Set the space that particles are being emitted into
		04/07/2017

		param[in] pType - A value representing one of the defined spaces in the EmitterSpace object
	*/
	set emitterSpace(pType) {
		//Clean the flag
		pType = Math.clamp(Math.round(Validate.type(pType, "number", 0, true)), EmitterSpace.WORLD, EmitterSpace.LOCAL);

		//Check if the space is different
		if (pType !== this.__Internal__Dont__Modify__.emitterSpace) {
			//Assign the new space
			this.__Internal__Dont__Modify__.emitterSpace = pType;

			//Reset the active particles
			this.__Internal__Dont__Modify__.activeParticles = 0;
		}
	},

	/*
		ParticleEmitter : maximum - Get the maximum number of particles available to the emitter
		04/07/2017

		return number - Returns a number storing the maximum number of particles
	*/
	get maximum() {
		return this.__Internal__Dont__Modify__.maximum;
	},

	/*
		ParticleEmitter : maximum - Set the maximum number of particles the emitter can use
		04/07/2017

		param[in] pVal - A number representing the new maximum particle count
	*/
	set maximum(pVal) {
		//Clean the flag
		pVal = Math.round(Math.max(Validate.type(pVal, "number", 0, true), 0));

		//Ensure the emitter is not currently running
		if (!this.__Internal__Dont__Modify__.running)
			this.__Internal__Dont__Modify__.maximum = pVal;
	},

	/*
		ParticleEmitter : emitRate - Get the current emit rate for the particles
		04/07/2017

		return number - Returns the number of particles emitted per second
	*/
	get emitRate() {
		return (1 / this.__Internal__Dont__Modify__.emitRate);
	},

	/*
		ParticleEmitter : emitRate - Set the emit rate of the Particle Emitter
		04/07/2017

		param[in] pVal - A number defining the number of particles emitted per second
	*/
	set emitRate(pVal) {
		//Clean the flag
		pVal = Validate.type(pVal, "number", 0, true);

		//Check value is valid
		if (pVal > 0) this.__Internal__Dont__Modify__.emitRate = (1 / pVal);

		//Otherwise stop the emitter
		else this.stop();
	},

	/*
		ParticleEmitter : direction - Get the direction particles are emitted
		04/07/2017

		NOTE: Direction is only used for EmitterType.DIRECTION and EmitterType.LINE emitters only

		return Vec2 - Returns a normalised Vec2 object defining the direction of emission
	*/
	get direction() {
		return new Vec2(this.__Internal__Dont__Modify__.direction);
	},

	/*
		ParticleEmitter : direction - Set the direction particles are emitted in
		04/07/2017

		NOTE: Direction is only used for EmitterType.DIRECTION and EmitterType.LINE emitters only

		param[in] pDir - A Vec2 object defining the new direction of emission
	*/
	set direction(pDir) {
		this.__Internal__Dont__Modify__.direction = Validate.instance(pDir, Vec2, null, true).normalized;
	},

	/*
		ParticleEmitter : lineLength - Get the length of the line the particles are emitted from
		04/07/2017

		NOTE: LineLength is only used for EmitterType.LINE emitters only

		return number - Returns a number defining the length of the emission line
	*/
	get lineLength() {
		return this.__Internal__Dont__Modify__.lineLength;
	},

	/*
		ParticleEmitter : lineLength - Set the length of the line the partciles are emitted from
		04/07/2017

		NOTE: LineLength is only used for EmitterType.LINE emitters only

		param[in] pVal - A number defining the new length of the mission line
	*/
	set lineLength(pVal) {
		this.__Internal__Dont__Modify__.lineLength = Math.max(Validate.type(pVal, "number", 0, true), 0);
	},

	/*
		ParticleEmitter : runTime - Get the length of time that particles will be emitted for
		04/07/2017

		NOTE: Negative run times indicate indefinite emission

		return number - Returns a number containing the total runtime in seconds
	*/
	get runTime() {
		return this.__Internal__Dont__Modify__.runTime;
	},

	/*
		ParticleEmitter : runTime - Set the length of time that particles will be emitted for
		04/07/2017

		NOTE: Negative run times indicate indefinite emission

		param[in] pVal - A number defining the amount of time in seconds the emitter will remain active		
	*/
	set runTime(pVal) {
		this.__Internal__Dont__Modify__.runTime = Validate.type(pVal, "number", 0, true);
	},

	/*
		ParticleEmitter : minLife - Get the minimum life span of a particle
		04/07/2017

		return number - Returns a number containing the minimum life span in seconds
	*/
	get minLife() {
		return this.__Internal__Dont__Modify__.minLife;
	},

	/*
		ParticleEmitter : minLife - Set the minimum life span of a particle
		04/07/2017

		param[in] pVal - A number defining the new minimum lifespan for a particle
	*/
	set minLife(pVal) {
		//Set the minimum
		this.__Internal__Dont__Modify__.minLife = Validate.type(pVal, "number", 0, true);

		//Ensure maximum is still >= then minimum
		this.__Internal__Dont__Modify__.maxLife = Math.max(this.__Internal__Dont__Modify__.maxLife, this.__Internal__Dont__Modify__.minLife);
	},

	/*
		ParticleEmitter : maxLife - Get the maximum life span of a particle
		04/07/2017

		return number - Returns a number containing the maximum life span in seconds
	*/
	get maxLife() {
		return this.__Internal__Dont__Modify__.maxLife;
	},

	/*
		ParticleEmitter : maxLife - Set the maximum life span of a particle
		04/07/2017

		param[in] pVal - A number defining the new maximum lifespan for a particle
	*/
	set maxLife(pVal) {
		//Set the maximum
		this.__Internal__Dont__Modify__.maxLife = Validate.type(pVal, "number", 0, true);

		//Ensure minimum is still <= then maximum
		this.__Internal__Dont__Modify__.minLife = Math.min(this.__Internal__Dont__Modify__.minLife, this.__Internal__Dont__Modify__.maxLife);
	},

	/*
		ParticleEmitter : minVelocity - Get the minimum velocity of a particle
		04/07/2017

		return number - Returns a number containing the minimum velocity in units per second
	*/
	get minVelocity() {
		return this.__Internal__Dont__Modify__.minVelocity;
	},

	/*
		ParticleEmitter : minVelocity - Set the minimum velocity of a particle
		04/07/2017

		param[in] pVal - A number defining the minimum velocity of a particle
	*/
	set minVelocity(pVal) {
		//Set the minimum
		this.__Internal__Dont__Modify__.minVelocity = Validate.type(pVal, "number", 0, true);

		//Ensure the maximum is still >= then minimum
		this.__Internal__Dont__Modify__.maxVelocity = Math.max(this.__Internal__Dont__Modify__.maxVelocity, this.__Internal__Dont__Modify__.minVelocity);
	},

	/*
		ParticleEmitter : maxVelocity - Get the maximum velocity of a particle
		04/07/2017

		return number - Returns a number containing the maximum velocity in units per second
	*/
	get maxVelocity() {
		return this.__Internal__Dont__Modify__.maxVelocity;
	},

	/*
		ParticleEmitter : maxVelocity - Set the maximum velocity of a particle
		04/07/2017

		param[in] pVal - A number defining the maximum velocity of a particle
	*/
	set maxVelocity(pVal) {
		//Set the maximum
		this.__Internal__Dont__Modify__.maxVelocity = Validate.type(pVal, "number", 0, true);

		//Ensure the minimum is still <= then maximum
		this.__Internal__Dont__Modify__.minVelocity = Math.min(this.__Internal__Dont__Modify__.minVelocity, this.__Internal__Dont__Modify__.maxVelocity);
	},

	/*
		ParticleEmitter : negRotation - Get the maximum negative rotation that can be applied to a particle
		04/07/2017

		return number - Returns the rotation as a negative number in radians
	*/
	get negRotation() {
		return this.__Internal__Dont__Modify__.negRotation;
	},

	/*
		ParticleEmitter : negRotation - Set the maximum negative rotation that can be applied to a particle
		04/07/2017

		param[in] pVal - A number defining the maximum negative rotation that can be applied to a particle (in radians / second)
	*/
	set negRotation(pVal) {
		this.__Internal__Dont__Modify__.negRotation = Math.abs(Validate.type(pVal, "number", 0, true)) * -1;
	},

	/*
		ParticleEmitter : posRotation - Get the maximum positive rotation that can be applied to a particle
		04/07/2017

		return number - Returns the rotation as a positive number in radians
	*/
	get posRotation() {
		return this.__Internal__Dont__Modify__.posRotation;
	},

	/*
		ParticleEmitter : posRotation - Set the maximum positive rotation that can be applied to a particle
		04/07/2017

		param[in] pVal - A number defining the maximum positive rotation that can be applied to a particle (in radians / second)
	*/
	set posRotation(pVal) {
		this.__Internal__Dont__Modify__.posRotation = Math.abs(Validate.type(pVal, "number", 0, true));
	},

	/*
		ParticleEmitter : startColor - Get the starting color for the particles
		04/07/2017

		NOTE: Only used if no image is in use

		return Color - Returns a Color object holding the values
	*/
	get startColor() {
		return new Color(this.__Internal__Dont__Modify__.startColor);
	},

	/*
		ParticleEmitter : startColor - Set the starting color for the particles
		04/07/2017

		NOTE: Only used if no image is in use

		param[in] pCol - A Color object holding the new values
	*/
	set startColor(pCol) {
		this.__Internal__Dont__Modify__.startColor = Validate.instance(pCol, Color, null, true);
	},

	/*
		ParticleEmitter : endColor - Get the end color for the particles
		04/07/2017

		NOTE: Only used if no image is in use

		return Color - Returns a Color object holding the vlaues
	*/
	get endColor() {
		return new Color(this.__Internal__Dont__Modify__.endColor);
	},

	/*
		ParticleEmitter : endColor - Set the end color for the particles
		04/07/2017

		NOTE: Only used if no image is in use

		param[in] pCol - A color object holding the new values
	*/
	set endColor(pCol) {
		this.__Internal__Dont__Modify__.endColor = Validate.instance(pCol, Color, null, true);
	},

	/*
		ParticleEmitter : image - Get the Image object being used to display particles
		04/07/2017

		return Image - Returns the Image object in use or null if none
	*/
	get image() {
		return this.__Internal__Dont__Modify__.image;
	},

	/*
		ParticleEmitter : image - Set the Image object to be used to display particles
		04/07/2017

		param[in] pImg - An Image object to use for display or null to remove
	*/
	set image(pImg) {
		//Check value
		if (!(pImg instanceof Image) && pImg !== null)
			throw new Error("Can not set a Particle Emitter's Image property to " + pImg + " (Type " + typeof pImg + ") Please use an Image object or null");

		//Assign the value
		this.__Internal__Dont__Modify__.image = pImg;
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                Management Functions                                        ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleEmitter : start - Start emitting particles with the current settings
		04/07/2017
	*/
	start: function() {
		//Ensure the emitter isnt already running
		if (this.__Internal__Dont__Modify__.running) return;

		//Clear all previous particles
		this.__Internal__Dont__Modify__.particles = [];

		//Create all of the particle object to be used
		for (var i = 0; i < this.__Internal__Dont__Modify__.maximum; i++)
			this.__Internal__Dont__Modify__.particles[i] = new Particle();

		//Reset the emit timer
		this.__Internal__Dont__Modify__.emitTimer = 0;

		//Flag the emitter as running
		this.__Internal__Dont__Modify__.running = true;
	},

	/*
		ParticleEmitter : stop - Stop the emission of particles
		04/07/2017

		param[in] pForce - An optional flag indicating that all active particles should be removed
						   (default false)
	*/
	stop: function(pForce) {
		//Reset the running flag
		this.__Internal__Dont__Modify__.running = false;

		//Check the force flag
		if (pForce === true)
			this.__Internal__Dont__Modify__.activeParticles = 0;
	},

	/*
		ParticleEmitter : emit - Emit a new particle with the current settings. This is called as required by the
								 update function
		04/07/2017
	*/
	emit: function() {
		//Check there are particles that can be emitted
		if (this.__Internal__Dont__Modify__.activeParticles >= this.__Internal__Dont__Modify__.maximum) return;

		//Get the next particle
		let particle = this.__Internal__Dont__Modify__.particles[this.__Internal__Dont__Modify__.activeParticles++];

		//Reset the lifetime
		particle.lifetime = 0;
		particle.lifespan = Math.randomRange(this.__Internal__Dont__Modify__.minLife, this.__Internal__Dont__Modify__.maxLife);

		//Set the starting size and color
		particle.size = this.__Internal__Dont__Modify__.startSize;
		particle.color.set(this.__Internal__Dont__Modify__.startColor);

		//Assign a rotation
		particle.rotation = 0;
		particle.rotationSpeed = Math.randomRange(this.__Internal__Dont__Modify__.negRotation, this.__Internal__Dont__Modify__.posRotation);

		//Give the particle a starting position
		switch (this.__Internal__Dont__Modify__.type) {
			case EmitterType.POINT:
			case EmitterType.DIRECTION:
				particle.position.reset();
				break;
			case EmitterType.LINE:
				//Get the adjacent vector
				let adj = this.__Internal__Dont__Modify__.direction.right;

				//Backtrack along the line by half of the line length
				let lineStart = adj.multi(-this.__Internal__Dont__Modify__.lineLength / 2);

				//Get a random position along the line
				particle.position = lineStart.addSet(adj.multiSet(Math.random() * this.__Internal__Dont__Modify__.lineLength));
				break;
			default:
				throw new Error("Unexpected error occured. Unkown emitter type " + this.__Internal__Dont__Modify__.type + " (Type " + typeof this.__Internal__Dont__Modify__.type + ") was used instead. Please use one of the values defined in the EmitterType object");
		}

		//Check if the emitter is emitting into World Space
		if (this.__Internal__Dont__Modify__.emitterSpace === EmitterSpace.WORLD)
			particle.position.addSet(this.__Internal__Dont__Modify__.position);

		//Assign a direction to the particle
		switch (this.__Internal__Dont__Modify__.type) {
			case EmitterType.POINT:
				particle.velocity.x = Math.randomRange(-1, 1);
				particle.velocity.y = Math.randomRange(-1, 1);
				particle.velocity.normalize();
				break;
			case EmitterType.DIRECTION:
			case EmitterType.LINE:
				particle.velocity.set(this.__Internal__Dont__Modify__.direction);
				break;
		}

		//Generate a random velocity magnitude
		particle.velocity.multiSet(Math.randomRange(this.__Internal__Dont__Modify__.minVelocity, this.__Internal__Dont__Modify__.maxVelocity));
	},

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	/////                                                                                                            ////
	/////                                                   Main Functions                                           ////
	/////                                                                                                            ////
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/*
		ParticleEmitter : update - Update the positions and values of the particles being emitted
		04/07/2017

		param[in] pDelta - The delta time for the cycle
	*/
	update: function(pDelta) {
		//Create an active flag
		let active = true;

		//Add onto the runtime progress
		if (this.__Internal__Dont__Modify__.running && this.__Internal__Dont__Modify__.runTime >= 0) {
			//Add onto the progress timer
			this.__Internal__Dont__Modify__.progress += pDelta;

			//Check if the runtime is over
			if (this.__Internal__Dont__Modify__.progress >= this.__Internal__Dont__Modify__.runTime)
				active = false;
		}

		//Update the active particles
		for (var i = 0; i < this.__Internal__Dont__Modify__.activeParticles;) {
			//Get the current particle
			let particle = this.__Internal__Dont__Modify__.particles[i];

			//Increase the lifetime
			particle.lifetime += pDelta;

			//Check if the particle is dead
			if (particle.lifetime >= particle.lifespan) {
				//Swap with the last alive particle
				this.__Internal__Dont__Modify__.particles[i] = this.__Internal__Dont__Modify__.particles[this.__Internal__Dont__Modify__.activeParticles - 1];
				this.__Internal__Dont__Modify__.particles[this.__Internal__Dont__Modify__.activeParticles - 1] = particle;

				//Reduce the active particle count
				this.__Internal__Dont__Modify__.activeParticles--;

				//Continue to the next particle
				continue;
			}

			//Move the active particle
			particle.position.addSet(particle.velocity.multi(pDelta));

			//Lerp for the size of the particle
			particle.size = Math.lerp(this.__Internal__Dont__Modify__.startSize, this.__Internal__Dont__Modify__.endSize, particle.lifetime / particle.lifespan);

			//Lerp for the color of the particle
			particle.color = colorLerp(this.__Internal__Dont__Modify__.startColor, this.__Internal__Dont__Modify__.endColor, particle.lifetime / particle.lifespan);

			//Add the rotation
			particle.rotation += particle.rotationSpeed * pDelta;

			//Increase the loop progress
			i++;
		}

		//Spawn new particles
		if (this.__Internal__Dont__Modify__.running && active) {
			//Add time to the emitter timer
			this.__Internal__Dont__Modify__.emitTimer += pDelta;

			//Emit while particles are owed
			while (this.__Internal__Dont__Modify__.emitTimer >= this.__Internal__Dont__Modify__.emitRate) {
				//Emit a new particle
				this.emit();

				//Reduce the emit timer by the emit rate
				this.__Internal__Dont__Modify__.emitTimer -= this.__Internal__Dont__Modify__.emitRate;
			}
		}

		//Return alive state
		return (active || this.__Internal__Dont__Modify__.activeParticles > 0);
	},

	/*
		ParticleEmitter : draw - Render the particles to the canvas
		04/07/2017

		param[in] pGraphics - The Graphics object being used to render
	*/
	draw: function(pGraphics) {
		//Flag if the rendering is to be done in local space
		let localFlag = this.__Internal__Dont__Modify__.emitterSpace === EmitterSpace.LOCAL;

		//Check the emission space of the emitter
		if (localFlag) pGraphics.beginRender(this.__Internal__Dont__Modify__.position.x, this.__Internal__Dont__Modify__.position.y);

		//Loop through all active particles
		for (var i = 0; i < this.__Internal__Dont__Modify__.activeParticles; i++) {
			//Get the current particle
			let particle = this.__Internal__Dont__Modify__.particles[i];

			//Create the particle transform
			let transform = createTransform(particle.position.x, particle.position.y, particle.rotation, particle.size, particle.size);

			//Push the transform onto the render stack
			if (pGraphics.pushTransform(transform)) {
				//Check if this Emitter is using an image
				if (this.__Internal__Dont__Modify__.image instanceof Image) {
					//Set the global alpha
					pGraphics.draw.globalAlpha = particle.color.a;

					//Render the particle image
					pGraphics.draw.drawImage(this.__Internal__Dont__Modify__.image, -.5, -.5, 1, 1);
				}

				//Draw the colored rectangle
				else {
					//Set the render the color
					pGraphics.draw.fillStyle = particle.color.rgba;

					//Render the particle square
					pGraphics.draw.fillRect(-.5, -.5, 1, 1);
				}

				//End this transform's render
				pGraphics.endRender();
			}
		}

		//Check for local emission space to end render
		if (localFlag) pGraphics.endRender();

		//Reset the global alpha
		pGraphics.draw.globalAlpha = 1;
	},
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Particle
 *      Author: Mitchell Croft
 *      Date: 25/10/2016
 *
 *      Version: 1.0
 *
 *      Requires:
 *      Vec2.js, Color.js
 *
 *      Purpose:
 *      Store relevant information related to the position
 *      and size of an individual particle
 **/

/*
    Particle : Constructor - Initialise with default values
    25/10/2016
*/
function Particle() {
	this.position = new Vec2();
	this.velocity = new Vec2();
	this.color = new Color();
	this.size = 0;
	this.rotation = 0;
	this.rotationSpeed = 0;
	this.lifetime = 0;
	this.lifespan = 0;
};