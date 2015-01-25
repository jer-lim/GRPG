#ifndef _UI_H                 // Prevent multiple definitions if this 
#define _UI_H                 // file is included in more than one place

#include "entity.h"
#include "constants.h"

namespace uiNS
{
	const int WIDTH = 190;                   // image width
	const int HEIGHT = 264;                  // image height
	const int X = GAME_WIDTH - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT - HEIGHT / 2;
}

// inherits from Entity class
class UI : public Entity
{
private:

public:
	// constructor
	UI();

	// inherited member functions
	virtual void draw();
	virtual bool initialize(Game *gamePtr);
	void update(float frameTime);
};
#endif

