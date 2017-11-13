#include "GameManager.hpp"

//! Include the required SDL2_Engine objects
#include <Globals.hpp>
#include <Debug/Logger.hpp>
#include "../Powerups/PowerupValues.hpp"

using namespace SDL2_Engine;

//! Store the length of time the speed scale to apply to explosion tiles
#define EXPLOSION_SPEED_MULT 2.f

namespace BombSquad {
	//! Define the singleton instance
	GameManager* GameManager::mInstance = nullptr;

	/*
		random - Get a 'random' float value in the scale of 0 - 1 (inclusive)
		Author: Mitchell Croft
		Created: 09/11/2017
		Modified: 09/11/2017

		return float - Returns a float value in the 0 - 1 range
	*/
	inline float random() { return ((float)rand() / (float)RAND_MAX); }

	/*
		sign - Get the sign of the specified value
		Author: Mitchell Croft
		Created: 12/11/2017
		Modified: 12/11/2017

		Template T - The type of pVal

		param[in] pVal - The value to be assessed

		return T - Returns either 1 or -1 in the type of T depending on pVal
	*/
	template<typename T>
	inline T sign(const T& pVal) { return (pVal < (T)0 ? (T)-1 : (T)1); }

	/*
		GameManager : create - Create the singleton instance
		Created: 10/11/2017
		Modified: 10/11/2017

		return bool - Returns true if the Game Manager was created successfully
	*/
	bool GameManager::create() {
		//Ensure that the instance hasn't been created yet
		assert(!mInstance);

		//Create the singleton instance
		mInstance = new GameManager();

		//Load the initial powerups 
		mInstance->mBlueprints = mInstance->mPowerLoader.loadDirectory("resources/Powerups/");

		//Return success
		return true;
	}

	/*
		GameManager : destroy - Deallocate memory used by Game Manager
		Created: 10/11/2017
		Modified: 10/11/2017
	*/
	void GameManager::destroy() {
		//Check the instance is setup
		if (mInstance) {
			//Remove the players 
			for (size_t i = 0; i < mInstance->mPlayers.size(); i++)
				mInstance->mPlayers[i].controls.unbindControlScheme();
			mInstance->mPlayers.clear();
			
			//Clear all active powerups
			mInstance->mPowerups.clear();
			mInstance->mBlueprints.clear();
			mInstance->mPowerLoader.dispose();

			//Clear the bombs
			mInstance->mBombs.clear();
			mInstance->mDeadlyTiles.dispose();

			//Clear the Singleton instance
			delete mInstance;
			mInstance = nullptr;
		}
	}

	/*
		GameManager : setPlayerControls - Set the control schemes the players will use to control their characters
		Created: 10/11/2017
		Modified: 10/11/2017

		param[in] pSchemes - A standard vector of ControlScheme objects to use

		NOTE:
		This also sets the number of players that will be in the game
	*/
	void GameManager::setPlayerControls(const std::vector<ControlScheme>& pSchemes) {
		//Check if there are players already in the list
		if (mInstance->mPlayers.size()) {
			//Remove the players 
			for (size_t i = 0; i < mInstance->mPlayers.size(); i++)
				mInstance->mPlayers[i].controls.unbindControlScheme();
			mInstance->mPlayers.clear();
		}

		//Resize the players vector to for the incoming values
		mInstance->mPlayers.resize(pSchemes.size());

		//Store the names of the Animations to load
		const char* PLAYER_ANIM_NAMES[] = { "player1", "player2", "player3", "player4" };

		//Create new player objects
		for (size_t i = 0; i < pSchemes.size(); i++) {
			//Get a reference to the current player object
			Player& curr = mInstance->mPlayers[i];

			//Set the player ID
			curr.id = (EPlayerID)i;

			//Setup the player animator
			if (!curr.animator.loadAnimations("resources/Animations/playerAnimations.objx", PLAYER_ANIM_NAMES[i])) {
				Globals::get<Debug::Logger>().logError("The Game Manager failed to load the animations for '%s' with the same name in the file 'resources/Animations/playerAnimations.objx'", PLAYER_ANIM_NAMES[i]);
			}

			//Assign the control scheme
			curr.controls = pSchemes[i];

			//Bind the controls
			if (!curr.controls.bindControlScheme((EPlayerNumber)((i + 1) * -1))) {
				Globals::get<Debug::Logger>().logError("The Game Manager failed to bind the controls for player %zu", i + 1);
			}
		}
	}

	/*
		GameManager : setupNewMap - Reset all values for a new game
		Created: 10/11/2017
		Modified: 13/11/2017
	*/
	void GameManager::setupNewGame() {
		//Clear all active powerups
		mInstance->mPowerups.clear();

		//Clear the bombs
		mInstance->mBombs.clear();
		mInstance->mDeadlyTiles.dispose();

		//Generate the new map
		mInstance->mCurrentMap = mInstance->mGenerator.generateMap(mInstance->mMapWidth, mInstance->mMapHeight, mInstance->mPlayers.size());

		//Create the deadly tile map
		mInstance->mDeadlyTiles.setBounds(mInstance->mCurrentMap.width(), mInstance->mCurrentMap.height());

		//Loop through and setup the players
		size_t index = 0;
		for (size_t x = 0; x < mInstance->mCurrentMap.width() && index != mInstance->mPlayers.size(); x++) {
			for (size_t y = 0; y < mInstance->mCurrentMap.height() && index != mInstance->mPlayers.size(); y++) {
				//Check if the tile is spawn point
				if (mInstance->mCurrentMap[x][y] == ETileType::SpawnPoint) {
					//Get a reference to the current player object
					Player& curr = mInstance->mPlayers[index++];

					//Reset the properties of the player
					curr.properties.reset();

					//Reset the animator back to default
					curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Down);
					curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Down);

					//Reset the alive flag
					curr.alive = true;

					//Stash the current position for the player
					curr.position = { x, y };

					//Reset the movement progress values
					curr.moveProg = { 0.f, 0.f };

					//Reset the placed bombs count
					curr.placedBombs = 0;
				}
			}
		}

		//Flag the game as not over
		mInstance->mGameOver = false;
	}

	/*
		GameManager : update - Update the current game state values
		Created: 10/11/2017
		Modified: 13/11/2017

		param[in] pDelta - The delta time for the current cycle
	*/
	void GameManager::update(const float& pDelta) {
		//Count the number of alive players
		size_t alive = 0;

		//Update the players
		for (size_t i = 0; i < mInstance->mPlayers.size(); i++) {
			//Get a reference to the current player
			Player& curr = mInstance->mPlayers[i];

			//Check the player is alive
			if (curr.alive) {
				//Update the properties
				curr.properties.update(pDelta);

				//Get the movement displacement
				float displacement = curr.properties.getSpeed() * pDelta;

				//Check if the player is moving
				if (curr.moveProg.x || curr.moveProg.y) {
					//Currently moving horizontally
					if (curr.moveProg.x) {
						//Further progression
						curr.moveProg.x += displacement * sign(curr.moveProg.x);

						//Check for tile completion
						if (abs(curr.moveProg.x) >= 1.f) {
							//Increase the position
							curr.position += coord((int)sign(curr.moveProg.x), 0);

							//Reset the move progress
							curr.moveProg.x = curr.moveProg.y = 0.f;
						}
					}

					//Currently moving vertically
					else {
						//Further progression
						curr.moveProg.y += displacement * sign(curr.moveProg.y);

						//Check for tile completion
						if (abs(curr.moveProg.y) >= 1.f) {
							//Increase the position
							curr.position += coord(0, (int)sign(curr.moveProg.y));

							//Reset the move progress
							curr.moveProg.x = curr.moveProg.y = 0.f;
						}
					}
				}

				//Otherwise check if the player is moving
				else {
					//Get movement axis
					float vertical = curr.controls.actionAxis(EPlayerAction::Move_Vertical)     * (curr.properties.isReversed() ? -1.f : 1.f);
					float horizontal = curr.controls.actionAxis(EPlayerAction::Move_Horizontal) * (curr.properties.isReversed() ? -1.f : 1.f);

					//Check if there is any movement
					if (vertical || horizontal) {
						//If vertical input is stronger
						if (abs(vertical) > abs(horizontal)) {
							//Check the coordinate is traversable
							if (!isTileBlocked(curr.position + coord(0, (int)sign(vertical))))
								curr.moveProg.y += displacement * sign(vertical);
						}

						//If horizontal input is stronger
						else {
							//Check the coordinate is traversable
							if (!isTileBlocked(curr.position + coord((int)sign(horizontal), 0)))
								curr.moveProg.x += displacement * sign(horizontal);
						}
					}
				}

				//Set the animator values
				if (curr.moveProg.x || curr.moveProg.y) {
					if (curr.moveProg.x) {
						if (curr.moveProg.x < 0.f) {
							curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Left);
							curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Left);
						} else {
							curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Right);
							curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Right);
						}
					} else {
						if (curr.moveProg.y < 0.f) {
							curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Up);
							curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Up);
						} else {
							curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Down);
							curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Down);
						}
					}
				}

				//Ensure the game isnt over
				if (!mInstance->mGameOver) {
					//Check for powerup collection
					auto powerup = mInstance->mPowerups.find(curr.position);
					if (powerup != mInstance->mPowerups.end()) {
						//Get the actor who is effected
						const EPowerupActor& ACTOR = powerup->second->getActor();

						//Store a pointer to the actor to affect
						Player* toEffect = nullptr;

						//Find the player to affect
						switch (ACTOR) {
						case EPowerupActor::Self: toEffect = &curr; break;
						case EPowerupActor::Enemy:
							do {
								toEffect = &mInstance->mPlayers[rand() % mInstance->mPlayers.size()];
							} while (!toEffect || !toEffect->alive || toEffect->id == curr.id);
							break;
						case EPowerupActor::Random:
							do {
								toEffect = &mInstance->mPlayers[rand() % mInstance->mPlayers.size()];
							} while (!toEffect || !toEffect->alive);
						}

						//Get the mask for the powerup
						const auto& MASK = powerup->second->getMask();

						//Check for detonate all flag
						if (MASK & EPowerupEffectFlags::Detonate_All) {
							//'Detonate' all the bombs
							for (auto it = mInstance->mBombs.begin(); it != mInstance->mBombs.end(); ++it)
								it->second.timer = 0.f;
						}

						//Ensure there is an actor for the remaining powerups
						if (toEffect) {
							//Check for teleport
							if (MASK & EPowerupEffectFlags::Teleport) {
								//Create a list of free coordinates
								coords available;

								//Reserve enough space for all points
								available.reserve(mInstance->mCurrentMap.width() * mInstance->mCurrentMap.height());

								//Loop through the map and find available spaces
								for (size_t x = 0; x < mInstance->mCurrentMap.width(); x++) {
									for (size_t y = 0; y < mInstance->mCurrentMap.height(); y++) {
										//Check the tile type
										if (mInstance->mCurrentMap[x][y] != ETileType::Restricted &&
											mInstance->mCurrentMap[x][y] != ETileType::Obstacle)
											available.push_back({ x, y });
									}
								}

								//Move the target to the random coordinate
								toEffect->position = available[rand() % available.size()];
							}

							//Check if the powerup is a one off only
							if (MASK & ((int)EPowerupEffectFlags::Detonate_All | (int)EPowerupEffectFlags::Teleport) &&		//If the one off flags are in the mask
								(MASK == ((int)EPowerupEffectFlags::Detonate_All | (int)EPowerupEffectFlags::Teleport) ||	//If the mask is only the combined one offs
									MASK == EPowerupEffectFlags::Detonate_All ||											//If the mask is only the detonate flag
									MASK == EPowerupEffectFlags::Teleport));

							//Otherwise apply the powerup to the target
							else toEffect->properties += *powerup->second;
						}

						//Remove the powerup from the list
						mInstance->mPowerups.erase(powerup);
					}

					//Check for bomb placement
					if (curr.controls.actionPressed(EPlayerAction::Place_Bomb) &&	//User pressed to drop a bomb
						(int)curr.placedBombs < curr.properties.getBombCount() &&		//The player can place more bombs					
						!isTileBlocked(curr.position)) {							//The tile is free
						//Increment the players bomb count
						curr.placedBombs++;

						//Create the bomb object
						Bomb newBomb;
						newBomb.owner = curr.id;
						newBomb.radius = curr.properties.getBlastRadius();
						newBomb.isMine = curr.properties.hasMines();
						newBomb.fuseLength = newBomb.timer = curr.properties.getFuseLength();

						//Add a new bomb to the list
						mInstance->mBombs.insert(std::pair<coord, Bomb>(curr.position, newBomb));
					}

					//Check for mine detonation
					if (curr.controls.actionPressed(EPlayerAction::Detonate_Mine)) {
						//Loop through available bombs and look for linked IDs
						for (auto it = mInstance->mBombs.begin(); it != mInstance->mBombs.end(); ++it) {
							if (it->second.owner == curr.id &&		//Check for ID match
								it->second.isMine)					//Check the bomb is a mine
								it->second.timer = 0.f;
						}
					}
				}

				//Check if the player is on deadly tile
				if (mInstance->mDeadlyTiles[curr.position.x][curr.position.y]) {
					//Kill the player
					curr.alive = false;

					//Play the death animation
					curr.animator.setAnimation(EAnimationSet::Head, EAnimation::Death);
					curr.animator.setAnimation(EAnimationSet::Body, EAnimation::Death);
				}
			}

			//Update the player animator
			curr.animator.update(pDelta);

			//Increment the alive counter
			alive += (curr.alive ? 1 : 0);
		}

		//Update the bombs
		for (auto it = mInstance->mBombs.begin(); it != mInstance->mBombs.end();) {
			//Burn fuse if bomb is not a mine
			if (!it->second.isMine) it->second.timer -= pDelta;

			//Check if the bomb has 'exploded'
			if (it->second.timer <= 0.f) {
				//Free up one of the owners bomb allocations
				mInstance->mPlayers[(int)it->second.owner].placedBombs--;

				//Store an array of the directions to apply destruction
				const coord OFFSETS[] = { { -1, 0 }, { 1, 0 }, { 0, -1 }, { 0, 1 } };
				const size_t OFFSET_COUNT = sizeof(OFFSETS) / sizeof(const coord);

				//Get the blast radius
				const int& BLST_RAD = it->second.radius;

				//Loop through the offset directions to apply blasts to
				for (size_t off = 0; off < OFFSET_COUNT; off++) {
					//Loop through for the radius
					for (int i = (off ? 1 : 0); i < BLST_RAD; i++) {
						//Get the coordinate to process
						const coord BLST = it->first + OFFSETS[off] * i;

						//Check the coordinate is on the map
						if (BLST.x < 0 || BLST.y < 0 ||
							BLST.x >= (int)mInstance->mCurrentMap.width() || BLST.y >= (int)mInstance->mCurrentMap.height())
							break;

						//Check for blocked tile
						else if (mInstance->mCurrentMap[BLST.x][BLST.y] == ETileType::Restricted) break;

						//Check for obstacle
						else if (mInstance->mCurrentMap[BLST.x][BLST.y] == ETileType::Obstacle) {
							//Set the tile to free
							mInstance->mCurrentMap[BLST.x][BLST.y] = ETileType::Free;

							//Check for powerup drop
							if (random() <= mInstance->mPowerupChance) 
								mInstance->mPowerups.insert(std::pair<coord, const Powerup*>(BLST, &mInstance->mBlueprints[rand() % mInstance->mBlueprints.size()]));

							//Stop the explosion short
							break;
						}

						//Check for powerup
						auto powerup = mInstance->mPowerups.find(BLST);
						if (powerup != mInstance->mPowerups.end()) {
							//Destroy the powerup
							mInstance->mPowerups.erase(powerup);

							//Stop the explosion short
							break;
						}

						//Check for bomb
						auto bomb = mInstance->mBombs.find(BLST);
						if (bomb != mInstance->mBombs.end() && bomb != it) {
							//Flag the bomb for processed 'destruction'
							bomb->second.timer = 0.f;

							//Stop the explosion short
							break;
						}

						//Flag the tile as deadly
						mInstance->mDeadlyTiles[BLST.x][BLST.y] = 1.f;
					}
				}

				//Remove the bomb from the list
				mInstance->mBombs.erase(it++);
			}

			//Increment the iterator
			else ++it;
		}

		//Update the deadly tiles
		for (size_t x = 0; x < mInstance->mDeadlyTiles.width(); x++) {
			for (size_t y = 0; y < mInstance->mDeadlyTiles.height(); y++) {
				//Check if the tile needs processing
				if (mInstance->mDeadlyTiles[x][y]) {
					//Subtract from the timer value
					mInstance->mDeadlyTiles[x][y] -= EXPLOSION_SPEED_MULT * pDelta;

					//Check for sub zero
					if (mInstance->mDeadlyTiles[x][y] < 0.f)
						mInstance->mDeadlyTiles[x][y] = 0.f;
				}
			}
		}

		//Check for match over
		if (!mInstance->mGameOver && alive < 2) {
			//Iterate over players to update scores
			for (size_t i = 0; i < mInstance->mPlayers.size(); i++) {
				//Get the current player
				Player& curr = mInstance->mPlayers[i];
				
				//Check alive status
				if (curr.alive) curr.wins++;
				else curr.losses++;
			}

			//Flag the game over
			mInstance->mGameOver = true;
		}
	}

	/*
		GameManager : isTileBlocked - Check to see if a specific tile is currently blocked
		Created: 12/11/2017
		Modified: 13/11/2017

		param[in] pPos - A coord object containing the positions to check

		return bool - Returns true if the tile is blocked
	*/
	bool GameManager::isTileBlocked(const coord& pPos) {
		//Check the coordinate is in bounds
		if (pPos.x < 0 || pPos.y < 0 ||
			pPos.x >= (int)mInstance->mCurrentMap.width() || pPos.y >= (int)mInstance->mCurrentMap.height())
			return true;

		//Check if the Tile Coordinate is Blocked or an Obstacle
		else if (mInstance->mCurrentMap[pPos.x][pPos.y] == ETileType::Restricted ||
			mInstance->mCurrentMap[pPos.x][pPos.y] == ETileType::Obstacle)
			return true;

		//Check if there is a bomb at the coordinate
		else return (mInstance->mBombs.find(pPos) != mInstance->mBombs.end());
	}

	/*
		GameManager : Constructor - Initialise with default values
		Created: 10/11/2017
		Modified: 13/11/2017
	*/
	GameManager::GameManager() : 
		mGameOver(true),
		mMapWidth(0), 
		mMapHeight(0)
	{}
}
