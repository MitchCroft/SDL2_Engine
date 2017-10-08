#pragma once

//! Include the base values for SDL2 Resources
#include "../ResourceValues.hpp"

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
		 *		Name: LocalResourceBase
		 *		Author: Mitchell Croft
		 *		Created: 04/10/2017
		 *		Modified: 05/10/2017
		 *
		 *		Purpose:
		 *		Provide a base point for resources to be created from
		**/
		class SDL2_LIB_INC LocalResourceBase {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Information--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//! Flag the type of resource that is inheriting this base
			const EResourceType type;

			/*
				LocalResourceBase : status - Retrieve the status of the current Resource
				Created: 05/10/2017
				Modified: 05/10/2017

				return const EResourceLoadStatus& - Returns a constant reference to the status value
			*/
			inline const EResourceLoadStatus& status() const { return mStatus; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Resource Interface----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				LocalResourceBase : Constructor - Initialise with default values
				Created: 04/10/2017
				Modified: 04/10/2017

				param[in] pType - The type of resource that is being instantiated
			*/
			LocalResourceBase(const EResourceType& pType) : type(pType), mStatus(EResourceLoadStatus::Unloaded) {}

			/*
				LocalResourceBase : Destructor - Force disposal call
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			inline virtual ~LocalResourceBase() {}

			/*
				LocalResourceBase : dispose - Clear allocated memory
				Created: 04/10/2017
				Modified: 04/10/2017
			*/
			virtual void dispose() = 0;

		protected:

			//! Store the load status of the resource
			EResourceLoadStatus mStatus;
		};
	}
}
