#include "Canvas.hpp"

//! Include the SDL2_Engine objects
#include "../Globals.hpp"
#include "../Debug/Logger.hpp"
#include "../Input/AxisInput.hpp"
#include "../Input/Mouse/Mouse.hpp"

#include "../Resources/Resources.hpp"
#include "../Resources/ResourceTypes/LocalResourceGeneric.hpp"
#include "../Resources/ResourceTypes/LocalResourceTexture.hpp"
#include "../Resources/ResourceTypes/LocalResourceFont.hpp"

#include "UIElements/Interfaces/IUIBase.hpp"
#include "UIElements/Interfaces/IUIAction.hpp"

#include "UIElements/UIPanel.hpp"
#include "UIElements/UILabel.hpp"
#include "UIElements/UIButton.hpp"
#include "UIElements/UITextbox.hpp"

#include "CanvasInitialiser.hpp"

//! Include the required STL containers
#include <vector>

//! Include GLM vector functions
#include <glm/ext.hpp>

//! Include required Objx Elements
#include <Objx_document.hpp>
#include <Objx_node.hpp>
#include <Objx_reader.hpp>

//! Flag the different possible movement directions
enum { Up, Down, Left, Right, Total };

namespace SDL2_Engine {
	namespace UI {
		/*
			deductLocation - Deduct the location of relative resources, stripping superfluous characters
			Author: Mitchell Croft
			Created: 13/10/2017
			Modified: 16/10/2017

			param[in] pParent - The original string location of a file
			param[in] pRelative - The file path of the second file, relative to pParent

			return std::string - Returns the clean filepath as a new std::string object
		*/
		std::string deductLocation(const std::string pParent, const std::string pRelative) {
			//Check if the pRelative is actually relative
			if (pRelative.find(':') != pRelative.npos) return pRelative;

			//Store a std::string of the current filepath
			std::string current = "";

			//Optomise the parent std::string
			int index = -1;
			size_t prog = 0;
			while (true) {
				//Grab the next std::string section
				std::string sub = pParent.substr(prog, (index = (int)pParent.find_first_of("/\\", index + 1)) + 1 - prog);

				//Check there was text extracted
				if (index == pParent.npos) break;

				//Check if its the current directory shorthand
				else if (sub == "./" || sub == ".\\") continue;

				//Check if its the directory up shorthand
				else if (sub == "../" || sub == "..\\")
					current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

				//Otherwise concatenate
				else current += sub;

				//Increase the progress
				prog += sub.length();
			}

			//Optomise the relative std::string
			index = -1;
			prog = 0;
			while (true) {
				//Grab the next std::string section
				std::string sub = pRelative.substr(prog, (index = (int)pRelative.find_first_of("/\\", index + 1)) + 1 - prog);

				//Increase the progress
				prog += sub.length();

				//Check if its the current directory shorthand
				if (sub == "./" || sub == ".\\") continue;

				//Check if its the directory up shorthand
				else if (sub == "../" || sub == "..\\")
					current = current.substr(0, current.find_last_of("/\\", current.length() - 2)) + '/';

				//Otherwise concatenate
				else current += sub;

				//Check there was text extracted
				if (index == pRelative.npos) {
					current += pRelative.substr(prog);
					break;
				}
			}

			//Return the combination
			return current;
		}

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
		 *		Modified: 16/10/2017
		 *		
		 *		Purpose:
		 *		Store internal Canvas data values
		**/
		struct Canvas::CanvasInternalData {
			//! Store a temporary pointer to the setup values
			const Initialisation::CanvasInitialiser* setup;

			//! Store all of the UI elements on the Canvas
			std::vector<UIElements::IUIBase*> uiElements;

			//! Keep an array of the Interactive UI Nodes
			InteractiveUINode* interactiveMap;

			//! Track how many elements are in the interactive map
			size_t mapSize;

			//! Flag the index of the currently selected object
			int selectedAction;

			//! Store the previous position of the mouse
			glm::ivec2 prevPos;

			//! Store the axis used to navigate the interactive elements 
			Input::vAxisName horizontalAxis;
			Input::vAxisName verticalAxis;
			Input::vAxisName confirmAxis;

			//! Store the callbacks used for loading UI elements
			Utilities::Action<UIElements::IUIBase*(const Objx::Node& pObj)> customLoadUI;
			Utilities::Action<void(UIElements::IUIAction* pObject, const uiTag& pTag)> actionSetup;

			//! Store the SDL2_Local Resources that are used by loaded UI elements
			std::vector<LocalResourceBase> loadedResources;

			/*
				CanvasInternalData : Constructor - Initialise with default values
				Created: 13/10/2017
				Modified: 16/10/2017

				param[in] pSetup - The CanvasInitialiser object to copy the values of
			*/
			inline CanvasInternalData(const Initialisation::CanvasInitialiser& pSetup) : setup(&pSetup), interactiveMap(nullptr), mapSize(0), selectedAction(-1) {}
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
			//Retrieve the logging object
			Debug::Logger& log = Globals::get<Debug::Logger>();

			//Attempt to load the file
			LocalResource<Generic> toLoad = Globals::get<Resources>().loadResource<Generic>(pFilepath);

			//Ensure that it was loaded correctly
			if (toLoad->status() != EResourceLoadStatus::Loaded) {
				log.logError("Canvas failed to load UI from Objx file '%s'. File was unable to be loaded", pFilepath);
				return false;
			}

			//Create the Objx Loader
			bool flag = false;
			Objx::Reader reader([&](const Objx::EStatusCode& pCode, const size_t& pLine, const size_t& pColumn, const char* pMsg) {
				//Output error header
				if (!flag) log.logError("Canvas encountered an error/errors when attempting to parse the Objx file '%s':", pFilepath);

				//Output error information
				log.logError("Line: %zu\tColumn: %zu\t\t%s\t(%s, %i)", pLine, pColumn, pMsg, Objx::statusCodeToString(pCode), (int)pCode);

				//Toggle flag
				flag = true;
			});

			//Attempt to parse the loaded data
			const Objx::Document DOC = reader.cparse(toLoad->data());

			//Check if an error was encountered
			if (flag) return false;

			//Check if there is a specific name to use
			if (pName) {
				//Get the element to navigate
				const Objx::xobjx SELECTED = DOC[pName];

				//Switch on the type of Node
				switch (SELECTED.getTypeMask()) {
				case Objx::ETypeMask::TYPE_OBJX:
					//Loop through properties
					SELECTED.forEachProp([&](const Objx::xstring& pName, const Objx::xobjx& pObj) {
						//Load the node
						loadUIElementFromObjx(pFilepath, pName.c_str(), pObj);

						//Continue the loop
						return true;
					});
					break;
				case Objx::ETypeMask::TYPE_OBJX | Objx::ETypeMask::TYPE_ARRAY:
					//Loop through the array
					for (auto it = SELECTED.cbegin<Objx::xobjx>(); it != SELECTED.cend<Objx::xobjx>(); ++it)
						loadUIElementFromObjx(pFilepath, "", *it);
					break;
				}
			}

			//Otherwise load from the Document
			else DOC.forEach([&](const Objx::xstring& pName, const Objx::xobjx& pObj) {
				//Load the node
				loadUIElementFromObjx(pFilepath, pName.c_str(), pObj);

				//Continue the loop
				return true;
			});

			//Rebuild the interaction map
			rebuildInteractionMap();

			//Return success
			return true;
		}

		/*
			Canvas : rebuildInteractionMap - Rebuild the internal interaction map to allow for users to navigate Actionable UI elements
			Created: 13/10/2017
			Modified: 02/11/2017
		*/
		void Canvas::rebuildInteractionMap() {
			//Clear the previous map
			destroyInteractiveMap();

			//Store the indexes of Action elements
			std::vector<UIElements::IUIAction*> interactable;
			interactable.reserve(mData->uiElements.size());

			//Calculate how many elements are in interactable
			for (size_t i = 0, COUNT = mData->uiElements.size(); i < COUNT; i++) {
				//Check if this UI element is alive
				if (!mData->uiElements[i]->isAlive()) continue;

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
			Modified: 16/10/2017

			return bool - Returns true if the Canvas was setup correctly
		*/
		bool Canvas::createInterface() {
			//Get the setup object
			auto setup = mData->setup;
			mData->setup = nullptr;

			//Copy over the setup values
			mData->horizontalAxis = setup->horizontalAxis;
			mData->verticalAxis = setup->verticalAxis;
			mData->confirmAxis = setup->confirmAxis;

			//Setup the callback actions
			mData->customLoadUI = setup->customLoader;
			mData->actionSetup = setup->actionSetup;
			
			//Retrieve the Logger and Axis Input objects
			const Debug::Logger& LOG = Globals::get<Debug::Logger>();
			const Input::AxisInput AXIS = Globals::get<Input::AxisInput>();

			//Check to see if the Axis names are present
			for (size_t i = 0; i < 3; i++) {
				//Check if the name exists
				if (!AXIS.hasAxis(setup->axisArray[i].c_str()))
					LOG.logWarning("Canvas object was given the as yet undefined Virtual Axis '%s' to use for player input navigation", setup->axisArray[i].c_str());
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
			Modified: 02/11/2017
		*/
		void Canvas::update() {
			//Loop through existing active UI elements
			for (int i = (int)mData->uiElements.size() - 1; i >= 0; --i) {
				//Check if the element is dead
				if (!mData->uiElements[i]->isAlive()) {
					//Check if the interaction map has been established
					if (mData->interactiveMap) {
						//Check to see if the element is an Actionable object
						if (auto temp = dynamic_cast<UIElements::IUIAction*>(mData->uiElements[i])) {
							//Loop through interaction map and see if the element is on it
							for (size_t i = 0; i < mData->mapSize; i++) {
								//Check if the objects are the same
								if (mData->interactiveMap[i].element == temp) {
									//Destroy the interactive map
									destroyInteractiveMap();

									//Break the search
									break;
								}
							}
						}
					}

					//Destroy the UI element
					mData->uiElements[i]->destroyUI();

					//Delete the UI element
					delete mData->uiElements[i];

					//Remove from the list
					mData->uiElements.erase(mData->uiElements.begin() + i);
				}
			}

			//Check if there are elements left
			if (!mData->uiElements.size() && mData->loadedResources.size())
				mData->loadedResources.clear();

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
			Canvas : loadUIElementFromObjx - Given an Objx Node descriptor, create a UI element
			Created: 16/10/2017
			Modified: 16/10/2017

			param[in] pOrigin - The filepath of the original Objx file loaded
			param[in] pName - The name that the Node was stored under
			param[in] pNode - The Objx Node to read UI setup data from
		*/
		void Canvas::loadUIElementFromObjx(const char* pOrigin, const char* pName, const Objx::Node& pNode) {
			//Create a buffer to store the element
			UIElements::IUIBase* buffer = nullptr;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////--------------------------------Load Type-Specific Data------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//Get the type of the Node
			Objx::xstring nodeType = pNode["type"].readVal<Objx::xstring>();

			//If the type is built in, load specific values
			if (nodeType == "panel") {
				//Create the panel object
				UIElements::UIPanel* panel = (UIElements::UIPanel*)stashUI(new UIElements::UIPanel());

				//Check it was created
				if (!panel) return;

				//Read the Colour Values
				unsigned int colourBuffer = 0;

				//Get the filter or fill colour
				const Objx::xstring fillColour = (pNode.hasProperty("filterColour") ? pNode["filterColour"].readVal<Objx::xstring>() : pNode["fillColour"].readVal<Objx::xstring>("FFFFFFFF"));

				//Read the Fill Colour
				sscanf_s(fillColour.c_str(), "%x", &colourBuffer);

				//Set the fill/filter colour
				panel->setFillColour(colourBuffer);

				//Get the border colour
				sscanf_s(pNode["borderColour"].readVal<Objx::xstring>("000000FF").c_str(), "%x", &colourBuffer);

				//Set the border colour
				panel->setBorderColour(colourBuffer);

				//Load an image
				if (pNode.hasProperty("image")) {
					//Attempt to load the picture
					LocalResource<Texture> img = Globals::get<Resources>().loadResource<Texture>(deductLocation(pOrigin, pNode["image"].readVal<Objx::xstring>()).c_str());

					//Check the status of the image
					if (img->status() == EResourceLoadStatus::Loaded) {
						//Assign the image to the panel
						panel->setImage(img->texture);

						//Store the local resource
						mData->loadedResources.push_back(img);
					}
				}

				//Assign the panel to the buffer
				buffer = panel;
			} else if (nodeType == "label") {
				//Create the label object
				UIElements::UILabel* label = (UIElements::UILabel*)stashUI(new UIElements::UILabel());

				//Check it was created
				if (!label) return;

				//Read the text
				label->setText(pNode["text"].readVal<Objx::xstring>().c_str());

				//Load a font
				if (pNode.hasProperty("font")) {
					//Attempt to load the font
					LocalResource<Font> fnt = Globals::get<Resources>().loadResource<Font>(deductLocation(pOrigin, pNode["font"].readVal<Objx::xstring>()).c_str(), pNode["fontSize"].readVal<Objx::xint>(12), pNode["fontIndex"].readVal<Objx::xint>());

					//Check the status of the font
					if (fnt->status() == EResourceLoadStatus::Loaded) {
						//Assign the font to the label
						label->setFont(fnt->font);

						//Store the local resource
						mData->loadedResources.push_back(fnt);
					}
				}

				//Read the Colour Values
				unsigned int colourBuffer = 0;

				//Get the text colour
				sscanf_s(pNode["colour"].readVal<Objx::xstring>("000000FF").c_str(), "%x", &colourBuffer);

				//Set the text Colour
				label->setColour(colourBuffer);

				//Get the text alignment
				const Objx::xstring TXT_ALIGN = pNode["alignment"].readVal<Objx::xstring>();

				//Find the alignment
				if (TXT_ALIGN == "left") label->setAlignment(Rendering::ETextAlignment::Left);
				else if (TXT_ALIGN == "center") label->setAlignment(Rendering::ETextAlignment::Center);
				else if (TXT_ALIGN == "right") label->setAlignment(Rendering::ETextAlignment::Right);
				else label->setAlignment(Rendering::ETextAlignment::Null);

				//Find the rendering type
				if (pNode["renderType"].readVal<Objx::xstring>() == "blended")
					label->setRenderType(Rendering::ETextRenderType::Blended);
				else label->setRenderType(Rendering::ETextRenderType::Solid);

				//Assign the label to the buffer
				buffer = label;
			} else if (nodeType == "button") {
				//Create the label object
				UIElements::UIButton* btn = (UIElements::UIButton*)stashUI(new UIElements::UIButton());

				//Check it was created
				if (!btn) return;

				//Read the text
				btn->setText(pNode["text"].readVal<Objx::xstring>().c_str());

				//Load a font
				if (pNode.hasProperty("font")) {
					//Attempt to load the font
					LocalResource<Font> fnt = Globals::get<Resources>().loadResource<Font>(deductLocation(pOrigin, pNode["font"].readVal<Objx::xstring>()).c_str(), pNode["fontSize"].readVal<Objx::xint>(12), pNode["fontIndex"].readVal<Objx::xint>(0));

					//Check the status of the font
					if (fnt->status() == EResourceLoadStatus::Loaded) {
						//Assign the font to the label
						btn->setFont(fnt->font);

						//Store the local resource
						mData->loadedResources.push_back(fnt);
					}
				}

				//Read the Colour Values
				unsigned int colourBuffer = 0;

				//Read the text colours
				if (pNode["textColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the text colours
					Objx::Node textColours = pNode["textColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = textColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						btn->setTextColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Read the fill colours
				if (pNode["fillColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the fill colours
					Objx::Node fillColours = pNode["fillColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = fillColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						btn->setFillColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Read the border colours
				if (pNode["borderColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the border colours
					Objx::Node borderColours = pNode["borderColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = borderColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						btn->setBorderColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Find the rendering type
				if (pNode["renderType"].readVal<Objx::xstring>() == "blended")
					btn->setRenderType(Rendering::ETextRenderType::Blended);
				else btn->setRenderType(Rendering::ETextRenderType::Solid);

				//Assign the button to the buffer
				buffer = btn;
			} else if (nodeType == "textbox") {
				//Create the label object
				UIElements::UITextbox* txt = (UIElements::UITextbox*)stashUI(new UIElements::UITextbox());

				//Check it was created
				if (!txt) return;

				//Read the text
				txt->setText(pNode["text"].readVal<Objx::xstring>().c_str());

				//Load a font
				if (pNode.hasProperty("font")) {
					//Attempt to load the font
					LocalResource<Font> fnt = Globals::get<Resources>().loadResource<Font>(deductLocation(pOrigin, pNode["font"].readVal<Objx::xstring>()).c_str(), pNode["fontSize"].readVal<Objx::xint>(12), pNode["fontIndex"].readVal<Objx::xint>(0));

					//Check the status of the font
					if (fnt->status() == EResourceLoadStatus::Loaded) {
						//Assign the font to the label
						txt->setFont(fnt->font);

						//Store the local resource
						mData->loadedResources.push_back(fnt);
					}
				}

				//Read the maximum length
				txt->setMaxLength(pNode["maxLength"].readVal<Objx::xint>(-1));

				//Read the input flags
				if (pNode.hasProperty("inputFlags")) {
					//Create a bitmask to store the value
					Utilities::Bitmask<Input::EKeyboardInputFlags> flags;

					//Get the flags object
					Objx::Node inputFlags = pNode["inputFlags"];

					//Iterate through the possible flags
					for (auto it = inputFlags.cbegin<Objx::xstring>(); it != inputFlags.cend<Objx::xstring>(); ++it) {
						//Check the string
						if (*it == "alphabetical")
							flags |= Input::EKeyboardInputFlags::Alphabetical;
						else if (*it == "numerical")
							flags |= Input::EKeyboardInputFlags::Numerical;
						else if (*it == "special")
							flags |= Input::EKeyboardInputFlags::Special;
						else if (*it == "space")
							flags |= Input::EKeyboardInputFlags::Space;
						else if (*it == "alphanumerical")
							flags |= Input::EKeyboardInputFlags::AlphaNumerical;
						else if (*it == "all")
							flags |= Input::EKeyboardInputFlags::All;
					}

					//Set the input flags
					txt->setInputFlags(flags);
				}

				//Read the Colour Values
				unsigned int colourBuffer = 0;

				//Read the text colours
				if (pNode["textColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the text colours
					Objx::Node textColours = pNode["textColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = textColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						txt->setTextColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Read the fill colours
				if (pNode["fillColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the fill colours
					Objx::Node fillColours = pNode["fillColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = fillColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						txt->setFillColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Read the border colours
				if (pNode["borderColours"].isType<Objx::xstring*>()) {
					//Store the string value
					Objx::xstring val = "";

					//Get the border colours
					Objx::Node borderColours = pNode["borderColours"];

					//Loop through the states
					for (size_t i = 0; i < 3; i++) {
						//Read the string
						val = borderColours.readArray<Objx::xstring>(i);

						//Check there is a value
						if (!val.length()) break;

						//Convert the colour value
						sscanf_s(val.c_str(), "%x", &colourBuffer);

						//Set the text colour
						txt->setBorderColour((UIElements::EActionState)i, colourBuffer);
					}
				}

				//Find the rendering type
				if (pNode["renderType"].readVal<Objx::xstring>() == "blended")
					txt->setRenderType(Rendering::ETextRenderType::Blended);
				else txt->setRenderType(Rendering::ETextRenderType::Solid);

				//Assign the textbox to the buffer
				buffer = txt;
			}

			//Otherwise pass it on to the custom UI callback
			else mData->customLoadUI(buffer, pNode);

			//Check it exists
			if (!buffer) return;

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Parse Common Data---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////
			
			//Set the tag
			buffer->setTag(pNode.hasProperty("tag") ? pNode["tag"].readVal<Objx::xstring>().c_str() : pName);

			//Set the position information
			buffer->setLocation({
				pNode["x"].readVal<Objx::xint>(),
				pNode["y"].readVal<Objx::xint>(),
				pNode["width"].readVal<Objx::xint>(),
				pNode["height"].readVal<Objx::xint>()
			});

			/////////////////////////////////////////////////////////////////////////////////////////////////////
			////////-----------------------------------Setup Action Data---------------------------------////////
			/////////////////////////////////////////////////////////////////////////////////////////////////////

			//Callback to get action setup information
			if (UIElements::IUIAction* action = dynamic_cast<UIElements::IUIAction*>(buffer))
				mData->actionSetup(action, buffer->getTag());
		}

		/*
			Canvas : updateActionUI - Update the Actionable UI elements
			Created: 13/10/2017
			Modified: 16/10/2017
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
					float horizontal = (IN.btnPressed(mData->horizontalAxis.c_str()) ? IN.getAxis(mData->horizontalAxis.c_str()) : 0.f);
					float vertical = (IN.btnPressed(mData->verticalAxis.c_str()) ? IN.getAxis(mData->verticalAxis.c_str()) : 0.f);

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
					bool takeAction = IN.btnPressed(mData->confirmAxis.c_str());

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
