#include "mapLoader.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "assets/map/";
	tileImageFolder = "assets/map/img/";
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

	Coordinates vpTopLeft = viewport->getTopLeft();
	Coordinates vpBottomRight = viewport->getBottomRight();

	float startX = tileNS::WIDTH / 2;
	float startY = tileNS::WIDTH / 2;
	for (int x = 0; x < worldMap.size(); ++x){
		for (int y = 0; y < worldMap[x].size(); ++y){

			// Load tiles in chunk
			char chunkId = worldMap[x][y];
			for (int cx = 0; cx < tileNS::CHUNK_WIDTH; ++cx){
				for (int cy = 0; cy < tileNS::CHUNK_HEIGHT; ++cy){

					// Load tiles
					char tileId = chunks[chunkId]->tile[cx][cy];

					float xPos = startX + (x * tileNS::CHUNK_WIDTH + cx) * tileNS::WIDTH;
					float yPos = startY + (y * tileNS::CHUNK_HEIGHT + cy) * tileNS::HEIGHT;

					Coordinates vpCoords = viewport->translate(xPos, yPos);
					float vpXPos = vpCoords.x;
					float vpYPos = vpCoords.y;

					// Is in viewport range
					if (vpXPos > 0 - tileNS::WIDTH / 2
						&& vpYPos > 0 - tileNS::HEIGHT / 2
						&& vpXPos < GAME_WIDTH + tileNS::WIDTH / 2
						&& vpYPos < GAME_HEIGHT + tileNS::HEIGHT / 2){

						TextureManager* textureManager;
						stringstream ss;
						ss << tileImageFolder << tileset[tileId].imageName;

						if (tileset[tileId].collidable){

							Tile* t = new Tile();

							if (tileTms.count(tileId) > 0){
								textureManager = tileTms[tileId];
							}
							else{
								textureManager = new TextureManager();
								textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
								tileTms[tileId] = textureManager;
							}

							t->initialize(gamePtr, textureManager);
							t->setX(xPos);
							t->setY(yPos);
							drawManager->addObject(t, 0);
							loadedTiles[x * tileNS::CHUNK_WIDTH + cx][y * tileNS::CHUNK_HEIGHT + cy] = new ManagedTile(t);
						} else {
							Image* t = new Image();
							if (tileTms.count(tileId) > 0){
								textureManager = tileTms[tileId];
							}
							else{
								textureManager = new TextureManager();
								textureManager->initialize(gamePtr->getGraphics(), ss.str().c_str());
								tileTms[tileId] = textureManager;
							}

							t->initialize(gamePtr->getGraphics(), tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
							t->setX(xPos);
							t->setY(yPos);
							drawManager->addObject(t, 0);
							loadedTiles[x * tileNS::CHUNK_WIDTH + cx][y * tileNS::CHUNK_HEIGHT + cy] = new ManagedTile(t);
						}
					}
				}
			}
		}
	}

	runtimeLog << viewport->getTopLeft().x << ", " << viewport->getTopLeft().y << endl;

}

void MapLoader::update(){
	for (unordered_map<int, unordered_map<int, ManagedTile*>>::iterator itx = loadedTiles.begin(); itx != loadedTiles.end(); ++itx){
		for (unordered_map<int, ManagedTile*>::iterator ity = loadedTiles[itx->first].begin(); ity != loadedTiles[itx->first].end(); ++ity){
			
		}
	}
}