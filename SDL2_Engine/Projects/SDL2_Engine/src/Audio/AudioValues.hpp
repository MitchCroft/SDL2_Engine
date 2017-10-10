#pragma once

namespace SDL2_Engine {
	//! Flag the audio formats to be supported by the Audio Device
	enum class ESupportedCodecs {
		//! Single values
		Flac		= 1 << 0,
		Mod			= 1 << 1,
		Mod_Plug	= 1 << 2,
		MP3			= 1 << 3,
		OGG			= 1 << 4,
		Fluid_Synth = 1 << 5,

		//! Groupings
		Group_Common = MP3 | OGG,
		Group_Mod	 = Mod | Mod_Plug,
		Group_All	 = Flac | Mod | Mod_Plug | MP3 | OGG | Fluid_Synth,
	};

	//! Store the audio output sample format flags 
	enum class ESampleFormat {
		//! Unsigned 8-bit samples
		U8 = 0x0008,

		//! Signed 8-bit samples
		S8 = 0x8008,

		//! Unsigned 16-bit samples
		U16_Small = 0x0010,
		U16_Big = 0x1010,
		
		//! Signed 16-bit samples
		S16_Small = 0x8010,
		S16_Big = 0x9010,

		//! 32-bit integer samples
		S32_Small = 0x8020,
		S32_Big = 0x9020,

		//! 32-bit floating point samples
		F32_Small = 0x8120,
		F32_Big = 0x9120,
	};
}