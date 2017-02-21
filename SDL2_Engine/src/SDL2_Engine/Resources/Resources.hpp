#pragma once

#include <memory>
#include <unordered_map>
#include <mutex>
#include <functional>

#include "../_Root/Types.hpp"
#include "ResourceTypes.hpp"

//! Prototype the SDL_Render object
struct SDL_Renderer;

namespace SDL2_Engine {
	namespace Resources {

		//! Define a simple shared pointer alias for resource types
		template<class T>
		using SDL2Resource = std::shared_ptr<ResourceTypes::__SDL2Resource<T>>;

		/*
		 *		Name: Resources 
		 *		Author: Mitchell Croft
		 *		Created: 03/02/2017
		 *		Modified: 03/02/2017
		 *		
		 *		Purpose:
		 *		Load and manage resources that are created
		 *		based on external files. The full list of supported 
		 *		resources can be found in ResourceTypePrototypes.hpp
		**/
		class Resources {
			//! Singleton values
			static Resources* mInstance;
			Resources();
			~Resources() = default;	

			//! Store the resources directory as a std::string
			std::string mResourcesDir;

			//! Keep a mutex to lock down files when editing
			std::recursive_mutex mLock;

			//! Store a pointer to the SDL_Renderer
			const SDL_Renderer* mRenderReference;

			//! Store the paths to the files that were found
			std::unordered_map<std::string, std::string> mFileLookup;

			//! Store resource specific information, based on their Hash ID
			std::unordered_map<resourceID, std::shared_ptr<ResourceTypes::ResourceBase>> mResourceLookup;
			std::unordered_map<resourceID, std::string> mResourceOrigin;
			 
			//! Store debug output information
			DebugCallback mWarningCB;
			DebugCallback mErrorCB;

			//! Recursive function used to find all resources
			void recurseResources(std::string pFilepath, std::vector<std::string>& pToFill) const;

			//! Function to load all of the resource types
			void loadResource(std::shared_ptr<ResourceTypes::ResourceBase> pResource);

			//! Process arguments to create Hash ID
			template<typename T, typename ... TArgs>
			resourceID generateID(TArgs ... pArgs);
			template<typename T, typename ... TArgs>
			inline void build_string(std::ostream& pStream, const T& pFirst, const TArgs& ... pArgs);
			inline void build_string(std::ostream& pStream) {}

		public:
			//! Initialisation and destruction
			static bool init(const char* pDirectory, const SDL_Renderer* pRenderer, DebugCallback pWarningCB = nullptr, DebugCallback pErrorCB = nullptr);
			static void destroy();

			//! Main Function
			static void update();
			static void scanResources();

			//! Getters
			template<class T, typename ... TArgs>
			static SDL2Resource<T> load(const char* pFilepath, TArgs ... pArgs);
			static std::string idToFilepath(const resourceID& pID);
		};

		/*
			Resources : load - Load a resource at a specified filepath
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pFilepath
			param[in] pArgs

			return SDL2Resource<T> - Returns a SDL2Resource object based on the template type specified
		*/
		template<class T, typename ... TArgs>
		static SDL2Resource<T> Resources::load(const char* pFilepath, TArgs ... pArgs) {
			//Ensure load of type is valid
			static_assert(std::is_base_of<ResourceTypes::ResourceBase, ResourceTypes::__SDL2Resource<T>>::value, "Can not load a Resource that is not of the SDL2_Resource type");

			//Generate the resourceID value
			const resourceID ID = mInstance->generateID<T>(pFilepath, pArgs...);

			//Lock the resource items
			std::lock_guard<std::recursive_mutex> guard(mInstance->mLock);

			//Look for an existing resource with the ID
			auto iter = mInstance->mResourceLookup.find(ID);

			//If there was no resource found
			if (iter == mInstance->mResourceLookup.end()) {
				//Create a new resource
				SDL2Resource<T> temp = SDL2Resource<T>(new ResourceTypes::__SDL2Resource<T>(ID, pArgs...));

				//Add the new resource to the map
				mInstance->mResourceLookup.insert(std::pair<resourceID, std::shared_ptr<ResourceTypes::ResourceBase>>(ID, temp));

				//Add the filepath to the resource origin lookup
				mInstance->mResourceOrigin.insert(std::pair<resourceID, std::string>(ID, pFilepath));

				//Return the new resource
				return temp;
			}

			//Get the found resource as T
			else return std::dynamic_pointer_cast<ResourceTypes::__SDL2Resource<T>>(iter->second);
		}

		/*
			Resources : generateID - Given an indiscriminate number of values, create a unique resource ID
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pArgs - The arguments to convert into a resourceID

			return resourceID - Returns a resourceID value which is (mostly) unique	to each individual resource
		*/
		template<typename T, typename ... TArgs>
		resourceID Resources::generateID(TArgs ... pArgs) {
			std::ostringstream o;
			build_string(o, resourceToString<T>(), pArgs...);
			return std::hash<std::string>{}(o.str());
		}

		/*
			Resources : build_string - Unpack the parameter pack, writing the values to a stream
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pStream - A std::ostream object that has the data written to
			param[in] pFirst - The next value to be processed
			param[in] pArgs - The reminding parameter values to evaluate
		*/
		template<typename T, typename ... TArgs>
		void Resources::build_string(std::ostream& pStream, const T& pFirst, const TArgs& ... pArgs) {
			pStream << pFirst;
			build_string(pStream, pArgs...);
		}
	}
}