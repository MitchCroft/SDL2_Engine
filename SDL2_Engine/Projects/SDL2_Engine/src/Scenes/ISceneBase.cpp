#include "ISceneBase.hpp"

namespace SDL2_Engine {
	namespace Scenes {
		/*
			ISceneBase : Constructor - Initialise with default values
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		ISceneBase::ISceneBase() : mRenderOrder(0U), mProperties(ESceneProperties::Active, ESceneProperties::Visible) {}

		/*
			ISceneBase : shutdown - Flag the Scene for shutdown
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void ISceneBase::shutdown() { if (mProperties & ESceneProperties::Active) mProperties ^= ESceneProperties::Active; }

		/*
			ISceneBase : show - Flag the Scene to be rendered
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void ISceneBase::show() { mProperties |= ESceneProperties::Visible; }

		/*
			ISceneBase : hide - Flag the Scene to be hidden
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void ISceneBase::hide() { if (mProperties & ESceneProperties::Visible) mProperties ^= ESceneProperties::Visible; }

		/*
			ISceneBase : prioritise - Flag the scene to take priority
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void ISceneBase::prioritise() { mProperties |= ESceneProperties::Priority; }

		/*
			ISceneBase : deprioritise - Flag the scene to remove priority
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void ISceneBase::deprioritise() { if (mProperties & ESceneProperties::Priority) mProperties ^= ESceneProperties::Priority; }
	}
}
