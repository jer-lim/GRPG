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
    const float ROTATION_RATE = 0.0f; // radians per second
    const float SPEED = 0;                // 100 pixels per second
    const float MASS = 0.0f;              // mass
    const int   TEXTURE_COLS = 1;           // texture has 8 columns
    const int   START_FRAME = 0;      // ship1 starts at frame 0
    const int   END_FRAME = 0;        // ship1 animation frames 0,1,2,3
    const float ANIMATION_DELAY = 0.0f;    // time between frames
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
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager* tm);
    void update(float frameTime);
};
#endif

