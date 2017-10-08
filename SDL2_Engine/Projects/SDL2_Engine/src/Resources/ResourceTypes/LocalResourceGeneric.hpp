#pragma once

//! Include the resource base point to inherit from
#include "LocalResource.hpp"

namespace SDL2_Engine {
	//! Prototype the Resource Manager
	class Resources;

	namespace ResourceTypes {
		/*
		 *		Name: LocalResource (Generic)
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 05/10/2017
		 *		
		 *		Purpose:
		 *		Specialise the LocalResource object to operate on generic files
		**/
		template<>
		class SDL2_LIB_INC __LocalResource<Generic> : public LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------------Data------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Generic) : data - Get the data stream 
				Created: 04/10/2017
				Modified: 04/10/2017

				return const char* - Returns the data stream as a constant byte array
			*/
			inline const char* data() const { return mData; }

			/*
				LocalResource (Generic) : size - Get the size of data buffer (in bytes)
				Created: 04/10/2017
				Modified: 04/10/2017

				return size_t - Returns the size as a size_t value
			*/
			inline const size_t& size() const { return mSize; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------Interface Implementations-------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResource (Generic) : Destructor - Deallocate memory
				Created: 04/10/2017
				Modified: 05/10/2017
			*/
			inline ~__LocalResource() override { dispose(); }

			/*
				LocalResource (Generic) : dispose - Unload resource information
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			void dispose() override;

		private:

			//! Assign as a friend of the Resources Manager
			friend class Resources;

			//! Store the data stream
			char* mData;

			//! Store the number of bytes in the file
			size_t mSize;

			/*
				LocalResource (Generic) : Constructor - Initialise with default values
				Created: 04/10/2017
				Modified: 04/10/2017

				param[in] pPath - The path of the data to load
			*/
			__LocalResource(const char* pPath);
		};
	}
}