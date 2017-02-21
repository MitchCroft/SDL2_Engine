#pragma once

namespace SDL2_Engine {
	/*
	 *		Name: Math
	 *		Author: Mitchell Croft
	 *		Created: 01/02/2017
	 *		Modified: 01/02/2017
	 *		
	 *		Purpose:
	 *		Group common, useful math operations inside a statically
	 *		accessible object
	**/
	struct Math {
		/*
			Math : Constants - A number of math constants that can be used for math calculations
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017
		*/
		const float Pi = 3.1415927f;
		const float Deg2Rad = Pi / 180.f;
		const float Rad2Deg = 180.f / Pi;

		/*
			Math : sign - Given a value of type T get the positive or negative sign
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pValue - The numeric value to be evaluated

			return T - Returns either a -1 or 1 in the type of T depending on the sign of the value
		*/
		template<typename T> 
		static inline T sign(const T& pValue) { return (pValue < (T)0 ? (T)-1 : (T)1); }

		/*
			Math : clamp - Clamp a value of type T, return the value restricted to a minimum and a maximum
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pValue - The value to be clamped
			param[in] pMin - The minimum value that can be returned
			param[in] pMax - The maximum value that can be returned

			return T - Returns the clamped value in the same type
		*/
		template<typename T>
		static inline T clamp(const T& pValue, const T& pMin, const T& pMax) { return (pValue < pMin ? pMin : (pValue > pMax ? pMax : pValue)); }

		/*
			Math : clamp01 - Clamp a value of type T between 0 and 1
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pValue - The value to be clamped

			return T - Returns the clamped value in the same type
		*/
		template<typename T>
		static inline T clamp01(const T& pValue) { return clamp(pValue, (T)0, (T)1); }

		/*
			Math : lerp - Preforms linear interpolation between two T values by pT
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pStart - The starting value to be interpolated from
			param[in] pEnd - The ending value to be interpolated to
			param[in] pT - The interpolation scale value (0 - 1 scale enforced)

			return T - Returns the interpolated value
		*/
		template<typename T> 
		static inline T lerp(const T& pStart, const T& pEnd, float pT) { pT = clamp01(pT); return T(pStart + (pEnd - pStart) * pT); }

		/*
			Math : lerpUnclamped - Preforms linear interpolation between two T values by an unclamped pT value
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pStart - The starting value to be interpolated from
			param[in] pEnd - The ending value to be interpolated to
			param[in] pT - The interpolation scale value

			return T - Returns the interpolated value
		*/
		template<typename T>
		static inline T lerpUnclamped(const T& pStart, const T& pEnd, const float& pT) { return T(pStart + (pEnd - pStart) * pT); }

		/*
			Math : inverseLerp - Calculate the linear interpolation scale given a value between two points
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pStart - The value used as the starting point
			param[in] pEnd - The value used as the ending point
			param[in] pValue - The value to be used to retrieve the linear interpolation scale

			return float - Returns the linear interpolation scale as float 
		*/
		template<typename T>
		static inline float inverseLerp(const T& pStart, const T& pEnd, const T& pValue) { return ((float)(pValue - pStart) / (float)(pEnd - pStart)); }

		/*
			Math : min - Given an indiscriminate number of numeric values, return the smallest of the group
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pFirst - The first value that is used as the starting point for the search
			param[in] pArgs - A variadic template parameter pack of values to evaluate

			return T - Returns a copy of the smallest value found
		*/
		template<typename T, typename ... TArgs>
		static inline T min(const T& pFirst, TArgs ... pArgs) { T value = pFirst; return extractMin(value, pArgs...); }

		/*
			Math : max - Given an indiscriminate number of numeric values, return the largest of the group
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in] pFirst - The first value that is used as the starting point for the search
			param[in] pArgs - A variadic template parameter pack of values to evaluate

			return T - Returns a copy of the largest value found
		*/
		template<typename T, typename ... TArgs>
		static inline T max(const T& pFirst, TArgs ... pArgs) { T value = pFirst; return extractMax(value, pArgs...); }

	private:
		//! Prevent the creation of Math objects (Math, not even once)
		Math() = delete; ~Math() = delete;

		/*
			Math : extractMin - Unpack the variadic template parameter pack to find the smallest value
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in/out] pSmallest - A reference to a value of type T storing the current smallest value
			param[in] pNext - A constant reference to the next value of type T to be compared against
			param[in] pRest - A parameter pack containing the rest of the values to evaluate against

			return T& - Returns a reference to the pSmallest parameter
		*/
		template<typename T, typename ... TArgs>
		static T& extractMin(T& pSmallest, const T& pNext, TArgs ... pRest) {
			//Compare the next value
			if (pNext < pSmallest) pSmallest = pNext;

			//Recurse down
			return extractMin(pSmallest, pRest...);
		}

		/*
			Math : extractMin - Finish the variadic template unpacking process
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in/out] pSmallest - A reference to the value storing the smallest value

			return T& - Returns a reference to the pSmallest parameter
		*/
		template<typename T>
		static inline T& extractMin(T& pSmallest) { return pSmallest; }

		/*
			Math : extractMax - Unpack the variadic template parameter pack to find the largest value
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in/out] pLargest - A reference to a value of type T storing the current largest value
			param[in] pNext - A constant reference to the next value of type T to be compared against
			param[in] pRest - A parameter pack containing the rest of the values to evaluate against 

			return T& - Returns a reference to the pLargest parameter
		*/
		template<typename T, typename ... TArgs>
		static T& extractMax(T& pLargest, const T& pNext, TArgs ... pRest) {
			//Compare the next value
			if (pNext > pLargest) pLargest = pNext;

			//Recurse down
			return extractMax(pLargest, pRest...);
		}

		/*
			Math : extractMax - Finish the variadic template unpacking process
			Author: Mitchell Croft
			Created: 01/02/2017
			Modified: 01/02/2017

			param[in/out] pLargest - A reference to the value storing the largest value

			return T& - Returns a reference to the pLargest parameter
		*/
		template<typename T>
		static inline T& extractMax(T& pLargest) { return pLargest; }
	};
}