#include "Logger.hpp"

#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <mutex>

#include <time.h>

#include "DebugColor.hpp"

namespace SDL2_Engine {
	#if defined(_DEBUG) || defined(GENERATE_DEBUG_REPORT)
	#ifdef _DEBUG
	/*
		outputToConsole - Output a c-string to the standard output
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 08/02/2017

		param[in] pColor - The DebugColor to output the text in
		param[in] pMessage - The c-string message to output
	*/
	inline void outputToConsole(const DebugColor& pColor, const char* pMessage) {
		//Get a handle to the standard output
		HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);

		//Set the output color
		SetConsoleTextAttribute(hnd, pColor.value());

		//Output the message
		printf("%s\n\n", pMessage);

		//Reset the output color
		SetConsoleTextAttribute(hnd, DebugColor::WHITE);
	}
	#endif
	#ifdef GENERATE_DEBUG_REPORT
	/*
		outputToFile - Output a c-string to the log text file
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 08/02/2017

		param[in] pMessage - The c-string message to output
	*/
	inline void outputToFile(const char* pMessage) {
		//Try to open the log file
		std::ofstream file(DEBUG_LOG_FILE_PATH, std::ios::out | std::ios::app);

		//Ensure that the file has been opened
		if (!file.is_open())
			throw std::runtime_error(std::string("Unable to open the file '") + DEBUG_LOG_FILE_PATH + "'");

		//Write the message to the file
		file << pMessage << "\n\n";
	}
	#endif

	/*
		timeStampString - Prepend the current time information to a c-string
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 23/01/2017

		param[in/out] pMessage - The c-string to have the time information added to
	*/
	void timeStampString(char* pMessage) {
		//Create a copy of the message buffer
		char temp[MAX_DEBUG_MSG_SIZE] = { '\0' };

		//Copy the message characters to temp buffer
		memcpy_s(temp, sizeof(char) * MAX_DEBUG_MSG_SIZE, pMessage, sizeof(char) * MAX_DEBUG_MSG_SIZE);

		//Create a buffer to hold the time
		char timeStamp[64] = { '\0' };

		//Get the raw time
		time_t rawTime;
		time(&rawTime);

		//Store the time in a time structure
		struct tm timeInfo;
		localtime_s(&timeInfo, &rawTime);

		//Get the time as ASCII
		asctime_s(timeStamp, 64, &timeInfo);

		//Concatenate the strings
		sprintf_s(pMessage, MAX_DEBUG_MSG_SIZE, "%s%s", timeStamp, temp);
	} 

	/*
		Logger : outputMessage - Timestamp the supplied message and pass the information to the output functions
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 08/02/2017

		param[in] pColor - The Debug Color to apply to the console output
		param[in] pMessage - The c-string to output to the different paths
	*/
	void Logger::outputMessage(const DebugColor& pColor, char* pMessage) {
		//Keep a mutex to prevent multiple threads clashing over write access
		static std::mutex lock;

		//Create a lock guard to control the mutex
		std::lock_guard<std::mutex> guard(lock);

		//Apply the time stamping to the message
		timeStampString(pMessage);

		#ifdef _DEBUG
		outputToConsole(pColor, pMessage);
		#endif
		#ifdef GENERATE_DEBUG_REPORT
		outputToFile(pMessage);
		#endif
	}

	/*
		Logger : log - Log a formatted string to the set up output locations
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 24/01/2017

		param[in] pFormat - A c-string with the format symbols defined in 
							http://www.cplusplus.com/reference/cstdio/fprintf/
		param[in] Additional Values - The values to be formatted into the c-string
	*/
	void Logger::logFormatted(const char* pFormat, ...) {
		//Create a char buffer to hold the compiled string
		char buffer[MAX_DEBUG_MSG_SIZE] = { '\0' };

		//Create a parameter list
		va_list args;
		va_start(args, pFormat);

		//Format the string into the buffer
		vsprintf_s(buffer, MAX_DEBUG_MSG_SIZE, pFormat, args);

		//End the argument list
		va_end(args);

		//Send the message to be outputted
		outputMessage(DebugColor::WHITE, buffer);
	}

	/*
		Logger : log - Log a formatted string to the set up output locations in a specified color
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 24/01/2017

		param[in] pOutputColor - A Debug Color value describing the color to 
								 output the message to the console in
		param[in] pFormat - A c-string with the format symbols defined in
							http://www.cplusplus.com/reference/cstdio/fprintf/
		param[in] Additional Values - The values to be formatted into the c-string
	*/
	void Logger::logFormatted(const DebugColor& pOutputColor, const char* pFormat, ...) {
		//Create a char buffer to hold the compiled string
		char buffer[MAX_DEBUG_MSG_SIZE] = { '\0' };

		//Create a parameter list
		va_list args;
		va_start(args, pFormat);

		//Format the string into the buffer
		vsprintf_s(buffer, MAX_DEBUG_MSG_SIZE, pFormat, args);

		//End the argument list
		va_end(args);

		//Send the message to be outputted
		outputMessage(pOutputColor, buffer);
	}

	/*
		Logger : logWarning - Log a formatted warning string to the set up output locations
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 24/01/2017

		param[in] pFormat - A c-string with the format symbols defined in
							http://www.cplusplus.com/reference/cstdio/fprintf/
		param[in] Additional Values - The values to be formatted into the c-string
	*/
	void Logger::logWarning(const char* pFormat, ...) {
		// Create a char buffer to hold the compiled string
		char buffer[MAX_DEBUG_MSG_SIZE] = { "WARNING: " };

		//Get the length of the starting elements
		const uint LENGTH = strlen(buffer);

		//Check there is room for the message
		if (LENGTH > MAX_DEBUG_MSG_SIZE)
			throw std::runtime_error("Can not write warning message to buffer as there is not enough room in the buffer");

		//Create a parameter list
		va_list args;
		va_start(args, pFormat);

		//Format the string into the buffer
		vsprintf_s(buffer + LENGTH, MAX_DEBUG_MSG_SIZE - LENGTH, pFormat, args);

		//End the argument list
		va_end(args);

		//Send the message to be outputted
		outputMessage(DebugColor::YELLOW, buffer);
	}

	/*
		Logger : logError - Log a formatted error string to the set up output locations
		Author: Mitchell Croft
		Created: 23/01/2017
		Modified: 24/01/2017

		param[in] pFormat - A c-string with the format symbols defined in
							http://www.cplusplus.com/reference/cstdio/fprintf/
		param[in] Additional Values - The values to be formatted into the c-string
	*/
	void Logger::logError(const char* pFormat, ...) {
		// Create a char buffer to hold the compiled string
		char buffer[MAX_DEBUG_MSG_SIZE] = { "ERROR: " };

		//Get the length of the starting elements
		const uint LENGTH = strlen(buffer);

		//Check there is room for the message
		if (LENGTH > MAX_DEBUG_MSG_SIZE)
			throw std::runtime_error("Can not write error message to buffer as there is not enough room in the buffer");

		//Create a parameter list
		va_list args;
		va_start(args, pFormat);

		//Format the string into the buffer
		vsprintf_s(buffer + LENGTH, MAX_DEBUG_MSG_SIZE - LENGTH, pFormat, args);

		//End the argument list
		va_end(args);

		//Send the message to be outputted
		outputMessage(DebugColor::RED, buffer);
	}
	#endif
}