#include "SceneManager.hpp"

//! Include the STL functionality
#include <vector>
#include <algorithm>

//! Include the SDL2_Engine values
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"
#include "SceneManagerInitialiser.hpp"
#include "ISceneBase.hpp"

namespace SDL2_Engine {
	namespace Scenes {
		/*
			organiseScenes - Organise the ISceneBase objects in terms priority and render depth
			Author: Mitchell Croft
			Created: 11/10/2017
			Modified: 11/10/2017

			param[in] pLeft - A pointer to the first ISceneBase object 
			param[in] pRight - A pointer to the second ISceneBase object

			return bool - Returns true if pLeft should proceed pRight
		*/
		inline bool organiseScenes(const ISceneBase* pLeft, const ISceneBase* pRight) {
			//Check for priority 
			if ((pLeft->getProperties() | pRight->getProperties()) & (int)ESceneProperties::Priority) {
				//Ignore if both have priority
				if (!((pLeft->getProperties() & pRight->getProperties()) & (int)ESceneProperties::Priority)) {
					//Return true if Left has priority
					return ((pLeft->getProperties() & ESceneProperties::Priority) != 0);
				}
			}

			//Check for render depth
			else return (pLeft->getRenderOrder() < pRight->getRenderOrder());
		}

		/*
		 *		Name: SceneManagerInternalData
		 *		Author: Mitchell Croft
		 *		Created: 11/10/2017
		 *		Modified: 11/10/2017
		 *		
		 *		Purpose:
		 *		Store the internal data used by the Scene Manager
		**/
		struct SceneManager::SceneManagerInternalData {
			//! Store a temporary pointer to the setup object
			Initialisation::SceneManagerInitialiser* setup = nullptr;

			//! Store a vector of the Scenes currently in use
			std::vector<ISceneBase*> activeScenes;

			//! Store a vector of Scenes to be added to active rotation next cycle
			std::vector<ISceneBase*> scenesToAdd;

			//! Flag if the program is running
			bool running = false;
		};

		/*
			SceneManager : removeScene - Flag the first scene of the specified type for removal
			Created: 11/10/2017
			Modified: 11/10/2017

			param[in] pID - The Type ID of the Scene to remove

			return bool - Returns true if a Scene of the specified type was flagged
		*/
		bool SceneManager::removeScene(const Utilities::typeID& pID) {
			//Loop through all active scenes
			for (int i = (int)mData->activeScenes.size() - 1; i >= 0; --i) {
				//Check if the ID matches
				if (mData->activeScenes[i]->mTypeID == pID) {
					//Flag for removal
					mData->activeScenes[i]->shutdown();

					//Return success
					return true;
				}
			}

			//Default not found
			return false;
		}

		/*
			SceneManager : removeScenes - Flag of scenes of the specified type for removal
			Created: 11/10/2017
			Modified: 11/10/2017

			param[in] pID - The Type ID of the Scene to remove

			return bool - Returns true if a Scene of the specified type was flagged
		*/
		bool SceneManager::removeScenes(const Utilities::typeID& pID) {
			//Store the success flag
			bool found = false;

			//Loop through all active scenes
			for (int i = (int)mData->activeScenes.size() - 1; i >= 0; --i) {
				//Check if the ID matches
				if (mData->activeScenes[i]->mTypeID == pID) {
					//Flag for removal
					mData->activeScenes[i]->shutdown();

					//Flag found
					found = true;
				}
			}

			//Return the success flag
			return found;
		}

		/*
			SceneManager : quit - Flag to the program that it should terminate
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void SceneManager::quit() { mData->running = false; }

		/*
			SceneManager : isRunning - Get the running flag of the program
			Created: 11/10/2017
			Modified: 11/10/2017

			return const bool& - Returns a constant reference to the running flag
		*/
		const bool& SceneManager::isRunning() const { return mData->running; }

		/*
			SceneManager : Constructor - Initialise with default values
			Created: 11/10/2017
			Modified: 11/10/2017

			param[in] pSetup - Defines how the Scene Manager should be setup
		*/
		SceneManager::SceneManager(Initialisation::SceneManagerInitialiser* pSetup) : mData(nullptr) {
			//Create initial data
			mData = new SceneManagerInternalData();

			//Stow the setup values
			mData->setup = pSetup;
		}

		/*
			SceneManager : createInterface - Verify and setup starting information
			Created: 11/10/2017
			Modified: 11/10/2017

			return bool - Returns true if the Resources Manager was setup correctly
		*/
		bool SceneManager::createInterface() {
			//Ensure there is a setup values
			if (!mData->setup) {
				//Output error
				Globals::get<Debug::Logger>().logError("Scene Manager Initialiser did not define a setup object to start the program with");
				return false;
			}

			//Get the setup values
			auto setup = mData->setup;
			mData->setup = nullptr;

			//Attempt to setup the initial Scene
			if (!initialiseScene(setup->initialScreen, setup->initialID)) {
				//Output error
				Globals::get<Debug::Logger>().logError("Scene Manager failed to create the Initial Scene supplied for the program");
				return false;
			}

			//Flag the program as running
			mData->running = true;

			//Return successful
			return true;
		}

		/*
			SceneManager : destroyInterface - Deallocate internal memory allocated
			Created: 11/10/2017
			Modified: 11/10/2017
		*/
		void SceneManager::destroyInterface() {
			//Check if there is data to operate on
			if (mData) {
				//Close any scenes to add
				if (mData->scenesToAdd.size()) {
					for (int i = (int)mData->scenesToAdd.size() - 1; i >= 0; --i) {
						//Destroy what you can
						mData->scenesToAdd[i]->destroyScene();

						//Delete the Scene
						delete mData->scenesToAdd[i];
					}
				}

				//Close any active scenes
				if (mData->activeScenes.size()) {
					for (int i = (int)mData->activeScenes.size() - 1; i >= 0; --i) {
						//Destroy what you can
						mData->activeScenes[i]->destroyScene();

						//Delete the Scene
						delete mData->activeScenes[i];
					}
				}

				//Delete the data
				delete mData;
				mData = nullptr;
			}
		}

		/*
			SceneManager : update - Update and render the contained Scenes
			Created: 11/10/2017
			Modified: 02/11/2017
		*/
		void SceneManager::update() {
			//Loop through existing currently active scenes
			for (int i = (int)mData->activeScenes.size() - 1; i >= 0; --i) {
				//Check to see if they are inactive
				if (!(mData->activeScenes[i]->mProperties & ESceneProperties::Active)) {
					//Clear up the memory allocated
					mData->activeScenes[i]->destroyScene();

					//Delete the Scene
					delete mData->activeScenes[i];

					//Remove the Scene pointer from the list
					mData->activeScenes.erase(mData->activeScenes.begin() + i);
				}
			}

			//Check if there are new Scenes to add to the active list
			if (mData->scenesToAdd.size()) {
				//Loop through the elements in the list
				for (size_t i = 0, COUNT = mData->scenesToAdd.size(); i < COUNT; i++) {
					//Attempt to create the Scene
					if (!mData->scenesToAdd[i]->createScene()) {
						//Output error
						Globals::get<Debug::Logger>().logError("Failed to create a new Scene with type ID '%zu'", mData->scenesToAdd[i]->mTypeID);

						//Clear up what it can
						mData->scenesToAdd[i]->destroyScene();

						//Delete the scene
						delete mData->scenesToAdd[i];
					}

					//If successful add to main rotation
					else mData->activeScenes.push_back(mData->scenesToAdd[i]);
				}

				//Clear the waiting list
				mData->scenesToAdd.clear();
			}

			//Ensure there are Scenes to process
			if (!mData->activeScenes.size()) {
				//Log the error
				Globals::get<Debug::Logger>().logError("There are no remaining active Scenes to control application execution. Assuming program should close...");

				//Quit the application
				quit();

				//Exit the function
				return;
			}

			//Sort remaining Scenes
			std::stable_sort(mData->activeScenes.begin(), mData->activeScenes.end());

			//Flag if there is a priority Scene in the list
			bool priorityFlag = false;
			for (size_t i = 0, COUNT = mData->activeScenes.size(); i < COUNT; ++i) {
				//Check if has priority
				if (mData->activeScenes[i]->mProperties & ESceneProperties::Priority) priorityFlag = true;

				//Otherwise check if priority has preceded it
				else if (priorityFlag) break;

				//Update the Scene
				mData->activeScenes[i]->update();
			}

			//Render Scenes
			for (int i = (int)mData->activeScenes.size() - 1; i >= 0; --i) {
				//Check if Scene is visible
				if (mData->activeScenes[i]->mProperties & ESceneProperties::Visible)
					mData->activeScenes[i]->render();
			}
		}

		/*
			SceneManager : initialiseScene - Initialise a new Scene object and if its successful add it to the list
			Created: 11/10/2017
			Modified: 16/10/2017

			param[in] pScene - A pointer to the ISceneBase based object to initialise
			param[in] pID - The ID of the type of Scene object

			return bool - Returns true if the Scene was successfully created
		*/
		bool SceneManager::initialiseScene(ISceneBase* pScene, const Utilities::typeID& pID) {
			//Assign the ID to the Scene
			pScene->mTypeID = pID;

			//Add the the scene to the list
			mData->scenesToAdd.push_back(pScene);

			//Return success 
			return true;
		}
	}
}
