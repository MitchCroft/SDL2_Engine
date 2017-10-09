#include "Endian.hpp"

namespace SDL2_Engine {
	namespace Utilities {
		namespace Endian {
			/*
				getEndianOrder - Get the current Endian order of the System
				Created: 09/10/2017
				Modified: 09/10/2017

				return EEndianOrder - Returns an EEndianOrder value describing the endian order of the system
			*/
			SDL2_LIB_INC EEndianOrder getEndianOrder() { return ((*(unsigned short*)"\0\xff" < 0x100) ? EEndianOrder::Big : EEndianOrder::Small); }

		}
	}
}