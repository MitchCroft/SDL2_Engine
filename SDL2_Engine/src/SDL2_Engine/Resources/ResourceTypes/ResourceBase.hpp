#pragma once

#include "../../Utilities/Properties/ReadOnlyProperty.hpp"
#include "../ResourceValues.hpp"

namespace SDL2_Engine {
	namespace Resources {
		//! Prototype the Resources singleton
		class Resources;

		namespace ResourceTypes {
			/*
			 *		Name: ResourceBase
			 *		Author: Mitchell Croft
			 *		Created: 02/02/2017
			 *		Modified: 02/02/2017
			 *
			 *		Purpose:
			 *		Provide a base point for the Resource loading aspects
			 *		of the namespace to store and manage the resource they
			 *		are responsible
			**/
			class ResourceBase {
			protected:
				//! Assign as a friend of the Resource Manager
				friend class Resources;

				//! Store a resource ID for the current resource
				const resourceID mID;

				//! Store the current status of the resource
				EResourceLoadStatus mStatus;

				//! Store the type of resource that is currently active
				const EResourceType mType;

				//! Protect the constructor / destructor
				inline ResourceBase(const resourceID& pID, const EResourceType& pType = EResourceType::Null) :
					mID(pID), mType(pType), mStatus(EResourceLoadStatus::Unloaded), id(mID), status(mStatus), type(mType) {}
				virtual inline ~ResourceBase() { freeMemory(); }

			public:
				//! Accessor read only properties
				Utilities::Properties::ReadOnlyProperty<resourceID> id;
				Utilities::Properties::ReadOnlyProperty<EResourceLoadStatus> status;
				Utilities::Properties::ReadOnlyProperty<EResourceType> type;

				//! Abstract function used to force memory to be freed
				virtual void freeMemory() {}
			};
		}
	}
}