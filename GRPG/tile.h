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
	const int CHUNK_WIDTH = 16; // Chunk width in tiles
	const int CHUNK_HEIGHT = 16; // Chunk height in tiles
	const int ZINDEX = 0;

	namespace type {
		const int FLOOR = 0;
		const int WALL = 1;
		const int SPAWNER = 2;
		const int SPAWNPOINT = 3;
		const int FISHINGSPOT = 4;
		const int MININGSPOT = 5;
	}
}

// inherits from Entity class
class Tile : public Entity 
{
protected:
	char id;
public:
    // constructor
	Tile(char i, bool collidable = true);
	~Tile(){};

    // inherited member functions
    virtual void draw(Viewport* viewport);
	//virtual bool initialize(Game *gamePtr, const char image[]);
	virtual bool initialize(Game *gamePtr, TextureManager* tm);
	virtual void update();
	virtual void update(float frameTime, Game* gamePtr = nullptr);

	virtual void spawn(){} // only activated for Spawner

	char getId(){ return id; }
	void setId(char i){ id = i; }
};
#endif

