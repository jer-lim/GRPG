#ifndef _MUSIC_TILE_H                 // Prevent multiple definitions if this 
#define _MUSIC_TILE_H                 // file is included in more than one place

#include "entity.h"
#include "constants.h"
#include "tile.h"
#include "NoiseManager.h"

class PlayableMusic
{
private:
	int songId;
public:
	PlayableMusic(int s)
	{
		songId = s;
	}

	virtual void update(float frameTime, Game* gamePtr = nullptr);
	virtual bool collidesWithPlayer() = 0;
	int getSongID() { return songId; }
	void setSongID(int s) { songId = s; }
};

// inherits from Entity class
class MusicTile : public Tile, public PlayableMusic
{
protected:
public:
	// constructor
	MusicTile(char i, int songId, bool collidable = true) : Tile(i, collidable), PlayableMusic(songId) {
	}
	~MusicTile(){};

	virtual bool initialize(Game *gamePtr, TextureManager* tm);

	virtual void draw(Viewport* viewport);

	virtual void update(float frameTime, Game* gamePtr = nullptr)
	{
		PlayableMusic::update(frameTime, gamePtr);
	}

	virtual bool collidesWithPlayer();
};
#endif

