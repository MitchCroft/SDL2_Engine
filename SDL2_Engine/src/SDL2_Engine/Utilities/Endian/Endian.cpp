#include "Endian.hpp"

namespace SDL2_Engine {
	namespace Utilities {
		namespace Endian {
			//! Define the static for the current Endian order
			const EEndianOrder SYSTEM_END_ORDER = ((*(unsigned short*)"\0\xff" < 0x100) ? ENDIAN_BIG : ENDIAN_SMALL);
		}
	}
}