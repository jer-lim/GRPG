#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
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
	bool collidable;
	string imageName;
};

struct ManagedTile {
	Tile* tile = nullptr;
	Image* image = nullptr;
	int x;
	int y;

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

	// Raw text-only data
	unordered_map<char, tileStruct> tileset;
	unordered_map<char, chunk*> chunks;
	unordered_map<int, unordered_map<int, char>> worldMap;

	// Loaded objects
	unordered_map<int, TextureManager*> tileTms;
	unordered_map<int, unordered_map<int, ManagedTile*>> loadedTiles;

public:
	MapLoader();
	void initialize(Game* game, DrawManager* dm, Viewport* vp);
	void load();
	void update();
};

#endif