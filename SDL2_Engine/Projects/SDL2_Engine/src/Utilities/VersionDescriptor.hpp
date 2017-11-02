#pragma once

//! Include the SDL_Engine management functionality
#include "../__LibraryManagement.hpp"

namespace SDL2_Engine {
	namespace Utilities {
		/*
		 *		Name: VersionDescriptor
		 *		Author: Mitchell Croft
		 *		Created: 02/11/2017
		 *		Modified: 02/11/2017
		 *		
		 *		Purpose:
		 *		Store version information and provide user-friendly element access
		**/
		struct SDL2_LIB_INC VersionDescriptor {
			//! Store a union of the internal data values
			union {
				//! Provide an interface for the individual version data values
				struct {
					unsigned int major : 16,
								 minor :  8,
								 patch :  8;
				};

				//! Store the complete version identifier in a single ID value (Use the full 32 bits of data)
				unsigned int versionID : 32;
			};

			/*
				VersionDescriptor : Default Constructor - Initialise with a default versionID value
				Created: 02/11/2017
				Modified: 02/11/2017

				param[in] pID - A starting Version Descriptor ID (Default 0)
			*/
			inline VersionDescriptor(const unsigned int& pID = 0) : versionID(pID) {}

			/*
				VersionDescriptor : Custom Constructor - Initialise with specific version values
				Created: 02/11/2017
				Modified: 02/11/2017

				param[in] pMajor - The 'major' version value to be stored in this object
				param[in] pMinor - The 'minor' version value to be stored in this object
				param[in] pPatch - The 'patch' version value to be stored in this object
			*/
			inline VersionDescriptor(const unsigned int& pMajor, const unsigned int& pMinor, const unsigned int& pPatch) : major(pMajor), minor(pMinor), patch(pPatch) {}

			/*
				VersionDescriptor : Assignment Operator - Assign the VersionDescriptor object to the value of 32-bit ID value
				Created: 02/11/2017
				Modified: 02/11/2017

				param[in] pID - An unsigned int value representing the new version values

				return VersionDescriptor& - Returns a reference to itself
			*/
			inline VersionDescriptor& operator=(const unsigned int& pID) { versionID = pID; return *this; }
		};
	}
}