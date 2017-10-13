#include "Logger.hpp"

//! Include the required headers to construct and output log messages
#include <windows.h>
#include <stdio.h>
#include <fstream>
#include <mutex>
#include <iomanip>
#include <ctime>

#include "LoggerInitialiser.hpp"

//! Define a maximum size for the logger output messages
#define SDL2_ENGINE_LOG_MSG_MAX_LEN 1024U

namespace SDL2_Engine {
	namespace Debug {
		//! Prototype the timestamp retrieval function
		void getTimeStamp(char pBuffer[64]);

		/*
			Logger : logFormatted - Log a formatted string to the defined output locations
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pFormat - A c-string that contains the text and formatting data to be output
			param[in] ... - The additional arguments to be inserted into the formatted string

			Note:
			See http://www.cplusplus.com/reference/cstdio/printf/ for formatting options
		*/
		void Logger::logFormatted(const char* pFormat, ...) const {
			//Create a buffer to hold the compiled string
			char buffer[SDL2_ENGINE_LOG_MSG_MAX_LEN] = { '\0' };

			//Create a parameter list
			va_list args;
			va_start(args, pFormat);

			//Format the string into the buffer
			vsprintf_s(buffer, pFormat, args);

			//End the argument list
			va_end(args);

			//Send the message to be outputted
			outputMessage(DebugColor::WHITE, buffer);
		}

		/*
			Logger : logFormatted - Log a formatted string to the defined output locations in a specific color
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pColor - The color to output the text in (Only applies to the standard output window)
			param[in] pFormat - A c-string that contains the text and formatting data to be output
			param[in] ... - The additional arguments to be inserted into the formatted string
		*/
		void Logger::logFormatted(const DebugColor& pColor, const char* pFormat, ...) const {
			//Create a buffer to hold the compiled string
			char buffer[SDL2_ENGINE_LOG_MSG_MAX_LEN] = { '\0' };

			//Create a parameter list
			va_list args;
			va_start(args, pFormat);

			//Format the string into the buffer
			vsprintf_s(buffer, pFormat, args);

			//End the argument list
			va_end(args);

			//Send the message to be outputted
			outputMessage(pColor, buffer);
		}

		/*
			Logger : logWarning - Log a formatted warning string to the defined output locations
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pFormat - A c-string that contains the text and formatting data to be output
			param[in] ... - The additional arguments to be inserted into the formatted string
		*/
		void Logger::logWarning(const char* pFormat, ...) const {
			//Create a buffer to hold the compiled string
			char buffer[SDL2_ENGINE_LOG_MSG_MAX_LEN] = { "WARNING: " };

			//Get the length of the starting elements
			const size_t LENGTH = strlen(buffer);

			//Create a parameter list
			va_list args;
			va_start(args, pFormat);

			//Format the string into the buffer
			vsprintf_s(buffer + LENGTH, SDL2_ENGINE_LOG_MSG_MAX_LEN - LENGTH, pFormat, args);

			//End the argument list
			va_end(args);

			//Send the message to be outputted
			outputMessage(DebugColor::YELLOW, buffer);
		}

		/*
			Logger : logError - Log a formatted error string to the defined output locations
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pFormat - A c-string that contains the text and formatting data to be output
			param[in] ... - The additional arguments to be inserted into the formatted string
		*/
		void Logger::logError(const char* pFormat, ...) const {
			//Create a buffer to hold the compiled string
			char buffer[SDL2_ENGINE_LOG_MSG_MAX_LEN] = { "ERROR: " };

			//Get the length of the starting elements
			const size_t LENGTH = strlen(buffer);

			//Create a parameter list
			va_list args;
			va_start(args, pFormat);

			//Format the string into the buffer
			vsprintf_s(buffer + LENGTH, SDL2_ENGINE_LOG_MSG_MAX_LEN - LENGTH, pFormat, args);

			//End the argument list
			va_end(args);

			//Send the message to be outputted
			outputMessage(DebugColor::RED, buffer);
		}

		/*
			Logger : setOutputLocation - Set the folder location where the log file will be created
			Created: 20/07/2017
			Modified: 13/10/2017

			param[in] pLocation - A string indicating the file location to store the file
		*/
		void Logger::setOutputLocation(const char* pLocation) {
			//Save the new location
			mLogLocation = pLocation;

			//Ensure that that is folder location
			mLogLocation = mLogLocation.substr(0, mLogLocation.find_last_of("/\\") + 1);

			//Check that there is a position left to save it
			if (!mLogLocation.length()) throw new std::runtime_error("Logger was given an invalid output location");
		}

		/*
			Logger : Constructor - Initialise with default values
			Created: 20/07/2017
			Modified: 13/10/2017

			param[in] pSetup - Defines how the Logger should be setup
		*/
		Logger::Logger(const Initialisation::LoggerInitialiser& pSetup) : 
			mLogOutput(pSetup.storeLog), 
			mLogLocation(pSetup.logLocation)
		{}

		/*
			Logger : outputMessage - Uniform messaging function used to relay the text to the destinations
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pDebugColor - The color value that is to be used when outputting to the standard out
			param[in] pMessage - The text to be output to the locations
		*/
		void Logger::outputMessage(const DebugColor& pColor, const char* pMessage) const {
			//Keep a mutex to prevent multiple threads clashing over write access
			static std::mutex lock;

			//Generate the timestamp
			char buffer[64];
			getTimeStamp(buffer);

			//Create a lock guard to control the mutex
			std::lock_guard<std::mutex> guard(lock);

			//Output the text to the specified destinations
			if (mLogOutput) outputToFile(buffer, pMessage);
			outputToSTDOutput(pColor, buffer, pMessage);
		}

		/*
			Logger : outputToSTDOutput - Output the message to the standard output
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in] pColor - The color to output the message in
			param[in] pTime - The timestamp for when the event occurred
			param[in] pMessage - The text to be output to the output
		*/
		void Logger::outputToSTDOutput(const DebugColor& pColor, const char* pTime, const char* pMessage) const {
			//Get a handle to the standard output
			HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);

			//Set the output color
			SetConsoleTextAttribute(hnd, pColor);

			//Output the message
			printf("%s%s\n\n", pTime, pMessage);

			//Reset the output color
			SetConsoleTextAttribute(hnd, DebugColor::WHITE);
		}
		
		/*
			Logger : outputToFile - Output the message to the specified output file
			Created: 20/07/2017
			Modified: 13/10/2017

			param[in] pTime - The timestamp for when the event occurred
			param[in] pMessage - The text to be output to the file
		*/
		void Logger::outputToFile(const char* pTime, const char* pMessage) const {
			//Try and open the log file
			std::wofstream file((mLogLocation + "Logger.log").c_str(), std::ios::out | std::ios::app);

			//Ensure the file is open
			if (!file.is_open()) throw std::runtime_error("Failed to to open the log file at the specified location");

			//Write the message to the file
			file << pTime << pMessage << "\n\n";

			//Close the file
			file.close();
		}

		/*
			getTimeStamp - Get the current timestamp as a string and store it in a buffer
			Author: Mitchell Croft
			Created: 20/07/2017
			Modified: 05/10/2017

			param[in/out] pBuffer - The string buffer to store the current timestamp
		*/
		void getTimeStamp(char pBuffer[64]) {
			//Get the current time
			std::time_t gTime = std::time(nullptr);

			//Get the local time
			std::tm lTime;
			localtime_s(&lTime, &gTime);

			//Place the time in the buffer
			strftime(pBuffer, 64, "%c\n", &lTime);
		}
	}
}