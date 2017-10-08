#pragma once

//! Allow for string streams in order to construct the log messages
#include <sstream>

//! Include the required SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"
#include "DebugColor.hpp"

namespace SDL2_Engine {
	//! Prototype the Initialiser
	namespace Initialisation { struct LoggerInitialiser; }

	namespace Debug {
		/*
		 *		Name: Logger
		 *		Author: Mitchell Croft
		 *		Created: 20/07/2017
		 *		Modified: 06/10/2017
		 *		
		 *		Purpose:
		 *		Allow for the logging of text to the output window and/or
		 *		an external log file
		**/
		class SDL2_LIB_INC Logger : public Utilities::IGlobal {
		public:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Output Logging Functions-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Logger : log - Compile a log message from an indiscriminate amount of arguments
				Created: 20/07/2017
				Modified: 20/07/2017

				param[in] pArgs - A parameter pack of values to string-ify
			*/
			template<typename ... TArgs>
			void log(const TArgs& ... pArgs) const {
				//Create a stream to hold the constructed string
				std::wostringstream o;

				//Build the string from the values
				buildString(o, pArgs...);

				//Send the message
				outputMessage(DebugColor::WHITE, o.str().c_str());
			}

			/*
				Logger : logFormatted - Log a formatted string to the defined output locations
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pFormat - A c-string that contains the text and formatting data to be output
				param[in] ... - The additional arguments to be inserted into the formatted string

				Note:
				See http://www.cplusplus.com/reference/cstdio/printf/ for formatting options
			*/
			void logFormatted(const char* pFormat, ...) const;

			/*
				Logger : logFormatted - Log a formatted string to the defined output locations in a specific color
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pColor - The color to output the text in (Only applies to the standard output window)
				param[in] pFormat - A c-string that contains the text and formatting data to be output
				param[in] ... - The additional arguments to be inserted into the formatted string
			*/
			void logFormatted(const DebugColor& pColor, const char* pFormat, ...) const;

			/*
				Logger : logWarning - Log a formatted warning string to the defined output locations
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pFormat - A c-string that contains the text and formatting data to be output
				param[in] ... - The additional arguments to be inserted into the formatted string
			*/
			void logWarning(const char* pFormat, ...) const;

			/*
				Logger : logError - Log a formatted error string to the defined output locations
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pFormat - A c-string that contains the text and formatting data to be output
				param[in] ... - The additional arguments to be inserted into the formatted string
			*/
			void logError(const char* pFormat, ...) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Behavior Modifiers--------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			/*
				Logger : isLoggingOutput - Get the current logging state of the Logger
				Created: 20/07/2017
				Modified: 20/07/2017

				return const bool& - Returns true if the Logger is logging to an external file
			*/
			inline const bool& isLoggingOutput() const noexcept { return mLogOutput; }

			/*
				Logger : setLoggingOutput - Set whether the the Logger will output to an external file
				Created: 20/07/2017
				Modified: 20/07/2017

				param[in] pState - A boolean value indicating the new logging state
			*/
			inline void setLoggingOutput(const bool& pState) noexcept { mLogOutput = pState; }

			/*
				Logger : getOutputLocation - Get the folder location where the log file is being generated
				Created: 20/07/2017
				Modified: 05/10/2017

				return const std::string& - Returns the location as a string
			*/
			inline const std::string& getOutputLocation() const noexcept { return *mLogLocation; }

			/*
				Logger : setOutputLocation - Set the folder location where the log file will be created
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pLocation - A string indicating the file location to store the file
			*/
			void setOutputLocation(const char* pLocation);

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Construction/Destruction-----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Logger : Constructor - Initialise with default values
				Created: 20/07/2017
				Modified: 06/10/2017

				param[in] pSetup - Defines how the Logger should be setup
			*/
			Logger(const Initialisation::LoggerInitialiser& pSetup);

			/*
				Logger : createInterface - Empty function
				Created: 20/07/2017
				Modified: 06/10/2017

				return bool - Returns true if the Logger was created successfully
			*/
			inline bool createInterface() override { return true; }

			/*
				Logger : destroyInterface - Deallocate the memory used by the logger values
				Created: 20/07/2017
				Modified: 20/07/2017
			*/
			void destroyInterface() override;

			/*
				Logger : update - Empty function, required to instantiate instances
				Created: 22/09/2017
				Modified: 22/09/2017
			*/
			inline void update() override {}

		private:
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-------------------------------------Behavior Values---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//! Flag if the output should be sent to a file
			bool mLogOutput;

			//! Store the location to output the log file
			std::string* mLogLocation;

			
			/*
				Logger : outputMessage - Uniform messaging function used to relay the text to the destinations
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pDebugColor - The color value that is to be used when outputting to the standard out
				param[in] pMessage - The text to be output to the locations
			*/
			void outputMessage(const DebugColor& pColor, const char* pMessage) const;

			/*
				Logger : outputToSTDOutput - Output the message to the standard output
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pColor - The color to output the message in
				param[in] pTime - The timestamp for when the event occurred
				param[in] pMessage - The text to be output to the output
			*/
			void outputToSTDOutput(const DebugColor& pColor, const char* pTime, const char* pMessage) const;

			/*
				Logger : outputToFile - Output the message to the specified output file
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pTime - The timestamp for when the event occurred
				param[in] pMessage - The text to be output to the file
			*/
			void outputToFile(const char* pTime, const char* pMessage) const;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Parameter Pack Extraction----------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			/*
				Logger : buildString - Construct a string of text based on an indiscriminate number of values
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pStream - The stream that collates the text information
				param[in] pVal - The next value to be processed
				param[in] pArgs - The remaining values to be processed
			*/
			template<typename T, typename ... TArgs>
			void buildString(std::ostream& pStream, const T& pVal, const TArgs& ... pArgs) const {
				//Place the next value in the stream
				pStream << pVal;

				//Recurse for the rest of the values
				buildString(pStream, pArgs...);
			}

			/*
				Logger : buildString - Finish the variadic template unpacking process
				Created: 20/07/2017
				Modified: 05/10/2017

				param[in] pStream - The stream that collates the text information
			*/
			inline void buildString(std::ostream& pStream) const noexcept {}
		};
	}
}