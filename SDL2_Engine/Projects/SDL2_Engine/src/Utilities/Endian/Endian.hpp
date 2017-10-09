#pragma once

//! Include the SDL2_Engine objects
#include "../../__LibraryManagement.hpp"

/*
*		Name: Endian
*		Author: Mitchell Croft
*		Created: 07/03/2017
*		Modified: 09/10/2017
*
*		Purpose:
*		Provide methods and information used to correct
*		endian issues with information written to and
*		from shared data streams
**/
namespace SDL2_Engine {
	namespace Utilities {
		namespace Endian {
			//! Define the types for the endian order
			enum class EEndianOrder { Small, Big };

			/*
				getEndianOrder - Get the current Endian order of the System
				Created: 09/10/2017
				Modified: 09/10/2017

				return EEndianOrder - Returns an EEndianOrder value describing the endian order of the system
			*/
			extern SDL2_LIB_INC EEndianOrder getEndianOrder();
			/*
				reverseBytes - Force the bytes of a value to flip themselves
				Author: Mitchell Croft
				Created: 06/03/2017
				Modified: 09/10/2017

				Template T - The type of value to reverse the byte order to
				Template N - The size (in bytes) of the type to reverse

				param[in] pVal - A constant reference to the value to swap

				return T - Returns a copy of pVal with the bytes switched
			*/
			template<typename T, const size_t N = sizeof(T)>
			inline T reverseBytes(const T& pVal) {
				//! Create a union for the byte information
				union { T value; unsigned char bytes[N]; } dat1, dat2;

				//! Store the original value
				dat1.value = pVal;

				//! Swap the bytes around
				for (int i = 0; i < N; ++i) dat2.bytes[i] = dat1.bytes[N - 1 - i];

				//! Return the new value
				return dat2.value;
			}

			/*
				convert - Convert values to and from a standard form
				Author: Mitchell Croft
				Created: 07/03/2017
				Modified: 09/10/2017

				Template T - The type of value to convert

				param[in] pVal - The value to be standardised

				return T - Returns the converted form of pVal
			*/
			template<typename T>
			inline T convert(const T& pVal) { return (getEndianOrder() == EEndianOrder::Big ? reverseBytes<T>(pVal) : pVal); }
		}
	}
}
