#ifndef _SKILL_LEVEL_UP_H                 // Prevent multiple definitions if this 
#define _SKILL_LEVEL_UP_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"

namespace skillLevelUpNS
{
	const char arrowLocation[] = "assets/ui/up_arrow.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	//Margin to move a bit further to prevent further collision detections.
	const int collideMargin = 2;

	//Every second
	const int upSpeed = 30;
	const float timeDisplay = 3;
}

class UI;

// A skill level that appears over the player when the player levels up
//Will slowly rise at the speed of skillLevelUpNS::upSpeed every second
//After skillLevelUpNS::timeDisplay has passed, deletes itself and informs the next skillLevelUp that it has been deleted
//If one is created but it will block the previous skill level up, the above one is pushed up until it's lowest pixel is
//at least skillLevelUpNS::collidMargin pixels away from the highest pixel in the lower one.
//This propengates to all of them.
class SkillLevelUp : public Entity
{
private:
	float deathTimer;
	SkillLevelUp* previousLevelUp;
	SkillLevelUp* nextLevelUp;
	string skillName;
	TextDX* font;
	UI* ui;
	float fontHeightHalf;
protected:

public:
	// constructor
	SkillLevelUp(Image* upArrow, SkillLevelUp* previousLU, string sn, TextDX* theFont, UI* u);

	// Destructor
	virtual ~SkillLevelUp() {}

	// inherited member functions
	virtual void draw(Viewport* viewport);
	
	virtual bool initialize(Game* gamePtr)
	{
		graphics = gamePtr->getGraphics();
		theGame = gamePtr;

		return true;
	}

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	//No textures, do nothing
	virtual void onLostDevice() {}

	// Restore resources
	//No textures, do nothing
	virtual void onResetDevice() {}

	virtual string getType(){ return "SKILL_LEVEL_UP"; }

	virtual void setPreviousLevelUp(SkillLevelUp* slu)
	{
		previousLevelUp = slu;
	}
	
	virtual void setNextLevelUp(SkillLevelUp* slu)
	{
		nextLevelUp = slu;
	}

	virtual void performRepositioning();
};
#endif