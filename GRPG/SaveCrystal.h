#ifndef _SAVE_CRYSTAL_H                 // Prevent multiple definitions if this 
#define _SAVE_CRYSTAL_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"

namespace saveCrystalNS
{
	const char location[] = "assets/misc/savecrystal.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	const int frames = 8;
}

// The save crystal, to save and load stats, as well as display stats
class SaveCrystal : public Entity
{
private:
	TextureManager* saveCrystalTexture;
	UI* ui;
protected:
	Behavior* useBehavior = nullptr;
	string examineText;
	virtual void setupBehaviors();
public:
	// constructor
	SaveCrystal();

	// Destructor
	virtual ~SaveCrystal();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, string examineText);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (useBehavior)
			vectorActiveBehaviors.push_back(useBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}


	virtual string getType(){ return "SAVE_CRYSTAL"; }
};
#endif