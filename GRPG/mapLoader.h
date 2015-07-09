// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

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
#include "constants.h"
#include "tile.h"
#include "spawner.h"

class DrawManager;
class Viewport;
class Resource;

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
	int numFrames;
	double frameTime;
	float globalFrameTime;
	int currentFrame = 0;
	int songId;
};

// Stores a tile, which can be either an entity or an image
struct ManagedTile {
	int type;
	Tile* tile = nullptr;
	Image* image = nullptr;
	Entity* entity = nullptr;

	ManagedTile(Tile* t, int ty){ tile = t; type = ty; }
	ManagedTile(Image* i, int ty){ image = i; type = ty; }
	void destroy(){
		if (tile != nullptr) SAFE_DELETE(tile);
		if (image != nullptr) SAFE_DELETE(image);
		if (entity != nullptr) SAFE_DELETE(entity);
	}
	~ManagedTile(){
		//handled by draw manager
		//if (spawner != nullptr) SAFE_DELETE(spawner);
		//if (tile != nullptr) SAFE_DELETE(tile);
		//if (image != nullptr) SAFE_DELETE(image);
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

	AStarNode(){}
	AStarNode(TileVector c){ tileCoords = c; }
	~AStarNode(){ 
		parent = nullptr;
	}
};

class MapLoader {

private:
	// Statistics
	LARGE_INTEGER timeStart, timeEnd, timerFreq;

	Game* gamePtr;
	DrawManager* drawManager;
	Viewport* viewport;

	Entity* victim;

	char spawnTileId;

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

	TileVector getBufferedTopLeftCoords(){ return TileVector(0 - tileNS::WIDTH * (bufferSize) - tileNS::WIDTH / 2, 0 - tileNS::HEIGHT * (bufferSize) - tileNS::HEIGHT / 2); }
	TileVector getBufferedBottomRightCoords(){ return TileVector(GAME_WIDTH + tileNS::WIDTH * (bufferSize) + tileNS::WIDTH / 2 - 1, GAME_HEIGHT + tileNS::HEIGHT * (bufferSize) + tileNS::HEIGHT / 2 - 1); }
public:
	MapLoader();
	~MapLoader();
	void initialize(Game* game);
	void setVictim(Entity* v){ victim = v; }
	void loadData();
	void loadMap();
	void update(float frameTime);
	bool canRequestPath(){
		return !pathRequestedThisFrame;
	}
	queue<VECTOR2> path(VECTOR2 startCoords, VECTOR2 endCoords);

	// External helpers
	VECTOR2 translateIdToCoords(char id);

	//Return the Tile that has that character, given that it has to be currently visible on screen.
	//If it is not on screen, returns nullptr.
	Tile* getTileWithId(char id);
};

#endif