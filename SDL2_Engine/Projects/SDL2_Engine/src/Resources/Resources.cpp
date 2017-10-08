#include "Resources.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_render.h>

//! Include STL objects that will be used to track and monitor resources
#include <unordered_map>

namespace SDL2_Engine {
	/*
	 *		Name: ResourcesInternalData
	 *		Author: Mitchell Croft
	 *		Created: 05/10/2017
	 *		Modified: 05/10/2017
	 *		
	 *		Purpose:
	 *		Manage the internal STL objects 
	**/
	struct Resources::ResourcesInternalData {
		//! Maintain a reference to the active Renderer object
		SDL_Renderer* renderer;

		//! Keep a map of all of the active resources
		std::unordered_map<size_t, LocalResourceBase> loadedResources;
	};

	/*
		Resources : Constructor - Initialise with default values
		Created: 05/10/2017
		Modified: 05/10/2017

		param[in] pRenderer - A pointer to the Renderer object, to be used to create Textures
	*/
	Resources::Resources(SDL_Renderer* pRenderer) : mData(nullptr) {
		//Create the internal data object
		mData = new ResourcesInternalData();

		//Store the Renderer pointer
		mData->renderer = pRenderer;
	}

	/*
		Resources : createInterface - Verify and setup starting information
		Created: 05/10/2017
		Modified: 05/10/2017

		return bool - Returns true if the Resources Manager was setup correctly
	*/
	bool Resources::createInterface() {
		return true;
		return (mData->renderer != nullptr);
	}

	/*
		Resources : destroyInterface - Deallocate internal memory allocated
		Created: 05/10/2017
		Modified: 05/10/2017
	*/
	void Resources::destroyInterface() {
		//Loop through all resources for forced disposal
		for (auto it : mData->loadedResources) 
			it.second->dispose();

		//Delete the data object
		delete mData;
	}

	/*
		Resources : update - Update active local resources
		Created: 05/10/2017
		Modified: 05/10/2017
	*/
	void Resources::update() {
		//Loop through all resources
		for (auto it = mData->loadedResources.begin(); it != mData->loadedResources.end();) {
			//Check if the resource can be removed
			if (it->second.unique() || (int)it->second->status() & ((int)EResourceLoadStatus::Error | (int)EResourceLoadStatus::Freed)) {
				//Force disposal of the resource
				it->second->dispose();

				//Remove the resource the managed list
				mData->loadedResources.erase(it++);
			}

			//Otherwise continue
			else ++it;
		}
	}

	/*
		Resources : getRenderer - Retrieve the Renderer object from the internal data
		Created: 05/10/2017
		Modified: 05/10/2017

		return SDL_Renderer* - Returns a pointer to the SDL_Renderer object
	*/
	SDL_Renderer* Resources::getRenderer() { return mData->renderer; }

	/*
		Resources : getResource - Retrieve a specific resource by ID
		Created: 05/10/2017
		Modified: 05/10/2017

		param[in] pID - The ID of the resource to retrieve

		return LocalResourceBase - Returns a shared pointer to a base Resource object or nullptr if not found
	*/
	LocalResourceBase Resources::getResource(const size_t& pID) const {
		//Search for the Resource
		auto it = mData->loadedResources.find(pID);

		//Return the resource
		return (it != mData->loadedResources.end() ? it->second : nullptr);
	}

	/*
		Resources : storeResource - Store a Local Resource under a specific ID value
		Created: 05/10/2017
		Modified: 05/10/2017

		param[in] pResource - A shared pointer to the resource to store
		param[in] pID - The ID to store the resource under
	*/
	void Resources::storeResource(LocalResourceBase pResource, const size_t& pID) { mData->loadedResources[pID] = pResource; }
}