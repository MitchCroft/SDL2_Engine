#pragma once

namespace SDL2_Engine {
	namespace Utilities {
		/*
		 *		Name: Bitmask
		 *		Author: Mitchell Croft
		 *		Created: 13/02/2017
		 *		Modified: 13/02/2017
		 *
		 *		Purpose:
		 *		Provide a method for gathering and using
		 *		bitmask values that can be constructed from
		 *		and indiscriminate number of types and values
		**/
		template<typename T, typename Val = int>
		class Bitmask {
			//! Store the mask value
			T mMask;

			//! Simple function for extracting mask values
			template<typename U, typename ... TArgs>
			inline void constructMask(const U& pFirst, TArgs ... pArgs) {
				mMask = (T)((Val)mMask | (Val)pFirst);
				constructMask(pArgs...);
			}
			inline void constructMask() {}

		public:
			//! Constructors
			inline Bitmask() : mMask((T)0) {}
			inline Bitmask(const Bitmask& pCopy) : mMask(pCopy.mMask) {}
			template<typename U>
			inline Bitmask(const U& pVal) : mMask((T)pVal) {}
			template<typename ... TArgs>
			inline Bitmask(TArgs ... pArgs) : mMask((T)0) { constructMask(pArgs...); }

			//! Getters
			inline const T& getValue() const { return mMask; }
			inline const Val getMask() const { return (Val)mMask; }

			//! Binary Operators
			template<typename U>
			inline Val operator|(const U& pVal) const { return ((Val)mMask | (Val)pVal); }
			template<typename U>
			inline Bitmask& operator|=(const U& pVal) { mMask = (T)((Val)mMask | (Val)pVal); return *this; }
			template<typename U>
			inline Val operator&(const U& pVal) const { return ((Val)mMask & (Val)pVal); }
			template<typename U>
			inline Bitmask& operator&=(const U& pVal) { mMask = (T)((Val)mMask & (Val)pVal); return *this; }

			//! Assignment operator
			inline Bitmask& operator=(const Bitmask& pCopy) { mMask = pCopy.mMask; return *this; }
			template<typename U>
			inline Bitmask& operator=(const U& pVal) { mMask = (T)pVal; return *this; }
		};

		template<typename U, typename T, typename Val>
		inline Val operator|(const U& pVal, const Bitmask<T, Val>& pMask) { return ((Val)pVal | (Val)pMask.getMask()); }
		template<typename U, typename T, typename Val>
		inline Val operator&(const U& pVal, const Bitmask<T, Val>& pMask) { return ((Val)pVal & (Val)pMask.getMask()); }
	}
}