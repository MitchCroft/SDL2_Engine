#include "LocalResourceGeneric.hpp"

//! Include logging capabilities
#include "../../Globals.hpp"
#include "../../Debug/Logger.hpp"

namespace SDL2_Engine {
	namespace ResourceTypes {
		/*
			LocalResource (Generic) : dispose - Unload resource information
			Created: 04/10/2017
			Modified: 04/10/2017
		*/
		void __LocalResource<Generic>::dispose() {
			//Check there is data to delete
			if (mData) {
				//Delete the data
				delete[] mData;

				//Reset the values
				mData = nullptr;
				mSize = 0;

				//Set the status flag
				mStatus = EResourceLoadStatus::Unloaded;
			}
		}

		/*
			LocalResource (Generic) : Constructor - Initialise with default values
			Created: 04/10/2017
			Modified: 04/10/2017

			param[in] pPath - The path of the data to load
		*/
		__LocalResource<Generic>::__LocalResource(const char* pPath) : ILocalResourceBase(EResourceType::Generic), mData(nullptr) {
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
				Globals::get<Debug::Logger>().logError("Local Resource (Generic) failed to open the file '%s'. Error: %s", pPath, buffer);

				//Flag error status
				mStatus = EResourceLoadStatus::Error;
				return;
			}

			//Seek to the end of the file
			fseek(file, 0, SEEK_END);

			//Get the size of the file
			mSize = ftell(file);

			//Seek back to the start of the file
			fseek(file, 0, SEEK_SET);

			//Create the resource's data array
			mData = new char[mSize + 1];

			//Read in the field data
			fread(mData, sizeof(char), mSize, file);

			//Close the file
			fclose(file);

			//Null terminate the stream
			mData[mSize] = '\0';

			//Flag loaded
			mStatus = EResourceLoadStatus::Loaded;
		}
	}
}