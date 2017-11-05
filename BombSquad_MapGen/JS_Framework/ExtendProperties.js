/*
 *      Name: ExtendProperties
 *      Author: Mitchell Croft
 *      Date: 27/09/2016
 *
 *      Version: 2.0
 *      Added support for setting up inheritance chains
 *
 *      Purpose:
 *      Add additional functionality to an objects
 *      prototype without overwriting pre-existing
 *      prototype information
 **/

/*
    ExtendProperties - Add the properties contained within pCollection to the
                       prototype of pObj
    27/09/2016

    param[in/out] pObj - The object type to add the properties to
    param[in] pCollection - An object containing the properties to include
                             in pObj

    Example:

    //Add a random direction property to Vec2
    ExtendProperties(Vec2, {
        get random() {
            return new Vec2(Math.random() * 2 - 1, Math.random() * 2 - 1).normalize();
        },
    });
*/
function ExtendProperties(pObj, pFirst, pSecond) {
    //Store a reference to the properties to use
    var inheritance = (typeof pFirst === "function" ? pFirst : null);
    var collection = (typeof pSecond === "object" ? pSecond : (typeof pFirst === "object" ? pFirst : null));

    //If there is an inheritance chain to implement
    if (inheritance !== null) {
        pObj.prototype = Object.create(inheritance.prototype);
        pObj.prototype.constructor = pObj;
    }

    //If there is a collection of properties to implement
    if (collection !== null) {
        //Store the descriptor of the property extracted from the collection
        var description;

        //Loop through all the properties inside the collection
        for (var prop in collection) {
            //Get the property descriptor for the prop
            description = Object.getOwnPropertyDescriptor(collection, prop);

            //Apply property if not undefined
            if (typeof description !== "undefined")
                Object.defineProperty(pObj.prototype, prop, description);
        }
    }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                Object Extensions                                           ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Math Extensions
 *      Author: Mitchell Croft
 *      Date: 23/11/2016
 *
 *      Version: 1.0
 *
 *      Purpose:
 *      Add additional functionality to the Math object
 **/

//Add angle conversion values
Math.rad2Deg = 180 / Math.PI;
Math.deg2Rad = Math.PI / 180;

/*
    Math : clamp - Clamps a specified value between a defined min and max
    24/11/2016

    param[in] pVal - The value to be clamped
    param[in] pMin - The minimum value that can be returned
    param[in] pMax - The maximum value that can be returned

    return number - Returns the value between the two points
*/
Math.clamp = function(pVal, pMin, pMax) {
    return (pVal < pMin ? pMin : (pVal > pMax ? pMax : pVal));
};

/*
    Math : clamp01 - Clamps a specified value between 0 and 1
    24/11/2016

    param[in] pVal - The value to be clamped

    return number - Returns the value clamped between 0 and 1
*/
Math.clamp01 = function(pVal) {
    return (pVal < 0 ? 0 : (pVal > 1 ? 1 : pVal));
};

/*
    Math : lerp - Linearly interpolate between two numbers
    24/11/2016

    param[in] pStart - The starting value to interpolate from
    param[in] pEnd - The ending value to interpolate to
    param[in] pT - The scale to indicate the progress between the points

    return number - Returns the interpolated value
*/
Math.lerp = function(pStart, pEnd, pT) {
    return (pStart + (pEnd - pStart) * pT);
};

/*
    Math : lerpClamped - Linearly interpolate between numbers, clamping the scale between 0 and 1
    24/11/2016

    param[in] pStart - The starting value to interpolate from
    param[in] pEnd - The ending value to interpolate to
    param[in] pT - The scale to indicate the progress between the points

    return number - Returns the interpolated value between the two points
*/
Math.lerpClamped = function(pStart, pEnd, pT) {
    //Clamp the scale between 0 and 1
    pT = Math.clamp01(pT);

    //Return the interpolation
    return (pStart + (pEnd - pStart) * pT);
};

/*
    Math : randomRange - Returns a random number between a defined range
    24/11/2016

    param[in] pMin - The minimum value of the range (inclusive)
    param[in] pMax - The maximum value of the range (exclusive)

    return number - Returns a random number between the defined range
*/
Math.randomRange = function(pMin, pMax) {
    return (Math.random() * (pMax - pMin) + pMin);
};

/*
    Math : sinT - Returns a value between 0-1 that changes over time
    20/05/2017

    param[in] pScale - A number containing the scale to apply (Default 1)

    return number - Returns a number in the range of 0-1
*/
Math.sinT = function(pScale) {
    //Clean the scale
    if (typeof pScale !== "number") pScale = 1;

    //Return the number
    return ((Math.sin(Date.now() * pScale / 1000) + 1) / 2);
};

/*
    Math : cosT - Returns a value between 0-1 that changes over time
    04/07/2017

    param[in] pScale - A number containing the scale to apply (Default 1)

    return number - Returns a number in the range of 0-1
*/
Math.cosT = function(pScale) {
    //Clean the scale
    if (typeof pScale !== "number") pScale = 1;

    //Return the number
    return ((Math.cos(Date.now() * pScale / 1000) + 1) / 2);
};

/*
 *      Name: String Extensions
 *      Author: Mitchell Croft
 *      Date: 20/05/2017
 *
 *      Version: 1.0
 *
 *      Purpose:
 *      Add string identification functionality
 **/

/*
    String : isAlpha - Checks to see if a single character is alphanumeric
    20/05/2016

    param[in] pChar - The single character to test

    return bool - Returns true if the character is alphabetical
*/
String.isAlpha = function(pChar) {
    return ((pChar >= 'a' && pChar <= 'z') || (pChar >= 'A' && pChar <= 'Z'));
};

/*
    String : isDigit - Checks to see if a single character is numerical
    20/05/2017

    param[in] pChar - The single character to test

    return bool - Returns true if the character is numerical
*/
String.isDigit = function(pChar) {
    return ((pChar >= '0') && (pChar <= '9'));
};

/*
    String : isAlphaNum - Checks to see if a single character is alphanumerical
    20/05/2017

    param[in] pChar - The single character to test

    return bool - Returns true if the character is alphanumerical
*/
String.isAlphaNum = function(pChar) {
    return (this.isAlpha(pChar) || this.isDigit(pChar));
};

/*
 *      Name: AsynchRequest
 *      Author: Mitchell Croft
 *      Date: 19/05/2017
 *
 *      Version: 1.0
 *
 *      Purpose:
 *      Send and recieve information for processing via
 *      a HTTP request
 **/

/*
    asynchRequest - Preform an asynchronous pass of information to a specified URL with
                    callback functionality to handle the response
    19/05/2017

    param[in] pURL - The URL to make the HTTP request to
    param[in] pCallbacks - An optional object containing functions that recieve text based on 
                           success state of the request. Functions should be applied to the 'success'
                           and 'failure' properties (Default null)
    param[in] pData - An object that holds information to pass to URL (Default null)
*/
function asynchRequest(pURL, pCallbacks, pData) {
    //Create the HTTP request
    let request = new XMLHttpRequest();

    //Set the response function
    request.onreadystatechange = function() {
        //Check the state
        if (request.readyState === XMLHttpRequest.DONE) {
            //Clean the callbacks object
            pCallbacks = Validate.type(pCallbacks, "object", {});

            //Check the request status
            if (request.status === 200) {
                //Raise the callback if it exists
                if (typeof pCallbacks["success"] === "function")
                    pCallbacks["success"](request.responseText);
            }

            //Output error message
            else {
                //Check if there is a callback
                if (typeof pCallbacks["failure"] === "function") pCallbacks["failure"]({
                    url: pURL,
                    error: request.status
                });

                //Otherwise ouput error message
                else throw new Error("Error occured when attempting to access " + pURL + ". ERROR: " + request.status);
            }
        }
    };

    //Open the connection to the requested URL
    request.open("POST", pURL, true);

    //Check if the data object exists
    let data = "";
    if (typeof pData === "object") {
        //Extract data from the object
        for (let key in pData) {
            //Ensure the property belongs to the object
            if (Object.prototype.hasOwnProperty.call(pData, key)) {
                //Check if there are previous values
                if (data !== "") data += "&";

                //Add the name of the key to the data string
                data += key + "=";

                //Encode the keys value
                data += encodeURIComponent(pData[key]);
            }
        }

        //Set the request header for data if data was extracted
        if (data !== "") request.setRequestHeader("Content-Type", "application/x-www-form-urlencoded");
    }

    //Send the request
    if (data === "") request.send();
    else request.send(data);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Path
 *      Author: Mitchell Croft
 *      Date: 28/11/2016
 *
 *      Version: 1.0
 *
 *      Purpose:
 *      Provide some basic file path functionality
 **/
var Path = {
    /*
        Path : getDirectory - Get the directory from a passed in filepath
        28/11/2016

        param[in] pFilePath - The filepath to find the final directory of

        return string - Returns the directory path as a string
    */
    getDirectory: function(pFilePath) {
        //Check if there is a '.' in the filepath
        if (pFilePath.lastIndexOf(".") === -1) {
            //Check if there is a seperator at the end of the string
            if (Math.max(pFilePath.lastIndexOf("\\"), pFilePath.lastIndexOf("/"), pFilePath.lastIndexOf("\/")) + 1 < pFilePath.length)
                return pFilePath + "/";

            //Otherwise return the basic string
            return pFilePath;
        }

        //Otherwise find the last seperator character
        return pFilePath.substring(0, Math.max(pFilePath.lastIndexOf("\\"), pFilePath.lastIndexOf("/"), pFilePath.lastIndexOf("\/")) + 1);
    },
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Validate
 *      Author: Mitchell Croft
 *      Date: 20/05/2017
 *
 *      Version: 1.0
 *
 *      Purpose:
 *      Provide some basic validation methods
 **/
var Validate = {
    /*
        Validate : type - Check the passed in value match a desired type
        20/05/2017

        param[in] pVal - The value to check
        param[in] pType - A string containing the type that should be evaluated
        param[in] pDefault - A default value to be given if the types don't match.
                             Only required if pStrict flag is set to false
        param[in] pStrict - A boolean flag to be used to indicate if an error should be thrown
                            if the types don't match (Default false)

        return value - Returns either the pVal or pDefault value depending on the results
    */
    type: function(pVal, pType, pDefault, pStrict) {
        //Clean the flags
        if (typeof pType !== "string") throw new Error("pType parameter must be a string to perform typeof comparison");
        if (typeof pStrict !== "boolean") pStrict = false;

        //If the value is not a match
        if (typeof pVal !== pType) {
            //Check if this validation is strict
            if (pStrict) throw new Error("Validation of value failed. " + pVal + " was supposed to be " + pType + " (Instead was '" + typeof pVal + "')");

            //Otherwise return the default
            else return pDefault;
        }

        //Return the value
        else return pVal;
    },

    /*
        Validate : instance - Check the passed in value is an instance of a desired type
        20/05/2017

        param[in] pVal - The value to check
        param[in] pType - The type to check for instance
        param[in] pDefault - A default object to be used if there is not an instance match
                             Only required if pStrict flag is set to false (Default uses new instance
                             of pType)
        param[in] pStrict - A boolean flag to be used to indicate if an error should be thrown
                            if pVal is not instance of pType (Default false)

        return value - Returns either the pVal or a new instance of pType depending the results
    */
    instance: function(pVal, pType, pDefault, pStrict) {
        //Clean the flags
        if (typeof pType !== "function") throw new Error("pType parameter must be a function to perform instanceof check");
        if (typeof pStrict !== "boolean") pStrict = false;

        //If the value is not an instance
        if (!(pVal instanceof pType)) {
            //Check if the validation is strict
            if (pStrict) throw new Error("Validation of instance failed. " + pVal + " was supposed to be an instance of " + pType);

            //Check if a default has been set
            else if (typeof pDefault !== "undefined") return pDefault;

            //Return a new instance of the object
            else return new pType();
        }

        //Else return the value
        else return pVal;
    },
};