#ifndef _RICH_DOOR_H                 // Prevent multiple definitions if this 
#define _RICH_DOOR_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "GameEvent_ItemUsed.h"

namespace richDoorNS
{
	const char location[] = "assets/misc/door.png";
	const int imageWidth = 32;
	const int imageHeight = 32;

	//Margin to move a bit further to prevent further collision detections.
	const int collideMargin = 2;
}

// The rich guy's door.
class RichDoor : public Entity
{
private:
	TextureManager* doorTexture;
	UI* ui;
protected:

	bool open;
	//The player gains a huge mining bonus the first time he mines this rock.
	bool firstMine;
	Behavior* openBehavior = nullptr;
	Behavior* pickLockBehavior = nullptr;
public:
	// constructor
	RichDoor();

	// Destructor
	virtual ~RichDoor();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, string examineText);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual bool getOpen() { return open; }
	virtual void setOpen(bool o);

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (openBehavior)
			vectorActiveBehaviors.push_back(openBehavior);
		if (pickLockBehavior)
			vectorActiveBehaviors.push_back(pickLockBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}


	virtual string getType(){ return "RICH_DOOR"; }
};
#endif