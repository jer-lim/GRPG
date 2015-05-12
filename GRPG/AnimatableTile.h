#ifndef ANIMATABLE_TILE_H                 // Prevent multiple definitions if this 
#define ANIMATABLE_TILE_H                 // file is included in more than one place

#include "tile.h"
#include "Entity.h"

// inherits from Entity class
class AnimatableTile : public Tile
{
protected:
	
public:
	// constructor
	AnimatableTile(char i, bool collidable = true) : Tile(i, collidable)
	{

	}

	virtual ~AnimatableTile(){};

	virtual bool initialize(Game* gamePtr, TextureManager* tm)
	{
		return Entity::initialize(gamePtr, tileNS::WIDTH, tileNS::HEIGHT, 5, tm);
	}

	//Starts the tile animation. If the tile animation has already started, resets it to the
	//first frame and restart it. The tile animation never loops.
	void startAnimating()
	{
		image.setCurrentFrame(0);
		image.setFrames(0, 4);
		image.setLoop(false);
		image.setFrameDelay(0.5f);
	}
	
	//Finishes the tile animation, setting the animation to the last frame immediately.
	//Does this regardless if the tile is currently animating or not
	void finishAnimating()
	{
		//image.setCurrentFrame(4);
		//iage.setFrames(4, 4);
		//image.setLoop(false);
	}
};
#endif

