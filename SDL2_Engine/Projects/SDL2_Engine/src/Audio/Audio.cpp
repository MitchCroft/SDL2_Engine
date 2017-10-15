#include "Audio.hpp"

//! Include the STL objects to manage SFX channels
#include <vector>

//! Include the SDL2_Engine values
#include "AudioInitialiser.hpp"
#include "../Globals.hpp"
#include "../Math.hpp"
#include "../Debug/Logger.hpp"

//! Include the SDL2 objects
#include <SDL.h>
#include <SDL_mixer.h>

namespace SDL2_Engine {
	/*
	 *		Name: AudioInternalData
	 *		Author: Mitchell Croft
	 *		Created: 10/10/2017
	 *		Modified: 10/10/2017
	 *		
	 *		Purpose:
	 *		Store the internal data used by the Audio manager
	**/
	struct Audio::AudioInternalData {
		//! Store a temporary pointer to the Initialiser
		const Initialisation::AudioInitialiser* setup;

		//! Store a boolean array to flag channel status
		std::vector<bool> channelStatus;

		//! Store a running total of the number of channels in use
		size_t usedChannels = 0;

		//! Store the current number of allocated SFX channels
		size_t SFXChannels = 0;

		//! Store the maximum possible number of SFX channels
		int maximumSFXChannels = 0;

		//! Flags if audio should be played by the interface
		bool playAudio = false;
	};

	/*
		sfxChannelComplete - Callback function for the Mixer library to manage channels that are in use
		Author: Mitchell Croft
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pChannel - The number of the channel that finished playing
	*/
	struct SFXManager { static void sfxChannelComplete(int pChannel) {
		//Get a pointer to the Global Audio object
		auto data = Globals::get<Audio>().mData;

		//Decrement the number of used channels
		data->usedChannels--;

		//Free the channel index
		data->channelStatus[pChannel] = false;
	}};

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
	int Audio::playSFX(Mix_Chunk* pSFX, const char& pVolume /*= 128*/, const size_t& pFadeTime /*= 0*/, const int& pLoops /*= -1*/, const int& pRunTime /*= -1*/, const int& pChannel /*= -1*/) {
		//Get the channel to play on
		int channel = (pChannel != -1 ? pChannel : reserveChannel());

		//Check there is a channel to use
		if (channel == -1) return -1;

		//Play the sound effect
		if ((channel = Mix_FadeInChannelTimed(channel, pSFX, (int)pLoops, (int)pFadeTime, (int)pRunTime)) == -1)
			Globals::get<Debug::Logger>().logError("Audio failed to play the sound effect during a call to playSFX(...). Error: %s", SDL_GetError());

		//Set the volume of the sound
		if (channel >= 0) Mix_Volume(channel, pVolume);

		//Return the used channel
		return channel;
	}

	/*
		Audio : setSFXVolume - Set the volume of a sound effect(s) that is playing
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pChannel - The channel to adjust the volume of, or -1 to change all
		param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest (Default 128)

		return int - Returns the volume level as an int, or an average where pChannel is -1
	*/
	int Audio::setSFXVolume(const int& pChannel, const char& pVolume) { return Mix_Volume(pChannel, pVolume); }

	/*
		Audio : toggleSFXPause - Toggle the pause/play state of a sound effect channel
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pPause - A bool state to indicate if the sound effect should be paused or resumed
		param[in] pChannel - The channel to change the state of, or -1 to change all
	*/
	void Audio::toggleSFXPause(const bool& pPause, const int& pChannel) {
		if (pPause) Mix_Pause(pChannel);
		else Mix_Resume(pChannel);
	}

	/*
		Audio : stopSFX - Stop a sound effect from playing
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pChannel - The channel to stop, or -1 to stop all
		param[in] pDelay - The time in milliseconds to wait before stopping the channels (Default 0)
	*/
	void Audio::stopSFX(const int& pChannel, const size_t& pDelay /*= 0*/) { Mix_ExpireChannel(pChannel, (int)pDelay); }

	/*
		Audio : fadeOutSFX - Fade out a playing sound effect
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pChannel - The channel to fade out, or -1 to stop all
		param[in] pFadeTime - The time in milliseconds to fade out the channels over
	*/
	void Audio::fadeOutSFX(const int& pChannel, const size_t& pFadeTime) { Mix_FadeOutChannel(pChannel, (int)pFadeTime); }

	/*
		Audio : reserveChannel - Retrieve a channel that will be reserved, and not used for random sound effects
		Created: 10/10/2017
		Modified: 10/10/2017

		return int - Returns the reserved channel as an int or -1 if there are no channels left
	*/
	int Audio::reserveChannel() { 
		//Check if there are any available
		if (mData->usedChannels == mData->SFXChannels && !increaseSFXChannels())
			return -1;

		//Loop through and search for an available channel
		for (int i = (int)mData->channelStatus.size() - 1; i >= 0; --i) {
			//Check if the channel is available
			if (!mData->channelStatus[i]) {
				//Flag channel index
				mData->channelStatus[i] = true;

				//Increase the channel usage
				mData->usedChannels++;

				//Return the channel ID
				return i;
			}
		}

		//Default return (Shouldn't be hit)
		return -1;
	}

	/*
		Audio : releaseChannel - Release a channel to allow it to be selected for random sound effects
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pChannel - An int defining the channel to release, or -1 to release all
	*/
	void Audio::releaseChannel(const int& pChannel) {
		//Check for release all
		if (pChannel < 0) {
			//Reset all status flags
			mData->channelStatus.clear();
			mData->channelStatus.resize(mData->SFXChannels);
			
			//Reset usage count
			mData->usedChannels = 0;
		}

		//Release single
		else {
			//Check for decrement
			if (mData->channelStatus[pChannel])
				mData->usedChannels--;

			//Reset bit
			mData->channelStatus[pChannel] = false;
		}
	}

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
	bool Audio::playMusic(_Mix_Music* pMusic, const char& pVolume /*= 128*/, const int& pFadeTime /*= 0*/, const int& pLoops /*= -1*/, const double& pPosition /*= 0.0*/) {
		//Play the music
		int res;
		if (res = Mix_FadeInMusicPos(pMusic, pLoops, pFadeTime, pPosition))
			Globals::get<Debug::Logger>().logError("Audio failed to play music during a playMusic(...) call. Error: %s", SDL_GetError());

		//Set the volume
		if (!res) Mix_VolumeMusic(pVolume);

		//Return success state
		return (res == 0);
	}

	/*
		Audio : setMusicVolume - Set the volume of the music playing
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pVolume - The volume to play at, where 0 is the lowest and 128 is the highest. -1 can be used to poll the current level

		return int - Returns the previous volume level
	*/
	int Audio::setMusicVolume(const char& pVolume) { return Mix_VolumeMusic(pVolume); }

	/*
		Audio : toggleMusicPause - Toggle the pause/play state of the current music
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pPause - A bool state to indicate if the music should be paused or resumed
	*/
	void Audio::toggleMusicPause(const bool& pPause) {
		if (pPause) Mix_PauseMusic();
		else Mix_ResumeMusic();
	}

	/*
		Audio : setMusicPos - Set the playback position of the current music
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pPosition - The position to start playing the music from

		return bool - Returns true if the position was set
	*/
	bool Audio::setMusicPos(const double& pPosition) {
		//Rewind the music to the start
		Mix_RewindMusic();

		//Set the position of the track
		int res;
		if (res = Mix_SetMusicPosition(pPosition))
			Globals::get<Debug::Logger>().logError("Audio failed to set the music's position to %f. Error: %s", pPosition, SDL_GetError());

		//Return success state
		return (res != -1);
	}

	/*
		Audio : stopMusic - Stop the playback of the current music
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pFadeTime - Optional time in milliseconds to fade out the music over (Default 0)
	*/
	void Audio::stopMusic(const size_t& pFadeTime /*= 0*/) {
		if (!Mix_FadeOutMusic((int)pFadeTime))
			Globals::get<Debug::Logger>().logError("Audio failed to stop music over %i milliseconds. Error: %s", pFadeTime, SDL_GetError());
	}

	/*
		Audio : Constructor - Initialise with default values
		Created: 10/10/2017
		Modified: 10/10/2017

		param[in] pSetup - Defines how the Audio Manager should be setup
	*/
	Audio::Audio(const Initialisation::AudioInitialiser& pSetup) : mData(nullptr) {
		//Create the data object
		mData = new AudioInternalData();

		//Stow the initialiser
		mData->setup = &pSetup;
	}

	/*
		Audio : createInterface - Verify and setup starting information
		Created: 10/10/2017
		Modified: 10/10/2017

		return bool - Returns true if the Resources Manager was setup correctly
	*/
	bool Audio::createInterface() {
		//Retrieve the temporary pointer to the setup information
		auto setup = mData->setup;
		mData->setup = nullptr;

		//Initialise SDL_Audio subsystems
		if (SDL_InitSubSystem(SDL_INIT_AUDIO)) {
			//Output error
			Globals::get<Debug::Logger>().logError("Audio failed to initialise the SDL2 Audio Subsystem. Error: %s", SDL_GetError());
			return false;
		}

		//Initialise Mixer
		int initFlags = Mix_Init(setup->supportedCodecs.getMask());
		if (initFlags != setup->supportedCodecs.getMask()) {
			//Construct a list of the codecs successfully included
			std::string supported = "";
			if (initFlags & (int)ESupportedCodecs::Flac)
				supported += "Flac ";
			if (initFlags & (int)ESupportedCodecs::Mod)
				supported += "Mod ";
			if (initFlags & (int)ESupportedCodecs::Mod_Plug)
				supported += "Mod_Plug ";
			if (initFlags & (int)ESupportedCodecs::MP3)
				supported += "MP3 ";
			if (initFlags & (int)ESupportedCodecs::OGG)
				supported += "OGG ";
			if (initFlags & (int)ESupportedCodecs::Fluid_Synth)
				supported += "Fluid_Synth";

			//Output the error
			Globals::get<Debug::Logger>().logError("Audio failed to initialise Mixer with the requested codec formats, only able to support '%s'. Error: %s", supported.c_str(), SDL_GetError());
			return false;
		}

		//Open the Audio device
		if (Mix_OpenAudio((int)setup->frequency, (unsigned short)setup->format, (int)setup->outputChannels, (int)setup->audioBufferSize)) {
			//Output the error
			Globals::get<Debug::Logger>().logError("Audio failed to open the Mixer Audio device. Error: %s", SDL_GetError());
			return false;
		}

		//Save the maximum number of channels that can be created
		Math& math = Globals::get<Math>();
		mData->maximumSFXChannels = (setup->maximumSFXChannels >= 0 ?  math.largest((int)setup->initialSFXChannels, setup->maximumSFXChannels) : -1);

		//Allocate the initial SFX channels
		mData->SFXChannels = Mix_AllocateChannels((int)setup->initialSFXChannels);

		//Allocate the initial vector flags
		mData->channelStatus.resize(mData->SFXChannels);

		//Apply the Channel finished callback
		Mix_ChannelFinished(SFXManager::sfxChannelComplete);

		//Return success
		return true;
	}

	/*
		Audio : destroyInterface - Deallocate internal memory allocated
		Created: 10/10/2017
		Modified: 10/10/2017
	*/
	void Audio::destroyInterface() {
		//Check there is internal data
		if (mData) {
			//Close the Mixer Audio device
			Mix_CloseAudio();

			//Closer Mixer
			Mix_Quit();

			//Close the SDL2 Audio subsystem
			SDL_QuitSubSystem(SDL_INIT_AUDIO);

			//Delete the internal data
			delete mData;
			mData = nullptr;
		}
	}

	/*
		Audio : increaseSFXChannels - Attempts to create more channels for SFX to operate on
		Created: 10/10/2017
		Modified: 10/10/2017

		return bool - Returns true if it was able to allocate more channels
	*/
	bool Audio::increaseSFXChannels() {
		//Check if the maximum can be reached
		if (mData->maximumSFXChannels != -1 && (int)mData->SFXChannels >= mData->maximumSFXChannels) 
			return false;

		//Allocate a new channel
		mData->SFXChannels = Mix_AllocateChannels((int)mData->SFXChannels + 1);

		//Resize the status vector
		mData->channelStatus.resize(mData->SFXChannels);

		//Return success
		return true;
	}
}