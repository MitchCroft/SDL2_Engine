#pragma once

#include "../../_Root/Types.hpp"

#include "SDL2Resource.hpp"
#include "../ResourceTypePrototypes.hpp"

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Resources singleton
		class Resources;

		namespace ResourceTypes {
			/*
			 *		Name: SDL2Resource (Generic)
			 *		Author: Mitchell Croft
			 *		Created: 02/02/2017
			 *		Modified: 03/02/2017
			 *
			 *		Purpose:
			 *		Store the data read from an external file
			**/

			template<>
			class __SDL2Resource<Generic> : public ResourceBase {
				//! Assign as a friend of the Resource Manager
				friend class Resources;

				//! Store an array of data
				unsigned char* mData;

				//! Store the length of the data array
				uint mSize;

				//! Constructor / Destructor
				inline __SDL2Resource(const resourceID& pID) :
					ResourceBase(pID, EResourceType::Generic),
					mData(nullptr), mSize(0U),
					data(mData), size(mSize) {}

			public:
				//! Provide property access to the data
				Utilities::Properties::ReadOnlyProperty<const unsigned char*> data;
				Utilities::Properties::ReadOnlyProperty<uint> size;

				//! Define clear memory operation
				inline void freeMemory() override { if (mData) { delete[] mData; mData = nullptr; mStatus = EResourceLoadStatus::Freed; } }

				//! Destructor is public to allow for resource destruction
				inline ~__SDL2Resource() override = default;
			};
		}
	}
}