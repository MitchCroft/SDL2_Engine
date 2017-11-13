#include "PowerupLoader.hpp"

//! Include objects used to navigate directories
#include <dirent.h>
#include <queue>

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include <Math.hpp>

#include <Resources/Resources.hpp>
#include <Resources/ResourceTypes/LocalResourceGeneric.hpp>

#include <Rendering/Renderer.hpp>
#include <Rendering/Colour.hpp>

#include <Utilities/Endian/Endian.hpp>

using namespace SDL2_Engine;

//! Include Objx for parsing
#include <Objx_node.hpp>
#include <Objx_document.hpp>
#include <Objx_reader.hpp>
using namespace Objx;

//! Include the SDL functionality
#include <SDL.h>
#include <SDL_image.h>

#include "../../Additionals.hpp"

namespace BombSquad {
	/*
		stringToPowerupActor - Convert a string to an EPowerupActor value
		Author: Mitchell Croft
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pStr - The string value to convert to an EPowerupActor value

		return EPowerupActor - Returns the corresponding EPowerupActor value or EPowerupActor::Null if not found
	*/
	EPowerupActor stringToPowerupActor(std::string pStr) {
		//Convert the string to lower case
		for (size_t i = 0; i < pStr.length(); i++)
			pStr[i] = tolower(pStr[i]);

		//Check the different possibilities
		if		(pStr == "self")	return EPowerupActor::Self;
		else if (pStr == "enemy")	return EPowerupActor::Enemy;
		else if (pStr == "random")	return EPowerupActor::Random;
		else						return EPowerupActor::Null;
	}

	/*
		PowerupLoader : loadDirectory - Load all Objx files in a directory as Powerups
		Created: 10/11/2017
		Modified: 13/11/2017

		param[in] pDir - the directory to start loading powerups from

		const vector<Powerup> - Returns a constant vector populated with the powerup blueprints
	*/
	const std::vector<Powerup> PowerupLoader::loadDirectory(const char* pDir) {
		//Get the required SDL2_Engine objects
		const Debug::Logger& LOG = Globals::get<Debug::Logger>();
		Resources& res = Globals::get<Resources>();
		const Math& MATH = Globals::get<Math>();
		Rendering::Renderer rend = Globals::get<Rendering::Renderer>();

		//Create a vector of the files found to process
		std::vector<std::string> toLoad;

		//Find all objx files
		{
			//Create a queue to hold the directories to search
			std::queue<std::string> toSearch;

			//Add the initial directory to the queue
			toSearch.push(pDir);

			//Store the name of the element being processed
			std::string elementName;

			//Store directory information
			DIR* directory		= nullptr;
			struct dirent* dir	= nullptr;

			//Continue to loop while there are directories to search
			while (toSearch.size()) {
				//Get the directory to search
				const std::string CURR_DIR = toSearch.front();
				toSearch.pop();

				//Attempt to open the directory
				directory = opendir(CURR_DIR.c_str());

				//Check the directory was opened
				if (!directory) 
					LOG.logError("PowerupLoader failed to open the directory '%s' when recursively searching '%s' for Objx Powerup descriptions", CURR_DIR.c_str(), pDir);

				//Directory is valid
				else {
					//Loop through and process all found elements
					while ((dir = readdir(directory))) {
						//Get the name of the current element
						elementName = dir->d_name;

						//Skip the navigation directories
						if (dir->d_type == DT_DIR && (elementName == "." || elementName == "..")) continue;

						//Stash directories in the search queue
						else if (dir->d_type == DT_DIR) {
							toSearch.push(CURR_DIR + elementName + '\\');
							continue;
						}

						//Get the file extension of the file
						std::string ext = elementName.substr(elementName.find_last_of('.') + 1);

						//Lower case-ify the extension
						for (size_t i = 0; i < ext.length(); i++)
							ext[i] = tolower(ext[i]);

						//Check if the file is objx
						if (ext == "objx") toLoad.push_back(CURR_DIR + elementName);
					}

					//Close the directory
					closedir(directory);
				}
			}
		}

		//Store a vector of loaded Powerups
		std::vector<Powerup> blueprints;

		//Check there are files to load
		if (!toLoad.size()) return blueprints;

		//Store an array of the required properties on a Powerup object
		const xstring REQ_PROP[] = { "actor", "spriteSheet", "sourceRect", "description" };
		const size_t REQ_PROP_COUNT = sizeof(REQ_PROP) / sizeof(const xstring);

		//Store the powerup frames to be laid over powerup sprites (according to the actor)
		SDL_Surface** powerupFrames = new SDL_Surface*[4] { nullptr, nullptr, nullptr, nullptr };

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

		{
			//Store an array or colour values to apply to filter the powerup frame
			const Rendering::Colour FILTER_COL[] = { Utilities::Endian::reverseBytes(Rendering::Colour::Black), Utilities::Endian::reverseBytes(Rendering::Colour::Green), Utilities::Endian::reverseBytes(Rendering::Colour::Red), Utilities::Endian::reverseBytes(Rendering::Colour::Yellow) };
			
			//Load the basic spritesheet
			SDL_Surface* sprites = IMG_Load("resources/Textures/Spritesheet.png");

			//Check the spritesheet was loaded
			if (!sprites) {
				LOG.logError("Powerup Loader failed to load the game standard spritesheet at 'resources/Textures/Spritesheet.png'. Could not create powerup sprites. Error: %s", SDL_GetError());
				return blueprints;
			}

			//Terrible hard coded values of the powerup frame on the spritesheet
			SDL_Rect from = { 224, 0, 32, 32 };

			//Loop through and setup the frames
			for (size_t i = 0; i < 4; i++) {
				//Terrible hard coded values for the individual powerup sprites
				SDL_Rect TO = { 0, 0, 32, 32 };

				//Create a surface to transplant the frame to
				powerupFrames[i] = SDL_CreateRGBSurface(SDL_SWSURFACE, TO.w, TO.h, 32, rmask, gmask, bmask, amask);

				//Check the surface was created properly
				if (!powerupFrames[i]) {
					//Output the error
					LOG.logError("Powerup Loader failed to create the initial powerup frame surface '%zu'. Error: %s", i, SDL_GetError());

					//Delete the loaded surfaces
					for (size_t j = 0; j < i; j++)
						SDL_FreeSurface(powerupFrames[j]);

					//Delete the spritesheet
					SDL_FreeSurface(sprites);

					//Exit the function
					return blueprints;
				}

				//Transplant the image information from the source to the new surface
				if (SDL_BlitSurface(sprites, &from, powerupFrames[i], &TO)) {
					//Output the error
					LOG.logError("Powerup Loader failed to transplant the powerup frame image data to powerup frame '%zu'. Error: %s", i, SDL_GetError());

					//Delete the loaded surfaces
					for (size_t j = 0; j <= i; j++)
						SDL_FreeSurface(powerupFrames[j]);

					//Delete the spritesheet
					SDL_FreeSurface(sprites);

					//Exit the function
					return blueprints;
				}

				//Lock the surface
				SDL_LockSurface(powerupFrames[i]);

				//Get the pixel data from the surface
				unsigned int* pixelData = (unsigned int*)powerupFrames[i]->pixels;

				//Loop through an set all non blank pixels to filter colour
				for (size_t j = 0, count = powerupFrames[i]->w * powerupFrames[i]->h; j < count; j++) {
					//Check the pixel 
					if (pixelData[j]) pixelData[j] = FILTER_COL[i].ID;
				}

				//Unlock the surface
				SDL_UnlockSurface(powerupFrames[i]);
			}

			//Free the sprites 
			SDL_FreeSurface(sprites);
		}

		//Loop through all found Objx files
		for (size_t i = 0; i < toLoad.size(); i++) {
			//Attempt to load the data from the file
			LocalResource<Generic> data = res.loadResource<Generic>(toLoad[i].c_str());

			//Check the data was loaded correctly
			if (data->status() != EResourceLoadStatus::Loaded) continue;

			//Store an error flag
			bool flag = false;

			//Create the reader object
			Reader reader([&](const EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
				//Check for error header
				if (!flag) LOG.logError("Powerup Loader encountered an error/errors when parsing '%s':", toLoad[i].c_str());

				//Output the error information
				LOG.logError("Line: %zu\tColumn: %zu\t\t%s (%s)", pLine, pColumn, pMsg, statusCodeToString(pCode));

				//Toggle the flag
				flag = true;
			}, 2);

			//Attempt to parse the file
			const Document DOC = reader.cparse(data->data());

			//Check if the flag was raised
			if (flag) continue;

			//Check there are elements on the document to process
			else if (!DOC.size()) continue;

			//Process all of the elements on the Document
			DOC.forEach([&](const xstring& pName, const Node& pNode) {
				//Check the required properties are listed
				bool reqFlag = false;
				for (size_t j = 0; j < REQ_PROP_COUNT; j++) {
					if (!pNode.hasProperty(REQ_PROP[j])) {
						//Output the error
						LOG.logError("Powerup Loader encountered an error\nObject '%s' in Document '%s' does not have the required property '%s'", pName.c_str(), toLoad[i].c_str(), REQ_PROP[j].c_str());

						//Toggle the error flag
						reqFlag = true;
					}
				}

				//Check if the flag was raised
				if (reqFlag) return true;

				//Create a new powerup object to modify
				Powerup newObj;

				//Read the basic required properties
				newObj.setActor(stringToPowerupActor(pNode["actor"].readVal<xstring>()));
				newObj.setDescription(pNode["description"].readVal<xstring>());
				newObj.setName(pNode.hasProperty("name") ? pNode["name"].readVal<xstring>() : pName);

				//Create the powerups sprite texture
				{
					//Store the deduced filepath
					const std::string REL_PATH = deductLocation(toLoad[i], pNode["spriteSheet"].readVal<xstring>());

					//Load the specified sprite sheet
					SDL_Surface* spritesheet = IMG_Load(REL_PATH.c_str());

					//Check the surface was loaded correctly
					if (!spritesheet) {
						//Output the error
						LOG.logError("Powerup Loader failed to open the spritesheet with deduced location '%s' (original '%s') listed in '%s' in on the Document '%s'.Error: %s",
							REL_PATH.c_str(), pNode["spriteSheet"].readVal<xstring>().c_str(), pName.c_str(), toLoad[i].c_str(), SDL_GetError());

						//Continue to next object
						return true;
					}

					//Terrible hard coded values for the individual powerup sprites
					SDL_Rect TO = { 0, 0, 32, 32 };

					//Create a surface to store the images on
					SDL_Surface* toSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, TO.w, TO.h, 32, rmask, gmask, bmask, amask);

					//Check the surface was created
					if (!toSurface) {
						//Output the error
						LOG.logError("Powerup Loader failed to create the surface used to compile the Powerup texture. ERROR: %s", SDL_GetError());

						//Free the spritesheet
						SDL_FreeSurface(spritesheet);

						//Continue to next object
						return true;
					}

					//Setup the frame to copy from the spritesheet
					SDL_Rect fromSpritesheet = {
						MATH.largest(0, pNode["sourceRect"]["x"].readVal<xint>()),
						MATH.largest(0, pNode["sourceRect"]["y"].readVal<xint>()),
						MATH.largest(0, pNode["sourceRect"]["width"].readVal<xint>()),
						MATH.largest(0, pNode["sourceRect"]["height"].readVal<xint>())
					};

					//Recertify values
					TO = { 0, 0, 32, 32 };

					//Blitz the spritesheet to the surface
					if (SDL_BlitSurface(spritesheet, &fromSpritesheet, toSurface, &TO)) {
						//Output error
						LOG.logError("Powerup Loader failed to transplant the image data from '%s' with the dimensions { x: %i y: %i width: %i height: %i } for the description '%s' on the Document '%s'. Error: %s", REL_PATH.c_str(), fromSpritesheet.x, fromSpritesheet.y, fromSpritesheet.w, fromSpritesheet.h, pName.c_str(), toLoad[i].c_str(), SDL_GetError());

						//Free the two surfaces
						SDL_FreeSurface(spritesheet);
						SDL_FreeSurface(toSurface);

						//Continue to next object
						return true;
					}

					//Free the spritesheet
					SDL_FreeSurface(spritesheet);

					//Recertify values
					TO = { 0, 0, 32, 32 };

					//Copy the powerups frame over the sprite
					if (SDL_BlitSurface(powerupFrames[(int)newObj.getActor()], &TO, toSurface, &TO)) {
						//Output error
						LOG.logError("Powerup Loader failed to transplant the Powerup Border image data for actor type '%i' on the object '%s' on the Document '%s'. Error: %s", (int)newObj.getActor(), pName.c_str(), toLoad[i].c_str(), SDL_GetError());

						//Free the two surfaces
						SDL_FreeSurface(toSurface);

						//Continue to next object
						return true;
					}

					//Create a texture from the surface
					SDL_Texture* tex = SDL_CreateTextureFromSurface(rend.getRenderer(), toSurface);

					//Check the texture was created successfully
					if (!tex) {
						//Output error
						LOG.logError("Powerup Loader failed to create a texture from the constructed sprite surface for the object '%s' on the Document '%s'. Error: %s", pName.c_str(), toLoad[i].c_str(), SDL_GetError());

						//Free the two surfaces
						SDL_FreeSurface(toSurface);

						//Continue to next object
						return true;
					}

					//Free the surface
					SDL_FreeSurface(toSurface);

					//Get the next texture ID
					size_t id = mTextureProg++;

					//Add the texture to the internal textures map
					mTextures.insert({ id, tex });

					//Assign the ID to the powerup
					newObj.setTextureID(id);
				}

				//Read the game modification properties
				newObj.setDuration((float)MATH.clamp(pNode["duration"].readVal<xdec>(-1.0), -1.0, 9999.9));
				if (pNode.hasProperty("fuseDuration")) newObj.setFuseLength((float)MATH.largest(pNode["fuseDuration"].readVal<xdec>(), 0.0));
				if (pNode.hasProperty("deltaBlastRadius")) newObj.setDeltaBlastRadius(pNode["deltaBlastRadius"].readVal<xint>(1));
				if (pNode.hasProperty("detonateAll") && pNode["detonateAll"].readVal<xbool>()) newObj.setDetonateAll();
				if (pNode.hasProperty("deltaBombCount")) newObj.setDeltaBombCount(pNode["deltaBombCount"].readVal<xint>());
				if (pNode.hasProperty("reverseControls")) newObj.setReverseControls(pNode["reverseControls"].readVal<xbool>());
				if (pNode.hasProperty("teleport") && pNode["teleport"].readVal<xbool>()) newObj.setTeleport();
				if (pNode.hasProperty("hasMines")) newObj.setMines(pNode["hasMines"].readVal<xbool>());
				if (pNode.hasProperty("speed")) newObj.setSpeed((float)pNode["speed"].readVal<xdec>());

				//Add the powerup object to the list of blueprints
				blueprints.push_back(newObj);

				//Process the next element
				return true;
			});
		}

		//Free the frame surfaces
		for (size_t i = 0; i < 4; i++)
			SDL_FreeSurface(powerupFrames[i]);

		//Delete the powerup frames array
		delete[] powerupFrames;

		//Return the loaded powerups
		return blueprints;
	}

	/*
		PowerupLoader : getTexture - Retrieve a specific texture based on it's ID
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pID - A size_t value describing the unique texture identifier

		return SDL_Texture* - Returns a pointer to the corresponding texture object or nullptr if none
	*/
	SDL_Texture* PowerupLoader::getTexture(const size_t& pID) const {
		//Search for the ID
		auto iter = mTextures.find(pID);

		//Return the found Texture
		return (iter != mTextures.end() ? iter->second : nullptr);
	}

	/*
		PowerupLoader : dispose - Deallocate loaded memory
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	void PowerupLoader::dispose() {
		//Loop through all textures stored in the map and destroy them
		for (auto it = mTextures.begin(); it != mTextures.end(); it++)
			SDL_DestroyTexture(it->second);

		//Clear the map
		mTextures.clear();
	}
}
