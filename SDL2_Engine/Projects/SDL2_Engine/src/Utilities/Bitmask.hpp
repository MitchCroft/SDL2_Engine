#pragma once

#include "../__LibraryManagement.hpp"
#include <type_traits>

namespace SDL2_Engine {
	namespace Utilities {

		/*
		 *		Name: Implementation
		 *		Author: Mitchell Croft
		 *		Created: 18/07/2017
		 *		Modified: 18/07/2017
		 *		
		 *		Purpose:
		 *		Conceal implementation elements of the Bitmask object in
		 *		a lower level namespace to reduce cluttering of the primary
		 *		use namespaces
		**/
		namespace Implementation {
			/*
			 *		Name: __EnumBaseType__
			 *		Author: Mitchell Croft
			 *		Created: 18/07/2017
			 *		Modified: 18/07/2017
			 *		
			 *		Templates:
			 *		T - The type to be evaluated
			 *		bool - A value that flags if the template T type is an enum
			 *		
			 *		Purpose:
			 *		Provide the base point for bool specialisations used to determine if 
			 *		a templated value is an enumeration type
			**/
			template<typename T, bool>
			struct __EnumBaseType__;

			/*
			 *		Name: __EnumBaseType__ (True)
			 *		Author: Mitchell Croft
			 *		Created: 18/07/2017
			 *		Modified: 18/07/2017
			 *		
			 *		Template:
			 *		T - The type to be treated as an enum value
			 *		
			 *		Purpose:
			 *		Retrieve the base type of an enumeration value
			**/
			template<typename T>
			struct __EnumBaseType__<T, true> { typedef typename std::underlying_type<T>::type type; };

			/*
			 *		Name: __EnumBaseType__ (False)
			 *		Author: Mitchell Croft
			 *		Created: 18/07/2017
			 *		Modified: 18/07/2017
			 *		
			 *		Template:
			 *		T - A non-enum type 
			 *		
			 *		Purpose:
			 *		Rename the base type of the template T value for
			 *		match with the 'True' specialisation
			**/
			template<typename T>
			struct __EnumBaseType__<T, false> { typedef T type; };
		}

		/*
		 *		Name: Bitmask
		 *		Author: Mitchell Croft
		 *		Created: 13/02/2017
		 *		Modified: 11/10/2017
		 *
		 *		Purpose:
		 *		Provide a method for gathering and using
		 *		bitmask values that can be constructed from
		 *		an indiscriminate number of types and values
		**/
		template<typename T>
		class SDL2_LIB_INC Bitmask {
		public:
			//! Define the underlying type that is used by this object
			typedef typename Implementation::__EnumBaseType__<T, std::is_enum<T>::value>::type baseType;

			//! Ensure that the type used is an integral value
			static_assert(std::is_integral<baseType>::value, "Bitmask template value 'T' must be an integral value");

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Constructors-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Bitmask : Constructor - Initialise with default values
				Created: 18/07/2017
				Modified: 18/07/2017
			*/
			inline Bitmask() : mMask(0) {}

			/*
				Bitmask : Copy Constructor - Copy the mask value of another Bitmask object
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pCopy - The Bitmask object to copy
			*/
			inline Bitmask(const Bitmask& pCopy) : mMask(pCopy.mMask) {}

			/*
				Bitmask : Value Constructor - Initialise the bitmask based on an indiscriminate amount of values
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pArgs - A parameter pack of different integral values to extract
			*/
			template<typename ... TArgs> 
			inline Bitmask(const TArgs& ... pArgs) : mMask(0) { extractValues(pArgs...); }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Constructors-------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Bitmask : getMask - Explicitly retrieves the internal bitmask
				Created: 18/07/2017
				Modified: 18/07/2017

				return const baseType& - Returns a constant reference to the mask value
			*/
			inline const baseType& getMask() const noexcept { return mMask; }

			/*
				Bitmask : getValue - Explicitly retrieves the internal bitmask cast to the original T type
				Created: 18/07/2017
				Modified: 18/07/2017

				return T - Returns the bitmask cast to the original T type
			*/
			inline const T getValue() const noexcept { return (T)mMask; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Binary Operators----------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Bitmask : nor - Perform a simple bitwise NOR operation 
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return baseType - Returns a 'baseType' value with the results of the NOR operation
			*/
			template<typename U>
			inline baseType nor(const U& pVal) const noexcept { return ~(mMask | (baseType)pVal); }

			/*
				Bitmask : norEquals - Perform a simple bitwise NOR operation and assign the Bitmask to the result
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask& norEquals(const U& pVal) const noexcept { mMask = ~(mMask | (baseType)pVal); return *this; }

			/*
				Bitmask : nand - Perform a simple bitwise NAND operation
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return baseType - Returns a 'baseType' value with the results of the NAND operation
			*/
			template<typename U>
			inline baseType nand(const U& pVal) const noexcept { return ~(mMask & (baseType)pVal); }

			/*
				Bitmask : nandEquals - Perform a simple bitwise NAND operation and assign the Bitmask to the result
				Created: 11/10/2017
				Modified: 11/10/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask nandEquals(const U& pVal) const noexcept { mMask = ~(mMask & (baseType)pVal); return *this; }

			/*
				Bitmask : Bitwise AND Operator - Perform a simple bitwise AND operation
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return baseType - Returns a 'baseType' value with the results of the AND operation
			*/
			template<typename U>
			inline baseType operator&(const U& pVal) const noexcept { return (mMask & (baseType)pVal); }

			/*
				Bitmask : Bitwise AND Assignment Operator - Perform a simple bitwise AND operation and assign the Bitmask to the result
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask& operator&=(const U& pVal) noexcept { mMask &= (baseType)pVal; return *this; }

			/*
				Bitmask : Bitwise OR Operator - Perform a simple bitwise OR operation
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return baseType - Returns a 'baseType' value with the results of the OR operation
			*/
			template<typename U>
			inline baseType operator|(const U& pVal) const noexcept { return (mMask | (baseType)pVal); }

			/*
				Bitmask : Bitwise OR Assignment Operator - Perform a simple bitwise OR operation and assign the Bitmask to the result
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask& operator|=(const U& pVal) noexcept { mMask |= (baseType)pVal; return *this; }

			/*
				Bitmask : Bitwise XOR Operator - Perform a simple bitwise XOR operation
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return baseType - Returns a 'baseType' value with the results of the XOR operation
			*/
			template<typename U>
			inline baseType operator^(const U& pVal) const noexcept { return (mMask ^ (baseType)pVal); }

			/*
				Bitmask : Bitwise XOR Assignment Operator - Perform a simple bitwise XOR operation and assign the Bitmask to the result
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' for the operation

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask& operator^=(const U& pVal) noexcept { mMask ^= (baseType)pVal; return *this; }

			/*
				Bitmask : Bit Shift Left Operator - Perform a simple bitwise shift of bits to the left by a defined amount
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pShift - The number of bits to shift the internal bitmask value by

				return int - Returns the shift result as an int value
			*/
			inline int operator<<(const int& pShift) const noexcept { return (mMask << pShift); }

			/*
				Bitmask : Bit Shift Left Assignment Operator - Perform a simple bitwise shift of bits to the left and assign the Bitmask to the result
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pShift - The number of bits to shift the internal bitmask value by

				return Bitmask& - Returns a reference to itself
			*/
			inline Bitmask& operator<<=(const int& pShift) noexcept { mMask <<= pShift; return *this; }

			/*
				Bitmask : Bit Shift Right Operator - Perform a simple bitwise shift of bits to the right by a defined amount
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pShift - The number of bits to shift the internal bitmask value by

				return int - Returns the shift result as an int value
			*/
			inline int operator>>(const int& pShift) const noexcept { return (mMask >> pShift); }

			/*
				Bitmask : Bit Shift Right Assignment Operator - Perform a simple bitwise shift of bits to the right and assign the Bitmask to the result
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pShift - The number of bits to shift the internal bitmask value by

				return Bitmask& - Returns a reference to itself
			*/
			inline Bitmask& operator>>=(const int& pShift) noexcept { mMask >>= pShift; return *this; }

			/*
				Bitmask : Bitwise Compliment Operator - Retrieve the complimentary bits of the current BitMask
				Created: 18/07/2017
				Modified: 18/07/2017

				return baseType - Returns a 'baseType' value with the results of the bit flipping operation
			*/
			inline baseType operator~() const noexcept { return ~mMask; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////----------------------------------Standard Operators---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Bitmask : baseType Cast Operator - Implicitly cast the Bitmask object to a baseType value
				Created: 18/07/2017
				Modified: 18/07/2017

				return baseType - Returns the internal Bitmask value
			*/
			inline operator baseType() const noexcept { return mMask; }

			/*
				Bitmask : Assignment Operator - Assign the value of the Bitmask to a specific numerical value
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' type

				return Bitmask& - Returns a reference to itself
			*/
			template<typename U>
			inline Bitmask& operator=(const U& pVal) noexcept { mMask = (baseType)pVal; return *this; }

			/*
				Bitmask : Equality Operator - Determines if the Bitmask object is equal to the passed in value
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' type

				return bool - Returns true if the two values are equal
			*/
			template<typename U>
			inline bool operator==(const U& pVal) const noexcept { return (mMask == (baseType)pVal); }

			/*
				Bitmask : Inequality Operator - Determines if the Bitmask object is unequal to the passed in value
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pVal - A value that can be converted to the integral 'baseType' type

				return bool - Returns true if the two values are unequal
			*/
			template<typename U>
			inline bool operator!=(const U& pVal) const noexcept { return (mMask != (baseType)pVal); }

		private:

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Extraction--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Bitmask : extractValues - Extract integral binary data from an indiscriminate number of values
				Created: 18/07/2017
				Modified: 18/07/2017

				param[in] pFirst - The next value to be processed
				param[in] pRest - A parameter pack of values left to process
			*/
			template<typename T, typename ... TArgs>
			inline void extractValues(const T& pFirst, const TArgs& ... pRest) {
				//Save the bit values store in the first value
				mMask |= (baseType)pFirst;

				//Recurse through the rest of the values
				extractValues(pRest...);
			}

			/*
				Bitmask : extractValues - End the recursive extraction of values
				Created: 18/07/2017
				Modified: 18/07/2017
			*/
			inline void extractValues() {}

			//! Store the internal mask value
			baseType mMask;
		};
	}
}