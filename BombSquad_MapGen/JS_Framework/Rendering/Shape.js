/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                 Object Definition                                          ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: Shape
 *      Author: Mitchell Croft
 *      Date: 09/08/2016
 *
 *      Requires:
 *      Color.js, Mat3.js, ExtendProperties.js
 *
 *      Version: 2.0
 *
 *      Purpose:
 *      Provide a method for storing 2D dimensional shape
 *      coordinate values for rendering using a transform.
 *      Coordinate values should be defined in a clockwise
 *      order.
 **/

/*
    Shape : Constructor - Initialise with default values
    09/08/2016

    param[in] pCopy - A Shape object to copy the values of (Default undefined)

    Example:

    //Create a new shape
    var myShape = new Shape();

    OR

    //Create a copy of my shape
    var copyShape = new Shape(myShape);
*/
function Shape(pCopy) {
    //Switch on the parameter type
    switch (typeof pCopy) {
        case "object":
            if (pCopy instanceof Shape) {
                //Copy point information
                this.points = [];
                for (var i = 0; i < pCopy.points.length; i++)
                    this.points[i] = new Vec2(pCopy.points[i]);

                //Copy the border line width
                this.outlineWidth = pCopy.outlineWidth;

                //Copy the fill color
                this.fillColor = new Color(pCopy.fillColor);

                //Copy the border color
                this.outlineColor = new Color(pCopy.outlineColor);
                break;
            }
        default:
            //Store an array of local space positions making up the shape (Vec2 objects)
            this.points = [];

            //Store the width of border line
            this.outlineWidth = 1;

            //Store the color to fill the shape in with
            this.fillColor = null;

            //Store the color to outline the shape with
            this.outlineColor = null;
            break;
    }
};

ExtendProperties(Shape, {
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                               Property Definitions                                         ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
      Shape : clone - Return a copy of the current object
      03/07/2017

      return Shape - Returns a new Shape instanceof

      Example:

      //Create a copy of the player shape
      var cpy = playerShape.clone;
    */
    get clone() {
        return new Shape(this);
    },

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /////                                                                                                            ////
    /////                                               Main Functions                                               ////
    /////                                                                                                            ////
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /*
        Shape : draw - Render the current Shape object to the passed in context
        09/08/2016

        param[in] pCtx - The context the the shape should be rendered to
        param[in] pTrans - An optional Mat3 object to be applied to the shapes
                            coordinate values
        param[in] pPointSize - Draws circles over the points of the shape
                                for debugging purposes in the specified size

        Example:

        //Draw simple shape
        myShape.draw(context);

        OR

        //Draw the player using their transform
        playerShape.draw(context, playerGlobalMat3);

        OR

        //Debug the player shape
        playerShape.draw(context, playerGlobalMat3, 3);
    */
    draw: function(pCtx, pTrans, pPointSize) {
        //Check there are colors to render the shape with
        if ((this.fillColor instanceof Color || this.outlineColor instanceof Color) && this.points.length) {
            //Save if a transform has been set
            var transformSet = pTrans instanceof Mat3;

            //Start the rendering path
            pCtx.beginPath();

            //Get the initial coordinate point
            var point = (transformSet ? pTrans.multiVec(this.points[0]) :
                this.points[0]);

            //Define the starting point
            pCtx.moveTo(point.x, point.y);

            //Loop through the points to define the path
            for (var i = 1, j = 0; j < this.points.length; i = (i + 1) % this.points.length, j++) {
                //Get the next point
                point = (transformSet ? pTrans.multiVec(this.points[i]) :
                    this.points[i]);

                //Draw a line to the point
                pCtx.lineTo(point.x, point.y);
            }

            //Close the shape
            pCtx.closePath();

            //Check if a fill color has been set
            if (this.fillColor instanceof Color) {
                //Change the fill style in use
                pCtx.fillStyle = this.fillColor.rgba;

                //Fill in the shape
                pCtx.fill();
            }

            //Check if the outline color was set
            if (this.outlineColor instanceof Color) {
                //Change the stroke style
                pCtx.strokeStyle = this.outlineColor.rgba;

                //Change the line width
                pCtx.lineWidth = this.outlineWidth;

                //Outline the shape
                pCtx.stroke();
            }

            //Check for debugging points
            if (typeof pPointSize === "number") {
                //Loop through the points to draw them
                for (var i = 0; i < this.points.length; i++) {
                    //Get the point
                    var point = (transformSet ? pTrans.multiVec(this.points[i]) :
                        this.points[i]);

                    //Start rendering circle
                    pCtx.beginPath();

                    //Define the circle
                    pCtx.arc(point.x, point.y, pPointSize, 0, Math.PI * 2);

                    //Finish rendering the circle
                    pCtx.closePath();

                    //Fill the circle
                    pCtx.fillStyle = "black";
                    pCtx.fill();

                    //Outline the circle
                    pCtx.lineWidth = 1;
                    pCtx.strokeStyle = "white";
                    pCtx.stroke();
                }
            }
        }
    },

    /*
        Shape : setInterpolatedPoints - Set the number of points for the shape. New
                                        points generated based on the current Shapes
                                        existing points. For best results use Shapes
                                        with an equal number of points
        09/08/2016

        param[in] pCount - The number of points that the shape should have (Minimum 3)

        return this - Returns itself once the function has ended

        Example:

        //Improve the point quality of a shape
        myShape.setInterpolatedPoints(50);
    */
    setInterpolatedPoints: function(pCount) {
        //Ensure there are at least three points in the shape
        if (this.points.length < 3 || pCount < 3 || this.points.length === pCount) return;

        //Create an array to hold the new point information
        var newPoints = [];

        //Get the progress increase per point
        var increment = this.points.length / pCount;

        //Loop through to generate the new points
        for (var i = 0, prog = 0; i < pCount; i++, prog += increment) {
            //Get the starting lerp index
            var startIndex = Math.floor(prog);

            //Get the ending lerp index
            var endIndex = (startIndex + 1) % this.points.length;

            //Get the lerp scale
            var scale = prog % 1;

            //Get the new point
            newPoints[i] = this.points[startIndex].lerp(this.points[endIndex], scale);
        }

        //Set the shapes new points
        this.points = newPoints;

        //Return itself
        return this;
    },

    /*
        Shape : morph - Morph the current Shape into the passed in shape value over time
        09/08/2016

        param[in] pEnd - The Shape object to be morphed into (Returned when pT is 1)
        param[in] pT - The scale with which to interpolate between the Shapes (0 - 1)

        return Shape - Returns a Shape object with the interpolated values

        Example:

        //Morph between two shapes over time
        var morphedShape = myShape.morph(otherShape, (Math.sin(Date.now() * 0.001) + 1) / 2);
    */
    morph: function(pEnd, pT) {
        //Set recursive function to find the HCD
        var HCD = function(pA, pB) {
            //If pB has no value return a
            if (!pB) return pA;

            //Recurse down
            return HCD(pB, pA % pB);
        };

        //Set function to find the lowest common multiple
        var LCM = function(pA, pB) {
            return Math.abs((pA * pB) / HCD(pA, pB));
        };

        //Find the number of points that need to be found
        var pointCount = LCM(this.points.length, pEnd.points.length);

        //Ensure the Shapes have the same number of points
        var start = (this.points.length === pointCount ? this :
            new Shape(this).setInterpolatedPoints(this.points.length * pEnd.points.length));
        var end = (pEnd.points.length === pointCount ? pEnd :
            new Shape(pEnd).setInterpolatedPoints(this.points.length * pEnd.points.length));

        //Create a new Shape object
        var temp = new Shape();

        //Interpolate the point information
        for (var i = 0; i < start.points.length; i++)
            temp.points[i] = start.points[i].lerp(end.points[i], pT);

        //Interpolate line width
        temp.outlineWidth = start.outlineWidth + (end.outlineWidth - start.outlineWidth) * pT;

        //Test fill color values
        if (start.fillColor instanceof Color || end.fillColor instanceof Color) {
            temp.fillColor = (start.fillColor instanceof Color ? start.fillColor : new Color(0, 0, 0, 0)).lerp(
                end.fillColor instanceof Color ? end.fillColor : new Color(0, 0, 0, 0),
                pT);
        }

        //Test outline color values
        if (start.outlineColor instanceof Color || end.outlineColor instanceof Color) {
            temp.outlineColor = (start.outlineColor instanceof Color ? start.outlineColor : new Color(0, 0, 0, 0)).lerp(
                end.outlineColor instanceof Color ? end.outlineColor : new Color(0, 0, 0, 0),
                pT);
        }

        //Return the morphed shape
        return temp;
    },
});

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                           Shape Type Defines                                               ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 *      Name: ShapeType
 *      Author: Mitchell Croft
 *      Date: 09/08/2016
 *
 *      Purpose:
 *      Name the numerical values given to primitive shapes
 *      that can be generated by the factory function createPrimitiveShape
 **/

var ShapeType = { SQUARE: 0, CIRCLE: 1, TRIANGLE: 2 };

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////                                                                                                            ////
/////                                                   Factory Function                                         ////
/////                                                                                                            ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
    Semi-constant variable, not modified through regular operation
    but can be modified at runtime in order to adjust the quality
    of the circles generated.
*/
var PRIMGEN_CIRCLE_SIDES = 25;

/*
    createPrimitiveShape - Get a new Shape object setup as a passed in primitive type
    09/08/2016

    param[in] pType - The type of shape to create from the ShapeType
                       value list (E.g. ShapeType.SQUARE)
    param[in] pSize - The size of the shape within its local space (Default 1)

    return Shape - Returns a new shape object setup with the points
                    in the defined shape

    Example:

    //Create a box obstacle
    var boxObstacle = createPrimitiveShape(ShapeType.SQUARE, 5);
*/
function createPrimitiveShape(pType, pSize) {
    //Create a new Shape
    var temp = new Shape();

    //Ensure size parameter is valid
    if (typeof pSize !== "number") pSize = 1;

    //Store the half size
    var halfSize = pSize / 2;

    //Switch on the type of shape to create
    switch (pType) {
        case ShapeType.SQUARE:
            //Define the square points
            temp.points.push(new Vec2(-1, -1).multiSet(halfSize));
            temp.points.push(new Vec2(1, -1).multiSet(halfSize));
            temp.points.push(new Vec2(1, 1).multiSet(halfSize));
            temp.points.push(new Vec2(-1, 1).multiSet(halfSize));
            break;
        case ShapeType.CIRCLE:
            //Get the angle increase per iteration
            var angleIncrease = (Math.PI * 2) / PRIMGEN_CIRCLE_SIDES;

            //Get the starting direction
            var dir = new Vec2(0, -1);

            //Iterate through sides and create points
            for (var i = 0; i < PRIMGEN_CIRCLE_SIDES; i++) {
                //Rotate the direction
                dir.rotate(angleIncrease);

                //Create the point
                temp.points.push(dir.multi(halfSize));
            }
            break;
        case ShapeType.TRIANGLE:
            //Define the traingle points
            temp.points.push(new Vec2(-1, -1).multiSet(halfSize));
            temp.points.push(new Vec2(1, -1).multiSet(halfSize));
            temp.points.push(new Vec2(0, 1).multiSet(halfSize));
            break;
        default:
            console.log("Unable to create a primitive shape based on the type of " + pType);
            break;
    }

    //Return the new shape
    return temp;
};