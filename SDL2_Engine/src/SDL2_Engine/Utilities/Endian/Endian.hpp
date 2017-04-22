#pragma once

/*
*		Name: Endian
*		Author: Mitchell Croft
*		Created: 07/03/2017
*		Modified: 07/03/2017
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
			enum EEndianOrder { ENDIAN_SMALL, ENDIAN_BIG };

			//! Store a global reference to the current Endian order
			extern const EEndianOrder SYSTEM_END_ORDER;

			/*
				reverseBytes - Force the bytes of a value to flip themselves
				Author: Mitchell Croft
				Created: 06/03/2017
				Modified: 06/03/2017

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
				Modified: 07/03/2017

				param[in] pVal - The value to be standardised

				return T - Returns the converted form of pVal
			*/
			template<typename T>
			inline T convert(const T& pVal) { return (SYSTEM_END_ORDER ? reverseBytes<T>(pVal) : pVal); }
		}
	}
}
