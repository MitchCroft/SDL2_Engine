#pragma once

//! Include SDL2_Engine objects
#include "../__LibraryManagement.hpp"
#include "../Utilities/Bitmask.hpp"
#include "AudioValues.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
		 *		Name: AudioInitialiser
		 *		Author: Mitchell Croft
		 *		Created: 10/10/2017
		 *		Modified: 10/10/2017
		 *		
		 *		Purpose:
		 *		Store initialisation values for the Audio object
		**/
		struct AudioInitialiser {
			//! Store the Audio Codecs that should be supported by the Manager (Default Group_Common)
			Utilities::Bitmask<ESupportedCodecs> supportedCodecs;

			//! Store the output sampling frequency of the Audio, in Hertz (Default 22050)
			size_t frequency;

			//! The format of the output samples (Default Signed 16-Bit samples)
			ESampleFormat format;

			//! The number of output sound channels (Defaults to 2 for stereo)
			size_t outputChannels;

			//! The size of the buffer (in bytes) used to process audio (Defaults to 1024)
			size_t audioBufferSize;

			//! The initial number of sound effect channels available (Default 8)
			size_t initialSFXChannels;

			//! The maximum number of sound effect channels that can be initialised, where < 0 is (theoretically) limitless (Default -1)
			int maximumSFXChannels;

			/*
				AudioInitialiser : Constructor - Initialise with default values
				Created: 10/10/2017
				Modified: 10/10/2017
			*/
			AudioInitialiser();
		};
	}
}