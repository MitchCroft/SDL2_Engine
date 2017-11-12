#pragma once

//! Include the Map Generation Values
#include "MapGenerationValues.hpp"

namespace BombSquad {
	/*
	 *		Name: Region
	 *		Author: Mitchell Croft
	 *		Created: 08/11/2017
	 *		Modified: 12/11/2017
	 *		
	 *		Purpose:
	 *		Monitor a collection of coordinates as a Region on a gameMap object
	**/
	struct Region {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------------Values------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Store all the coordinates contained in the region
		coords contained;

		//! Store all the coordinates on the edge of the region
		coords edges;

		//! Store a hit map used to represent the coords in space
		hitMap locationMap;

		//! Store a depth map of the coords from the points outside of the region
		depthMap distanceFromExternal;

		//! Store the 'center of mass' for the region
		coord center;

		//! Flag if this region is connected to the 'main' region
		bool linkedToMain;

		//! Store the Regions that the current is linked to
		std::vector<Region*> links;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Construction---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Region : Constructor - Initialise with default values
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pCoords - A collection of coords that are contained within the region
			param[in] pWidth - The width of the map that the coordinates exist on
			param[in] pHeight - The height of the map that the coordinates exist on
		*/
		Region(const coords& pCoords, const size_t& pWidth, const size_t& pHeight);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------------Data Collection--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Region : analyseDepth - Flood search each coordinate to find the distance from a coordinate not 
									contained in the region
			Created: 08/11/2017
			Modified: 08/11/2017
		*/
		void analyseDepth();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------------Management----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Region : recursiveLinkToMain - Recursively traverse the links vector and toggle the linkedToMain 
										   flag
			Created: 09/11/2017
			Modified: 09/11/2017
		*/
		void recursiveLinkToMain();
	};
}