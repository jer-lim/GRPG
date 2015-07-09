#include "MusicTile.h"
#include "grpg.h"

void PlayableMusic::update(float frametime, Game* gameptr)
{
	if (songId != -1)
	{
		if (collidesWithPlayer())
		{
			if (SoundManager::getcurrentMusicId() != songId)
			{
				SoundManager::playMusic(songId);
			}
		}
	}
}

bool MusicTile::initialize(Game *gamePtr, TextureManager* tm)
{
	thePlayer = ((Grpg*)gamePtr)->getPlayer();
	bool result = Tile::initialize(gamePtr, tm);
	collisionType = entityNS::BOX;
	return result;
}

bool MusicTile::collidesWithPlayer()
{
	VECTOR2 collisionVector;
	return (thePlayer->collidesWith(*this, collisionVector));
}

void MusicTile::draw(Viewport* viewport)
{
	Tile::draw(viewport);
}