#pragma once

//! Include the Objx standard values
#include "Objx_values.hpp"

//! Include the standard output stream base
#include <ostream>

namespace Objx {
	/*
	 *		Name: Writer
	 *		Author: Mitchell Croft
	 *		Created: 21/09/2017
	 *		Modified: 21/09/2017
	 *		
	 *		Purpose:
	 *		Convert a Objx::Document object into outputtable text for storage in a file
	 *		or transmission over network
	**/
    class OBJX_LIB_INC Writer {
	public:
		/*
			Writer : serialise - Serialise an Objx::Document and store the text in an output stream
			Created: 21/09/2017
			Modified: 21/09/2017

			param[in] pOutput - A standard output stream that will receive the serialised text
			param[in] pDoc - The Objx Document to be serialised
		*/
		static void serialise(std::ostream& pOutput, const Document& pDoc) noexcept;

		/*
			Writer : serialise - Serailise an Objx::Document and return the generated text
			Created: 21/09/2017
			Modified: 21/09/2017

			param[in] pDoc - The Objx Document to be serialised

			return xstring - Generated text is returned as an xstring object
		*/
		static xstring serialise(const Document& pDoc) noexcept;
    };
}