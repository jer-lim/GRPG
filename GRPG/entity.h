// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 entity.h v1.0

#ifndef _ENTITY_H               // Prevent multiple definitions if this 
#define _ENTITY_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "image.h"
#include "input.h"
#include "game.h"
#include "destination.h"
#include "Character.h"

namespace entityNS
{
    enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX};
    const float GRAVITY = 6.67428e-11f;         // gravitational constant
}

class Entity : public Destination
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
    Input   *input;         // pointer to the input system
    HRESULT hr;             // standard return type
    bool    active;         // only active entities may collide
    bool    rotatedBoxReady;    // true when rotated collision box is ready
	Destination* destination;			//The destination of movement
	float	x;				// The x location
	float	y;				// The y location
	Image	image;			// The image that is drawn on the screen
	Graphics* graphics;		// A pointer to the graphics object
	Character* character;	// Reference to the character that this entity refers to (NPC? Enemy? etc.)

	TextureManager* textureM; //This needs to be set on the entity's creation by any entity inheriting from this

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

  public:
    // Constructor
    Entity();
    // Destructor
    virtual ~Entity() {}

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
	virtual Character* getCharacter() { return character; }

	Image* getImage(){ return &image; }
	void setImage(Image i){ image = i; }

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
	virtual void setDestination(Destination* d) { destination = d; }

    // Set radius of collision circle.
    virtual void setCollisionRadius(float r)    {radius = r;}

    ////////////////////////////////////////
    //         Other functions            //
    ////////////////////////////////////////

    // Update Entity.
    // typically called once per frame
    // frameTime is used to regulate the speed of movement and animation
    virtual void update(float frameTime);

	// Initialize Entity
	// Pre: *gamePtr = pointer to Game object
	//      width = width of Image in pixels  (0 = use full texture width)
	//      height = height of Image in pixels (0 = use full texture height)
	//      ncols = number of columns in texture (1 to n) (0 same as 1)
	//		whichTexture = the texture that this entity reads from
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, const char whichTexture[]);

    // Initialize Entity
    // Pre: *gamePtr = pointer to Game object
    //      width = width of Image in pixels  (0 = use full texture width)
    //      height = height of Image in pixels (0 = use full texture height)
    //      ncols = number of columns in texture (1 to n) (0 same as 1)
	//		whichCharacter = the character that this entity refers to
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, Character* character);

	// Initialize entity using a pre-initialized TextureManager
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager* tm);

    // Activate Entity.
    virtual void activate();

	// Draws the Entity onto the screen
	virtual void draw();

    // Empty ai function to allow Entity objects to be instantiated.
    virtual void ai(float frameTime, Entity &ent);

    // Is this entity outside the specified rectangle?
    virtual bool outsideRect(RECT rect);

    // Does this entity collide with ent?
    virtual bool collidesWith(Entity &ent, VECTOR2 &collisionVector);

    // Damage this Entity with weapon.
    virtual void damage(int weapon);

	// Move towards a specific destination (Can be a Point or an Entity)
	void move(Destination* d);

	// Checks if the mouse is inside the sprite of this Entity
	virtual bool mouseInside();
};

#endif
