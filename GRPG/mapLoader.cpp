// Module: Games Programming
// Assignment 2: GRPG
// Student Name: Jeremy Lim
// Student Number: S10122326F

#include "mapLoader.h"
#include "drawManager.h"
#include "Viewport.h"
#include "Spawner.h"
#include "Resource.h"
#include "cooker.h"
#include "AnimatableTile.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "assets/map/";
	tileImageFolder = "assets/map/img/";
	bufferSize = 1;

	tileWidth = ceil(GAME_WIDTH / tileNS::WIDTH) + 2 * bufferSize + 1;
	tileHeight = ceil(GAME_HEIGHT / tileNS::HEIGHT) + 2 * bufferSize + 1;

	pathRequestedThisFrame = false;
}

MapLoader::~MapLoader(){
	
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx){
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity){
			delete ity->second;
			ity->second = nullptr;
		}
		loadedTiles[itx->first].clear();
	}
	loadedTiles.clear();
	

	
	for (unordered_map<int, TextureManager*>::iterator it = tileTms.begin(); it != tileTms.end(); ++it){
		delete it->second;
		it->second = nullptr;
	}
	tileTms.clear();

	
	for (unordered_map<int, unordered_map<int, char>>::iterator it = worldMap.begin(); it != worldMap.end(); ++it){
		worldMap[it->first].clear();
	}
	worldMap.clear();

	for (unordered_map<char, chunk*>::iterator it = chunks.begin(); it != chunks.end(); ++it){
		delete it->second;
		it->second = nullptr;
	}
	chunks.clear();

	tileset.clear();
	
	//delete victim;//victim is always player and player is deleted at the end
}

void MapLoader::initialize(Game* game){
	gamePtr = game;
	drawManager = gamePtr->getDrawManager();
	viewport = gamePtr->getViewport();
}

void MapLoader::loadData(){

	QueryPerformanceCounter(&timeStart);
	QueryPerformanceFrequency(&timerFreq);

	runtimeLog << "Starting map startup sequence" << endl;
	runtimeLog << "Starting map info load" << endl;

	string trash;

	// Load tileset
	ifstream tilestream;
	tilestream.open(mapFolder + "tiles.gdef");
	if (tilestream.is_open()){
		char tileId;
		int tileType;
		int numFrames;
		double frameTime;
		string tileFileName;
		while (!tilestream.eof()){

			tilestream >> tileId;
			if (tileId != EOF && tileId != '\0'){
				tilestream >> tileType;
				tilestream >> numFrames;
				tilestream >> frameTime;
				tilestream >> tileFileName;

				//Insert into a map
				if (tileType == tileNS::type::SPAWNPOINT){
					tileset[tileId].type = tileNS::type::FLOOR;
					spawnTileId = tileId;
				}
				else{
					tileset[tileId].type = tileType;
				}

				tileset[tileId].imageName = tileFileName;
				tileset[tileId].numFrames = numFrames;
				tileset[tileId].frameTime = frameTime;
				tileset[tileId].globalFrameTime = 0.0f;

				if (tileType == tileNS::type::SPAWNER){
					tilestream >> tileset[tileId].spawnId;
					tilestream >> tileset[tileId].spawnCooldown;
				}
				else{
					tilestream >> trash >> trash;
				}
				runtimeLog << "Loaded tile " << tileId << endl;
			}
		}

		tilestream.close();
	}
	else
	{
		runtimeLog << "Failed to open tiles" << endl;
	}

	// Load chunks

	ifstream chunkstream;
	chunkstream.open(mapFolder + "chunks.gdef");
	if (chunkstream.is_open()){
		char chunkId;
		while (!chunkstream.eof()){

			chunkstream >> chunkId;
			// Clear endline from stream
			getline(chunkstream, trash);
			chunkId = (int)chunkId;
			chunk *tempChunk = new chunk;
			//runtimeLog << "Created chunk1" << endl;
			//runtimeLog << "New memory allocation at 0x" << tempChunk << endl; // NEWLOGGING
			for (int y = 0; y < 16; ++y){
				for (int x = 0; x < 16; ++x){
					tempChunk->tile[x][y] = chunkstream.get();
				}
				getline(chunkstream, trash);
			}


			runtimeLog << "Loaded chunk " << chunkId << endl;

			//Insert into a map
			chunks[chunkId] = tempChunk;
		}

		chunkstream.close();
	}
	else
	{
		runtimeLog << "Failed to open chunks" << endl;
	}

	// Load world map

	ifstream mapstream;
	mapstream.open(mapFolder + "worldmap.gdef");
	if (mapstream.is_open()){
		string trash;
		char tempChar;
		int x = 0, y = 0;
		while (!mapstream.eof()){

			tempChar = mapstream.get();
			if (tempChar == '\n'){
				y++;
				x = 0;
			}
			else if (tempChar != EOF){ // not EOF character
				worldMap[x++][y] = tempChar;
			}
		}

		runtimeLog << "Loaded world map" << endl;
		mapstream.close();
	}
	else
	{
		runtimeLog << "Failed to open worldmap" << endl;
	}

	runtimeLog << "Finished loading map" << endl;

	// Set spawn point
	for (int x = 0; x < worldMap.size(); ++x){
		for (int y = 0; y < worldMap[x].size(); ++y){

			// Load tiles in chunk
			char chunkId = worldMap[x][y];
			for (int cx = 0; cx < tileNS::CHUNK_WIDTH; ++cx){
				for (int cy = 0; cy < tileNS::CHUNK_HEIGHT; ++cy){

					// Load tiles
					char tileId = chunks[chunkId]->tile[cx][cy];

					int tileX = x * tileNS::CHUNK_WIDTH + cx;
					int tileY = y * tileNS::CHUNK_HEIGHT + cy;

					float xPos = tileNS::WIDTH / 2 + tileX * tileNS::WIDTH;
					float yPos = tileNS::HEIGHT / 2 + tileY * tileNS::HEIGHT;

					VECTOR2 vpCoords = viewport->translate(xPos, yPos);
					float vpXPos = vpCoords.x;
					float vpYPos = vpCoords.y;

					if (tileId == spawnTileId){
						gamePtr->setStartLocation(VECTOR2(xPos, yPos));
					}
				}
			}
		}
	}

}

void MapLoader::loadMap(){

	runtimeLog << "Starting initial scene build" << endl;

	// Display world map
	// Load each chunk

	TileVector bufferedTopLeftCoords = getBufferedTopLeftCoords();
	TileVector bufferedBottomRightCoords = getBufferedBottomRightCoords();

	runtimeLog << "Top Left: " << bufferedTopLeftCoords.x << ", " << bufferedTopLeftCoords.y << endl;
	runtimeLog << "Bottom Right: " << bufferedBottomRightCoords.x << ", " << bufferedBottomRightCoords.y << endl;
	for (int x = 0; x < worldMap.size(); ++x){
		for (int y = 0; y < worldMap[x].size(); ++y){

			// Load tiles in chunk
			char chunkId = worldMap[x][y];
			for (int cx = 0; cx < tileNS::CHUNK_WIDTH; ++cx){
				for (int cy = 0; cy < tileNS::CHUNK_HEIGHT; ++cy){

					// Load tiles
					char tileId = chunks[chunkId]->tile[cx][cy];

					int tileX = x * tileNS::CHUNK_WIDTH + cx;
					int tileY = y * tileNS::CHUNK_HEIGHT + cy;

					float xPos = tileNS::WIDTH / 2 + tileX * tileNS::WIDTH;
					float yPos = tileNS::HEIGHT / 2 + tileY * tileNS::HEIGHT;

					VECTOR2 vpCoords = viewport->translate(xPos, yPos);
					float vpXPos = vpCoords.x;
					float vpYPos = vpCoords.y;

					if (tileId == spawnTileId){
						gamePtr->setStartLocation(VECTOR2(xPos, yPos));
					}

					// Is in viewport range
					if (vpXPos > bufferedTopLeftCoords.x
						&& vpYPos > bufferedTopLeftCoords.y
						&& vpXPos < bufferedBottomRightCoords.x
						&& vpYPos < bufferedBottomRightCoords.y){

						//runtimeLog << "Loading tile " << tileX << ", " << tileY << " (chunk " << x+1 << ", " << y+1 << ") with coords " << xPos << ", " << yPos << " at vp coords " << vpXPos << ", " << vpYPos << endl;
						ManagedTile* mt = loadTile(tileX, tileY);
						//runtimeLog << "Created ManagedTile1" << endl;
						//runtimeLog << "New memory allocation at 0x" << mt << endl; // NEWLOGGING
						loadedTiles[tileX][tileY] = mt;
					}
				}
			}
		}
	}

	runtimeLog << "Finished initial scene build" << endl;
	runtimeLog << "Finished map startup sequence" << endl;

	QueryPerformanceCounter(&timeEnd);
	runtimeLog << "Map startup sequence finished in " << ((float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart) << " seconds" << endl;

	/* Memory leak test 
	TextureManager* textureManager;
	stringstream ss;
	ss << tileImageFolder << tileset['0'].imageName;
	textureManager = new TextureManager();
	textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
	for(int i = 0; i < 1000; ++i){
		Tile* t = new Tile();
		t->initialize(gamePtr, textureManager);

		//Image* t = new Image();
		//t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);

		delete t;
	}
	*/
}

char MapLoader::getTileIdAtLocation(int tileX, int tileY){
	// Get Chunk ID
	int chunkX = tileX / tileNS::CHUNK_WIDTH;
	int chunkY = tileY / tileNS::CHUNK_HEIGHT;
	char chunkId = worldMap[chunkX][chunkY];

	// Get Tile ID
	int chunkTileX = tileX % tileNS::CHUNK_WIDTH;
	int chunkTileY = tileY % tileNS::CHUNK_HEIGHT;

	return chunks[chunkId]->tile[chunkTileX][chunkTileY];
}

VECTOR2 MapLoader::getCoordsAtTileLocation(int tileX, int tileY){
	return VECTOR2(tileNS::WIDTH / 2 + tileX * tileNS::WIDTH,
		tileNS::HEIGHT / 2 + tileY * tileNS::HEIGHT);
}

ManagedTile* MapLoader::loadTile(int tileX, int tileY){

	char tileId = getTileIdAtLocation(tileX, tileY);

	// Get Tile Position
	VECTOR2 tilePos = getCoordsAtTileLocation(tileX, tileY);

	TextureManager* textureManager;
	stringstream ss;
	ss << tileImageFolder << tileset[tileId].imageName;

	if (tileTms.count(tileId) > 0){
		textureManager = tileTms[tileId];
	}
	else{
		textureManager = new TextureManager();
		//runtimeLog << "Created TM1" << endl;
		//runtimeLog << "New memory allocation at 0x" << textureManager << endl; // NEWLOGGING
		textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
		tileTms[tileId] = textureManager;
	}

	if (tileset[tileId].type != tileNS::type::FLOOR){
		Tile* t = nullptr;
		if (tileset[tileId].type == tileNS::type::COOKER){
			t = new Cooker(tileId);
		}
		else if (tileset[tileId].type == tileNS::type::FISHINGSPOT) {
			t = new Resource(tileId);
			((Resource*)t)->initialize(gamePtr, resourceNS::FISHING, textureManager);
		}
		else if (tileset[tileId].type == tileNS::type::MININGSPOT) {
			t = new Resource(tileId);
			((Resource*)t)->initialize(gamePtr, resourceNS::MINING, textureManager);
		}
		else if (tileset[tileId].type == tileNS::type::SPAWNER){
			t = new Spawner(tileId, gamePtr, tileset[tileId].spawnId, tileset[tileId].spawnCooldown, 0);
			//runtimeLog << "Created spawner1" << endl;
			//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
		}
		else if (tileset[tileId].type == tileNS::type::WALL){

			t = new Tile(tileId);
			//runtimeLog << "Created tile1" << endl;
			//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
		}
		else if (tileset[tileId].type == tileNS::type::ANIMATABLE){
			t = new AnimatableTile(tileId, false);
			t->initialize(gamePtr, textureManager);
			//runtimeLog << "Created animtable1" << endl;
			//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
		}

		//Hello developers! Found a crash here?
		//The reasoning for this is that you have a tile in your chunks definition with a specific char it's looking for
		//But no corresponding tile ID was found in your tiles defininition.
		//You can find the tile the game was attempting to search for in the 'tileId' variable.
		t->spawn();
		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);

		// Image things
		t->getImage()->setFrames(0, tileset[tileId].numFrames - 1);
		t->getImage()->setFrameDelay(tileset[tileId].frameTime);
		t->getImage()->setCols(tileset[tileId].numFrames);
		t->getImage()->setCurrentFrame(tileset[tileId].currentFrame);
		t->getImage()->setAnimTimer(tileset[tileId].globalFrameTime);

		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t, tileset[tileId].type);
	}
	else if (tileset[tileId].type == tileNS::type::FLOOR){

		Image* t = new Image();
		//runtimeLog << "Created image1" << endl;
		//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING

		t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, tileset[tileId].numFrames, tileset[tileId].frameTime, textureManager);
		t->setCurrentFrame(tileset[tileId].currentFrame);
		t->setAnimTimer(tileset[tileId].globalFrameTime);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t, tileNS::type::FLOOR);
	}
}

void MapLoader::update(float frameTime){

	for (auto it = tileset.begin(); it != tileset.end(); ++it){
		tileStruct* ts = &it->second;
		ts->globalFrameTime += frameTime;
		if (ts->globalFrameTime > ts->frameTime){
			ts->globalFrameTime -= ts->frameTime;
			ts->currentFrame++;
			if (ts->currentFrame < 0 || ts->currentFrame > ts->numFrames - 1){
				ts->currentFrame = 0;
			}
		}
	}

	pathRequestedThisFrame = false;

	queue<TileVector> toMove;
	queue<TileVector> toMoveTo;
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx)
	{
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity)
		{
			// Get coordinates on the map based on tile count
			int tileX = itx->first;
			int tileY = ity->first;

			int changeX = 0, changeY = 0;

			VECTOR2 vpCoords;
			int xPos, yPos;
			
			ManagedTile* mt = ity->second;
			
			if (mt->tile != nullptr){
				Tile* t = mt->tile;
				vpCoords = viewport->translate(t->getX(), t->getY());
				xPos = t->getX();
				yPos = t->getY();
				t->update();
				//runtimeLog << "ENTITY is at " << vpCoords.x << ", " << vpCoords.y << endl;
			}
			else if (mt->image != nullptr){
				Image* t = mt->image;
				vpCoords = viewport->translate(t->getX(), t->getY());
				xPos = t->getX();
				yPos = t->getY();
				//runtimeLog << "IMAGE is at " << vpCoords.x << ", " << vpCoords.y << endl;
			}

			TileVector bufferedTopLeftCoords = getBufferedTopLeftCoords();
			TileVector bufferedBottomRightCoords = getBufferedBottomRightCoords();

			// If offscreen, move to other side of screen
			if (vpCoords.x < bufferedTopLeftCoords.x){
				changeX = tileWidth;
			}
			else if (vpCoords.x > bufferedBottomRightCoords.x){
				changeX = -tileWidth;
			}

			if (vpCoords.y < bufferedTopLeftCoords.y){
				changeY = tileHeight;
			}
			else if (vpCoords.y > bufferedBottomRightCoords.y){
				changeY = -tileHeight;
			}

			//runtimeLog << changeX << " " << changeY << endl;

			//Apply change
			if (changeX != 0 || changeY != 0){
				int newTileX = tileX + changeX;
				int newTileY = tileY + changeY;
				if (newTileX >= 0 && newTileY >= 0
					&& newTileX < worldMap.size() * tileNS::CHUNK_WIDTH && newTileY < worldMap[0].size() * tileNS::CHUNK_HEIGHT){
					//runtimeLog << "Moving tile " << tileX << ", " << tileY << " (chunk " << (tileX / 16 + 1) << ", " << (tileY / 16 + 1) << ") with coords " << xPos << ", " << yPos << " at vp coords " << vpCoords.x << ", " << vpCoords.y << endl;
					toMove.push(TileVector(tileX, tileY));
					toMoveTo.push(TileVector(newTileX, newTileY));
				}
			}
		}
	}

	while (!toMove.empty()){
		// Move recorded location of tile
		TileVector oldLocation = toMove.front();
		ManagedTile* mt = loadedTiles[oldLocation.x][oldLocation.y];
		TileVector newLocation = toMoveTo.front();

		if (loadedTiles[newLocation.x].count(newLocation.y)){
			ManagedTile* m = loadedTiles[newLocation.x][newLocation.y];
			if (m->tile != nullptr) drawManager->removeObject(m->tile);
			else if (m->image != nullptr) drawManager->removeObject(m->image);
			m->destroy();
			delete m;
			loadedTiles[newLocation.x].erase(newLocation.y);
		}
		loadedTiles[newLocation.x][newLocation.y] = mt;
		loadedTiles[oldLocation.x].erase(oldLocation.y);
		
		char oldTileId = getTileIdAtLocation(oldLocation.x, oldLocation.y);
		char newTileId = getTileIdAtLocation(newLocation.x, newLocation.y);

		// New tile location
		VECTOR2 tilePos = getCoordsAtTileLocation(newLocation.x, newLocation.y);

		tileStruct newTileInfo = tileset[newTileId];
		
		// If they are different tiles or are unique entities (not floors and walls), need to delete and change it
		if (newTileId != oldTileId || (newTileInfo.type != tileNS::type::FLOOR && newTileInfo.type != tileNS::type::WALL)){

			// If both are the same class, just change textureManagers and misc info (Floors and walls only, others need to be recreated)
			if (newTileInfo.type == mt->type && (newTileInfo.type == tileNS::type::FLOOR || newTileInfo.type == tileNS::type::WALL)){
				TextureManager* textureManager;
				stringstream ss;
				ss << tileImageFolder << newTileInfo.imageName;

				// use pre-existing texturemanager if already exists
				if (tileTms.count(newTileId) > 0){
					textureManager = tileTms[newTileId];
				}
				else{
					textureManager = new TextureManager();
					//runtimeLog << "Created TM2" << endl;
					//runtimeLog << "New memory allocation at 0x" << textureManager << endl; // NEWLOGGING
					textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
					tileTms[newTileId] = textureManager;
				}

				if (mt->tile != nullptr){
					mt->tile->getImage()->setTextureManager(textureManager);
					mt->tile->getImage()->setFrames(0, newTileInfo.numFrames - 1);
					mt->tile->getImage()->setFrameDelay(newTileInfo.frameTime);
					mt->tile->getImage()->setCols(newTileInfo.numFrames);
					mt->tile->getImage()->setCurrentFrame(newTileInfo.currentFrame);
					mt->tile->getImage()->setAnimTimer(newTileInfo.globalFrameTime);

					mt->tile->setId(newTileId);
				}
				else if(mt->image != nullptr){
					mt->image->setTextureManager(textureManager);
					mt->image->setFrames(0, newTileInfo.numFrames - 1);
					mt->image->setFrameDelay(newTileInfo.frameTime);
					mt->image->setCols(newTileInfo.numFrames);
					mt->image->setCurrentFrame(newTileInfo.currentFrame);
					mt->image->setAnimTimer(newTileInfo.globalFrameTime);
				}
			}
			// Otherwise need a new object
			else{
				// Clear old data
				if (mt->tile != nullptr){
					Tile* t = mt->tile;
					delete t;
					drawManager->removeObject(mt->tile);
					mt->tile = nullptr;
				}
				else if(mt->image != nullptr) {
					Image* t = mt->image;
					delete t;
					drawManager->removeObject(mt->image);
					mt->image = nullptr;
				}

				// Make new tile

				TextureManager* textureManager;
				stringstream ss;
				ss << tileImageFolder << newTileInfo.imageName;

				// use pre-existing texturemanager if already exists
				if (tileTms.count(newTileId) > 0){
					textureManager = tileTms[newTileId];
				}
				else{
					textureManager = new TextureManager();
					//runtimeLog << "Created TM3" << endl;
					//runtimeLog << "New memory allocation at 0x" << textureManager << endl; // NEWLOGGING
					textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
					tileTms[newTileId] = textureManager;
				}
				if (newTileInfo.type != tileNS::type::FLOOR){
					Tile* t = nullptr;
					if (newTileInfo.type == tileNS::type::COOKER){
						t = new Cooker(newTileId);
					}
					else if (newTileInfo.type == tileNS::type::SPAWNER){
						t = new Spawner(newTileId, gamePtr, newTileInfo.spawnId, newTileInfo.spawnCooldown, 0);
						//runtimeLog << "Created Spawner2" << endl;
						//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
					}
					else if (newTileInfo.type == tileNS::type::WALL){

						t = new Tile(newTileId);
						//runtimeLog << "Created Tile2" << endl;
						//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
					}
					else if(newTileInfo.type == tileNS::type::FISHINGSPOT){
						t = new Resource(newTileId);
						((Resource*)t)->initialize(gamePtr, resourceNS::FISHING, textureManager);
						//runtimeLog << "Created Fishing2" << endl;
						//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
					}
					else if(newTileInfo.type == tileNS::type::MININGSPOT){
						t = new Resource(newTileId);
						((Resource*)t)->initialize(gamePtr, resourceNS::MINING, textureManager);
						//runtimeLog << "Created Mining2" << endl;
						//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING
					}
					else if (newTileInfo.type == tileNS::type::ANIMATABLE) {
						t = new AnimatableTile(newTileId, false);
						t->initialize(gamePtr, textureManager);
					}

					t->initialize(gamePtr, textureManager);
					t->setX(tilePos.x);
					t->setY(tilePos.y);
					t->spawn();

					// Image things
					t->getImage()->setFrames(0, newTileInfo.numFrames - 1);
					t->getImage()->setFrameDelay(newTileInfo.frameTime);
					t->getImage()->setCols(newTileInfo.numFrames);
					t->getImage()->setCurrentFrame(newTileInfo.currentFrame);
					t->getImage()->setAnimTimer(newTileInfo.globalFrameTime);

					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
					mt->type = newTileInfo.type;
				}
				else if (newTileInfo.type == tileNS::type::FLOOR){

					Image* t = new Image();
					//runtimeLog << "Created Image2" << endl;
					//runtimeLog << "New memory allocation at 0x" << t << endl; // NEWLOGGING

					t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, newTileInfo.numFrames, newTileInfo.frameTime, textureManager);
					t->setCurrentFrame(newTileInfo.currentFrame);
					t->setAnimTimer(newTileInfo.globalFrameTime);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->image = t;
					mt->type = tileNS::type::FLOOR;
				}
			}
		}

		// Move actual location of tile
		if (mt->tile != nullptr){
			mt->tile->setX(tilePos.x);
			mt->tile->setY(tilePos.y);
		}
		else if(mt->image != nullptr){
			mt->image->setX(tilePos.x);
			mt->image->setY(tilePos.y);
		}

		//runtimeLog << "Removed tile from " << toErase.front().x << ", " << toErase.front().y << endl;
		toMove.pop();
		toMoveTo.pop();
	}
}

TileVector MapLoader::getNearestTile(VECTOR2 coords){
	return TileVector(coords.x / tileNS::WIDTH, coords.y / tileNS::HEIGHT);
}

queue<VECTOR2> MapLoader::path(VECTOR2 startCoords, VECTOR2 endCoords){

	LARGE_INTEGER pfStart, pfEnd, pfFreq;
	QueryPerformanceCounter(&pfStart);
	QueryPerformanceFrequency(&pfFreq);

	queue<VECTOR2> path; // The actual path of coordinates to follow
	stack<VECTOR2> reversePath; // Reverse path to use to trace route back to start from end
	map<int, AStarNode> openList; // Open list
	map<int, AStarNode> closedList; // Closed list, don't consider these nodes
	bool pathFound = false;
	int nodesExplored = 0;

	float diagonalCost = sqrt(pow(tileNS::WIDTH, 2) + pow(tileNS::HEIGHT, 2)); // Precompute diagonal cost once so we don't have to do it later

	// Create start node and add to open list
	TileVector startNodeTile = getNearestTile(startCoords);
	AStarNode startNode = AStarNode(startNodeTile);
	startNode.collectiveCost = 0;
	startNode.estimatedCostToEnd = 0;
	startNode.totalCost = 0;
	openList[0] = startNode;

	// Note down where the end node is
	TileVector endNodeTile = getNearestTile(endCoords);
	char endTileId = getTileIdAtLocation(endNodeTile.x, endNodeTile.y);
	tileStruct endTileInfo = tileset[endTileId];

	AStarNode currentNode = openList[0];
	map<int, AStarNode>::iterator currentNodeIt;

	//runtimeLog << "Start node is " << startNodeTile.x << ", " << startNodeTile.y << endl;
	//runtimeLog << "End node is " << endNodeTile.x << ", " << endNodeTile.y << endl;

	// While there are tiles to find or path is not found, find path
	while (!openList.empty() && !pathFound && endTileInfo.type != tileNS::type::WALL){
		// Give up if no path found in reasonable time
		if (nodesExplored > 1000) break;

		// Find node with lowest cost and use it as currentNode
		currentNode = openList.begin()->second;
		currentNodeIt = openList.begin();
		for (map<int, AStarNode>::iterator it = openList.begin(); it != openList.end(); ++it){
			if (it->second.totalCost < currentNode.totalCost){
				currentNode = it->second;
				currentNodeIt = it;
			}
		}

		nodesExplored++;

		//runtimeLog << "Checking node at " << currentNode->tileCoords.x << ", " << currentNode->tileCoords.y << " with cost " << currentNode->totalCost << ". Estimate to end " << currentNode->estimatedCostToEnd << endl;

		// Move currentNode from openList to closeList
		openList.erase(currentNodeIt);
		int currNodeOnClosedList = closedList.size();
		closedList[closedList.size()] = currentNode;

		if (currentNode.tileCoords.x == endNodeTile.x && currentNode.tileCoords.y == endNodeTile.y){
			pathFound = true;
			//runtimeLog << "Found path!" << endl;
			break;
		}

		// Close off blocked corner
		unordered_map<int, unordered_map<int, bool>> walkableCorners;
		walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y - 1] = true;
		walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y + 1] = true;
		walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y - 1] = true;
		walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y + 1] = true;

		for (int x = currentNode.tileCoords.x - 1; x < currentNode.tileCoords.x + 2; ++x){
			for (int y = currentNode.tileCoords.y - 1; y < currentNode.tileCoords.y + 2; ++y){
				// Get tile information
				char tileId = getTileIdAtLocation(x, y);
				tileStruct tileInfo = tileset[tileId];

				if (tileInfo.type == tileNS::type::WALL || tileInfo.type == tileNS::type::COOKER){
					// Top left + top right
					if (x == currentNode.tileCoords.x && y == currentNode.tileCoords.y - 1){
						walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y - 1] = false;
						walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y - 1] = false;
					}
					// Top right + bottom right
					else if (x == currentNode.tileCoords.x + 1 && y == currentNode.tileCoords.y){
						walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y - 1] = false;
						walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y + 1] = false;
					}
					// Bottom left + bottom right
					else if (x == currentNode.tileCoords.x && y == currentNode.tileCoords.y + 1){
						walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y + 1] = false;
						walkableCorners[currentNode.tileCoords.x + 1][currentNode.tileCoords.y + 1] = false;
					}
					// Top left + bottom left
					else if (x == currentNode.tileCoords.x - 1 && y == currentNode.tileCoords.y){
						walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y - 1] = false;
						walkableCorners[currentNode.tileCoords.x - 1][currentNode.tileCoords.y + 1] = false;
					}
				}
			}
		}

		// Search surrounding nodes
		for (int x = currentNode.tileCoords.x - 1; x < currentNode.tileCoords.x + 2; ++x){
			for (int y = currentNode.tileCoords.y - 1; y < currentNode.tileCoords.y + 2; ++y){

				// Limit search to the map
				if (x < 0) x++;
				else if (x > worldMap.size() * tileNS::CHUNK_WIDTH) break;
				if (y < 0) y++;
				else if (y > worldMap[0].size() * tileNS::CHUNK_HEIGHT) break;

				// Get tile information
				char tileId = getTileIdAtLocation(x, y);
				tileStruct tileInfo = tileset[tileId];

				// Check if tile is on closed list
				bool isOnClosedList = false;
				for (map<int, AStarNode>::iterator it = closedList.begin(); it != closedList.end(); ++it){
					if (it->second.tileCoords.x == x && it->second.tileCoords.y == y){
						isOnClosedList = true;
					}
				}

				// If tile is walkable and is not on closed list, add it to open list
				if ((tileInfo.type != tileNS::type::WALL) && !isOnClosedList){
					AStarNode newNode = AStarNode(TileVector(x, y));
					newNode.parent = &closedList[currNodeOnClosedList];

					bool toAdd = true;

					float addedCost;
					// If both change in x and change in y exist, moving diagonally hence more cost
					if (abs(x - currentNode.tileCoords.x) > 0 && abs(y - currentNode.tileCoords.y) > 0){
						addedCost = diagonalCost;

						// If corner is blocked, don't add to openList
						if (!walkableCorners[x][y]){
							toAdd = false;
						}
					}
					else{
						addedCost = tileNS::WIDTH;
					}
					newNode.collectiveCost = currentNode.collectiveCost + addedCost;
					newNode.estimatedCostToEnd = (abs(x - endNodeTile.x) + abs(y - endNodeTile.y)) * tileNS::WIDTH;
					newNode.totalCost = newNode.collectiveCost + newNode.estimatedCostToEnd;

					//runtimeLog << "Adding new node at " << x << ", " << y << " with cost " << newNode->totalCost << endl;

					if (toAdd){
						// Check if the node is already in openList
						for (map<int, AStarNode>::iterator it = openList.begin(); it != openList.end(); ++it){
							if (newNode.tileCoords.x == it->second.tileCoords.x && newNode.tileCoords.y == it->second.tileCoords.y){

								// If new node is better, delete old node
								if (newNode.totalCost < it->second.totalCost){
									//runtimeLog << "Replacing node at " << x << ", " << y << " with cost " << it->second->totalCost << " for cost " << newNode->totalCost << endl;
									it = openList.erase(it);
									break;
								}
								else{
									toAdd = false;
								}
							}
						}
					}

					if (toAdd){
						if (openList.empty()) openList[0] = newNode;
						else{
							map<int, AStarNode>::iterator lastIt = openList.end();
							lastIt--;
							int key = lastIt->first;
							openList[++key] = newNode;
						}
					}

				}
			}
		}

	}

	if (pathFound){
		// Put coordinates of target in, so it will be the last node in the real path
		reversePath.push(endCoords);

		// Trace route back to start from target node
		while (currentNode.parent != nullptr){
			reversePath.push(getCoordsAtTileLocation(currentNode.tileCoords.x, currentNode.tileCoords.y));
			currentNode = *currentNode.parent;
		}
		reversePath.push(getCoordsAtTileLocation(currentNode.tileCoords.x, currentNode.tileCoords.y));

		// Reverse the stack into a queue
		while (!reversePath.empty()){
			path.push(reversePath.top());
			reversePath.pop();
		}
		// Remove first node if already standing on that node
		path.pop();
	}

	// Destroy everything here
	// MEMORY LEAK PLUGGED
	/*for (map<int, AStarNode*>::iterator it = openList.begin(); it != openList.end(); ++it){
		AStarNode* n = it->second;
		//runtimeLog << "Deleting " << n->tileCoords.x << ", " << n->tileCoords.y << endl;
		delete n;
		it->second = nullptr;
	}
	openList.clear();
	if (closedList.size() > 0){
		for (map<int, AStarNode*>::iterator it = closedList.begin(); it != closedList.end(); ++it){
			AStarNode* n = it->second;
			//runtimeLog << "Deleting " << n->tileCoords.x << ", " << n->tileCoords.y << endl;
			delete n;
			it->second = nullptr;
		}
		closedList.clear();
	}*/

	QueryPerformanceCounter(&pfEnd);

	runtimeLog << "Nodes explored: " << nodesExplored << endl;
	if (pathFound){
		runtimeLog << "Path length: " << path.size() << endl;
		runtimeLog << "First node: " << path.front().x << ", " << path.front().y << endl;
	}
	runtimeLog << "Time taken: " << ((float)(pfEnd.QuadPart - pfStart.QuadPart) / (float)pfFreq.QuadPart) << " seconds" << endl;
	runtimeLog << endl;
	
	return path;
}

VECTOR2 MapLoader::translateIdToCoords(char id){
	for (int x = 0; x < worldMap.size(); ++x){
		for (int y = 0; y < worldMap[x].size(); ++y){

			// Load tiles in chunk
			char chunkId = worldMap[x][y];
			for (int cx = 0; cx < tileNS::CHUNK_WIDTH; ++cx){
				for (int cy = 0; cy < tileNS::CHUNK_HEIGHT; ++cy){

					// Load tiles
					char tileId = chunks[chunkId]->tile[cx][cy];

					if (tileId == id){

						int tileX = x * tileNS::CHUNK_WIDTH + cx;
						int tileY = y * tileNS::CHUNK_HEIGHT + cy;

						float xPos = tileNS::WIDTH / 2 + tileX * tileNS::WIDTH;
						float yPos = tileNS::HEIGHT / 2 + tileY * tileNS::HEIGHT;

						return VECTOR2(xPos, yPos);
					}
				}
			}
		}
	}

	return VECTOR2(-1, -1);
}

Tile* MapLoader::getTileWithId(char id)
{
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx)
	{
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity)
		{
			if (ity->second->tile != nullptr)
			{
				if (ity->second->tile->getId() == id)
				{
					return ity->second->tile;
				}
			}
		}
	}
	return nullptr;
}