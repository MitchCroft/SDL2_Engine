#pragma once

//! Include the library management definitions
#include "__LibraryManagement.hpp"
#include "Utilities/TypeID.hpp"

namespace SDL2_Engine {
	//! Prototype the IGlobal interface
	namespace Utilities { class IGlobal; }

	/*
	 *		Name: Globals
	 *		Author: Mitchell Croft
	 *		Created: 19/07/2017
	 *		Modified: 11/10/2017
	 *		
	 *		Purpose:
	 *		Manage a number of Global Interface objects from a single location.
	**/
	class SDL2_LIB_INC Globals {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Interface Functions--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Globals : addInterface - Add a new interface object to the Globals singleton
			Created: 19/07/2017
			Modified: 11/10/2017

			Template T - The type of Interface based object to be added to the Globals object
			Template TArgs - Parameter pack of initialisation types

			param[in] pArgs - Parameter pack of values used to initialise the IGlobal interface

			return T* - Returns a pointer to the constructed interface or nullptr if it failed
		*/
		template<typename T, typename ... TArgs>
		static T* addInterface(const TArgs ... pArgs) {
			//Assert template types of the IGlobal interface
			static_assert(std::is_base_of<Utilities::IGlobal, T>::value, "Interfaces added to the Globals singleton object are required to inherit from the abstract IGlobal object");

			//Get the ID of the type
			const auto id = Utilities::typeToID<T>();

			//Check if the interface already exists
			if (mInstance->interfaceExists(id))
				return false;

			//Add the interface to the Globals object
			return (T*)mInstance->introduceInterface(new T(pArgs...), id);
		}

		/*
			Globals : get - Get an interface of the specified type from the Globals manager
			Created: 19/07/2017
			Modified: 11/10/2017

			Template T - The type of Interface to retrieve from the Globals object

			return T& - Returns a reference to the desired interface

			WARNING:
			Will return a bad reference if the interface does not exist
		*/
		template<typename T> 
		static inline T& get() { 
			//Assert template types of the IGlobal interface
			static_assert(std::is_base_of<Utilities::IGlobal, T>::value, "Interfaces retrieved from the Globals singleton object are required to inherit from the abstract IGlobal object");

			//Return the Interface
			return *(T*)mInstance->retrieve(Utilities::typeToID<T>()); 
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Helper Functions---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		/*
			Globals : interfaceExists - Check to see if an interface of a specific type exists
			Created: 04/10/2017
			Modified: 11/10/2017

			Template T - The type of interface to check for

			return bool - Returns true if the specified interface exists in the Globals Manager
		*/
		template<typename T>
		static inline bool interfaceExists() {
			//Assert template types of the IGlobal interface
			static_assert(std::is_base_of<Utilities::IGlobal, T>::value, "Interfaces retrieved from the Globals singleton object are required to inherit from the abstract IGlobal object");

			//Convert type to ID
			return mInstance->interfaceExists(Utilities::typeToID<T>());
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Main Functions-----------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Globals : create - Create the Globals singleton
			Created: 19/07/2017
			Modified: 04/10/2017
		*/
		static void create();

		/*
			Globals : update - Updates all internal IGlobal interface objects
			Created: 19/07/2017
			Modified: 04/10/2017
		*/
		static void update();

		/*
			Globals : destroy - Destroy all internal IGlobal interface objects in the reverse order of creation
			Created: 19/07/2017
			Modified: 04/10/2017
		*/
		static void destroy();


	private:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------Singleton Values---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Store a singleton instance
		static Globals* mInstance;

		//! Privatize the constructor and destructor
		Globals(); inline ~Globals() = default;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Management Values---------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		
		//! Define the internal protected elements of the Global Manager
		struct GlobalsInternalData;
		GlobalsInternalData* mData;

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Management Functions-------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		//! Setup an interface object for inclusion in the Globals object
		Utilities::IGlobal* introduceInterface(Utilities::IGlobal* pInter, const Utilities::typeID& pID);

		//! Check if an interface with a specified ID exists in the current manager
		bool interfaceExists(const Utilities::typeID& pID) const;

		//! Retrieve the pointer to the specified interface
		Utilities::IGlobal* retrieve(const Utilities::typeID& pID) const;
	};
}