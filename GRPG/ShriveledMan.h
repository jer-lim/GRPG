#ifndef _SHRIVELED_MAN_H                 // Prevent multiple definitions if this 
#define _SHRIVELED_MAN_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Dragonfire.h"

namespace shriveledManNS
{
	const char location[] = "assets/npcs/sprites/shriveled_man.png";
	const int imageWidth = 64;
	const int imageHeight = 64;
}

//The shrieveld man, the main antagonist of the mysterious artifact questline
class ShriveledMan : public Entity
{
private:
	TextureManager* manTexture;
	UI* ui;
protected:

public:
	// constructor
	ShriveledMan();

	// Destructor
	virtual ~ShriveledMan();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, NPC* npc);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual string getType(){ return "SHRIVELED_MAN"; }
};
#endif