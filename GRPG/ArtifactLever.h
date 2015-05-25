#ifndef _ARTIFACT_LEVER_H                 // Prevent multiple definitions if this 
#define _ARTIFACT_LEVER_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "GameEvent_ItemUsed.h"

namespace leverDisplayNS
{
	const char location[] = "assets/misc/lever.png";
	const int imageWidth = 6;
	const int imageHeight = 28;
	const int imageCols = 2;
}

//The artifact, encased in it's display case
class ArtifactLever : public Entity
{
private:
	TextureManager* leverTexture;
	UI* ui;
protected:
	bool pulled;

	Behavior* pullLeverBehavior = nullptr;
public:
	// constructor
	ArtifactLever();

	// Destructor
	virtual ~ArtifactLever();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, string examineText);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual bool getPulled() { return pulled; }
	virtual void setPulled(bool o);

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (pullLeverBehavior)
			vectorActiveBehaviors.push_back(pullLeverBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}


	virtual string getType(){ return "ARTIFACT_LEVER"; }

	virtual void setFrame(int i)
	{
		image.setCurrentFrame(i);
	}

};
#endif