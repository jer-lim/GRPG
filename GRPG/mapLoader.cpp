#include "mapLoader.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "assets/map/";
	tileImageFolder = "assets/map/img/";
}

void MapLoader::initialize(Game* game, Graphics* g, EntityManager* em){
	gamePtr = game;
	graphics = g;
	entityManager = em;
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

	int startX = 16;
	int startY = 16;
	for (int x = 0; x < worldMap.size(); ++x){
		for (int y = 0; y < worldMap[x].size(); ++y){

			// Load tiles in chunk
			char chunkId = worldMap[x][y];
			for (int cx = 0; cx < 16; ++cx){
				for (int cy = 0; cy < 16; ++cy){

					// Load tiles
					char tileId = chunks[chunkId]->tile[cx][cy];

					int xPos = startX + (x * 16 + cx) * tileNS::WIDTH;
					int yPos = startY + (y * 16 + cy) * tileNS::HEIGHT;

					if (xPos < GAME_WIDTH + 16 && yPos < GAME_HEIGHT + 16){

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
								textureManager->initialize(graphics, ss.str().c_str());
								tileTms[tileId] = textureManager;
							}

							t->initialize(gamePtr, textureManager);
							t->setX(xPos);
							t->setY(yPos);
							entityManager->addEntity(t, 0);
						} else {
							Image* t = new Image();
							if (tileTms.count(tileId) > 0){
								textureManager = tileTms[tileId];
							}
							else{
								textureManager = new TextureManager();
								textureManager->initialize(graphics, ss.str().c_str());
								tileTms[tileId] = textureManager;
							}

							t->initialize(graphics, tileNS::WIDTH, tileNS::HEIGHT, 1, textureManager);
							t->setX(xPos);
							t->setY(yPos);
							entityManager->addImage(t, 0);
						}
					}
				}
			}
		}
	}

}