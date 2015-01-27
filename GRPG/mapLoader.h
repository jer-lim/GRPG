#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <sstream>
#include <cmath>
#include "globals.h"
#include "tile.h"
#include "drawManager.h"
#include "Viewport.h"

using namespace std;

struct chunk {
	char tile[16][16];
};

struct tileStruct {
	int type;
	string imageName;
};

struct ManagedTile {
	Tile* tile = nullptr;
	Image* image = nullptr;

	ManagedTile(Tile* t){ tile = t; }
	ManagedTile(Image* i){ image = i; }
};

class MapLoader {

protected:

	Game* gamePtr;
	DrawManager* drawManager;
	Viewport* viewport;

	string mapFolder;
	string tileImageFolder;
	int bufferSize; // width of tiles beyond the border to load

	// Calculated variables
	int tileWidth; // Width of the game in number of tiles
	int tileHeight; // Height of the game in number of tiles

	// Raw text-only data
	unordered_map<char, tileStruct> tileset;
	unordered_map<char, chunk*> chunks;
	unordered_map<int, unordered_map<int, char>> worldMap;

	// Loaded objects
	unordered_map<int, TextureManager*> tileTms;
	unordered_map<int, unordered_map<int, ManagedTile*>> loadedTiles;

private:
	char getTileIdAtLocation(int tileX, int tileY);
	VECTOR2 getCoordsAtTileLocation(int tileX, int tileY);

	VECTOR2 getBufferedTopLeftCoords(){ return VECTOR2(0 - tileNS::WIDTH * bufferSize - tileNS::WIDTH / 2, 0 - tileNS::HEIGHT * bufferSize - tileNS::HEIGHT / 2); }
	VECTOR2 getBufferedBottomRightCoords(){ return VECTOR2(GAME_WIDTH + tileNS::WIDTH * bufferSize + tileNS::WIDTH / 2, GAME_HEIGHT + tileNS::HEIGHT * bufferSize + tileNS::HEIGHT / 2); }
public:
	MapLoader();
	void initialize(Game* game, DrawManager* dm, Viewport* vp);
	void load();
	ManagedTile* loadTile(int tileX, int tileY);
	void update();
};

#endif