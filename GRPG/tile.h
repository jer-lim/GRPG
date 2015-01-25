#ifndef TILE_H                 // Prevent multiple definitions if this 
#define TILE_H                 // file is included in more than one place

#include "entity.h"
#include "constants.h"

namespace tileNS
{
    const int WIDTH = 32;                   // image width
    const int HEIGHT = 32;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
}

// inherits from Entity class
class Tile : public Entity
{
private:

public:
    // constructor
    Tile();

    // inherited member functions
    virtual void draw();
	virtual bool initialize(Game *gamePtr, const char image[]);
    void update(float frameTime);
};
#endif

