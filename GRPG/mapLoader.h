#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "globals.h"
#include "tile.h"

using namespace std;

struct chunk {
	char tile[16][16];
};

struct tileStruct {
	bool collidable;
	const char* imageName;
};

class MapLoader {

protected:

	Game* gamePtr;
	Graphics* graphics;

	string mapFolder;
	unordered_map<char, tileStruct> tileset;
	unordered_map<char, chunk*> chunks;
	unordered_map<int, unordered_map<int, char>> worldMap;

public:
	MapLoader();
	void initialize(Game* game, Graphics* g);
	void load();
};

#endif