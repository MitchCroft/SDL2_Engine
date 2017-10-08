#pragma once

//! Include the required base values
#include "../Utilities/IGlobal.hpp"
#include "ResourceTypes/LocalResource.hpp"

//! Include Shared Pointers to handle resource acquisition and sharing
#include <memory>

//! Get information on types used
#include <typeinfo>

//! Prototype the SDL_Renderer object
struct SDL_Renderer;

namespace SDL2_Engine {
	//! Define a simple alias' to be used for the Local Resource objects
	template<typename T>
	using LocalResource = std::shared_ptr<ResourceTypes::__LocalResource<T>>;
	typedef std::shared_ptr<ResourceTypes::LocalResourceBase> LocalResourceBase;

	/*
	 *		Name: Resources
	 *		Author: Mitchell Croft
	 *		Created: 05/10/2017
	 *		Modified: 05/10/2017
	 *		
	 *		Purpose:
	 *		Provide uniform functionality for loading and managing local resources
	**/
	class SDL2_LIB_INC Resources : public Utilities::IGlobal {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Resource Acquisition-------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Resources : loadResource (Generic) - Loads one of the standard Resource types (not Texture) with supplied parameters
			Created: 05/10/2017
			Modified: 05/10/2017

			Template T - The type of Resource to load (E.g. SFX, Music, Cursor etc.)
			Template TArgs - A parameter pack of values to used to setup the resource and act as an identifying key

			param[in] pArgs - The parameter pack to use for key and resource construction

			return LocalResource<T> - Returns a shared pointer to the Local Resource requested
		*/
		template<typename T, typename ... TArgs>
		LocalResource<typename std::enable_if<!std::is_same<Texture, T>::value, T>::type> loadResource(TArgs ... pArgs) {
			//Ensure load type is valid
			static_assert(std::is_base_of<ResourceTypes::LocalResourceBase, ResourceTypes::__LocalResource<T>>::value, "Can not load a Local Resource that is of an undefined type");

			//Generate the ID key for the resource
			const size_t ID = hashData(typeToID<T>, pArgs...);

			//Look for an existing Resource with the ID
			auto res = getResource(ID);

			//If there was no resource found
			if (!res) {
				//Create the new resource
				LocalResource<T> temp = LocalResource<T>(new ResourceTypes::__LocalResource<T>(pArgs...));

				//Store the resource in the manager, if loaded 
				if (temp->status() == EResourceLoadStatus::Loaded) storeResource(temp, ID);

				//Return the new resource
				return temp;
			}

			//Otherwise cast and return found resource
			else return std::dynamic_pointer_cast<ResourceTypes::__LocalResource<T>>(res);
		}

		/*
			Resources : loadResource (Texture) - Loads a local Texture object, supplying a Renderer pointer for construction
			Created: 05/10/2017
			Modified: 05/10/2017

			Template T - Function specilisation for T == Texture
			Template TArgs - A parameter pack of values used to setup the Texture and act as an identifying key

			param[in] pArgs - The parameter pack to use for key and resource construction

			return LocalResource<Texture> - Returns a shared pointer to the Local Resource requested
		*/
		template<typename T, typename ... TArgs>
		LocalResource<typename std::enable_if<std::is_same<Texture, T>::value, Texture>::type> loadResource(TArgs ... pArgs) {
			//Generate the ID key for the resource
			const size_t ID = hashData(typeToID<T>, pArgs...);

			//Look for an existing Resource with the ID
			auto res = getResource(ID);

			//If there was no resource found
			if (!res) {
				//Create the new resource
				LocalResource<T> temp = LocalResource<T>(new ResourceTypes::__LocalResource<T>(getRenderer(), pArgs...));

				//Store the resource in the manager, if loaded 
				if (temp->status() == EResourceLoadStatus::Loaded) storeResource(temp, ID);

				//Return the new resource
				return temp;
			}

			//Otherwise cast and return found resource
			else return std::dynamic_pointer_cast<ResourceTypes::__LocalResource<T>>(res);
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Helper Functions---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Resources : typeToID - Convert a template type value into a unique hash code
			Created: 19/07/2017
			Modified: 05/10/2017

			Template T - A generic, non-void type

			return typeID - Returns the ID as typeID value
		*/
		template<typename T>
		static inline size_t typeToID() {
			//Get the type information
			const std::type_info& type = typeid(T);

			//Return the hash code
			return type.hash_code();
		}

		/*
			Resources : hashData - Convert a series of data to a single hash code
			Created: 05/10/2017
			Modified: 05/10/2017

			Template TArgs  |
							- A parameter pack of stream-able data that will make up the hash key
			param[in] pArgs |

			return size_t - Returns the hash code as a size_t value
		*/
		template<typename ... TArgs>
		static inline size_t hashData(const TArgs& ... pArgs) {
			//Create a string stream to store data values
			std::ostringstream os;

			//Output the data to the stream
			compileStream(os, pArgs...);

			//Return the hash code
			return std::hash<std::string>{}(os.str());
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Resources : Constructor - Initialise with default values
			Created: 05/10/2017
			Modified: 05/10/2017

			param[in] pRenderer - A pointer to the Renderer object, to be used to create Textures
		*/
		Resources(SDL_Renderer* pRenderer);

		/*
			Resources : createInterface - Verify and setup starting information
			Created: 05/10/2017
			Modified: 05/10/2017

			return bool - Returns true if the Resources Manager was setup correctly
		*/
		bool createInterface() override;

		/*
			Resources : destroyInterface - Deallocate internal memory allocated
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		void destroyInterface() override;

		/*
			Resources : update - Update active local resources
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		void update() override;

	private:
		//! Define the internal protected elements for the Resource Manager
		struct ResourcesInternalData;
		ResourcesInternalData* mData;

		//! Function for retrieving the Renderer object from internal data
		SDL_Renderer* getRenderer();

		//! Retrieve an active resource with a specific ID
		LocalResourceBase getResource(const size_t& pID) const;

		//! Store a Local Resource in the Manager
		void storeResource(LocalResourceBase pResource, const size_t& pID);

		//! Store data in an output stream
		template<typename T, typename ... TArgs>
		static void compileStream(std::ostream& pStream, const T& pFirst, const TArgs& ... pArgs) {
			pStream << pFirst;
			compileStream(pStream, pArgs...);
		}
		inline static void compileStream(std::ostream& pStream) {}
	};
}