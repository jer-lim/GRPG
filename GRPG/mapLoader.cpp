#include "mapLoader.h"
#include "drawManager.h"
#include "Viewport.h"
#include "Spawner.h"

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
			if (tileType == tileNS::type::SPAWNER){
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
		textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
		tileTms[tileId] = textureManager;
	}
	if (tileset[tileId].type == tileNS::type::SPAWNER){
		Spawner* t = new Spawner(gamePtr, tileset[tileId].spawnId, tileset[tileId].spawnCooldown, victim);

		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		t->spawn();
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
	else if (tileset[tileId].type == tileNS::type::WALL){

		Tile* t = new Tile();

		t->initialize(gamePtr, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
	else if (tileset[tileId].type == tileNS::type::FLOOR){

		Image* t = new Image();

		t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
		t->setX(tilePos.x);
		t->setY(tilePos.y);
		drawManager->addObject(t, tileNS::ZINDEX);
		return new ManagedTile(t);
	}
}

void MapLoader::update(){

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
		
		//CRIME SCENE
		if (loadedTiles[newLocation.x].count(newLocation.y)){
			ManagedTile* m = loadedTiles[newLocation.x][newLocation.y];
			if (m->tile != nullptr){
				drawManager->removeObject(m->tile);
			}
			else if (m->image != nullptr){
				drawManager->removeObject(m->image);
			}
			delete m;
		}
		loadedTiles[newLocation.x][newLocation.y] = mt;
		loadedTiles[oldLocation.x].erase(oldLocation.y);
		//END CRIME SCENE
		//VERDICT: NOT GUILTY
		
		char oldTileId = getTileIdAtLocation(oldLocation.x, oldLocation.y);
		char newTileId = getTileIdAtLocation(newLocation.x, newLocation.y);

		// New tile location
		VECTOR2 tilePos = getCoordsAtTileLocation(newLocation.x, newLocation.y);

		tileStruct oldTileInfo = tileset[oldTileId];
		tileStruct newTileInfo = tileset[newTileId];
		
		// If they are different tiles or are spawners, need to delete and change it
		if (newTileId != oldTileId || newTileInfo.type == tileNS::type::SPAWNER){

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
				// DEFINITE MEMORY LEAK HERE
				if (mt->tile != nullptr){
					// WHY ISN'T THIS REALLY DELETED
					//delete mt->tile;

					Tile* t = mt->tile;
					delete t;
					drawManager->removeObject(mt->tile);
					mt->tile = nullptr;
				}
				else {
					// WHY ISN'T THIS REALLY DELETED
					//delete mt->image;

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
					textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
					tileTms[newTileId] = textureManager;
				}

				if (newTileInfo.type == tileNS::type::SPAWNER){
					Spawner* t = new Spawner(gamePtr, newTileInfo.spawnId, newTileInfo.spawnCooldown, victim);

					t->initialize(gamePtr, textureManager);
					t->setX(tilePos.x);
					t->setY(tilePos.y);
					t->spawn();
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
				}
				if (newTileInfo.type == tileNS::type::WALL){

					Tile* t = new Tile();

					t->initialize(gamePtr, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->tile = t;
				}
				else if (newTileInfo.type == tileNS::type::FLOOR){

					Image* t = new Image();

					t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
					drawManager->addObject(t, tileNS::ZINDEX);
					mt->image = t;
				}
			}
		}

		// Move actual location of tile
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

TileVector MapLoader::getNearestTile(VECTOR2 coords){
	return TileVector(coords.x / tileNS::WIDTH, coords.y / tileNS::HEIGHT);
}

queue<VECTOR2> MapLoader::path(VECTOR2 startCoords, VECTOR2 endCoords){

	LARGE_INTEGER pfStart, pfEnd, pfFreq;
	QueryPerformanceCounter(&pfStart);
	QueryPerformanceFrequency(&pfFreq);

	queue<VECTOR2> path; // The actual path of coordinates to follow
	stack<VECTOR2> reversePath; // Reverse path to use to trace route back to start from end
	map<int, AStarNode*> openList; // Open list, ordered by f score
	map<int, AStarNode*> closedList; // Closed list, don't consider these nodes
	bool pathFound = false;
	int nodesExplored = 0;

	float diagonalCost = sqrt(pow(tileNS::WIDTH, 2) + pow(tileNS::HEIGHT, 2)); // Precompute diagonal cost once so we don't have to do it later

	// Create start node and add to open list
	TileVector startNodeTile = getNearestTile(startCoords);
	AStarNode* startNode = new AStarNode(startNodeTile);
	startNode->collectiveCost = 0;
	startNode->estimatedCostToEnd = 0;
	startNode->totalCost = 0;
	openList[0] = startNode;

	// Note down where the end node is
	TileVector endNodeTile = getNearestTile(endCoords);
	char endTileId = getTileIdAtLocation(endNodeTile.x, endNodeTile.y);
	tileStruct endTileInfo = tileset[endTileId];

	AStarNode* currentNode = openList[0];
	map<int, AStarNode*>::iterator currentNodeIt;

	runtimeLog << "Start node is " << startNodeTile.x << ", " << startNodeTile.y << endl;
	runtimeLog << "End node is " << endNodeTile.x << ", " << endNodeTile.y << endl;

	// While there are tiles to find or path is not found, find path
	while (!openList.empty() && !pathFound && endTileInfo.type != tileNS::type::WALL){
		// Give up if no path found in reasonable time
		if (nodesExplored > 1000) break;

		// Find node with lowest cost and use it as currentNode
		currentNode = openList.begin()->second;
		currentNodeIt = openList.begin();
		for (map<int, AStarNode*>::iterator it = openList.begin(); it != openList.end(); ++it){
			if (it->second->totalCost < currentNode->totalCost){
				currentNode = it->second;
				currentNodeIt = it;
			}
		}

		nodesExplored++;

		//runtimeLog << "Checking node at " << currentNode->tileCoords.x << ", " << currentNode->tileCoords.y << " with cost " << currentNode->totalCost << ". Estimate to end " << currentNode->estimatedCostToEnd << endl;

		// Move currentNode from openList to closeList
		openList.erase(currentNodeIt);
		closedList[closedList.size()] = currentNode;

		if (currentNode->tileCoords.x == endNodeTile.x && currentNode->tileCoords.y == endNodeTile.y){
			pathFound = true;
			//runtimeLog << "Found path!" << endl;
			break;
		}

		// Close off blocked corner
		unordered_map<int, unordered_map<int, bool>> walkableCorners;
		walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y - 1] = true;
		walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y + 1] = true;
		walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y - 1] = true;
		walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y + 1] = true;

		for (int x = currentNode->tileCoords.x - 1; x < currentNode->tileCoords.x + 2; ++x){
			for (int y = currentNode->tileCoords.y - 1; y < currentNode->tileCoords.y + 2; ++y){
				// Get tile information
				char tileId = getTileIdAtLocation(x, y);
				tileStruct tileInfo = tileset[tileId];

				if (tileInfo.type == tileNS::type::WALL){
					// Top left + top right
					if (x == currentNode->tileCoords.x && y == currentNode->tileCoords.y - 1){
						walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y - 1] = false;
						walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y - 1] = false;
					}
					// Top right + bottom right
					else if (x == currentNode->tileCoords.x + 1 && y == currentNode->tileCoords.y){
						walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y - 1] = false;
						walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y + 1] = false;
					}
					// Bottom left + bottom right
					else if (x == currentNode->tileCoords.x && y == currentNode->tileCoords.y + 1){
						walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y + 1] = false;
						walkableCorners[currentNode->tileCoords.x + 1][currentNode->tileCoords.y + 1] = false;
					}
					// Top left + bottom left
					else if (x == currentNode->tileCoords.x - 1 && y == currentNode->tileCoords.y){
						walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y - 1] = false;
						walkableCorners[currentNode->tileCoords.x - 1][currentNode->tileCoords.y + 1] = false;
					}
				}
			}
		}

		// Search surrounding nodes
		for (int x = currentNode->tileCoords.x - 1; x < currentNode->tileCoords.x + 2; ++x){
			for (int y = currentNode->tileCoords.y - 1; y < currentNode->tileCoords.y + 2; ++y){

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
				for (map<int, AStarNode*>::iterator it = closedList.begin(); it != closedList.end(); ++it){
					if (it->second->tileCoords.x == x && it->second->tileCoords.y == y){
						isOnClosedList = true;
					}
				}

				// If tile is walkable and is not on closed list, add it to open list
				if ((tileInfo.type == tileNS::type::FLOOR || tileInfo.type == tileNS::type::SPAWNER) && !isOnClosedList){
					AStarNode* newNode = new AStarNode(TileVector(x, y));
					newNode->parent = currentNode;

					bool toAdd = true;

					float addedCost;
					// If both change in x and change in y exist, moving diagonally hence more cost
					if (abs(x - currentNode->tileCoords.x) > 0 && abs(y - currentNode->tileCoords.y) > 0){
						addedCost = diagonalCost;

						// If corner is blocked, don't add to openList
						if (!walkableCorners[x][y]){
							toAdd = false;
						}
					}
					else{
						addedCost = tileNS::WIDTH;
					}
					newNode->collectiveCost = currentNode->collectiveCost + addedCost;
					newNode->estimatedCostToEnd = (abs(x - endNodeTile.x) + abs(y - endNodeTile.y)) * tileNS::WIDTH;
					newNode->totalCost = newNode->collectiveCost + newNode->estimatedCostToEnd;

					runtimeLog << "Adding new node at " << x << ", " << y << " with cost " << newNode->totalCost << endl;

					if (toAdd){
						// Check if the node is already in openList
						for (map<int, AStarNode*>::iterator it = openList.begin(); it != openList.end(); ++it){
							if (newNode->tileCoords.x == it->second->tileCoords.x && newNode->tileCoords.y == it->second->tileCoords.y){

								// If new node is better, delete old node
								if (newNode->totalCost < it->second->totalCost){
									runtimeLog << "Replacing node at " << x << ", " << y << " with cost " << it->second->totalCost << " for cost " << newNode->totalCost << endl;
									AStarNode* n = it->second;
									n->~AStarNode();
									delete n;
									it->second = 0;
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
							map<int, AStarNode*>::iterator lastIt = openList.end();
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
		while (currentNode->parent != nullptr){
			reversePath.push(getCoordsAtTileLocation(currentNode->tileCoords.x, currentNode->tileCoords.y));
			currentNode = currentNode->parent;
		}
		reversePath.push(getCoordsAtTileLocation(currentNode->tileCoords.x, currentNode->tileCoords.y));

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
	for (map<int, AStarNode*>::iterator it = openList.begin(); it != openList.end(); ++it){
		AStarNode* n = it->second;
		runtimeLog << "Deleting " << n->tileCoords.x << ", " << n->tileCoords.y << endl;
		delete n;
		it->second = nullptr;
	}
	openList.clear();
	if (closedList.size() > 0){
		for (map<int, AStarNode*>::iterator it = closedList.begin(); it != closedList.end(); ++it){
			AStarNode* n = it->second;
			runtimeLog << "Deleting " << n->tileCoords.x << ", " << n->tileCoords.y << endl;
			delete n;
			it->second = nullptr;
		}
		closedList.clear();
	}

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