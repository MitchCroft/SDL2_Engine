#pragma once

//! Include the STL map to store loaded textures in
#include <unordered_map>

//! Include the powerup values
#include "Powerup.hpp"

//! Prototype the SDL_Texture object
struct SDL_Texture;

namespace BombSquad {
	/*
	 *		Name: PowerupLoader
	 *		Author: Mitchell Croft
	 *		Created: 10/11/2017
	 *		Modified: 11/11/2017
	 *		
	 *		Purpose:
	 *		Recursively load all Objx files within a directory, treating them as
	 *		powerup descriptions and setup powerup objects that represent the 
	 *		loaded values
	**/
	class PowerupLoader {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------Construction/Destruction--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PowerupLoader : Constructor - Initialise with default values
			Created: 10/11/2017
			Modified: 11/11/2017
		*/
		inline PowerupLoader() : mTextureProg(0) {}

		/*
			PowerupLoader : Destructor - Deallocate loaded memory
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline ~PowerupLoader() { dispose(); }

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-----------------------------------Functionality-------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		/*
			PowerupLoader : loadDirectory - Load all Objx files in a directory as Powerups
			Created: 10/11/2017
			Modified: 11/11/2017

			param[in] pDir - the directory to start loading powerups from

			const vector<Powerup> - Returns a constant vector populated with the powerup blueprints
		*/
		const std::vector<Powerup> loadDirectory(const char* pDir);

		/*
			PowerupLoader : getTexture - Retrieve a specific texture based on it's ID
			Created: 10/11/2017
			Modified: 10/11/2017

			param[in] pID - A size_t value describing the unique texture identifier

			return SDL_Texture* - Returns a pointer to the corresponding texture object or nullptr if none
		*/
		SDL_Texture* getTexture(const size_t& pID) const;

		/*
			PowerupLoader : dispose - Deallocate loaded memory
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		void dispose();

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////-------------------------------------Redacted----------------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			PowerupLoader : Copy Constructor - Prevent the copying of values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline PowerupLoader(const PowerupLoader&) = delete;

		/*
			PowerupLoader : Assignment Operator - Prevent the copying of values
			Created: 10/11/2017
			Modified: 10/11/2017
		*/
		inline PowerupLoader& operator=(const PowerupLoader&) = delete;

	private:
		//! Store the current texture ID progress
		size_t mTextureProg;

		//! Store a map of the loaded textures
		std::unordered_map<size_t, SDL_Texture*> mTextures;
	};
}