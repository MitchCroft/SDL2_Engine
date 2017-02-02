#pragma once

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

namespace SDL2_Engine {
	namespace Resources {
		/*
		 *		Name: SDL2Resource (Generic)
		 *		Author: Mitchell Croft
		 *		Created: 02/02/2017
		 *		Modified: 02/02/2017
		 *		
		 *		Purpose:
		 *		Store the data read from an external file 
		**/

		template<>
		class SDL2Resource<Generic> : public ResourceBase {
			//! Assign as a friend of the Resource Manager
			friend class Resources;

			//! Store an array of data
			unsigned char* mData;

			//! Store the length of the data array
			unsigned int mSize;

			//! Constructor / Destructor
			inline SDL2Resource(const resourceID& pID) : 
				ResourceBase(pID, EResourceType::Generic),
				mData(nullptr), mSize(0U),
				data(mData), size(mSize) {}
			inline ~SDL2Resource() override = default;

		public:
			//! Provide property access to the data
			Utilities::Properties::ReadOnlyProperty<const unsigned char*> data;
			Utilities::Properties::ReadOnlyProperty<unsigned int> size;

			//! Define clear memory operation
			inline void freeMemory() override { if (mData) { delete[] mData; mData = nullptr; mStatus = EResourceLoadStatus::Freed; } }
		};
	}
}