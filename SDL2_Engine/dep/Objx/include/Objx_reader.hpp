#pragma once

//! Include the Objx standard values
#include "Objx_values.hpp"

namespace Objx {
	/*
	 *		Name: Reader
	 *		Author: Mitchell Croft
	 *		Created: 14/08/2017
	 *		Modified: 05/10/2017
	 *		
	 *		Purpose:
	 *		Provide the functionality to be used to parse user input
	 *		in the standard Objx form for storage in Objx::Document and
	 *		Objx::Node components
	**/
    class OBJX_LIB_INC Reader {
	public:
		///////////////////////////////////////////////////////////////////////////////////
		////////-----------------------Constructor/Destructor----------------------////////
		///////////////////////////////////////////////////////////////////////////////////
		
		/*
			Reader : Default Constructor - Initialise with default values
			Created: 14/08/2017
			Modified: 05/10/2017

			param[in] pCallback - The function to be used to output error information during 
								  the parsing process (Default nullptr)
			param[in] pMaxDepth - The maximum number of Objx layers that can be read from an 
								  external data stream (Default 10)
		*/
		Reader(const ErrorCB& pCallback = nullptr, const size_t& pMaxDepth = 10U);

		/*
			Reader : Destructor - Deallocate used memory
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		~Reader();

		///////////////////////////////////////////////////////////////////////////////////
		////////--------------------------Parsing Functions------------------------////////
		///////////////////////////////////////////////////////////////////////////////////
		
		/*
			Reader : parse - Parse user input data from a stream and return an Document with the data
			Created: 14/08/2017
			Modified: 21/08/2017

			param[in] pData - A pointer to the beginning of the text data to be processed
			param[in] pStrict - Flags if the Document object should avoid allowing multiple Objx
								Nodes with the same name (Default false)

			return Document - Returns a Document object that has been setup to contain all parsed data
		*/
		Document parse(const char* pData, const bool& pStrict = false) noexcept;

		/*
			Reader : cparse - Parse user input data from a stream and return an Document with the data
			Created: 14/08/2017
			Modified: 21/08/2017

			param[in] pData - A pointer to the beginning of the text data to be processed
			param[in] pStrict - Flags if the Document object should avoid allowing multiple Objx
								Nodes with the same name (Default false)

			return const Document - Returns a constant Document object that has been setup to contain all parsed data
		*/
		const Document cparse(const char* pData, const bool& pStrict = false) noexcept;

		/*
			Reader : parse - Parse user input data from a stream and store the values in an existing 
							 Document object
			Created: 14/08/2017
			Modified: 22/08/2017

			param[in] pData - A pointer to the beginning of the text data to be processed
			param[in/out] pDoc - The Document object to have the new parsed values added to
		*/
		void parse(const char* pData, Document& pDoc) noexcept;

	private:
		//! Prototype internally used object and containers
		class SubString; class TokenPool;

		//! Store a pointer to the error callback function
		const ErrorCB* ERR_CB;

		//! Store the maximum Objx hierarchy depth
		const size_t MAX_OBJX_DEPTH;

		//! Store the indexed position into the current data stream
		size_t mPosition;

		//! Store the current hierarchy read depth
		size_t mReadDepth;

		//! Store pointer markers to track progress through a stream
		const char* mStart;
		const char* mCurrent;

		//! Store a pointer to the Tokens in use
		TokenPool* mTokens;

		///////////////////////////////////////////////////////////////////////////////////
		////////-----------------------Parsing Functionality-----------------------////////
		///////////////////////////////////////////////////////////////////////////////////
		
		/*-----------------------------------General-------------------------------------*/
		
		//! Shorthand function for outputting error information
		void out(const EStatusCode& pCode, const size_t& pPosition, const char* pFormat, ...) const noexcept;

		/*--------------------------------Tokenisation------------------------------------*/

		//! Move the progress markers
		void progress(const int& pVal) noexcept;

		//! Convert a data stream to processable tokens
		void tokeniseIdentifier() noexcept;
		bool tokeniseAssignment() noexcept;
		bool tokeniseControl() noexcept;
		bool tokeniseValue() noexcept;

		//! Extract segments of the data stream
		SubString extractSection(const bool& pConsume = false) noexcept;
		SubString extractNumber() noexcept;
		SubString extractString() noexcept;

		//! Skip through sections of the data stream
		void ignoreCharacters() noexcept;

		//! Iterate over the current progress point and look for matching syntax
		bool match(const char* pFind, const bool& pBuffer = true) noexcept;

		/*----------------------------------Parsing------------------------------------*/

		//! Determine the type of the value
		Node generateNode() const noexcept;

		//! Generate a Node object based on Token data
		void readValue(Node& pNode) const noexcept;
		void readObjx(Node& pNode) const noexcept;
		void readArray(Node& pNode) const noexcept;
    };
}