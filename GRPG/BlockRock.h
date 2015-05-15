#ifndef _BLOCK_ROCK_H                 // Prevent multiple definitions if this 
#define _BLOCK_ROCK_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Dragonfire.h"

namespace blockRockNS
{
	const char location[] = "assets/misc/blockrock.png";
	const int imageWidth = 30;
	const int imageHeight = 15;
	//Margin to move a bit further to prevent further collision detections.
	const int collideMargin = 2;
}

// The rock that is used to cover Aidil's cave, 
// It performs 2 tasks: 1. It sets a mining level of 20 required to at least access Aidil's cave.
// 2. It will also block off the player's exit during phase 2 and 3 on the Aidil fight, turning it into a 
// "DO or DIE" situation (sort of teleports for Epic Easter Eggs).
// This entity holds it's own TextureManager and will delete it once destroyed.
// It also has it's own initalization method, which should be used.
// Note that this rock will block the player's movement, and can only be mined while the player's y coordinate
// is less than this rock's y coordinate.
class BlockRock : public Entity
{
private:
	TextureManager* rockTexture;
	UI* ui;
protected:
	float fallSpeed;
	//Rock becomes disabled if mined.
	//If disabled, no longer blocks collision for the player and is not drawn onto the screen.
	//However, it is still in the game. It will be freed once the game ends and draw manager deletes it.
	//The rock is marked as "disabled" instead of outright deleted so that the spawner doesn't
	//spawn another one.
	bool disabled;
	//The player gains a huge mining bonus the first time he mines this rock.
	bool firstMine;
	Behavior* blockRockMineBehavior = nullptr;
public:
	// constructor
	BlockRock();

	// Destructor
	virtual ~BlockRock();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, string examineText);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual bool getDisabled() { return disabled; }
	virtual void setDisabled(bool d) { disabled = d; }
	virtual bool hasMiningBonus() { return firstMine; }
	virtual void mined() { firstMine = false; }

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (blockRockMineBehavior)
			vectorActiveBehaviors.push_back(blockRockMineBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}


	virtual string getType(){ return "BLOCK_ROCK"; }

	//Start's the rock falling animation. Moves the rock to the top of the screen and causes it to drop to it's
	//current location. 
	virtual void startFall(Viewport* vp);
};
#endif