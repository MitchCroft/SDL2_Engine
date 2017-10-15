#include "Canvas.hpp"

//! Include the SDL2_Engine objects
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"
#include "../Input/AxisInput.hpp"
#include "../Input/Mouse/Mouse.hpp"

#include "UIElements/Interfaces/IUIBase.hpp"
#include "UIElements/Interfaces/IUIAction.hpp"

#include "CanvasInitialiser.hpp"

//! Include the required STL containers
#include <vector>

//! Include GLM vector functions
#include <glm/ext.hpp>

//! Flag the different possible movement directions
enum { Up, Down, Left, Right, Total };

namespace SDL2_Engine {
	namespace UI {
		/*
		 *		Name: InteractiveUINode
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 15/10/2017
		 *		
		 *		Purpose:
		 *		Manage a network of UI elements that can be interacted with by the user
		**/
		struct InteractiveUINode {
			//! Store a pointer to current interactive element
			UIElements::IUIAction* element;

			//! Store the cardinal directions 
			InteractiveUINode* connections[Total];

			/*
				InteractiveUINode : Constructor - Initialise with default values
				Created: 15/10/2017
				Modified: 15/10/2017
			*/
			inline InteractiveUINode() : element(nullptr) { connections[Up] = connections[Down] = connections[Left] = connections[Right] = nullptr; }
		};

		/*
		 *		Name: CanvasInternalData
		 *		Author: Mitchell Croft
		 *		Created: 12/10/2017
		 *		Modified: 14/10/2017
		 *		
		 *		Purpose:
		 *		Store internal Canvas data values
		**/
		struct Canvas::CanvasInternalData {
			//! Store all of the UI elements on the Canvas
			std::vector<UIElements::IUIBase*> uiElements;

			//! Keep an array of the Interactive UI Nodes
			InteractiveUINode* interactiveMap;

			//! Track how many elements are in the interactive map
			size_t mapSize;

			//! Flag the index of the currently selected object
			int selectedAction;

			//! Store the setup values
			Initialisation::CanvasInitialiser setup;

			//! Store the previous position of the mouse
			glm::ivec2 prevPos;

			/*
				CanvasInternalData : Constructor - Initialise with default values
				Created: 13/10/2017
				Modified: 13/10/2017

				param[in] pSetup - The CanvasInitialiser object to copy the values of
			*/
			inline CanvasInternalData(const Initialisation::CanvasInitialiser& pSetup) : setup(pSetup), interactiveMap(nullptr), mapSize(0), selectedAction(-1) {}
		};

		/*
			Canvas : getUI - Get the first UI element with the specified tag
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pTag - The tag string to look for

			return UIElements::IUIBase* - Returns a pointer to the first UIElement with the specified tag or nullptr if not found
		*/
		UIElements::IUIBase* Canvas::getUI(const uiTag& pTag) {
			//Loop through elements to find
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; ++i) {
				//Check the tag
				if (mData->uiElements[i]->getTag() == pTag)
					return mData->uiElements[i];
			}

			//Return failure
			return nullptr;
		}

		/*
			Canvas : setActionObject - Set the active interactive object
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pObj - A pointer to the IUIAction object to make active

			return bool - Returns true if the supplied object was found and selected
		*/
		bool Canvas::setActionObject(const UIElements::IUIAction* pObj) {
			//Loop through the interactive map
			for (size_t i = 0; i < mData->mapSize; i++) {
				//Check for pointer match
				if (mData->interactiveMap[i].element == pObj) {
					mData->selectedAction = (int)i;
					return true;
				}
			}

			//Default failure
			return false;
		}

		/*
			Canvas : setActionObject - Set the active interactive object base on a tag
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pTag - The tag string to look for

			return bool - Returns true if a matching IUIAction was found
		*/
		bool Canvas::setActionObject(const uiTag& pTag) {
			//Store a pointer to the case IUIBase object
			UIElements::IUIBase* buffer = nullptr;

			//Loop through the elements
			for (size_t i = 0; i < mData->mapSize; i++) {
				//Cast the object
				buffer = (UIElements::IUIBase*)mData->interactiveMap[i].element;

				//Check the tag
				if (buffer->getTag() == pTag) {
					mData->selectedAction = (int)i;
					return true;
				}
			}

			//Default failure
			return false;
		}

		/*
			Canvas : destroyUI - Destroy the first UI element with the specified tag
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pTag - The tag string to look for

			return bool - Returns true if a UI element was found and removed

			Note:
			If the UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
			a call to rebuildInteractionMap before user input will work
		*/
		bool Canvas::destroyUI(const uiTag& pTag) {
			//Loop through elements to find
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; ++i) {
				//Check the tag
				if (mData->uiElements[i]->getTag() == pTag) {
					mData->uiElements[i]->kill();
					return true;
				}
			}

			//Return failure
			return false;
		}

		/*
			Canvas : destroyUI - Destroy the first UI element of the specified type
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pType - An EUIType value defining the type of UI element to destroy

			return bool - Returns true if a UI element was found and removed

			Note:
			If the UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
			a call to rebuildInteractionMap before user input will work
		*/
		bool Canvas::destroyUI(const EUIType& pType) {
			//Loop through elements to find
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; ++i) {
				//Check the type
				if (mData->uiElements[i]->type == pType) {
					mData->uiElements[i]->kill();
					return true;
				}
			}

			//Return failure
			return false;
		}

		/*
			Canvas : clearCanvas - Clear the Canvas of all UI elements with a specific tag
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pTag - The tag string to look for

			return bool - Returns true if a UI element was found and removed

			Note:
			If a UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
			a call to rebuildInteractionMap before user input will work
		*/
		bool Canvas::clearCanvas(const uiTag& pTag) {
			//Flag if any elements have been found
			bool flag = false;

			//Loop through the elements
			for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i) {
				//Check the tag
				if (mData->uiElements[i]->getTag() == pTag) {
					mData->uiElements[i]->kill();
					flag = true;
				}
			}

			//Return the flag
			return flag;
		}

		/*
			Canvas : clearCanvas - Clear the Canvas of all UI elements of a specific type
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pType - An EUIType value defining the type of UI element to destroy

			return bool - Returns true if a UI element was found and removed

			Note:
			If a UI element destroyed is an IUIAction derived class, the interaction map will become invalidated and will require
			a call to rebuildInteractionMap before user input will work
		*/
		bool Canvas::clearCanvas(const EUIType& pType) {
			//Flag if any elements have been found
			bool flag = false;

			//Loop through the elements
			for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i) {
				//Check the type
				if (mData->uiElements[i]->type == pType) {
					mData->uiElements[i]->kill();
					flag = true;
				}
			}

			//Return the flag
			return flag;
		}

		/*
			Canvas : clearCanvas - Remove all UI elements from the Canvas
			Created: 13/10/2017
			Modified: 13/10/2017
		*/
		void Canvas::clearCanvas() {
			//Loop through all elements
			for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i)
				mData->uiElements[i]->kill();
		}

		/*
			Canvas : loadCanvasFromObjx - Load a collection of UI elements from an Objx file
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pFilepath - The filepath of the UI Objx descriptor
			param[in] pName - The optional name of an Objx element within the Objx file to use as the Canvas (Default nullptr uses all elements in the root of the document)

			return bool - Returns true if it was able to load elements from the file
		*/
		bool Canvas::loadCanvasFromObjx(const char* pFilepath, const char* pName /*= nullptr*/) {
			//TODO
			return false;
		}

		/*
			Canvas : rebuildInteractionMap - Rebuild the internal interaction map to allow for users to navigate Actionable UI elements
			Created: 13/10/2017
			Modified: 15/10/2017
		*/
		void Canvas::rebuildInteractionMap() {
			//Clear the previous map
			destroyInteractiveMap();

			//Store the indexes of Action elements
			std::vector<UIElements::IUIAction*> interactable;
			interactable.reserve(mData->uiElements.size());

			//Calculate how many elements are in interactable
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; i++) {
				//Check if this is interactable
				if (auto pointer = dynamic_cast<UIElements::IUIAction*>(mData->uiElements[i]))
					interactable.push_back(pointer);
			}

			//Check if there are any interactable elements
			if (!interactable.size()) return;

			//Save the length of the map
			mData->mapSize = interactable.size();

			//Create the array to store the interactive map
			mData->interactiveMap = new InteractiveUINode[mData->mapSize];

			//Store the Action elements in the map
			for (size_t i = 0; i < mData->mapSize; i++)
				mData->interactiveMap[i].element = interactable[i];

			//Setup the connections between the Nodes
			for (size_t i = 0; i < mData->mapSize; i++) {
				//Store the location of current
				const UIElements::UIBounds& posI = dynamic_cast<UIElements::IUIBase*>(mData->interactiveMap[i].element)->getLocation();

				//Track the distances between all other UI elements
				InteractiveUINode* nodes[Total];
				memset(nodes, 0, sizeof(InteractiveUINode*) * Total);
				float distances[Total]{ FLT_MAX, FLT_MAX, FLT_MAX, FLT_MAX };

				//Loop through the other Nodes
				for (size_t j = 0; j < mData->mapSize; j++) {
					//Skip itself
					if (i == j) continue;

					//Get the position of this other
					const UIElements::UIBounds& posJ = dynamic_cast<UIElements::IUIBase*>(mData->interactiveMap[j].element)->getLocation();

					//Store temporary distance values
					const glm::vec2 SEP_VEC = posJ.distance(posI);

					//Get the distance
					const float DIST = glm::length(SEP_VEC);

					//Get the direction
					const glm::vec2 DIR = glm::normalize(SEP_VEC);

					//Check Up
					if (DIR.y < 0.f && DIST < distances[Up]) {
						distances[Up] = DIST;
						nodes[Up] = &mData->interactiveMap[j];
					}

					//Check down
					if (DIR.y > 0.f && DIST < distances[Down]) {
						distances[Down] = DIST;
						nodes[Down] = &mData->interactiveMap[j];
					}

					//Check Left
					if (DIR.x < 0.f && DIST < distances[Left]) {
						distances[Left] = DIST;
						nodes[Left] = &mData->interactiveMap[j];
					}

					//Check right
					if (DIR.x > 0.f && DIST < distances[Right]) {
						distances[Right] = DIST;
						nodes[Right] = &mData->interactiveMap[j];
					}
				}

				//Store the results in Node Map
				memcpy_s(mData->interactiveMap[i].connections, sizeof(InteractiveUINode*) * Total, nodes, sizeof(InteractiveUINode*) * Total);
			}
		}

		/*
			Canvas : Constructor - Initialise with default values
			Created: 12/10/2017
			Modified: 13/10/2017

			param[in] pSetup - Defines how the Canvas should be setup
		*/
		Canvas::Canvas(const Initialisation::CanvasInitialiser& pSetup) : mData(nullptr) { mData = new CanvasInternalData(pSetup); }

		/*
			Canvas : createInterface - Verify and setup starting information
			Created: 12/10/2017
			Modified: 13/10/2017

			return bool - Returns true if the Canvas was setup correctly
		*/
		bool Canvas::createInterface() {
			//Retrieve the Logger and Axis Input objects
			const Debug::Logger& LOG = Globals::get<Debug::Logger>();
			const Input::AxisInput AXIS = Globals::get<Input::AxisInput>();
			
			//Check to see if the Axis names are present
			for (size_t i = 0; i < 3; i++) {
				//Check if the name exists
				if (!AXIS.hasAxis(mData->setup.axisArray[i].c_str()))
					LOG.logWarning("Canvas object was given the as yet undefined Virtual Axis '%s' to use for player input navigation", mData->setup.axisArray[i].c_str());
			}

			//Return success
			return true;
		}

		/*
			Canvas : destroyInterface - Deallocate internal memory allocated
			Created: 12/10/2017
			Modified: 13/10/2017
		*/
		void Canvas::destroyInterface() {
			//Check data exists
			if (mData) {
				//Clear up the interactive map
				destroyInteractiveMap();

				//Destroy the stored UI elements
				for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i) {
					mData->uiElements[i]->destroyUI();
					delete mData->uiElements[i];
				}

				//Delete data
				delete mData;
				mData = nullptr;
			}
		}

		/*
			Canvas : update - Update and render the contained UI elements
			Created: 12/10/2017
			Modified: 13/10/2017
		*/
		void Canvas::update() {
			//Loop through existing active UI elements
			for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i) {
				//Check if the element is dead
				if (!mData->uiElements[i]->isAlive()) {
					//If element is IUIAction, invalidate navigation map
					if (mData->interactiveMap && dynamic_cast<UIElements::IUIAction*>(mData->uiElements[i]))
						destroyInteractiveMap();

					//Destroy the UI element
					mData->uiElements[i]->destroyUI();

					//Delete the UI element
					delete mData->uiElements[i];

					//Remove from the list
					mData->uiElements.erase(mData->uiElements.begin() + i);
				}
			}

			//Update the Actionable items
			updateActionUI();
			
			//Update and render all UI elements
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; i++) {
				mData->uiElements[i]->update();
				mData->uiElements[i]->render();
			}
		}

		/*
			Canvas : stashUI - Attempt to create and stash a new UI element
			Created: 13/10/2017
			Modified: 13/10/2017

			param[in] pNew - A pointer to the new UI element object

			return UIElements::IUIBase* - Returns the IUIBase pointer if setup wa ssuccessful, otherwise nullptr
		*/
		UIElements::IUIBase* Canvas::stashUI(UIElements::IUIBase* pNew) {
			//Attempt to create the UI element
			if (!pNew->createUI()) {
				//Clear up what it can
				pNew->destroyUI();

				//Delete the new element
				delete pNew;

				//Return failure
				return nullptr;
			}

			//Add the element to the internal list
			mData->uiElements.push_back(pNew);

			//Return the successful object
			return pNew;
		}

		/*
			Canvas : updateActionUI - Update the Actionable UI elements
			Created: 13/10/2017
			Modified: 15/10/2017
		*/
		void Canvas::updateActionUI() {
			//Update all of the Actionable elements
			if (mData->mapSize) {
				//Get the Axis Input object
				Input::AxisInput& IN = Globals::get<Input::AxisInput>();

				//Get the Mouse object
				const Input::Mouse& MOUSE = Globals::get<Input::Mouse>();

				//Get the new mouse position
				glm::ivec2 newPos = MOUSE.getPos();

				//Check if the position has changed
				if (newPos != mData->prevPos) {
					//Reset the selected item index
					mData->selectedAction = -1;

					//Loop through and see if the mouse overlaps any UI element
					UIElements::IUIBase* buffer = nullptr;
					for (size_t i = 0; i < mData->mapSize; i++) {
						//Get the element
						buffer = dynamic_cast<UIElements::IUIBase*>(mData->interactiveMap[i].element);

						//Check if contains mouse
						if (buffer->getLocation().contains(newPos.x, newPos.y)) {
							//Set the selected item index
							mData->selectedAction = (int)i;

							//Break the search
							break;
						}
					}

					//Save the new position
					mData->prevPos = newPos;
				}

				//Rely on Axis input 
				else {
					//Get the axis inputs
					float horizontal = (IN.btnPressed(mData->setup.horizontalAxis.c_str()) ? IN.getAxis(mData->setup.horizontalAxis.c_str()) : 0.f);
					float vertical = (IN.btnPressed(mData->setup.verticalAxis.c_str()) ? IN.getAxis(mData->setup.verticalAxis.c_str()) : 0.f);

					//Check there was input
					if (horizontal || vertical) {
						//Check if there is an existing  selection
						if (mData->selectedAction < 0) mData->selectedAction = 0;

						//Calculate movement direction
						else {
							//Get the movement direction
							int dir;
							if (abs(horizontal) > abs(vertical))
								dir = (horizontal < 0 ? Left : Right);
							else dir = (vertical < 0 ? Down : Up);

							//Get the next selected object
							auto next = mData->interactiveMap[mData->selectedAction].connections[dir];

							//Set the new activated object
							if (next) setActionObject(next->element);
						}
					}
				}

				//Update UI element state information
				for (size_t i = 0; i < mData->mapSize; i++) {
					//Skip over locked elements
					if (mData->interactiveMap[i].element->getState() == UIElements::EActionState::Locked) continue;

					//Otherwise set based on selected action
					else mData->interactiveMap[i].element->setState(mData->selectedAction == (int)i ? UIElements::EActionState::Highlighted : UIElements::EActionState::Default);
				}

				//Check for selection of UI element
				if (mData->selectedAction >= 0) {
					//Flag the action should be run
					bool takeAction = IN.btnPressed(mData->setup.confirmAxis.c_str());

					//Check if the mouse should be considered
					if (!takeAction && MOUSE.buttonPressed(Input::EMouseButton::Left)) {
						//Get the selected object
						UIElements::IUIBase* buffer = dynamic_cast<UIElements::IUIBase*>(mData->interactiveMap[mData->selectedAction].element);

						//Take action if clicked while in UI bounds
						takeAction = buffer->getLocation().contains(newPos.x, newPos.y);
					}

					//Call the action of the UI element
					if (takeAction) mData->interactiveMap[mData->selectedAction].element->action();
				}
			}
		}

		/*
			Canvas :destroyInteractiveMap - Clear up memory associated with the interactive map
			Created: 13/10/2017
			Modified: 13/10/2017
		*/
		void Canvas::destroyInteractiveMap() {
			//Check if the map exists
			if (mData->interactiveMap) {
				//Delete the elements
				delete[] mData->interactiveMap;

				//Reset the values
				mData->interactiveMap = nullptr;
				mData->mapSize = 0;
				mData->selectedAction = -1;
			}
		}
	}
}
