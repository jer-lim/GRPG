#ifndef _AIDIL_H                 // Prevent multiple definitions if this 
#define _AIDIL_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Dragonfire.h"

namespace aidilNS
{
	const char dragonfireLocation[] = "assets/misc/dragonfire.png";
	const int dragonfireWidth = 32;
	const int dragonfireHeight = 32;

	const float dragonfireStartDelay = 5;
	const float dragonfireNormalDelay = 15;
	const float dragonfireDelayDeviation = 5;
	const float dragonfireWarningTime = 2;
	const float dragonfireActiveTime = 4;
	const float dragonfireSpread = PI/6;
	const float dragonfireSpeed = 60;
	const float dragonfireSpawnedPerSecond = 1000;
	const int dragonfireDistance = 400;
	const int dragonfireMinDamage = 20;
	const int dragonfireMaxDamage = 45;

	const int DRAGONFIRE_COOLDOWN = 1;
	const int DRAGONFIRE_WARNING = 2;
	const int DRAGONFIRE_ACTIVE = 3;
	
	const DWORD DRAGONFIRE_WARN_COLOUR = SETCOLOR_ARGB(60, 255, 0, 0);

	const char aidilFlyLocation[] = "assets/npcs/sprites/aidil_fly.png";
	const int aidilFlyWidth = 128;
	const int aidilFlyHeight = 128;
	const int aidilFlyCols = 6;

	const float phaseTwoTime = 20;
	const int healthThresholdForPhase2 = 133;
	//Time it takes for Aidil to ascend to the skies or descend (in seconds).
	const float flyAnimationTime = 2;
}

// Yes Aidil you get your own class, congratulations!
// Be prepared to be an even bigger boss like no other!
// This aidil class will contain the aidil mechanics that aidil will use in his fights,
// mainly the timed dragonfire as well as the phase transitions between the 3 phases.
class Aidil : public Entity
{
private:
	//Stores the current phase aidil is currently in (1, 2, 3).
	//If phase 2+ and kills player, MUST call reset() to reset the fight else the player will never
	//be able to fight aidil again.
	int currentPhase;

	bool combatBegun;

	Image* dragonfireImage;
	TextureManager* dragonfireTexture;
	Player* thePlayer;
	float dragonfireCooldownTimer;
	//The current direction aidil is blowing his dragonfire at.
	//-1 for no dragonfire currently active.
	//WARNING: in radians.
	float currentDragonfireAngle;
	//The two different angles that dictate the range of the dragonfire.
	float topDragonfireAngle;
	float botDragonfireAngle;

	float dragonfireWarningTimer;
	float dragonfireActiveTimer;
	//Time till the next dragonfire appears
	float dragonfireCounter;
	int dragonfireStatus;
	//Locations of where the dragonfire will appear. (Viewport not taken into account)
	VECTOR2 dragonfireLocations[3];
	//Primitive used to draw the dragonfire warning triangle.
	VertexC vtx[3];
	LP_VERTEXBUFFER vertexBuffer;

	Image* aidilFlyingImage;
	TextureManager* aidilFlyTexture;

	Image* normalImage;
	UI* ui;
	float phase2Timer;
	int scaleChange;
	Behavior* oldAttackBehavior = nullptr;
	Behavior* oldExamineBehavior = nullptr;
	float oldMovementSpeed;
	//Tracks if the entrance of aidil's home has been made not visible at any point of time.
	//Whenever that happens, if the tile would ever become visible, it needs to be animated back to the end
	//so that it continues blocking the player.
	bool tileVisible;
protected:
	//Calculate the final location of a specific point,
	//given that, from the passed in x and y co-ordinates, a person travels a certain
	//distance at a specific angle.
	VECTOR2 getFinalLocation(float startX, float startY, float angle, float distance);
public:
	// constructor
	Aidil();

	// Destructor
	virtual ~Aidil();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, NPC* aidilInfo);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual string getType(){ return "AIDIL"; }
};
#endif