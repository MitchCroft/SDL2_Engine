#pragma once

#include "../__LibraryManagement.hpp"
#include "../Utilities/IGlobal.hpp"

//! Prototype the SDL objects
struct Mix_Chunk;
struct _Mix_Music;

namespace SDL2_Engine {
	//! Prototype the AudioInitialiser
	namespace Initialisation { struct AudioInitialiser; }

	/*
	 *		Name: Audio
	 *		Author: Mitchell Croft
	 *		Created: 10/10/2017
	 *		Modified: 10/10/2017
	 *		
	 *		Purpose:
	 *		Provide basic functionality for managing music and
	 *		audio channels. Makes use of Mix_ChannelFinished to manage
	 *		active channels, assign chunks to available channels and
	 *		create more when required
	**/
	class SDL2_LIB_INC Audio : public Utilities::IGlobal {
	public:
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////------------------------------------SFX Functionality--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Audio : playSFX - Fade in a sound effect over a period of time with specified values
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pSFX - A pointer to the loaded Sound Effect to play
			param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest (Default 128)
			param[in] pFadeTime - The number of milliseconds to fade the sound effect in over (Default 0)
			param[in] pLoops - The number of times to repeat the sound effect, where -1 is endless (Default -1)
			param[in] pRunTime - The number of milliseconds that the sound effect should play for, where -1 is endless (Default -1)
			param[in] pChannel - The channel to use to play the sound effect on, where -1 finds the first available (Default -1)

			return int - Returns the channel that the sound effect was played on. Returns -1 if unable to play sound effect
		*/
		int playSFX(Mix_Chunk* pSFX,
					const char& pVolume = 128,
					const size_t& pFadeTime = 0,
					const int& pLoops = -1,
					const int& pRunTime = -1,
					const int& pChannel = -1);

		/*
			Audio : setSFXVolume - Set the volume of a sound effect(s) that is playing
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pChannel - The channel to adjust the volume of, or -1 to change all
			param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest (Default 128)

			return int - Returns the volume level as an int, or an average where pChannel is -1
		*/
		int setSFXVolume(const int& pChannel,
						 const char& pVolume);

		/*
			Audio : toggleSFXPause - Toggle the pause/play state of a sound effect channel
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pPause - A bool state to indicate if the sound effect should be paused or resumed
			param[in] pChannel - The channel to change the state of, or -1 to change all
		*/
		void toggleSFXPause(const bool& pPause,
							const int& pChannel);

		/*
			Audio : stopSFX - Stop a sound effect from playing
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pChannel - The channel to stop, or -1 to stop all
			param[in] pDelay - The time in milliseconds to wait before stopping the channels (Default 0)
		*/
		void stopSFX(const int& pChannel,
					 const size_t& pDelay = 0);

		/*
			Audio : fadeOutSFX - Fade out a playing sound effect
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pChannel - The channel to fade out, or -1 to stop all
			param[in] pFadeTime - The time in milliseconds to fade out the channels over
		*/
		void fadeOutSFX(const int& pChannel,
						const size_t& pFadeTime);

		/*
			Audio : reserveChannel - Retrieve a channel that will be reserved, and not used for random sound effects
			Created: 10/10/2017
			Modified: 10/10/2017

			return int - Returns the reserved channel as an int or -1 if there are no channels left
		*/
		int reserveChannel();

		/*
			Audio : releaseChannel - Release a channel to allow it to be selected for random sound effects
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pChannel - An int defining the channel to release, or -1 to release all
		*/
		void releaseChannel(const int& pChannel);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////----------------------------------Music Functionality--------------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Audio : playMusic - Play a background music file with specified values
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pMusic - The music file to play
			param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest (Default 128)
			param[in] pFadeTime - The number of milliseconds to fade the music in over (Default 0)
			param[in] pLoops - The number of times to repeat the music, where -1 is endless (Default -1)
			param[in] pPosition - The position to start playing the music from

			return bool - Returns true if the music was started successfully
		*/
		bool playMusic(_Mix_Music* pMusic,
					   const char& pVolume = 128,
					   const int& pFadeTime = 0,
					   const int& pLoops = -1,
					   const double& pPosition = 0.0);

		/*
			Audio : setMusicVolume - Set the volume of the music playing
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest. -1 can be used to poll the current level

			return int - Returns the previous volume level
		*/
		int setMusicVolume(const char& pVolume);

		/*
			Audio : toggleMusicPause - Toggle the pause/play state of the current music
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pPause - A bool state to indicate if the music should be paused or resumed
		*/
		void toggleMusicPause(const bool& pPause);

		/*
			Audio : setMusicPos - Set the playback position of the current music
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pPosition - The position to start playing the music from

			return bool - Returns true if the position was set
		*/
		bool setMusicPos(const double& pPosition);

		/*
			Audio : stopMusic - Stop the playback of the current music
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pFadeTime - Optional time in milliseconds to fade out the music over (Default 0)
		*/
		void stopMusic(const size_t& pFadeTime = 0);

		/////////////////////////////////////////////////////////////////////////////////////////////////////
		////////--------------------------------Construction/Destruction-----------------------------////////
		/////////////////////////////////////////////////////////////////////////////////////////////////////

		/*
			Audio : Constructor - Initialise with default values
			Created: 10/10/2017
			Modified: 10/10/2017

			param[in] pSetup - Defines how the Audio Manager should be setup
		*/
		Audio(const Initialisation::AudioInitialiser& pSetup);

		/*
			Audio : createInterface - Verify and setup starting information
			Created: 10/10/2017
			Modified: 10/10/2017

			return bool - Returns true if the Resources Manager was setup correctly
		*/
		bool createInterface() override;

		/*
			Audio : destroyInterface - Deallocate internal memory allocated
			Created: 10/10/2017
			Modified: 10/10/2017
		*/
		void destroyInterface() override;

		/*
			Audio : update - Empty function
			Created: 10/10/2017
			Modified: 10/10/2017
		*/
		inline void update() override {}

	private:
		//! Assign as friend of the SFXManager
		friend struct SFXManager;

		//! Define the internal protected elements for the Renderer
		struct AudioInternalData;
		AudioInternalData* mData;

		//! Allocate more channels for sound effects
		bool increaseSFXChannels();
	};
}