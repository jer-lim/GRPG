#ifndef MAP_LOADER_H
#define MAP_LOADER_H

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <queue>
#include <stack>
#include <sstream>
#include <cmath>
#include "globals.h"
#include "tile.h"

class DrawManager;
class Viewport;

using namespace std;

// Stores 2D array of tile IDs that the chunk consists of
struct chunk {
	char tile[16][16];
};

// Stores the information about tiles
struct tileStruct {
	int type;
	string imageName;
	int spawnId;
	int spawnCooldown;
};

// Stores a tile, which can be either an entity or an image
struct ManagedTile {
	Tile* tile = nullptr;
	Image* image = nullptr;

	ManagedTile(Tile* t){ tile = t; }
	ManagedTile(Image* i){ image = i; }

	~ManagedTile(){
		if (tile != nullptr) delete tile;
		if (image != nullptr) delete image;
	}
};

// Stores the location of a tile with respect to the world map - 0, 0 being the top left tile
struct TileVector {
	int x;
	int y;
	TileVector(){ x = -1; y = -1; }
	TileVector(int x2, int y2){ x = x2; y = y2; }
};

struct AStarNode {
	TileVector tileCoords;
	float collectiveCost;
	float estimatedCostToEnd;
	float totalCost;
	AStarNode* parent = nullptr;

	AStarNode(TileVector c){ tileCoords = c; }
	~AStarNode(){ parent = nullptr; }
};

class MapLoader {

private:
	Game* gamePtr;
	DrawManager* drawManager;
	Viewport* viewport;

	Entity* victim;

	string mapFolder;
	string tileImageFolder;
	int bufferSize; // width of tiles beyond the border to load

	// Pathfinding
	bool pathRequestedThisFrame;

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

	// Helper functions
	ManagedTile* loadTile(int tileX, int tileY);

	// Get char ID of tile at the tile-based coordinates
	char getTileIdAtLocation(int tileX, int tileY);
	// Get raw coordinates of the tile at the tile-based coordinates
	VECTOR2 getCoordsAtTileLocation(int tileX, int tileY);
	// Get the tile-based coordinates from raw coordinates
	TileVector getNearestTile(VECTOR2 coords);

	TileVector getBufferedTopLeftCoords(){ return TileVector(0 - tileNS::WIDTH * bufferSize - tileNS::WIDTH / 2, 0 - tileNS::HEIGHT * bufferSize - tileNS::HEIGHT / 2); }
	TileVector getBufferedBottomRightCoords(){ return TileVector(GAME_WIDTH + tileNS::WIDTH * bufferSize + tileNS::WIDTH / 2, GAME_HEIGHT + tileNS::HEIGHT * bufferSize + tileNS::HEIGHT / 2); }
public:
	MapLoader();
	~MapLoader();
	void initialize(Game* game);
	void setVictim(Entity* v){ victim = v; }
	void load();
	void update();
	bool canRequestPath(){
		return !pathRequestedThisFrame;
	}
	queue<VECTOR2> path(VECTOR2 startCoords, VECTOR2 endCoords);
};

#endif