#ifndef _DRAGONFIRE_H                 // Prevent multiple definitions if this 
#define _DRAGONFIRE_H                 // file is included in more than one place

#include "entity.h"
#include "constants.h"

// inherits from Entity class
class Dragonfire : public Entity
{
protected:
public:
	// constructor
	Dragonfire(){};
	~Dragonfire(){};

	// inherited member functions
	virtual void draw(Viewport* viewport);
	//virtual bool initialize(Game *gamePtr, const char image[]);

	//Initalize the dragonfire to appear and start flying towards a specific location with a speciifc angle.
	//Will auto destroy itself once it reaches the destination.
	//Note: rot is in radians.
	virtual bool initialize(Game *gamePtr, TextureManager* tm, Destination* d, float rot);
	virtual void update(float frameTime, Game* gamePtr = nullptr);
};
#endif

