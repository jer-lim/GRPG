#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <queue>
#include <vector>

#include "image.h"
#include "input.h"
#include "game.h"
#include "destination.h"
#include "Person.h"
#include "InventoryItem.h"
#include "InventoryFood.h"
#include "InventoryEquipment.h"
#include "Viewport.h"
//#include "Interactable.h"
//#include "Behavior.h"
//#include "ViewBehavior.h"
//#include "PickupBehavior.h"
//#include "DropBehavior.h"
#include "ViewBehaviorNPC.h"
#include "ViewBehaviorItem.h"
#include "EatBehavior.h"
#include "CookBehavior.h"
#include "Button.h"
#include "Inventory.h"

class AttackBehavior;
class PickupBehavior;
class DropBehavior;
class Player;

namespace entityNS
{
    enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX};
    const float GRAVITY = 6.67428e-11f;         // gravitational constant
	const float animationWait = 0.2f;

	// Health bar
	const UINT healthBarWidth = 64;
	const UINT healthBarHeight = 8;
	const float healthDisplay = 2;

	// Blood splat
	extern Image miss;
	extern Image hit;
	extern TextDX splatText;
	const float splatTime = 1.2;
}

/*
Entities Animation:
The entity's 2nd frame is always the stand still frame where the entity does nothing.
If the entity is moving, then constantly swap between the 1st and 2nd frame
If the entity is attacking, then constantly swpa between the 2nd all the way to the last frame
*/

class Entity : public Destination//, public Interactable
{
    // Entity properties
  protected:
    entityNS::COLLISION_TYPE collisionType;
    VECTOR2 center;         // center of entity
    float   radius;         // radius of collision circle
    VECTOR2 distSquared;    // used for calculating circle collision
    float   sumRadiiSquared;
    // edge specifies the collision box relative to the center of the entity.
    // left and top are typically negative numbers
    RECT    edge;           // for BOX and ROTATED_BOX collision detection
    VECTOR2 corners[4];     // for ROTATED_BOX collision detection
    VECTOR2 edge01,edge03;  // edges used for projection
    float   edge01Min, edge01Max, edge03Min, edge03Max; // min and max projections
    float   health;         // current health from 0 to Character's max health
    float   rr;             // Radius squared variable
    Input   *input = nullptr;         // pointer to the input system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    bool    rotatedBoxReady;    // true when rotated collision box is ready

	// New Entity variables added that are generic

	float	x;				// logical X location
	float	y;				// logical Y location
	VECTOR2 oldViewport;	// The old viewport top left location to check if re-drawing are needed
	VECTOR2 oldLocation;	// The old location this entity was at to check if re-drawing are needed
	bool anchored;			// Anchored entities don't move when the viewport moves
	Image	image;			// The image that is drawn on the screen
	Graphics* graphics = nullptr;		// A pointer to the graphics object
	TextureManager* textureM = nullptr; //This needs to be set on the entity's creation by any entity inheriting from this
	Button* backHealth = nullptr;			//Vertex data for the entirety of the health bar
	Button* availableHealth = nullptr;		// Vertex data for only the health that the player has
	float displayTime;					//How long the health bar is going to be displayed.

	float splatTime;				//How long the hit splat will stay on the screen
	int damageTaken;				//How many damage this entity took

	Game* theGame;					// Reference to the game so Entity can inform it when it dies

	// Pathfinding
	queue<VECTOR2> path;
	LARGE_INTEGER lastPathfindTime;
	VECTOR2 currentDestination;

	// New Entity variables specific to GRPG

	Destination* destination = nullptr;			//The destination of movement
	Person* person = nullptr;	// Reference to the character that this entity refers to (NPC? Enemy? etc.
	InventoryItem* inventoryItem = nullptr;
	Entity* victim = nullptr;	//If this entity is attacking someone, victim is that poor entity
	float attackCooldown; //How long more before the entity can attack again

	vector<Behavior*> vectorActiveBehaviors;

	//To know what to do when the player is near an NPC
	Behavior* npcAction = nullptr;

	//To calculate aggro (Range & player's combat level)
	Player* thePlayer;
	//The entity can never move too far from the spawn location
	VECTOR2 spawnLocation;

	//Inventory for both players and friendlies
	Inventory* inventory;


    // --- The following functions are protected because they are not intended to be
    // --- called from outside the class.
    // Circular collision detection 
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideCircle(Entity &ent, VECTOR2 &collisionVector);
    // Axis aligned box collision detection
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between boxes
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBox(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection between box and circle
    // Pre: &ent = Other entity
    // Post: &collisionVector contains collision vector
    virtual bool collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector);
    // Separating axis collision detection helper functions
    void computeRotatedBox();
    bool projectionsOverlap(Entity &ent);
    bool collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector);

	// Returns a random number from 1 to 0
	float getRandomNumber()
	{
		return ((float)rand() / (RAND_MAX));
	}

	void takeDamage(int atk, int str, int def);

  public:
	  //Behaviors
	  Behavior* viewBehavior = nullptr;//View name -> display description
	  //NPC
	  Behavior* blacksmithBehavior = nullptr;//Blacksmith popup
	  Behavior* talkBehavior = nullptr; //Ability to talk
	  Behavior* tradeBehavior = nullptr;//store popup
	  Behavior* teleportBehavior = nullptr;
	  Behavior* healBehavior = nullptr;
	  Behavior* stoveBehavior = nullptr;
	  //Enemy
	  Behavior* attackBehavior = nullptr;//Attack name -> perform attack
	  //Mining and fishing also use this - when the rock/fihsing spot health reach 0, it drops loot
	  //Item
	  Behavior* pickupBehavior = nullptr;//Pickup name -> pickup obj
	  Behavior* dropBehavior = nullptr;//Drop name -> drop obj
	  Behavior* cookBehavior = nullptr;//Cook name -> cook obj if fire nearby
	  Behavior* buyBehavior = nullptr;
	  Behavior* sellBehavior = nullptr;
	  Behavior* eatBehavior = nullptr;
	  //Resource
	  Behavior* fishBehavior = nullptr; //Fishing
	  Behavior* mineBehavior = nullptr; //Mining

    // Constructor
    Entity();
    // Destructor
	virtual ~Entity();

    ////////////////////////////////////////
    //           Get functions            //
    ////////////////////////////////////////

    // Return center of scaled Entity as screen x,y.
    virtual const VECTOR2* getCenter()   
    {
        center = VECTOR2(getX(), getY());
        return &center;
    }

	virtual D3DXVECTOR2 getVector()
	{
		return VECTOR2(getX(), getY());
	}

    // Return radius of collision circle.
    virtual float getRadius() const     {return radius;}

    // Return RECT structure used for BOX and ROTATED_BOX collision detection.
    virtual const RECT& getEdge() const {return edge;}

    // Return corner c of ROTATED_BOX
    virtual const VECTOR2* getCorner(UINT c) const
    {
        if(c>=4) 
            c=0;
        return &corners[c]; 
    }

    // Return active.
    virtual bool  getActive()         const {return active;}

    // Return health;
    virtual float getHealth()         const {return health;}

	// Return x location
	virtual float getX()			  { return x; }

	// Return x location
	virtual float getY()			  { return y; }

    // Return collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

	// Returns the texture manager that this entity uses
	virtual TextureManager* getTextureManager() { return textureM; }

	// Returns the character that this entity refers to
	virtual Person* getPerson() { return person; }
	// Returns the character that this entity refers to
	virtual InventoryItem* getInventoryItem() { return inventoryItem; }
	virtual void setInventoryItem(InventoryItem* i) { inventoryItem = i; }

	// Returns the current destination that this entity is heading towards
	// Returns a pointer to 0 if no current destination
	virtual Destination* getDestination() { return destination; }

	// Returns the current entity that this entity is attempting to attack
	// Returns a pointer to 0 if no current victim
	virtual Entity* getVictim() { return victim; }

	Image* getImage(){ return &image; }
	void setImage(Image i){ image = i; }

	// Returns the edge that is used for box and rotated box collision
	virtual RECT getEdge() { return edge; }

	Inventory* getInventory(){ return inventory; }
	virtual void setInventory(Inventory* i) { inventory = i; }

    ////////////////////////////////////////
    //           Set functions            //
    ////////////////////////////////////////

    // Set active.
    virtual void  setActive(bool a)         {active = a;}

    // Set health.
    virtual void setHealth(float h)         {health = h;}

	// Set x location
	virtual void setX(float xL)				{ x = xL; }

	// Set y location
	virtual void setY(float yL)				{ y = yL; }

	// Causes the player to move towards a specific destination (Can be a Point or an Entity)
	virtual void setDestination(Destination* d) { move(d); }

	// Causes the player to start attacking a specific entity
	virtual void setVictim(Entity* e) { victim = e; }

    // Set radius of collision circle.
    virtual void setCollisionRadius(float r)    {radius = r;}

	// Set which NPC's behavior action() function will be called
	//once the player hits the npc
	void setNPCAction(Behavior* b) { npcAction = b; }
	
	// Sets the spawn points which this NPC is created from
	// If the NPC wanders too far from the spawn point, he returns
	virtual void setSpawnPoint(VECTOR2 location) {
		spawnLocation = location; 
	}

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Update Entity.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime, Game* gamePtr = nullptr);

	// Initialize Entity
	// Pre: *gamePtr = pointer to Game object
	//      width = width of Image in pixels  (0 = use full texture width)
	//      height = height of Image in pixels (0 = use full texture height)
	//      ncols = number of columns in texture (1 to n) (0 same as 1)
	//		whichTexture = the texture that this entity reads from
	//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
	// Post: returns true if successful, false if failed
	//virtual bool initialize(Game *gamePtr, int width, int height, int ncols, const char whichTexture[], bool anc = false);

    // Initialize Entity
    // Pre: *gamePtr = pointer to Game object
	//		whichCharacter = the character that this entity refers to
	//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
	// Post: returns true if successful, false if failed
	virtual bool initialize(Game *gamePtr, Person* character, bool anc = false);
	virtual bool initialize(Game *gamePtr, InventoryItem* invItem, bool inInventory = false);
	// Initialize entity using a pre-initialized TextureManager
	// Pre: *gamePtr = pointer to Game object
	//      width = width of Image in pixels  (0 = use full texture width)
	//      height = height of Image in pixels (0 = use full texture height)
	//      ncols = number of columns in texture (1 to n) (0 same as 1)
	//		tm = the texture that has already been initalized (tm->initalize(...);)
	//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager* tm, bool anc = false);

    // Activate Entity.
    virtual void activate();

	// Draws the Entity onto the screen
	virtual void draw(Viewport* viewport);

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity &ent);

    // Is this entity outside the specified rectangle?
    virtual bool outsideRect(RECT rect);

    // Does this entity collide with ent?
    virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

	// This entity has been damaged by another entity
	// Pass in the other entity's attack and strength.
	// Returns the amount of damage dealt.
    virtual int damage(int atk, int str);

	// Move towards a specific destination (Can be a Point or an Entity)
	void move(Destination* d);

	// Start moving towards an entity. If close enough (colliding), automatically attack
	void attack(Entity* e);

	// Checks if the mouse is inside the sprite of this Entity
	virtual bool mouseInside(Viewport* vp);

	// Return whether the entity is an attackable enemy
	virtual string getType(){ return "ENTITY"; }
	bool isEnemy();
	// Resets the coordinates of the available health portion of this entity
	virtual void resetAvailableHealth(VECTOR2 topLeftViewport);
	// Resets the coordinates of the health portion of this entity
	// Note that this only resets the health part, which is the background of the health
	// To re-draw the red part as well, call resetAvailableHealth.
	virtual void resetHealth(VECTOR2 topLeftViewport);
	// Never actually releases the entity because it is likely reused
	virtual void release(){  }

	//Behavior related
	//void setCookBehavior
	void setDropBehavior(Behavior* b) {
		//if (dropBehavior != nullptr)//should be handled outside, as when the behavior is deleted any further actions will cause a crash
			//SAFE_DELETE(dropBehavior);
		dropBehavior = b;
	}
	void setPickupBehavior(Behavior* b) {
		//if (pickupBehavior != nullptr)
			//SAFE_DELETE(pickupBehavior);
		pickupBehavior = b;
	}
	void setCookBehavior(Behavior* b) {
		//if (cookBehavior != nullptr)
			//SAFE_DELETE(cookBehavior);
		cookBehavior = b;
	}
	void setEatBehavior(Behavior* b) {
		//if (eatBehavior != nullptr)
			//SAFE_DELETE(eatBehavior);
		eatBehavior = b;
	}
	void setupVectorActiveBehaviors()
	{//Must be in order, as they will be displayed in this order 
		vectorActiveBehaviors.clear();
		if (stoveBehavior)
			vectorActiveBehaviors.push_back(stoveBehavior);
		if (buyBehavior)
			vectorActiveBehaviors.push_back(buyBehavior);
		if (sellBehavior)
			vectorActiveBehaviors.push_back(sellBehavior);
		if (eatBehavior)
			vectorActiveBehaviors.push_back(eatBehavior);
		if (pickupBehavior)
			vectorActiveBehaviors.push_back(pickupBehavior);
		if (cookBehavior)//must come under pickup
			vectorActiveBehaviors.push_back(cookBehavior);
		if (dropBehavior)
			vectorActiveBehaviors.push_back(dropBehavior);
		if (talkBehavior)
			vectorActiveBehaviors.push_back(talkBehavior);
		if (attackBehavior)
			vectorActiveBehaviors.push_back(attackBehavior);
		if (fishBehavior)
			vectorActiveBehaviors.push_back(fishBehavior);
		if (mineBehavior)
			vectorActiveBehaviors.push_back(mineBehavior);
		if (tradeBehavior)
			vectorActiveBehaviors.push_back(tradeBehavior);
		if (teleportBehavior)
			vectorActiveBehaviors.push_back(teleportBehavior);
		if (healBehavior)
			vectorActiveBehaviors.push_back(healBehavior);
		if (blacksmithBehavior)
			vectorActiveBehaviors.push_back(blacksmithBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}
	vector<Behavior*> getVectorActiveBehaviors(){ return vectorActiveBehaviors; }
	// Displays the text that will be shown when the mouse is over it
	Behavior* getTopMostBehavior()
	{
		if (vectorActiveBehaviors.size() > 0)
		{
			return vectorActiveBehaviors.at(0);
		}
		return nullptr;
	}

	// Releases the destination held by this entity, if any
	// And then sets it to 0
	virtual void releaseDestination()
	{
		if (destination != nullptr)
		{
			destination->release();
			destination = nullptr;
		}
	}

	void setAnchored(bool b)
	{
		anchored = b;
	}
};

#endif
