#include "Globals.hpp"

//! Include the Singleton Interface object
#include "Utilities/IGlobal.hpp"

//! Include the required standard objects
#include <unordered_map>
#include <vector>

namespace SDL2_Engine {

	//! Declare the static singleton instance
	Globals* Globals::mInstance = nullptr;

	/*
	 *		Name: GlobalsInternalData
	 *		Author: Mitchell Croft
	 *		Created: 04/10/2017
	 *		Modified: 04/10/2017
	 *		
	 *		Purpose:
	 *		Store internal standard library objects
	**/
	struct Globals::GlobalsInternalData {
		//! Store the map of the separate Global interfaces
		std::unordered_map<typeID, Utilities::IGlobal*> interfaces;

		//! Store the order in which to update the Global interfaces
		std::vector<typeID> updateOrder;
	};

	/*
		Globals : Constructor - Initialise with default values
		Created: 19/07/2017
		Modified: 04/10/2017
	*/
	Globals::Globals() : mData(nullptr) {}

	/*
		Globals : introduceInterface - Setup a new interface object for addition into the Globals map
		Created: 19/07/2017
		Modified: 09/10/2017

		param[in] pInter - A pointer to the new interface object to setup
		param[in] pID - The ID of the original type of the interface

		return IGlobal* - Returns a pointer if the interface was created successfully or nullptr if it failed
	*/
	Utilities::IGlobal* Globals::introduceInterface(Utilities::IGlobal* pInter, const typeID& pID) {
		//Attempt to setup the interface
		if (!pInter->createInterface()) {
			//Destroy the interface
			pInter->destroyInterface();

			//Delete the pointer
			delete pInter;

			//return failure
			return nullptr;
		}

		//Add the interface to the management values
		mData->interfaces[pID] = pInter;
		mData->updateOrder.push_back(pID);

		//Return success
		return pInter;
	}

	/*
		Globals : interfaceExists - Check to see if a specified interface exists
		Created: 04/10/2017
		Modified: 04/10/2017

		param[in] pID - The ID of the interface to search for

		return bool - Returns true if an Interface with the specified ID exists in the Manager
	*/
	bool Globals::interfaceExists(const typeID& pID) const { return (mData->interfaces.find(pID) != mData->interfaces.end()); }

	/*
		Globals : retrieve - Retrieve a pointer to the specified interface
		Created: 04/10/2017
		Modified: 04/10/2017

		param[in] pID - The ID of the interface to search for

		return IGlobal* - Returns a IGlobal interface pointer to the specified interface
	*/
	Utilities::IGlobal* Globals::retrieve(const typeID& pID) const {
		//Look for the interface
		auto iter = mData->interfaces.find(pID);

		//Return the pointer
		return (iter != mData->interfaces.end() ? iter->second : nullptr);
	}

	/*
		Globals : create - Create the Globals singleton
		Created: 19/07/2017
		Modified: 04/10/2017
	*/
	void Globals::create() {
		//Ensure the Singleton hasn't been created already
		if (mInstance) throw new std::runtime_error("Can not create Globals Singleton instance as it already exists. Are you calling Globals::create() in multiple places?");

		//Create the singleton instance
		mInstance = new Globals();

		//Create the data container
		mInstance->mData = new GlobalsInternalData();
	}

	/*
		Globals : update - Updates all internal IGlobal interface objects
		Created: 19/07/2017
		Modified: 04/10/2017
	*/
	void Globals::update() {
		//Loop through the different interfaces and call their update functions
		for (size_t i = 0, length = mInstance->mData->updateOrder.size(); i < length; i++)
			mInstance->mData->interfaces[mInstance->mData->updateOrder[i]]->update();
	}

	/*
		Globals : destroy - Destroy all internal IGlobal interface objects in the reverse order of creation
		Created: 19/07/2017
		Modified: 04/10/2017
	*/
	void Globals::destroy() {
		//Check the singleton instance has been created
		if (mInstance) {
			//Check the STL objects where created
			if (mInstance->mData) {
				//Reverse through the interfaces and destroy them
				for (int i = (int)mInstance->mData->updateOrder.size() - 1; i >= 0; i--) {
					//Get the interface
					auto inter = mInstance->mData->interfaces[mInstance->mData->updateOrder[i]];

					//Run the destroy function
					inter->destroyInterface();

					//Delete the interface
					delete inter;
				}

				//Delete the data object
				delete mInstance->mData;
			}

			//Delete the singleton instance
			delete mInstance;

			//Reset the pointer
			mInstance = nullptr;
		}
	}
}