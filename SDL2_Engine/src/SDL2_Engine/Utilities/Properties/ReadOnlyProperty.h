#pragma once

namespace SDL2_Engine {
	namespace Utilities {
		namespace Properties {

			/*
			*		Name: ReadOnlyProperty
			*		Author: Mitchell Croft
			*		Created: 20/01/2017
			*		Modified: 20/01/2017
			*
			*		Purpose:
			*		Provide a method of using a value as a user normally would
			*		while restricting write access
			*
			*		WARNING:
			*		The ReadOnlyProperty object should be destroyed at the same time
			*		as the value that they reference internally
			**/
			template<class T> 
			class ReadOnlyProperty {
				//! Store a reference to the property that is being managed
				T& mValue;

			public:
				//! Delete the default constructor to prevent NULL setup
				ReadOnlyProperty() = delete;

				//! Allow for simple construction, pointing the reference at a value
				inline ReadOnlyProperty(T& pValue) : mValue(pValue) {}

				//! Delete the copy constructor
				ReadOnlyProperty(const ReadOnlyProperty<T>& pCopy) = delete;

				//! Provide an explicit method for returning the value
				inline const T& value() const { return mValue; }

				//! Allow for implicit conversion to the type 'T'
				inline operator const T&() const { return mValue; }

				//! Delete the assignment operator
				inline ReadOnlyProperty<T>& operator=(const ReadOnlyProperty<T>& pCopy) = delete;
			};
		}
	}
}