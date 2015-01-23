#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include "globals.h"

using namespace std;

struct chunk {
	char tile[16][16];
};

class MapLoader {

private:
	string mapFolder;
	unordered_map<char, string> tileset;
	unordered_map<char, chunk*> chunks;
	unordered_map<int, unordered_map<int, char>> worldMap;

public:
	MapLoader();
	void load();
};

#endif