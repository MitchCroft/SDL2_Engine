#pragma once

//! Include the required Map Generation object
#include "Region.hpp"

namespace BombSquad {
	/*
	 *		Name: RegionIdentifier
	 *		Author: Mitchell Croft
	 *		Created: 08/11/2017
	 *		Modified: 08/11/2017
	 *		
	 *		Purpose:
	 *		Identify specific areas of coordinates on a gameMap
	**/
	struct RegionIdentifier {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------------Values------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Store the regions on the map
		std::vector<Region> regions;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Construction---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			RegionIdentifier : Constructor - Initialise with default values
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pIdent - An ETileType value identifying the specifier for the region
			param[in] pMap - A gameMap object containing the values to process
		*/
		RegionIdentifier(const ETileType& pIdent, const gameMap& pMap);
	};
}