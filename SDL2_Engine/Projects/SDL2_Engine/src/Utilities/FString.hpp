#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"

//! Include the STL functions
#include <cstring>
#include <string.h>

namespace SDL2_Engine {
	namespace Utilities {
		/*
		*		Name: FString
		*		Author: Mitchell Croft
		*		Created: 12/10/2017
		*		Modified: 12/10/2017
		*
		*		Template N - A size_t value defining the maximum length of the string
		*
		*		Purpose:
		*		Provide basic string functionality with a fixed length
		**/
		template<size_t N>
		class FString {
		public:
			//! Ensure that there is at minimum enough space for a character and null terminator
			static_assert(N >= 1, "FString can not have a fixed length of less then 1. It requires space for at least one character");

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Construction--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				FString : Custom Constructor - Initialise with default values
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pString - A c-string containing the values to copy (Default nullptr)
			*/
			inline FString(const char* pString = nullptr) : mString("\0"), mLength(0) { if (pString) *this = pString; }

			/*
				FString : Custom Constructor - Initialise with a single char value
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pChar - The single character to store in the FString
			*/
			FString(const char& pChar);

			/*
				FString : Copy Constructor - Initialise with the values of another FString
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pCopy - A FString with a length of C
			*/
			template<size_t C>
			inline FString(const FString<C>& pCopy) : mString("\0"), mLength(0) { *this = pCopy; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////---------------------------------Main Functions--------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				FString : toLower - Get a copy of the current, converted to lower case
				Created: 12/10/2017
				Modified: 12/10/2017

				return FString<N> - Returns an FString object of the same length as the original
			*/
			FString toLower() const;

			/*
				FString : toUpper - Get a copy of the current, converted to upper case
				Created: 12/10/2017
				Modified: 12/10/2017

				return FString<N> - Returns an FString object of the same length as the original
			*/
			FString toUpper() const;

			/*
				FString : clear - Empty the string of character information
				Created: 12/10/2017
				Modified: 12/10/2017

				return FString<N>& - Returns a reference to itself
			*/
			inline FString& clear() { mString[0] = '\0'; mLength = 0; return *this; }

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Getters------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				FString : length - Get the current length of the string
				Created: 12/10/2017
				Modified: 12/10/2017

				return const size_t& - Returns a constant reference to the internal size_t value
			*/
			inline const size_t& length() const { return mLength; }

			/*
				FString : c_str - Get the raw, internal c-string pointer
				Created: 12/10/2017
				Modified: 12/10/2017

				return const char* - Returns a constant pointer to the internal array
			*/
			inline const char* c_str() const { return mString; }

			/*
				FString : find - Find the starting index of the first occurrence of a character sequence
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pSequence - The sequence of characters to be found
				param[in] pStartIndex - The index from which to start searching (Default 0)

				return int - Returns the index of the start of the sequence or -1 if not found
			*/
			int find(const char* pSequence, const size_t& pStartIndex = 0U) const;

			/*
				FString : rfind - Find the starting index of the last occurrence of a character sequence
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pSequence - The sequence of characters to be found
				param[in] pStartIndex - The index from which to start searching (Default N)

				return int - Returns the index of the start of the sequence or -1 if not found
			*/
			int rfind(const char* pSequence, const size_t& pStartIndex = N) const;

			/*
				FString : find_first_of - Find the index of the first occurrence of one of the specified characters
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pFind - The characters to look for
				param[in] pStartIndex - The index to start searching from (Default 0)

				return int - Returns the index of first character encountered as part of pFind
			*/
			int find_first_of(const char* pFind, const size_t& pStartIndex = 0) const;

			/*
				FString : find_first_not_of - Find the index of the first occurrence of not one of the specified characters
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pAvoid - The characters to ignore
				param[in] pStartIndex - The index to start searching from (Default 0)

				return int - Returns the index of the first character not found in pAvoid, or -1 if not found
			*/
			int find_first_not_of(const char* pAvoid, const size_t& pStartIndex = 0) const;

			/*
				FString :  find_last_of - Find the index of the last occurrence of one of the specified characters
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pFind - The characters to look for
				param[in] pStartIndex - The index to start searching from (Default N)

				return int - Returns the index of the first character encountered as part of pFind
			*/
			int find_last_of(const char* pFind, const size_t& pStartIndex = N) const;

			/*
				FString : find_last_not_of - Find the index of the last occurrence of not one of the specified characters
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pAvoid - The characters to ignore
				param[in] pStartIndex - The index to start searching from (Default N)

				return int - Returns the index of the last character not found in pAvoid, or -1 if not found
			*/
			int find_last_not_of(const char* pAvoid, const size_t& pStartIndex = N) const;

			/*
				FString : substr - Retrieve a sub section of the current FString
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pStart - The index to start copying characters from
				param[in] pLength - The number of characters to take, where < 0 is to the end of the FString (Default -1)

				return FString<N> - Returns an FString object of the same length as the original, with the requested characters
			*/
			FString substr(const size_t& pStart, const int& pLength = -1) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////------------------------------------Setters------------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				FString : append - Append characters to the end of the current
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pString - The sequence of characters to append

				return FString<N>& - Returns a reference to itself
			*/
			FString& append(const char* pString);

			/*
				FString : prepend - Prepend characters to the beginning of the current
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pString - The sequence of characters to prepend

				return FString<N>& - Returns a reference to itself
			*/
			FString& prepend(const char* pString);

			/*
				FString : replace - Replace a sequence of characters with another
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pFind - The sequence of characters to find in the current
				param[in] pReokace - The sequence of characters to replace pFind with

				return FString<N>& - Returns a reference to itself
			*/
			FString& replace(const char* pFind, const char* pReplace);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Operators-----------------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				FString : Equality Operator (FString) - Compare two FString's to determine if they are equal
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the two FString's are equivalent
			*/
			template<size_t C>
			inline bool operator==(const FString<C>& pRight) const { return (strcmp(mString, pRight.c_str()) == 0); }

			/*
				FString : Equality Operator (C-String) - Compare the current against a C-String to determine if they are equal
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The C-String to compare against

				return bool - Returns true if the two strings are equivalent
			*/
			inline bool operator==(const char* pRight) const { return (strcmp(mString, pRight) == 0); }

			/*
				FString : Inequality Operator (FString) - Compare two FString's to determine if they are unequal
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the two FString's are different
			*/
			template<size_t C>
			inline bool operator!=(const FString<C>& pRight) const { return !(*this == pRight); }

			/*
				FString : Inequality Operator (C-String) - Compare the current against a C-String to determine if they are different
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The C-String to compare against

				return bool - Returns true if the two strings are different
			*/
			inline bool operator!=(const char* pRight) const { return !(*this == pRight); }

			/*
				FString : Greater than Operator - Compare ordering information of two FStrings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the current is alphabetically after pRight
			*/
			template<size_t C>
			inline bool operator>(const FString<C>& pRight) const { return (strcmp(mString, pRight.c_str()) > 0); }

			/*
				FString : Greater than or Equal Operator - Compare ordering information of two FStrings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the current is alphabetically equal to or after pRight
			*/
			template<size_t C>
			inline bool operator>=(const FString<C>& pRight) const { return (strcmp(mString, pRight.c_str()) >= 0); }

			/*
				FString : Less than Operator - Compare ordering information of two FStrings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the current is alphabetically before pRight
			*/
			template<size_t C>
			inline bool operator<(const FString<C>& pRight) const { return (strcmp(mString, pRight.c_str()) < 0); }

			/*
				FString : Less than or Equal Operator - Compare ordering information of two FStrings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to compare against

				return bool - Returns true if the current is alphabetically before or equal to pRight
			*/
			template<size_t C>
			inline bool operator<=(const FString<C>& pRight) const { return (strcmp(mString, pRight.c_str()) <= 0); }

			/*
				FString : Concatenation Operator (FString) - Get the result of combining two FStrings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to combine with the current

				return FString<N + C> - Returns a new FString with the size (N + C)
			*/
			template<size_t C>
			inline FString<N + C> operator+(const FString<C>& pRight) const { return FString<N + C>(mString).append(pRight.c_str()); }

			/*
				FString : Concatenation Operator (C-String) - Get the result of combining two strings
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The C-String to combine with the current

				return FString<N> - Returns a new FString with size N
			*/
			inline FString operator+(const char* pRight) const { return FString<N>(mString).append(pRight); }

			/*
				FString : Concatenation Assignment Operator (FString) - Combine the current FString with another
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to combine with the current

				return FString<N>& - Returns a reference to itself
			*/
			template<size_t C>
			inline FString& operator+=(const FString<C>& pRight) { return append(pRight.c_str()); }

			/*
				FString : Concatenation Assignment Operator (C-String) - Combine the current FString with a C-String
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The C-String to combine with the current

				return FString<N>& - Returns a reference to itself
			*/
			inline FString& operator+=(const char* pRight) { return append(pRight); }

			/*
				FString : Assignment Operator (FString) - Assign the current FString to the values of another
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The FString to copy

				return FString<N>& - Returns a reference to itself
			*/
			template<size_t C>
			FString& operator=(const FString<C>& pRight);

			/*
				FString : Assignment Operator (C-String) - Assign the current FString to the values of a C-String
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pRight - The C-String to copy

				return FString<N>& - Returns a reference to itself
			*/
			FString& operator=(const char* pRight);

			/*
				FString : Subscript Operator (Non-Const) - Provide access to the internal array
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pIndex - The index to access

				return char& - Returns a reference to an internal character
			*/
			inline char& operator[](const size_t& pIndex) { return mString[pIndex]; }

			/*
				FString : Subscript Operator (Const) - Provide access to the internal array
				Created: 12/10/2017
				Modified: 12/10/2017

				param[in] pIndex - The index to access

				return const char& - Returns a constant reference to an internal character
			*/
			inline const char& operator[](const size_t& pIndex) const { return mString[pIndex]; }

		private:
			//! Store the char array
			char mString[N + 1];

			//! Store the length of the string
			size_t mLength;
		};

		/*
			FString : Custom Constructor - Initialise with a single char value
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pChar - The single character to store in the FString
		*/
		template<size_t N>
		FString<N>::FString(const char& pChar) : mString("\0"), mLength(0) {
			//Save the first character
			mString[0] = pChar;

			//Save the length
			mLength = 1;

			//Null terminate
			mString[mLength] = '\0';
		}

		/*
			FString : toLower - Get a copy of the current, converted to lower case
			Created: 12/10/2017
			Modified: 12/10/2017

			return FString<N> - Returns an FString object of the same length as the original
		*/
		template<size_t N>
		FString<N> FString<N>::toLower() const {
			//Make a copy of the data
			FString<N> temp(mString);

			//Loop through and to lower
			for (size_t i = 0; temp.mString[i] != '\0'; i++)
				temp.mString[i] = tolower(temp.mString[i]);

			//Return the copy
			return temp;
		}

		/*
			FString : toUpper - Get a copy of the current, converted to upper case
			Created: 12/10/2017
			Modified: 12/10/2017

			return FString<N> - Returns an FString object of the same length as the original
		*/
		template<size_t N>
		FString<N> FString<N>::toUpper() const {
			//Make a copy of the data
			FString<N> temp(mString);

			//Loop through and to upper
			for (size_t i = 0; temp.mString[i] != '\0'; i++)
				temp.mString[i] = toupper(temp.mString[i]);

			//Return the copy
			return temp;
		}

		/*
			FString : find - Find the starting index of the first occurrence of a character sequence
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pSequence - The sequence of characters to be found
			param[in] pStartIndex - The index from which to start searching (Default 0)

			return int - Returns the index of the start of the sequence or -1 if not found
		*/
		template<size_t N>
		int FString<N>::find(const char* pSequence, const size_t& pStartIndex /*= 0*/) const {
			//Store the length of the character sequence
			const int SEQ_LEN = strlen(pSequence);

			//Store the last possible index the sequence can exist in
			const int LAST_IND = (int)mLength - SEQ_LEN;

			//Start searching through the current FString
			for (int i = pStartIndex; i <= LAST_IND; i++) {
				//Check for match
				if (strncmp(mString + i, pSequence, SEQ_LEN) == 0)
					return i;
			}

			//Default not found
			return -1;
		}

		/*
			FString : rfind - Find the starting index of the last occurrence of a character sequence
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pSequence - The sequence of characters to be found
			param[in] pStartIndex - The index from which to start searching (Default N)

			return int - Returns the index of the start of the sequence or -1 if not found
		*/
		template<size_t N>
		int FString<N>::rfind(const char* pSequence, const size_t& pStartIndex /*= N*/) const {
			//Store the length of the character sequence
			const int SEQ_LEN = strlen(pSequence);

			//Store the last possible index the sequence can exist in
			const int LAST_IND = (int)mLength - SEQ_LEN;

			//Start searching through the current FString
			for (int i = (LAST_IND < (int)pStartIndex ? LAST_IND : pStartIndex); i >= 0; --i) {
				//Check for match
				if (strncmp(mString + i, pSequence, SEQ_LEN) == 0)
					return i;
			}

			//Default not found
			return -1;
		}

		/*
			FString : find_first_of - Find the index of the first occurrence of one of the specified characters
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pFind - The characters to look for
			param[in] pStartIndex - The index to start searching from (Default 0)

			return int - Returns the index of first character encountered as part of pFind
		*/
		template<size_t N>
		int FString<N>::find_first_of(const char* pFind, const size_t& pStartIndex /*= 0*/) const {
			//Store the length of the character sequence
			const size_t SEQ_LEN = strlen(pFind);

			//Start looking through characters
			for (size_t i = pStartIndex; i < mLength; i++) {
				//Compare character sequence
				for (size_t j = 0; j < SEQ_LEN; j++) {
					if (mString[i] == pFind[j])
						return i;
				}
			}

			//Default not found
			return -1;
		}

		/*
			FString : find_first_not_of - Find the index of the first occurrence of not one of the specified characters
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pAvoid - The characters to ignore
			param[in] pStartIndex - The index to start searching from (Default 0)

			return int - Returns the index of the first character not found in pAvoid, or -1 if not found
		*/
		template<size_t N>
		int FString<N>::find_first_not_of(const char* pAvoid, const size_t& pStartIndex /*= 0*/) const {
			//Store the length of the character sequence
			const size_t SEQ_LEN = strlen(pAvoid);

			//Start looking through characters
			for (size_t i = pStartIndex; i < mLength; i++) {
				//Flag found matching character
				bool flag = false;

				//Compare character sequence
				for (size_t j = 0; j < SEQ_LEN; j++) {
					if (mString[i] == pAvoid[j]) {
						flag = true;
						break;
					}
				}

				//Check if it was not found
				if (!flag) return i;
			}

			//Default not found
			return -1;
		}

		/*
			FString :  find_last_of - Find the index of the last occurrence of one of the specified characters
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pFind - The characters to look for
			param[in] pStartIndex - The index to start searching from (Default N)

			return int - Returns the index of the first character encountered as part of pFind
		*/
		template<size_t N>
		int FString<N>::find_last_of(const char* pFind, const size_t& pStartIndex /*= N*/) const {
			//Store the length of the character sequence
			const size_t SEQ_LEN = strlen(pFind);

			//Start looking through characters
			for (size_t i = (pStartIndex >= mLength ? (int)mLength - 1 : pStartIndex); i >= 0; --i) {
				//Compare character sequence
				for (size_t j = 0; j < SEQ_LEN; j++) {
					if (mString[i] == pFind[j])
						return i;
				}
			}

			//Default not found
			return -1;
		}

		/*
			FString : find_last_not_of - Find the index of the last occurrence of not one of the specified characters
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pAvoid - The characters to ignore
			param[in] pStartIndex - The index to start searching from (Default N)

			return int - Returns the index of the last character not found in pAvoid, or -1 if not found
		*/
		template<size_t N>
		int FString<N>::find_last_not_of(const char* pAvoid, const size_t& pStartIndex /*= N*/) const {
			//Store the length of the character sequence
			const size_t SEQ_LEN = strlen(pAvoid);

			//Start looking characters
			for (int i = (pStartIndex >= mLength ? (int)mLength - 1 : pStartIndex); i >= 0; --i) {
				//Flag found matching character
				bool flag = false;

				//Compare character sequence
				for (size_t j = 0; j < SEQ_LEN; j++) {
					if (mString[i] == pAvoid[j]) {
						flag = true;
						break;
					}
				}

				//Check if it was not found
				if (!flag) return i;
			}

			//Default not found
			return -1;
		}

		/*
			FString : substr - Retrieve a sub section of the current FString
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pStart - The index to start copying characters from
			param[in] pLength - The number of characters to take, where < 0 is to the end of the FString (Default -1)

			return FString<N> - Returns an FString object of the same length as the original, with the requested characters
		*/
		template<size_t N>
		FString<N> FString<N>::substr(const size_t& pStart, const int& pLength) const {
			//Clean the length value
			const int LENGTH = (pLength < 0 ? (int)mLength - (int)pStart : (pStart + pLength >= mLength ? (int)mLength - (int)pStart : pLength));

			//Check if there is a String to extract
			if (LENGTH <= 0) return FString();

			//Create a String to hold the sequence
			FString<N> temp;

			//Copy the selection into the temp
			memcpy_s(temp.mString, sizeof(char) * N, mString + pStart, sizeof(char) * LENGTH);

			//Set the new length value
			temp.mLength = LENGTH;

			//Null terminate the new String
			temp.mString[LENGTH] = '\0';

			//Return the new String
			return temp;
		}

		/*
			FString : append - Append characters to the end of the current
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pString - The sequence of characters to append

			return FString<N>& - Returns a reference to itself
		*/
		template<size_t N>
		FString<N>& FString<N>::append(const char* pString) {
			//Get the length of the sequence
			const size_t LENGTH = strlen(pString);

			//Get the remaining space in the current
			const size_t REMAINING = (int)N - (int)mLength;

			//Check there is space left
			if (REMAINING) {
				//Save the number of characters to copy
				const size_t TO_CPY = (LENGTH < REMAINING ? LENGTH : REMAINING);

				//Copy the new characters
				memcpy_s(mString + mLength, sizeof(char) * REMAINING, pString, sizeof(char) * TO_CPY);

				//Save the new length
				mLength += TO_CPY;

				//Null terminate
				mString[mLength] = '\0';
			}

			//Return a reference to itself
			return *this;
		}

		/*
			FString : prepend - Prepend characters to the beginning of the current
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pString - The sequence of characters to prepend

			return FString<N>& - Returns a reference to itself
		*/
		template<size_t N>
		FString<N>& FString<N>::prepend(const char* pString) {
			//Get the length of the new characters
			const size_t LENGTH = strlen(pString);

			//Get the complete length of the new string
			const size_t TOTAL_LENGTH = mLength + LENGTH;

			//Get the number of characters that can be copied
			const size_t TO_CPY = (TOTAL_LENGTH < N ? TOTAL_LENGTH : N);

			//Create a buffer
			char buffer[N + 1];

			//Calculate the number of characters to copy
			const size_t PREP_SIZE = (LENGTH < TO_CPY ? LENGTH : TO_CPY);

			//Copy the characters in
			memcpy_s(buffer, sizeof(char) * N, pString, sizeof(char) * PREP_SIZE);

			//Get the final characters to copy over
			const size_t OLD_SIZE = (mLength < (TO_CPY - PREP_SIZE) ? mLength : TO_CPY - PREP_SIZE);

			//Copy the remaining characters
			memcpy_s(buffer + PREP_SIZE, sizeof(char) * (N - PREP_SIZE), mString, sizeof(char) * OLD_SIZE);

			//Set the null terminator
			buffer[TO_CPY] = '\0';

			//Set the new length
			mLength = TO_CPY;

			//Copy the old buffer over
			memcpy_s(mString, sizeof(char) * (N + 1), buffer, sizeof(char) * (N + 1));

			//Return itself
			return *this;
		}

		/*
			FString : replace - Replace a sequence of characters with another
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pFind - The sequence of characters to find in the current
			param[in] pReokace - The sequence of characters to replace pFind with

			return FString<N>& - Returns a reference to itself
		*/
		template<size_t N>
		FString<N>& FString<N>::replace(const char* pFind, const char* pReplace) {
			//Store the length of the c strings
			const size_t FIND_LEN = strlen(pFind);
			const size_t REPL_LEN = strlen(pReplace);

			//Store the number of replacements that need to be made
			size_t replaceCount = 0;

			//Store the return index
			int foundIndex = -(int)FIND_LEN;

			//Count the number of replacements to make
			while ((foundIndex = find(pFind, foundIndex += FIND_LEN)) != -1) replaceCount++;

			//If no occurrences, exit
			if (!replaceCount) return *this;

			//Calculate the size of the new string
			const size_t FINAL_LENGTH = mLength + REPL_LEN * replaceCount - FIND_LEN * replaceCount;

			//Store the number of characters to copy
			const size_t CPY_TOTAL = (FINAL_LENGTH < N ? FINAL_LENGTH : N);

			//Create a buffer to hold the new string
			char buffer[N + 1];

			//Track the progress through the old and new strings
			size_t oldProg = 0, newProg = 0;

			//Reset the found index
			foundIndex = -(int)FIND_LEN;

			//Loop for construction of the new string
			while (true) {
				//Get the next found index
				if ((foundIndex = find(pFind, foundIndex += FIND_LEN)) != -1) {
					//Get the number of characters to copy
					int toCpy = (foundIndex - (int)oldProg);
					toCpy = (toCpy < (int)(CPY_TOTAL - newProg) ? toCpy : CPY_TOTAL - newProg);

					//Check there is enough room
					if (toCpy <= 0 && newProg >= CPY_TOTAL) break;

					//Copy the characters leading up to the found index
					memcpy_s(buffer + newProg, sizeof(char) * (N - newProg), mString + oldProg, sizeof(char) * toCpy);

					//Update the new progress value
					newProg += (foundIndex - oldProg);

					//Get the number of characters to copy
					toCpy = REPL_LEN;
					toCpy = (toCpy < (int)(CPY_TOTAL - newProg) ? toCpy : CPY_TOTAL - newProg);

					//Check there is enough room
					if (toCpy <= 0 && newProg >= CPY_TOTAL) break;

					//Copy the replacement characters into the buffer
					memcpy_s(buffer + newProg, sizeof(char) * (N - newProg), pReplace, sizeof(char) * toCpy);

					//Update the progress values
					newProg += REPL_LEN;
					oldProg = foundIndex + FIND_LEN;
				}

				//If there are no more found instances, copy the rest of the string
				else {
					//Get the number of characters to copy
					int toCpy = (mLength - (int)oldProg);
					toCpy = (toCpy < (int)(CPY_TOTAL - newProg) ? toCpy : CPY_TOTAL - newProg);

					//Check there is enough room
					if (toCpy <= 0 && newProg >= CPY_TOTAL) break;

					//Copy the remaining characters
					memcpy_s(buffer + newProg, sizeof(char) * (N - newProg), mString + oldProg, sizeof(char) * toCpy);

					//Break from the loop
					break;
				}
			}

			//Set the new length
			mLength = CPY_TOTAL;

			//Null terminate the string
			buffer[mLength] = '\0';

			//Copy over the buffer
			memcpy_s(mString, sizeof(char) * N, buffer, sizeof(char) * N);

			//Return itself
			return *this;
		}

		/*
			FString : Assignment Operator (FString) - Assign the current FString to the values of another
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pRight - The FString to copy

			return FString<N>& - Returns a reference to itself
		*/
		template<size_t N>
		template<size_t C>
		FString<N>& FString<N>::operator=(const FString<C>& pRight) {
			//Store the number of characters to copy
			const size_t TO_CPY = (pRight.length() < N ? pRight.length() : N);

			//Copy the string
			memcpy_s(mString, sizeof(char) * N, pRight.c_str(), sizeof(char) * TO_CPY);

			//Null terminate the string
			mString[TO_CPY] = '\0';

			//Save the length
			mLength = TO_CPY;

			//Return itself
			return *this;
		}

		/*
			FString : Assignment Operator (C-String) - Assign the current FString to the values of a C-String
			Created: 12/10/2017
			Modified: 12/10/2017

			param[in] pRight - The C-String to copy

			return FString<N>& - Returns a reference to itself
		*/
		template<size_t N>
		inline FString<N>& FString<N>::operator=(const char* pRight) {
			//Get the length of the string
			const size_t LENGTH = strlen(pRight);

			//Store the number of characters to copy
			const size_t TO_CPY = (LENGTH < N ? LENGTH : N);

			//Copy the string
			memcpy_s(mString, sizeof(char) * N, pRight, sizeof(char) * TO_CPY);

			//Null terminate the string
			mString[TO_CPY] = '\0';

			//Save the length
			mLength = TO_CPY;

			//Return itself
			return *this;
		}
	}
}