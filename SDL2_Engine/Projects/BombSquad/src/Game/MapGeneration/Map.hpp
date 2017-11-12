#pragma once

//! Include standard memory manipulation functions
#include <string.h>

namespace BombSquad {
	/*
	 *		Name: Map
	 *		Author: Mitchell Croft
	 *		Created: 08/11/2017
	 *		Modified: 11/11/2017
	 *		
	 *		Purpose:
	 *		Store a 2D map of data values that can be used to represent
	 *		various pieces of information
	**/
	template<typename T>
	class Map {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Map : Constructor - Initialise with default values
			Created: 08/11/2017
			Modified: 08/11/2017
		*/
		inline Map() : mMap(nullptr), mWidth(0), mHeight(0) {}

		/*
			Map : Custom Constructor - Initialise with a set of dimensions
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pWidth - The desired width of the Map object
			param[in] pHeight - The desired height of the Map object
		*/
		inline Map(const size_t& pWidth, const size_t& pHeight) : mMap(nullptr), mWidth(0), mHeight(0) { setBounds(pWidth, pHeight); }

		/*
			Map : Copy Constructor - Copy the values of another Map object
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pCopy - The Map object to copy
		*/

		inline Map(const Map& pCopy) : mMap(nullptr), mWidth(0), mHeight(0) { *this = pCopy; }

		/*
			Map : setBounds - Initialise the map with a set of dimensions
			Created: 08/11/2017
			Modified: 08/11/2017

			param[in] pWidth - The desired width of the Map object
			param[in] pHeight - The desired height of the Map object

			NOTE:
			This function resets the data on the map to the default state
		*/
		void setBounds(const size_t& pWidth, const size_t& pHeight) {
			//Free previously allocated memory
			dispose();

			//Stash the dimension values
			mWidth = pWidth;
			mHeight = pHeight;

			//Create the new array
			mMap = new T[mWidth * mHeight];
			
			//Set default values
			memset(mMap, 0, sizeof(T) * mWidth * mHeight);
		}

		/*
			Map : dispose - Clear allocated memory within the Map
			Created: 08/11/2017
			Modified: 08/11/2017
		*/
		void dispose() {
			//Check if the map is allocated
			if (mMap) {
				//Delete the data
				delete[] mMap;

				//Reset the pointer
				mMap = nullptr;

				//Reset the dimensions
				mWidth = mHeight = 0;
			}
		}

		/*
			Map : Destructor - Deallocate memory allocated by the Map
			Created: 08/11/2017
			Modified: 08/11/2017
		*/
		inline ~Map() { dispose(); }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------------Getters--------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Map : width - Get the width of the Map 
			Created: 08/11/2017
			Modified: 08/11/2017

			return const size_t& - Returns a constant reference to the internal width value
		*/
		inline const size_t& width() const { return mWidth; }

		/*
			Map : height - Get the height of the Map
			Created: 08/11/2017
			Modified: 08/11/2017

			return const size_t& - Returns a constant reference to the internal height value
		*/
		inline const size_t& height() const { return mHeight; }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////---------------------------------------Operators-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			Map : Assignment Operator - Copy the values of another Map object
			Created: 08/11/2017
			Modified: 11/11/2017

			param[in] pCopy - The Map object to copy

			return Map& - Returns a reference to itself
		*/
		Map& operator=(const Map& pCopy) {
			//Setup map with the same dimensions
			setBounds(pCopy.mWidth, pCopy.mHeight);

			//Copy the data values
			memcpy_s(mMap, sizeof(T) * mWidth * mHeight, pCopy.mMap, sizeof(T) * mWidth * mHeight);

			//Return itself
			return *this;
		}

		/*
			Map : Subscript Operator - Access the internal value map values via sub-script
			Created: 08/11/2017
			Modified: 11/11/2017

			param[in] pIndex - The width index of the map to retrieve

			return T* - Returns a pointer for that specified row
		*/
		inline T* operator[](const size_t& pIndex) { return &mMap[pIndex * mWidth]; }

		/*
			Map : Const Subscript Operator - Access the internal value map values via sub-script
			Created: 08/11/2017
			Modified: 11/11/2017

			param[in] pIndex - The width index of the map to retrieve

			return const T* - Returns a constant pointer for that specified row
		*/
		inline const T* operator[](const size_t& pIndex) const { return &mMap[pIndex * mWidth]; }

	private:
		//! Store a map of values
		T* mMap;

		//! Store the dimensions of the map
		size_t mWidth, mHeight;
	};
}