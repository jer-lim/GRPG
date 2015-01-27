#include "mapLoader.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "assets/map/";
	tileImageFolder = "assets/map/img/";
	bufferSize = 2;

	tileWidth = ceil(GAME_WIDTH / tileNS::WIDTH) + 2 * bufferSize + 1;
	tileHeight = ceil(GAME_HEIGHT / tileNS::HEIGHT) + 2 * bufferSize + 1;
}

void MapLoader::initialize(Game* game, DrawManager* dm, Viewport* vp){
	gamePtr = game;
	drawManager = dm;
	viewport = vp;
}

void MapLoader::load(){

	// Load tileset
	ifstream tilestream;
	tilestream.open(mapFolder + "tiles.gdef");
	if (tilestream.is_open()){
		char tileId;
		int tileCollidable;
		string tileFileName;
		while (!tilestream.eof()){

			tilestream >> tileId;
			tilestream >> tileCollidable;
			tilestream >> tileFileName;

			//Insert into a map
			if (tileCollidable == 1) tileset[tileId].collidable = TRUE;
			else tileset[tileId].collidable = FALSE;
			tileset[tileId].imageName = tileFileName;
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
		string trash;
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

					// Is in viewport range
					if (vpXPos > 0 - tileNS::WIDTH * bufferSize - tileNS::WIDTH / 2
						&& vpYPos > 0 - tileNS::HEIGHT * bufferSize - tileNS::HEIGHT / 2
						&& vpXPos < GAME_WIDTH + tileNS::WIDTH * bufferSize + tileNS::WIDTH / 2
						&& vpYPos < GAME_HEIGHT + tileNS::HEIGHT * bufferSize + tileNS::HEIGHT / 2){

						loadedTiles[tileX][tileY] = loadTile(tileX, tileY);
					}
				}
			}
		}
	}
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
		textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
		tileTms[tileId] = textureManager;
	}

	if (tileset[tileId].collidable){

		Tile* t = new Tile();	

		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
	else {

		Image* t = new Image();

		t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
}

void MapLoader::update(){
	queue<VECTOR2> toMove;
	queue<VECTOR2> toMoveTo;
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx){
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity){
			// Get coordinates on the map based on tile count
			int tileX = itx->first;
			int tileY = ity->first;

			int changeX = 0, changeY = 0;
			
			ManagedTile* mt = ity->second;
			if (mt->tile != nullptr){
				Tile* t = mt->tile;
				VECTOR2 vpCoords = viewport->translate(t->getX(), t->getY());

				// If offscreen, move to other side of screen
				if (vpCoords.x < 0 - tileNS::WIDTH * bufferSize - tileNS::WIDTH / 2){
					changeX = tileWidth;
				}
				else if (vpCoords.x > GAME_WIDTH + tileNS::WIDTH * bufferSize + tileNS::WIDTH / 2){
					changeX = -tileWidth;
				}

				if (vpCoords.y < 0 - tileNS::HEIGHT * bufferSize - tileNS::HEIGHT / 2){
					changeY = tileHeight;
				}
				else if (vpCoords.y > GAME_HEIGHT + tileNS::HEIGHT * bufferSize + tileNS::HEIGHT / 2){
					changeY = -tileHeight;
				}
			}
			else if (mt->image != nullptr){
				Image* t = mt->image;
				VECTOR2 vpCoords = viewport->translate(t->getX(), t->getY());

				// If offscreen, move to other side of screen
				if (vpCoords.x < 0 - tileNS::WIDTH * bufferSize - tileNS::WIDTH / 2){
					changeX = tileWidth;
				}
				else if (vpCoords.x > GAME_WIDTH + tileNS::WIDTH * bufferSize + tileNS::WIDTH / 2){
					changeX = -tileWidth;
				}

				if (vpCoords.y < 0 - tileNS::HEIGHT * bufferSize - tileNS::HEIGHT / 2){
					changeY = tileHeight;
				}
				else if (vpCoords.y > GAME_HEIGHT + tileNS::HEIGHT * bufferSize + tileNS::HEIGHT / 2){
					changeY = -tileHeight;
				}
			}

			//runtimeLog << changeX << " " << changeY << endl;

			//Apply change
			if (changeX != 0 || changeY != 0){
				int newTileX = tileX + changeX;
				int newTileY = tileY + changeY;
				if (newTileX >= 0 && newTileY >= 0){
					toMove.push(VECTOR2(tileX, tileY));
					toMoveTo.push(VECTOR2(newTileX, newTileY));
				}
			}
		}
	}

	while (!toMove.empty()){
		// Move recorded location of tile
		VECTOR2 oldLocation = toMove.front();
		ManagedTile* mt = loadedTiles[oldLocation.x][oldLocation.y];
		VECTOR2 newLocation = toMoveTo.front();
		loadedTiles[newLocation.x][newLocation.y] = mt;
		loadedTiles[toMove.front().x].erase(toMove.front().y);

		char oldTileId = getTileIdAtLocation(oldLocation.x, oldLocation.y);
		char newTileId = getTileIdAtLocation(newLocation.x, newLocation.y);

		// If they are different tiles, need to change it
		if (newTileId != oldTileId){
			tileStruct oldTileInfo = tileset[oldTileId];
			tileStruct newTileInfo = tileset[newTileId];

			// If both are collidable / not collidable, they are both entities / images so just change textureManagers
			if (newTileInfo.collidable == oldTileInfo.collidable){
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
					runtimeLog << "Switching tiles from" << oldTileId << " to " << newTileId << endl;
					mt->image->setTextureManager(textureManager);
				}
			}
			// Incompatible types, need new Entity / Image to store
			else{
				// Clear old data
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

				if (newTileInfo.collidable){

					Tile* t = new Tile();

					t->initialize(gamePtr, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
				}
				else {

					Image* t = new Image();

					t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->image = t;
				}
			}
		}

		// Move actual location of tile
		VECTOR2 tilePos = getCoordsAtTileLocation(newLocation.x, newLocation.y);
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