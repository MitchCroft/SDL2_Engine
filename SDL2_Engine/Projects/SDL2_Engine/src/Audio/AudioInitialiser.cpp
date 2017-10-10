#include "AudioInitialiser.hpp"

//! Include Endian information
#include "../Utilities/Endian/Endian.hpp"

namespace SDL2_Engine {
	namespace Initialisation {
		/*
			AudioInitialiser : Constructor - Initialise with default values
			Created: 10/10/2017
			Modified: 10/10/2017
		*/
		AudioInitialiser::AudioInitialiser() :
			supportedCodecs(ESupportedCodecs::Group_Common),
			frequency(22050),
			format(Utilities::Endian::getEndianOrder() == Utilities::Endian::EEndianOrder::Small ? ESampleFormat::S16_Small : ESampleFormat::S16_Big),
			outputChannels(2),
			audioBufferSize(1024),
			initialSFXChannels(8),
			maximumSFXChannels(-1)
		{}
	}
}
