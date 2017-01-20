#pragma once

namespace SDL2_Engine {
	namespace Utilities {
		namespace Properties {

			//! Alias the function pointer used for assigning validation callbacks to the ValidateProperty object
			template<class T> using ValidationCallback = T(*)(const T&);

			/*
			 *		Name: ValidatedProperty
			 *		Author: Mitchell Croft
			 *		Created: 20/01/2017
			 *		Modified: 20/01/2017
			 *
			 *		Purpose:
			 *		Provide a method of using a value as a user normally would
			 *		while providing a method of validating the values that are set
			 *
			 *		WARNING:
			 *		The ValidateProperty object should be destroyed at the same time
			 *		as the value that they reference internally
			**/
			template<class T>
			class ValidatedProperty {
				//! Store a reference to the property that is being managed
				T& mValue;

				//! Store a pointer to the callback function used for validation
				ValidationCallback<T> mCallback;

			public:
				//! Delete the default constructor to prevent NULL setup
				ValidatedProperty() = delete;

				//! Allow for simple construction, pointing the reference and callback at a value
				inline ValidatedProperty(T& pValue, ValidationCallback<T> pCB) : mValue(pValue), mCallback(pCB) {}

				//! Delete the copy constructor
				ValidatedProperty(const ValidatedProperty<T>& pCopy) = delete;

				//! Provide an explicit method for returning the value
				inline const T& value() const { return mValue; }

				//! Allow for implicit conversion to the type 'T'
				inline operator const T&() const { return mValue; }

				//! Allow for assigning of a new value, after validation
				inline ValidatedProperty& operator=(const T& pValue) { mValue = mCallback(pValue); return *this; }
			};
		}
	}
}