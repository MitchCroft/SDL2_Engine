#pragma once

#include "LoggerForwardDeclares.hpp"

//! Allow for string streams in order to construct the log messages
#include <sstream>

//! Define the maximum size for a logger output message
#define MAX_DEBUG_MSG_SIZE 512

//! Define the filepath to store the log file at
#define DEBUG_LOG_FILE_PATH "log.txt"

//! Define used to output log messages to the log.txt file
#define GENERATE_BEBUG_REPORT

namespace SDL2_Engine {
	/*
	 *		Name: Logger
	 *		Author: Mitchell Croft
	 *		Created: 23/01/2017
	 *		Modified: 24/01/2017
	 *		
	 *		Purpose:
	 *		Allow for logging text to the output window
	 *		or a log file
	**/
	class Logger {
		//! Remove the constructor and destructor
		Logger() = delete; ~Logger() = delete;

		#if defined(_DEBUG) || defined(GENERATE_BEBUG_REPORT)
		static void outputMessage(const DebugColor& pColor, char* pMessage);

		template<typename T, typename ... U>
		inline static void build_string(std::ostream& pStream, const T& pVal, const U& ... pArgs);
		inline static void build_string(std::ostream& pStream) {}

	public:
		template<typename ... T>
		static void log(const T& ... pArgs);

		static void logFormatted(const char* pFormat, ...);

		static void logFormatted(const DebugColor& pOutputColor, const char* pFormat, ...);

		static void logWarning(const char* pFormat, ...);

		static void logError(const char* pFormat, ...);
		#else
	public:
		template<typename ... T>
		static inline void log(const T& ... pArgs) {}

		template<typename ... T>
		static inline void logFormatted(const T& ... pArgs) {}

		template<typename ... T>
		static inline void logWarning(const T& ... pArgs) {}

		template<typename ... T>
		static inline void logError(const T& ... pArgs) {}
		#endif
	};

	#if defined(_DEBUG) || defined(GENERATE_BEBUG_REPORT)
	/*
		Logger : build_string - Build a c-string from a number of indiscriminate types
		Author: Mitchell Croft
		Created: 24/01/2017
		Modified: 24/01/2017

		param[in] pStream - The data stream to store the c-string information
		param[in] pVal - The next value to be processed for the c-string
		param[in] pArgs - The remaining arguments to be processed
	*/
	template<typename T, typename ...U>
	inline void Logger::build_string(std::ostream& pStream, const T& pVal, const U& ...pArgs) {
		//Output the next value to the stream
		pStream << pVal;

		//Recurse down
		build_string(pStream, pArgs...);
	}

	/*
		Logger : log - Log a c-string built from indiscriminate types to the set up output locations
		Author: Mitchell Croft
		Created: 24/01/2017
		Modified: 24/01/2017

		param[in] pArgs - The argument values that are to be constructed into a c-string
	*/
	template<typename ...T>
	inline void Logger::log(const T& ...pArgs) {
		//Create a stream to hold the c-string
		std::ostringstream o;

		//Pass the values to be evaluated
		build_string(o, pArgs...);

		//Create a buffer to store the c-string data
		char buffer[MAX_DEBUG_MSG_SIZE];

		//Get the constructed string
		std::string construct = o.str();

		//Copy the string data over
		memcpy_s(buffer, sizeof(char) * MAX_DEBUG_MSG_SIZE, construct.c_str(), sizeof(char) * ((int)construct.size() + 1 > MAX_DEBUG_MSG_SIZE ? MAX_DEBUG_MSG_SIZE : construct.size() + 1));

		//Output the string
		outputMessage(DebugColor::WHITE, buffer);
	}
	#endif
}