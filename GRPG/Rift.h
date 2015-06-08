#ifndef _RIFT_H                 // Prevent multiple definitions if this 
#define _RIFT_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"

namespace riftNS
{
	const char location[] = "assets/misc/rift.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	const int imageCols = 1;

	//in seconds
	const int timeForFullRotation = 5;
}

//The rift, which start appearing during the Mysterious Artifact quest, and appears everywhere from now on.
class Rift : public Entity
{
private:
	TextureManager* riftTexture;
	UI* ui;

protected:
	Behavior* enterBehavior;
	virtual void setupBehaviors();
public:
	// constructor
	Rift();

	// Destructor
	virtual ~Rift();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, NPC* character);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (enterBehavior)
			vectorActiveBehaviors.push_back(enterBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}

	virtual string getType(){ return "RIFT"; }
};
#endif