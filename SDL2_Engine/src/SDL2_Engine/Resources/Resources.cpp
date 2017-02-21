#include "Resources.hpp"

//! Included to use variable argument lists
#include <cstdarg>

//! Included to access the folder search
#include <dirent.h>

//! Included to access the SDL loading functions
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>

//! Included to be able to parse cursor JSON files
#include "../../jsoncpp/json/json.h"

//! Included to be able to construct and setup Cursors
#include "ResourceTypes/Descriptors/Cursor.hpp"

namespace SDL2_Engine {
	namespace Resources {
		//! Define static
		Resources* Resources::mInstance = nullptr;

		/*
			simpleOutput - Provide a very basic, simple output to be used in the event that
						   no DebugCallbacks are supplied during initialisation
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pFormat - A c-string with the format symbols defined in
							http://www.cplusplus.com/reference/cstdio/fprintf/
			param[in] Additional Values - The values to be formatted into the c-string
		*/
		inline void simpleOutput(const char* pFormat, ...) { 
			va_list args;
			va_start(args, pFormat);
			vprintf((std::string(pFormat) + '\n').c_str(), args);
			va_end(args);
		}

		/*
			Resource : Constructor - Initialise with default values
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017
		*/
		Resources::Resources() :
			mRenderReference(nullptr),
			mWarningCB(nullptr),
			mErrorCB(nullptr)
		{}

		/*
			Resources : recureResources - Traverse the resources folder and find all files
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pFilepath - A std::string object containing the filepath to search
			param[in] pToFill - A std::vector of std::string objects, storing the different 
								filepaths that were found
		*/
		void Resources::recurseResources(std::string pFilepath, std::vector<std::string>& pToFill) const  {
			//Store the different found file names
			std::string fileName = "";

			//Store a pointer to the current directory
			DIR* directory = nullptr;

			//Store a pointer to the current file in use
			struct dirent* dirp = nullptr;

			//Attempt to open the directory
			directory = opendir(pFilepath.c_str());

			//Check the file opened successfully
			if (!directory) {
				mWarningCB("The Resources Manager was unable to open the directory '%s' while scanning for resources", pFilepath.c_str());
				return;
			}

			//Loop while the directory can be searched
			while ((dirp = readdir(directory))) {
				//Get the current items name
				fileName = dirp->d_name;

				//Manage found directories
				if (dirp->d_type == DT_DIR) {
					//Skip the navigation directories ("." && "..")
					if (fileName == "." || fileName == "..") continue;

					//Recurse into found directories
					else { recurseResources(pFilepath + fileName + "\\", pToFill); continue; }
				}

				//If the resource is a file add it to the vector
				else if (dirp->d_type == DT_REG) pToFill.push_back(pFilepath + fileName);
			}

			//Close the directory
			closedir(directory);
		}

		/*
			Resources : loadResource - Load a resource based on its type and ID
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			NOTE: SDL2 (Main, Image and Audio) must be initialised before this function is called
		*/
		void Resources::loadResource(std::shared_ptr<ResourceTypes::ResourceBase> pResource) {
			//Get the file that is to be loaded
			const std::string& TO_LOAD = mResourcesDir + mResourceOrigin[pResource->mID];

			//Check the file exists
			if (mFileLookup.find(TO_LOAD) == mFileLookup.end()) {
				pResource->mStatus = EResourceLoadStatus::Error;
				mErrorCB("'%s' could not be found for loading. Try calling Resources::scanResources() to re-scan the '%s' directory", TO_LOAD.c_str(), mResourcesDir.c_str());
				return;
			}

			//Switch based on the type of the resource
			switch (pResource->mType) {
				#pragma region Texture
			case EResourceType::Texture: {
				//Cast the resource to the Texture Resource type
				ResourceTypes::__SDL2Resource<Texture>* res = (ResourceTypes::__SDL2Resource<Texture>*)pResource.get();

				//Attempt to load the resource
				SDL_Texture* temp = IMG_LoadTexture((SDL_Renderer*)mRenderReference, TO_LOAD.c_str());

				//Check the texture was loaded properly
				if (!temp) {
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to open the file '%s' as a Texture resource (ID: %i). ERROR: %s", TO_LOAD.c_str(), res->mID, IMG_GetError());
					return;
				}

				//Set the blend mode for the texture
				if (res->blendMode && SDL_SetTextureBlendMode(temp, (SDL_BlendMode)res->blendMode)) {
					//Output the error message
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to apply the blend mode to the texture resource '%s' (ID: %i, Blend Mode: %i). ERROR: %s", TO_LOAD.c_str(), res->mID, res->blendMode, SDL_GetError());

					//Destroy the created texture
					SDL_DestroyTexture(temp);

					//Exit the function
					return;
				}

				//Assign the SDL_Texture to the resource
				res->mTexture = temp;

				break; }
				#pragma endregion

				#pragma region SFX
			case EResourceType::SFX: {
				//Cast the resource to the SFX Resource type
				ResourceTypes::__SDL2Resource<SFX>* res = (ResourceTypes::__SDL2Resource<SFX>*)pResource.get();

				//Attempt to load the resource
				Mix_Chunk* temp = Mix_LoadWAV(TO_LOAD.c_str());

				//Check the file loaded properly
				if (!temp) {
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to open the file '%s' as a SFX resource (ID: %i). ERROR: %s", TO_LOAD.c_str(), res->mID, Mix_GetError());
					return;
				}

				//Assign the Mix_Chunk to the resource
				res->mSFX = temp;

				break; }
				#pragma endregion

				#pragma region Music
			case EResourceType::Music: {
				//Cast the resource to the Music Resource type
				ResourceTypes::__SDL2Resource<Music>* res = (ResourceTypes::__SDL2Resource<Music>*)pResource.get();

				//Attempt to load the resource
				Mix_Music* temp = Mix_LoadMUS(TO_LOAD.c_str());

				//Check the file loaded properly
				if (!temp) {
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to open the file '%s' as a Music resource (ID: %i). ERROR: %s", TO_LOAD.c_str(), res->mID, Mix_GetError());
					return;
				}

				//Assign the Mix_Music to the resource
				res->mMusic = temp;

				break; }
				#pragma endregion

				#pragma region Cursor
			case EResourceType::Cursor: {
				//Cast the resource to the Cursor Resource type
				ResourceTypes::__SDL2Resource<Cursor>* res = (ResourceTypes::__SDL2Resource<Cursor>*)pResource.get();

				#pragma region Read the File
				//Attempt to open the file
				FILE* file;
				fopen_s(&file, TO_LOAD.c_str(), "rb");

				//Check the file was opened
				if (!file) {
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to open the file '%s' for parsing. Could not load the Cursor Resource. ID: %i", TO_LOAD.c_str(), res->mID);
					return;
				}

				//Seek to the end of the file
				fseek(file, 0, SEEK_END);

				//Get the size of the file
				uint size = ftell(file);

				//Seek back to the start of the file
				fseek(file, 0, SEEK_SET);

				//Create a char array to hold the JSON information
				char* data = new char[size + 1];

				//Read in the JSON file
				fread(data, sizeof(char), size, file);

				//Close the file
				fclose(file);

				//Null terminate the string
				data[size] = 0;
				#pragma endregion

				#pragma region Parse the JSON
				//Create the JSON objects to parse the string
				Json::Value root;
				Json::Reader reader;

				//Attempt to parse the string
				if (!reader.parse(data, root)) {
					//Setup error message
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to parse the JSON Cursor Resource file '%s' (ID: %i). ERROR: %s", TO_LOAD.c_str(), res->mID, reader.getFormattedErrorMessages().c_str());

					//Delete the data 
					delete[] data;

					return;
				}

				//Delete the data 
				delete[] data;
				#pragma endregion

				//Create a vector to store the frame information in
				std::vector<SDL_Rect> framesPos;

				#pragma region Validate JSON Values
				{
					//Define a simple struct to contain the validation checking information
					struct Validate { std::string value; std::string type; std::function<bool(const Json::Value&)> testFunc; };

					//Create an array of required value types
					const Validate REQUIRED_VALUES[] = {
						{"src", "string", [](const Json::Value& pValue) { return pValue.isString(); }},
						{"frameWidth", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } },
						{"frameHeight", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } },
						{"fps", "decimal", [](const Json::Value& pValue) { return pValue.isDouble(); } },
						{"frames", "object array", [](const Json::Value& pValue) { return pValue.isArray(); } }
					};

					//Loop through all required values to check they are present
					for (const auto& VAL : REQUIRED_VALUES) {
						//Ensure the required information is available in the JSON file
						if (!root.isMember(VAL.value) || !VAL.testFunc(root[VAL.value])) {
							res->mStatus = EResourceLoadStatus::Error;
							mErrorCB("Failed to load Cursor JSON Resource '%s' as it is needs the value '%s' to be a '%s'. Resource ID: %i", TO_LOAD.c_str(), VAL.value.c_str(), VAL.type.c_str(), res->mID);
							return;
						}
					}

					//Check every frame objects lists an 'x' and 'y' attribute
					const Validate FRAME_REQUIRE[] = {
						{ "x", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } },
						{ "y", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } },
						{ "w", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } },
						{ "h", "uint", [](const Json::Value& pValue) { return pValue.isUInt(); } }
					};

					//Loop through the frames array
					for (const auto& frame : root["frames"]) {
						//Loop through the frame requirements
						for (const auto& VAL : FRAME_REQUIRE) {
							if (!frame.isMember(VAL.value) || !VAL.testFunc(frame[VAL.value])) {
								res->mStatus = EResourceLoadStatus::Error;
								mErrorCB("Failed to load Cursor JSON resource '%s' as it requires the frame list of objects to have the value '%s' to be a '%s'. Resource ID: %i", TO_LOAD.c_str(), VAL.value.c_str(), VAL.type.c_str(), res->mID);
								return;
							}
						}

						//Save the frame position information
						framesPos.push_back({ frame["x"].asInt(), frame["y"].asInt(), frame["w"].asInt(), frame["h"].asInt() });
					};
				}
				#pragma endregion

				//Create the Cursor_Container object to store the data in
				ResourceTypes::Descriptors::Cursor* cursor = new ResourceTypes::Descriptors::Cursor();

				//Store all important JSON information in the cursor object
				cursor->totalFrames = framesPos.size();
				cursor->fps = root["fps"].asFloat();

				//Get the directory of the loading file
				const std::string SRC_IMAGE = TO_LOAD.substr(0, TO_LOAD.find_last_of("/\\") + 1) + root["src"].asCString();

				#pragma region Load SDL Cursor Values
				//Load the source image as a surface
				SDL_Surface* srcSurface = IMG_Load(SRC_IMAGE.c_str());

				//Test the surface was created
				if (!srcSurface) {
					//Output the error message
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to load the source image (Location defined as '%s') for the Cursor Resource '%s' (ID: %i). ERROR: %s", SRC_IMAGE.c_str(), TO_LOAD.c_str(), res->mID, IMG_GetError());

					//Delete the allocated cursor
					delete cursor;

					return;
				}

				//CREATE SURFACE MASKS
				Uint32 rmask, gmask, bmask, amask;
				/* SDL interprets each pixel as a 32-bit number, so our masks must depend
				on the endianness (byte order) of the machine */
				#if SDL_BYTEORDER == SDL_BIG_ENDIAN
				rmask = 0x000000ff;
				gmask = 0x0000ff00;
				bmask = 0x00ff0000;
				amask = 0xff000000;
				#else
				rmask = 0xff000000;
				gmask = 0x00ff0000;
				bmask = 0x0000ff00;
				amask = 0x000000ff;
				#endif

				//Store the scaled frame dimensions
				const int SCL_WID = (int)(root["frameWidth"].asFloat() * res->scale);
				const int SCL_HEI = (int)(root["frameHeight"].asFloat() * res->scale);

				//Store the calculated 'hot' zone for the cursor
				const int HOT_X = (int)(SCL_WID * res->xHot);
				const int HOT_Y = (int)(SCL_HEI * res->yHot);

				//Loop through all the frames
				for (uint i = 0; i < cursor->totalFrames; i++) {
					//Create a surface to transplant the image to
					SDL_Surface* toSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, SCL_WID, SCL_HEI, 32, rmask, gmask, bmask, amask);

					//Check surface loaded
					if (!toSurface) {						   
						//Output the error message
						res->mStatus = EResourceLoadStatus::Error;
						mErrorCB("Failed to create the Cursor Resource '%s' ID: %i. Failed to allocate SDL_Surface for frame %i based on the source image '%s'. ERROR: %s", TO_LOAD.c_str(), res->mID, i, SRC_IMAGE.c_str(), IMG_GetError());

						//Clear allocated memory
						for (auto& c : cursor->frameList) SDL_FreeCursor(c);
						SDL_FreeSurface(srcSurface);
						delete cursor;
						return;
					}

					//Setup the transfer regions
					SDL_Rect to = { 0, 0, SCL_WID, SCL_HEI };

					//Copy the image information for the source to the toSurface
					SDL_BlitSurface(srcSurface, &framesPos[i], toSurface, &to);

					//Setup the new SDL_Cursor object
					SDL_Cursor* tempCursor = SDL_CreateColorCursor(toSurface, HOT_X, HOT_Y);

					//Check the cursor loaded
					if (!tempCursor) {
						//Output the error message
						res->mStatus = EResourceLoadStatus::Error;
						mErrorCB("Failed to create the Cursor Resource '%s' ID: %i. Failed to allocate SDL_Cursor for frame %i based on the source image '%s'. ERROR: %s", TO_LOAD.c_str(), res->mID, i, SRC_IMAGE.c_str(), IMG_GetError());

						//Deallocate SDL objects
						for (auto& c : cursor->frameList) SDL_FreeCursor(c);
						SDL_FreeSurface(toSurface);
						SDL_FreeSurface(srcSurface);
						delete cursor;
						return;
					}

					//Add the new cursor frame to the Cursor container object
					cursor->frameList.push_back(tempCursor);

					//Free the toSurface
					SDL_FreeSurface(toSurface);
				}

				//Free the image source surface
				SDL_FreeSurface(srcSurface);
				#pragma endregion

				//Give the Cursor Container to the resource
				res->mCursor = cursor;
				break; }
				#pragma endregion

				#pragma region Font
			case EResourceType::Font:
				break;
				#pragma endregion

				#pragma region Generic
			case EResourceType::Generic: {
				//Cast the resource to the Generic Resource type
				ResourceTypes::__SDL2Resource<Generic>* res = (ResourceTypes::__SDL2Resource<Generic>*)pResource.get();

				//Attempt to open the file
				FILE* file;
				fopen_s(&file, TO_LOAD.c_str(), "rb");

				//Check the file was opened
				if (!file) {
					res->mStatus = EResourceLoadStatus::Error;
					mErrorCB("Failed to open the file '%s' as a Generic resource. ID: %i", TO_LOAD.c_str(), res->mID);
					return;
				}

				//Seek to the end of the file
				fseek(file, 0, SEEK_END);

				//Get the size of the file
				uint size = ftell(file);

				//Seek back to the start of the file
				fseek(file, 0, SEEK_SET);

				//Create the resource data array
				res->mData = new unsigned char[size + 1];

				//Read in the file data
				fread(res->mData, sizeof(unsigned char), size, file);

				//Close the file
				fclose(file);

				//Null terminate the data
				res->mData[size] = 0;

				//Store the amount of data read in
				res->mSize = size;
				break; }
				#pragma endregion
			default:
				mErrorCB("Attempted to load a resource with an invalid type. ID: %i Type: %i", pResource->mID, pResource->mType);
				return;
			}

			//Update the resources load state
			pResource->mStatus = EResourceLoadStatus::Loaded;
		}

		/*
			Resources : init - Startup the Resources singleton, creating the instance
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pDirectory - A c-string defining the directory that will be used as the Resources
								   folder
			param[in] pRenderer - A reference to the SDL_Renderer object that the Resource Manager
								  uses to create textures
			param[in] pWarningCB - A debug callback function pointer that allows for the output 
								   of warning formatted strings	(Default nullptr uses printf)
			param[in] pErrorCB - A debug callback function pointer that allows for the output
								 of error formatted strings (Default nullptr uses pWarningCB)

			return bool - Returns true if the Resource Manager was initialised successfully
		*/
		bool Resources::init(const char* pDirectory, const SDL_Renderer* pRenderer, DebugCallback pWarningCB /* = nullptr */, DebugCallback pErrorCB /* = nullptr */) {
			//Check the debug outputs are valid
			if (!pWarningCB) pWarningCB = simpleOutput;
			if (!pErrorCB) pErrorCB = pWarningCB;

			//Check the singleton hasn't already been created
			if (mInstance) {
				//Output warning
				mInstance->mWarningCB("Attempted to re-create the SDL2_Engine::Resources singleton after it has been initialised");

				//Return failure
				return false;
			}

			//Check the directory isn't null
			if (!pDirectory) {
				//Output error 
				pErrorCB("Attempted to create the SDL2_Engine::Resouces singleton without supplying a directory to pull resources from. This is required to load resources");

				//Return failure
				return false;
			}

			//Check the renderer is present
			if (!pRenderer) {
				//Output error
				pErrorCB("Attempted to create the SDL2_Engine::Resources singleton without supplying a valid SDL_Renderer. This is required to load texture resources");

				//Return failure
				return false;
			}

			//Create the Resource singleton instance
			if (!(mInstance = new Resources())) {
				//Output error message
				pErrorCB("Failed to create the SDL2_Engine::Resources singleton");
				
				//Return failed
				return false;
			}

			//Assign the working directory
			mInstance->mResourcesDir = pDirectory;

			//Ensure the directory is directory ended
			if (mInstance->mResourcesDir.find_last_of("/\\") != (int)mInstance->mResourcesDir.length() - 1)
				mInstance->mResourcesDir += '\\';

			//Assign the SDL_Renderer reference
			mInstance->mRenderReference = pRenderer;

			//Find all initial resources 
			scanResources();

			//Assign the debug output values
			mInstance->mWarningCB = pWarningCB;
			mInstance->mErrorCB = pErrorCB;

			//Return success
			return true;
		}

		/*
			Resources : destroy - Shutdown the Resource Manager and clear all loaded memory
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017
		*/
		void Resources::destroy() {
			//Check the singleton has been created
			if (mInstance) {
				//Loop through and unload all loaded resources
				for (auto& i = mInstance->mResourceLookup.cbegin(); i != mInstance->mResourceLookup.cend(); i++) {
					//Free the resource memory
					i->second->freeMemory();

					//Check that the resource is now unique and will be destroyed
					if (!i->second.unique())
						mInstance->mWarningCB("Resource (ID: %i) was freed, however a reference to this resource still exists", i->first);
				}

				//Clear the loaded resources map
				mInstance->mResourceLookup.clear();

				//Delete the singleton
				delete mInstance;

				//Reset the instance pointer
				mInstance = nullptr;
			}
		}

		/*
			Resources : update - Load and destroy resources as the situation arises
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017
		*/
		void Resources::update() {
			//Lock the resources
			if (!mInstance->mLock.try_lock()) return;

			try {
				//Loop through all current resources
				for (auto it = mInstance->mResourceLookup.cbegin(); it != mInstance->mResourceLookup.cend();) {
					//Check if resource can be removed
					if (it->second.unique() || (int)it->second->mStatus & ((int)EResourceLoadStatus::Error | (int)EResourceLoadStatus::Freed)) {
						//Look for the resource origin lookup key
						auto origin = mInstance->mResourceOrigin.find(it->second->mID);

						//If the value was found remove the entry
						if (origin != mInstance->mResourceOrigin.end())
							mInstance->mResourceOrigin.erase(origin);
						
						mInstance->mResourceLookup.erase(it++);

						//Continue on the loop
						continue;
					}

					//Check if resource has been loaded
					if (it->second->mStatus == EResourceLoadStatus::Unloaded)
						mInstance->loadResource(it->second);

					//Increase the iterator
					++it;
				}
			} catch (const std::exception& pExc) {
				mInstance->mErrorCB("An error occurred within the SDL2_Engine::Resource Manager during a call to the update function. ERROR: %s", pExc.what());
			} catch (const std::string& pExc) {
				mInstance->mErrorCB("An error occurred within the SDL2_Engine::Resource Manager during a call to the update function. ERROR: %s", pExc.c_str());
			} catch (...) {
				mInstance->mErrorCB("An unknown error occurred within the SDL2_Engine::Resource Manager during a call to the update function.");
			}

			//Unlock the resources
			mInstance->mLock.unlock();
		}

		/*
			Resources : scanResources - Clear the currently identified resources and re-scan the resources directory
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017
		*/
		void Resources::scanResources() {
			//Lock down the files
			std::lock_guard<std::recursive_mutex> guard(mInstance->mLock);

			//Clear the previously found resources
			mInstance->mFileLookup.clear();

			//Create a vector to store all the found files in
			std::vector<std::string> foundFiles;

			//Start the file scan
			mInstance->recurseResources(mInstance->mResourcesDir.c_str(), foundFiles);

			//Loop through all found files and add them to the lookup directory
			for (auto& file : foundFiles) {
				//Get the index location of the first directory separator
				uint index = file.find_first_of("/\\");

				//Isolate the resources folder relative path
				std::string relativePath = file.substr(index + 1);

				//Insert the value into the map
				mInstance->mFileLookup.insert(std::pair<std::string, std::string>(relativePath, relativePath));
			}
		}

		/*
			Resources : idToFilepath - Get the filepath of a resource given its ID value
			Author: Mitchell Croft
			Created: 03/02/2017
			Modified: 03/02/2017

			param[in] pID - The resourceID value of the resource to lookup

			return std::string - Returns the filepath contained in a standard string object
		*/
		std::string Resources::idToFilepath(const resourceID& pID) {
			//Search for the ID in the map
			auto it = mInstance->mResourceOrigin.find(pID);

			//Return the results
			return (it != mInstance->mResourceOrigin.end() ? mInstance->mResourcesDir + mInstance->mResourceOrigin[pID] : "");
		}
	}
}