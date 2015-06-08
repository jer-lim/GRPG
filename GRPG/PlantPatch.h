#ifndef _PLANT_PATCH_H                 // Prevent multiple definitions if this 
#define _PLANT_PATCH_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"

namespace plantPatchNS
{
	const char location[] = "assets/misc/patch.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	const int imageCols = 4;

	const int WEEDS = 0;
	const int EMPTY = 1;
	const int HOLE = 2;
	const int PLANTED = 3;
}

//The plant patch, used in the Quest "Mysterious artifact"
class PlantPatch : public Entity
{
private:
	TextureManager* plantPatchTexture;
	Behavior* inspectBehavior;
	Behavior* rakeBehavior;
	Behavior* digBehavior;
	Behavior* plantBehavior;
	UI* ui;
	//Refers to numbers as per defined in plantPatchNS: WEEDS, EMPTY, HOLE, PLANTED
	int status;
protected:
	
	string examineText;
	virtual void setupBehaviors();
public:
	// constructor
	PlantPatch();

	// Destructor
	virtual ~PlantPatch();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, string examineText);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual int getStatus() { return status; }
	virtual void setStatus(int s);

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (inspectBehavior)
			vectorActiveBehaviors.push_back(inspectBehavior);
		if (plantBehavior)
			vectorActiveBehaviors.push_back(plantBehavior);
		if (rakeBehavior)
			vectorActiveBehaviors.push_back(rakeBehavior);
		if (digBehavior)
			vectorActiveBehaviors.push_back(digBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}

	virtual string getType(){ return "PLANT_PATCH"; }
};
#endif