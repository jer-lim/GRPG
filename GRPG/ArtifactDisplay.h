#ifndef _ARTIFACT_DISPLAY_H                 // Prevent multiple definitions if this 
#define _ARTIFACT_DISPLAY_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "GameEvent_ItemUsed.h"

namespace artifactDisplayNS
{
	const char location[] = "assets/misc/Artifact_display_case.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	const int imageCols = 4;
}

//The artifact, encased in it's display case
class ArtifactDisplay : public Entity
{
private:
	TextureManager* artifactTexture;
	UI* ui;
protected:

	bool open;
	
	Behavior* takeArtifactBehavior = nullptr;
public:
	// constructor
	ArtifactDisplay();

	// Destructor
	virtual ~ArtifactDisplay();

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
		if (takeArtifactBehavior)
			vectorActiveBehaviors.push_back(takeArtifactBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}


	virtual string getType(){ return "ARTIFACT_DISPLAY"; }

	virtual void setFrame(int i)
	{
		image.setCurrentFrame(i);
	}

};
#endif