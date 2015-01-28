#include "mapLoader.h"
#include "drawManager.h"
#include "Viewport.h"
#include "Spawner.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "assets/map/";
	tileImageFolder = "assets/map/img/";
	bufferSize = 2;

	tileWidth = ceil(GAME_WIDTH / tileNS::WIDTH) + 2 * bufferSize + 1;
	tileHeight = ceil(GAME_HEIGHT / tileNS::HEIGHT) + 2 * bufferSize + 1;
}

void MapLoader::initialize(Game* game){
	gamePtr = game;
	drawManager = gamePtr->getDrawManager();
	viewport = gamePtr->getViewport();
}

void MapLoader::load(){

	LARGE_INTEGER timeStart, timeEnd, timerFreq;
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
		string tileFileName;
		while (!tilestream.eof()){

			tilestream >> tileId;
			tilestream >> tileType;
			tilestream >> tileFileName;

			//Insert into a map
			tileset[tileId].type = tileType;
			tileset[tileId].imageName = tileFileName;
			if (tileType == 2){ //spawner
				tilestream >> tileset[tileId].spawnId;
				tilestream >> tileset[tileId].spawnCooldown;
			}
			else{
				tilestream >> trash >> trash;
			}
			runtimeLog << "Loaded tile " << tileId << endl;
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
			}else if (tempChar != EOF){ // not EOF character
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

	runtimeLog << "Starting initial scene build" << endl;

	// Display world map
	// Load each chunk

	VECTOR2 vpTopLeft = viewport->getTopLeft();
	VECTOR2 vpBottomRight = viewport->getBottomRight();
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

					TileVector bufferedTopLeftCoords = getBufferedTopLeftCoords();
					TileVector bufferedBottomRightCoords = getBufferedBottomRightCoords();

					// Is in viewport range
					if (vpXPos > bufferedTopLeftCoords.x
						&& vpYPos > bufferedTopLeftCoords.y
						&& vpXPos < bufferedBottomRightCoords.x
						&& vpYPos < bufferedBottomRightCoords.y){

						loadedTiles[tileX][tileY] = loadTile(tileX, tileY);
					}
				}
			}
		}
	}

	runtimeLog << "Finished initial scene build" << endl;
	runtimeLog << "Finished map startup sequence" << endl;

	QueryPerformanceCounter(&timeEnd);
	runtimeLog << "Map startup sequence finished in " << ((float)(timeEnd.QuadPart - timeStart.QuadPart) / (float)timerFreq.QuadPart) << " seconds" << endl;

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

TileVector MapLoader::getCoordsAtTileLocation(int tileX, int tileY){
	return TileVector(tileNS::WIDTH / 2 + tileX * tileNS::WIDTH,
		tileNS::HEIGHT / 2 + tileY * tileNS::HEIGHT);
}

ManagedTile* MapLoader::loadTile(int tileX, int tileY){

	char tileId = getTileIdAtLocation(tileX, tileY);

	// Get Tile Position
	TileVector tilePos = getCoordsAtTileLocation(tileX, tileY);

	TextureManager* textureManager;
	stringstream ss;
	ss << tileImageFolder << tileset[tileId].imageName;

	if (tileTms.count(tileId) > 0){
		textureManager = tileTms[tileId];
	}
	else{
		textureManager = new TextureManager();
		textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
		tileTms[tileId] = textureManager;
	}
	if (tileset[tileId].type == 2){
		Spawner* t = new Spawner(gamePtr, tileset[tileId].spawnId, tileset[tileId].spawnCooldown, victim);

		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		t->spawn();
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
	else if (tileset[tileId].type == 1){

		Tile* t = new Tile();

		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
	else if(tileset[tileId].type == 0){

		Image* t = new Image();

		t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
}

void MapLoader::update(){
	queue<TileVector> toMove;
	queue<TileVector> toMoveTo;
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx){
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity){
			// Get coordinates on the map based on tile count
			int tileX = itx->first;
			int tileY = ity->first;

			int changeX = 0, changeY = 0;

			VECTOR2 vpCoords;
			
			ManagedTile* mt = ity->second;
			if (mt->tile != nullptr){
				Tile* t = mt->tile;
				vpCoords = viewport->translate(t->getX(), t->getY());
			}
			else if (mt->image != nullptr){
				Image* t = mt->image;
				vpCoords = viewport->translate(t->getX(), t->getY());
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
		loadedTiles[newLocation.x][newLocation.y] = mt;
		loadedTiles[toMove.front().x].erase(toMove.front().y);

		char oldTileId = getTileIdAtLocation(oldLocation.x, oldLocation.y);
		char newTileId = getTileIdAtLocation(newLocation.x, newLocation.y);

		// If they are different tiles, need to change it
		if (newTileId != oldTileId){
			tileStruct oldTileInfo = tileset[oldTileId];
			tileStruct newTileInfo = tileset[newTileId];

			// If both are the same class, just change textureManagers
			if (newTileInfo.type == oldTileInfo.type){
				TextureManager* textureManager;
				stringstream ss;
				ss << tileImageFolder << newTileInfo.imageName;

				// use pre-existing texturemanager if already exists
				if (tileTms.count(newTileId) > 0){
					textureManager = tileTms[newTileId];
				}
				else{
					textureManager = new TextureManager();
					textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
					tileTms[newTileId] = textureManager;
				}

				if (mt->tile != nullptr){
					mt->tile->getImage()->setTextureManager(textureManager);
				}
				else{
					mt->image->setTextureManager(textureManager);
				}
			}
			// Incompatible types, need new Entity / Image to store
			else{
				// Clear old data
				// MEMORY LEAK HERE
				if (mt->tile != nullptr){
					//delete mt->tile;
					drawManager->removeObject(mt->tile);
					mt->tile = nullptr;
				}
				else {
					//delete mt->image;
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
					textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
					tileTms[newTileId] = textureManager;
				}

				if (newTileInfo.type == 2){
					Spawner* t = new Spawner(gamePtr, newTileInfo.spawnId, newTileInfo.spawnCooldown, victim);

					t->initialize(gamePtr, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
				}
				if (newTileInfo.type == 1){

					Tile* t = new Tile();

					t->initialize(gamePtr, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
				}
				else if(newTileInfo.type == 0){

					Image* t = new Image();

					t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->image = t;
				}
			}
		}

		// Move actual location of tile
		TileVector tilePos = getCoordsAtTileLocation(newLocation.x, newLocation.y);
		if (mt->tile != nullptr){
			mt->tile->setX(tilePos.x);
			mt->tile->setY(tilePos.y);
		}
		else{
			mt->image->setX(tilePos.x);
			mt->image->setY(tilePos.y);
		}

		//runtimeLog << "Removed tile from " << toErase.front().x << ", " << toErase.front().y << endl;
		toMove.pop();
		toMoveTo.pop();
	}
}