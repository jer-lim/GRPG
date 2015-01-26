#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <sstream>
#include "globals.h"
#include "tile.h"
#include "drawManager.h"

using namespace std;

struct chunk {
	char tile[16][16];
};

struct tileStruct {
	bool collidable;
	string imageName;
};

class MapLoader {

protected:

	Game* gamePtr;
	DrawManager* drawManager;

	string mapFolder;
	string tileImageFolder;

	// Raw text-only data
	unordered_map<char, tileStruct> tileset;
	unordered_map<char, chunk*> chunks;
	unordered_map<int, unordered_map<int, char>> worldMap;

	// Loaded objects
	unordered_map<int, TextureManager*> tileTms;

public:
	MapLoader();
	void initialize(Game* game, DrawManager* dm);
	void load();
};

#endif