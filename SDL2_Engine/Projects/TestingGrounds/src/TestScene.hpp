#pragma once

//! Include the base Scene object
#include <Scenes/ISceneBase.hpp>
#include <Resources/Resources.hpp>

/*
 *		Name: TestScene
 *		Author: Mitchell Croft
 *		Created: 11/10/2017
 *		Modified: 17/10/2017
 *		
 *		Purpose:
 *		Test the SDL2_Engine functionality
**/
class TestScene : public SDL2_Engine::Scenes::ISceneBase {
public:
	/*
		TestScene : createScene - Initialise the values for the Test Scene
		Created: 11/10/2017
		Modified: 17/10/2017

		return bool - Returns true if the Scene was initialised successfully
	*/
	bool createScene() override;
	
	/*
		TestScene : destroyScene - Deallocate memory used for the Test Scene
		Created: 11/10/2017
		Modified: 11/10/2017
	*/
	void destroyScene() override;

	/*
		TestScene : update - Update the TestScene values
		Created: 11/10/2017
		Modified: 16/10/2017
	*/
	void update() override;

	/*
		TestScene : render - Render the Test Scene
		Created: 11/10/2017
		Modified: 16/10/2017
	*/
	void render() override;

private:
	//! Store the cursor used for the window
	SDL2_Engine::LocalResource<SDL2_Engine::Cursor> mCursor;
};