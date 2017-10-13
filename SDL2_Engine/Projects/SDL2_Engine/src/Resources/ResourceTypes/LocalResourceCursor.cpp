#include "LocalResourceCursor.hpp"

//! Include logging and Math capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"
#include "../../Math.hpp"
#include "../../Utilities/Endian/Endian.hpp"

//! Include the SDL2 definitions
#include <SDL.h>
#include <SDL_image.h>

//! Include the Objx Reader
#include <Objx_document.hpp>
#include <Objx_reader.hpp>
using namespace Objx;

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			deductLocation - Deduct the location of relative resources, stripping superfluous characters
			Author: Mitchell Croft
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pParent - The original string location of a file
			param[in] pRelative - The file path of the second file, relative to pParent

			return std::string - Returns the clean filepath as a new std::string object
		*/
		std::string deductLocation(const std::string pParent, const std::string pRelative) {
			//Check if the pRelative is actually relative
			if (pRelative.find(':') != pRelative.npos) return pRelative;

			//Store a std::string of the current filepath
			std::string current = "";

			//Optomise the parent std::string
			int index = -1;
			size_t prog = 0;
			while (true) {
				//Grab the next std::string section
				std::string sub = pParent.substr(prog, (index = pParent.find_first_of("/\\", index + 1)) + 1 - prog);

				//Check there was text extracted
				if (index == pParent.npos) break;

				//Check if its the current directory shorthand
				else if (sub == "./" || sub == ".\\") continue;

				//Check if its the directory up shorthand
				else if (sub == "../" || sub == "..\\")
					current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

				//Otherwise concatenate
				else current += sub;

				//Increase the progress
				prog += sub.length();
			}

			//Optomise the relative std::string
			index = -1;
			prog = 0;
			while (true) {
				//Grab the next std::string section
				std::string sub = pRelative.substr(prog, (index = pRelative.find_first_of("/\\", index + 1)) + 1 - prog);

				//Check if its the current directory shorthand
				if (sub == "./" || sub == ".\\") continue;

				//Check if its the directory up shorthand
				else if (sub == "../" || sub == "..\\")
					current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

				//Otherwise concatenate
				else current += sub;

				//Check there was text extracted
				if (index == pRelative.npos) break;

				//Increase the progress
				prog += sub.length();
			}

			//Return the combination
			return current;
		}

		/*
			LocalResource (Cursor) : dispose - Unload resource information
			Created: 05/10/2017
			Modified: 05/10/2017
		*/
		void __LocalResource<Cursor>::dispose() {
			//Check the cursors exist
			if (mFrames) {
				//Loop through and unload all frames
				for (size_t i = 0; i < mFrameCount; i++)
					if (mFrames[i]) SDL_FreeCursor(mFrames[i]);

				//Delete the Frames array
				delete[] mFrames;

				//Reset the pointer
				mFrames = nullptr;

				//Flag unloaded
				mStatus = EResourceLoadStatus::Freed;
			}
		}

		/*
			LocalResource (Cursor) : Constructor - Initialise with default values
			Created: 05/10/2017
			Modified: 13/10/2017

			param[in] pPath - The path of the Objx file to load
		*/
		__LocalResource<Cursor>::__LocalResource(const char* pPath) : ILocalResourceBase(EResourceType::Cursor), mFrames(nullptr), mFrameCount(0), mFPS(0.f) {
			//Get a reference to the Logger object
			const Debug::Logger& log = Globals::get<Debug::Logger>();

			#pragma region Open and Read File Data
			//Attempt to open the file
			FILE* file;
			fopen_s(&file, pPath, "rb");

			//Check the file was opened
			if (!file) {
				//Create a character buffer to store error information in
				char buffer[512] = { '\0' };

				//Get the error information
				strerror_s(buffer, errno);

				//Output error message
				log.logError("Local Resource (Cursor) failed to open the file '%s'. Error: %s", pPath, buffer);

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
				return;
			}

			//Seek to the end of the file
			fseek(file, 0, SEEK_END);

			//Get the size of the file
			const size_t SIZE = ftell(file);

			//Seek back to the start of the file
			fseek(file, 0, SEEK_SET);

			//Create the resource's data array
			char* data = new char[SIZE + 1];

			//Read in the field data
			fread(data, sizeof(char), SIZE, file);

			//Close the file
			fclose(file);

			//Null terminate the stream
			data[SIZE] = '\0';
			#pragma endregion

			#pragma region Parse Read Data
			//Flag if an error occurred during the parsing process
			bool parsingError = false;

			//Create the Objx parser object
			Reader reader([&](const EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
				//Output the error header
				if (!parsingError) log.logError("Local Resource (Cursor) encountered error(s) parsing '%s':", pPath);

				//Output the error information
				log.logError("Line: %zu\tColumn: %zu\t\t%s (%s)", pLine, pColumn, pMsg, statusCodeToString(pCode));

				//Flag an error for the continuing process
				parsingError = true;
			}, 2);

			//Parse the data
			const Document DOC = reader.cparse(data);

			//Delete the file data
			delete[] data;

			//If an error occurred
			if (parsingError) {
				//Flag error status
				mStatus = EResourceLoadStatus::Error;
				return;
			}
			#pragma endregion

			#pragma region Create Cursor Frames
			//Get the first Objx in the document
			const xobjx OBJ = (*DOC.cbegin());

			//Check how many definitions are in the document
			if (DOC.size() != 1) {
				//If there is more then 1
				if (DOC.size()) {
					//Loop through all elements and display warning
					DOC.forEach([&](const xstring& pName, const Node& pNode) {
						//Output the name of the Objx being used
						log.logWarning("Multiple Objx's were found in '%s', using '%s' to generate the Cursor", pPath, pName.c_str());

						//Exit the loop
						return false;
					});
				}

				//If there are none
				else {
					//Output the error
					log.logError("Local Resource (Cursor) could not be loaded as the Objx file '%s' was empty", pPath);

					//Flag error
					mStatus = EResourceLoadStatus::Error;
					return;
				}
			}

			//Get the Math object
			const Math& math = Globals::get<Math>();

			//Retrieve the FPS
			mFPS = (float)math.largest(OBJ["fps"].readVal<xdec>(), 0.001);

			//Convert FPS to inverse
			mFPS = (1.f / mFPS);

			//Get the cursor dimensions
			const int CURSOR_W = math.largest(OBJ["frameWidth"].readVal<xint>(1), 1);
			const int CURSOR_H = math.largest(OBJ["frameHeight"].readVal<xint>(1), 1);

			//Read shared information
			const int HOTX = (int)(math.clamp01(OBJ["hotX"].readVal<xdec>()) * (xdec)CURSOR_W);
			const int HOTY = (int)(math.clamp01(OBJ["hotY"].readVal<xdec>()) * (xdec)CURSOR_H);

			//Setup RGBA masks for the endian order of the machine
			Uint32 rmask, gmask, bmask, amask;
			if (Utilities::Endian::getEndianOrder() == Utilities::Endian::EEndianOrder::Big) {
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
			} else {
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
			}

			//Convert the Objx filepath to a string
			const std::string FILE_PATH = pPath;

			//Get the directory of the source image file
			const std::string SRC_LOC = deductLocation(pPath, OBJ["source"].readVal<xstring>());
			
			//Load the spritesheet/source image
			SDL_Surface* spritesheetSrc = IMG_Load(SRC_LOC.c_str());

			//Check the surface was loaded was correctly
			if (!spritesheetSrc) {
				//Output error message
				log.logError("Failed to load the image described in the file '%s'. Interpreted image filepath was '%s'. Error: %i", pPath, SRC_LOC.c_str(), SDL_GetError());

				//Store error status
				mStatus = EResourceLoadStatus::Error;
				return;
			}

			//Get the array of frames to generate
			xobjx frames = OBJ["frames"];

			//Check there are frames to load
			if (!frames.isType<xobjx*>()) {
				//Create an all encompassing Objx for the surface
				xobjx frame;
				frame.writeProp("x", 0);
				frame.writeProp("y", 0);
				frame.writeProp("w", spritesheetSrc->w);
				frame.writeProp("h", spritesheetSrc->h);

				//Write the frame to the frames list
				frames.writeArray(&frame, 1);
			}

			//Store the number of frames in the animation
			mFrameCount = frames.size();

			//Create the array of Cursors to hold the final array
			mFrames = new SDL_Cursor*[mFrameCount];

			//Loop through the frames 
			for (size_t i = 0; i < mFrameCount; i++) {
				//Create a surface to transplant the frame to
				SDL_Surface* toSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, CURSOR_W, CURSOR_H, 32, rmask, gmask, bmask, amask);

				//Check the surface was loaded
				if (!toSurface) {
					//Output the error message
					log.logError("Local Resource (Cursor) failed to create surface %zu to transplant frame from image '%i'. Error: %s", i, SRC_LOC.c_str(), SDL_GetError());

					//Clear allocated memory
					for (size_t j = 0; j < i; j++)
						SDL_FreeCursor(mFrames[j]);
					delete[] mFrames;
					mFrames = nullptr;
					mFrameCount = 0;
					SDL_FreeSurface(spritesheetSrc);

					//Flag error
					mStatus = EResourceLoadStatus::Error;
					return;
				}

				//Get the frame for this iteration
				xobjx frame = frames.readArray<xobjx>(i);

				//Setup the transfer regions
				SDL_Rect to = { 0, 0, CURSOR_W, CURSOR_H };
				SDL_Rect from = { math.largest(0, frame["x"].readVal<xint>()),
								  math.largest(0, frame["y"].readVal<xint>()),
								  math.largest(0, frame["w"].readVal<xint>()),
								  math.largest(0, frame["h"].readVal<xint>())
				};

				//Copy the image information from the source to the new surface
				if (SDL_BlitSurface(spritesheetSrc, &from, toSurface, &to)) {
					//Output the error message
					log.logError("Local Resource (Cursor) failed to transplant frame %zu image data from '%i' to an isolated surface. Error: %s", i, SRC_LOC.c_str(), SDL_GetError());

					//Clear allocated memory
					for (size_t j = 0; j < i; j++)
						SDL_FreeCursor(mFrames[j]);
					delete[] mFrames;
					mFrames = nullptr;
					mFrameCount = 0;
					SDL_FreeSurface(spritesheetSrc);
					SDL_FreeSurface(toSurface);

					//Flag error
					mStatus = EResourceLoadStatus::Error;
					return;
				}

				//Setup the new SDL_Cursor object
				mFrames[i] = SDL_CreateColorCursor(toSurface, HOTX, HOTY);

				//Check the cursor was created 
				if (!mFrames[i]) {
					//Output the error message
					log.logError("Local Resource (Cursor) failed to create Cursor frame %zu from image data from '%i'. Error: %s", i, SRC_LOC.c_str(), SDL_GetError());

					//Clear allocated memory
					for (size_t j = 0; j < i; j++)
						SDL_FreeCursor(mFrames[j]);
					delete[] mFrames;
					mFrames = nullptr;
					mFrameCount = 0;
					SDL_FreeSurface(spritesheetSrc);
					SDL_FreeSurface(toSurface);

					//Flag error
					mStatus = EResourceLoadStatus::Error;
					return;
				}
			}
			#pragma endregion

			//Flag loaded
			mStatus = EResourceLoadStatus::Loaded;
		}
	}
}