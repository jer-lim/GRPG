#include "mapLoader.h"

using namespace std;

MapLoader::MapLoader(){
	mapFolder = "map/";
}

void MapLoader::load(){

	ofstream log;
	log.open("log.txt");


	// Load tileset
	ifstream tilestream;
	tilestream.open(mapFolder + "tiles.gdef");
	if (tilestream.is_open()){
		char tileId;
		string tileFileName;
		while (!tilestream.eof()){

			tilestream >> tileId;
			tilestream >> tileFileName;

			//Insert into a map
			tileset[tileId] = tileFileName;
			log << "Loaded tile " << tileId << endl;
		}

		tilestream.close();
	}
	else
	{
		log << "Failed to open tiles" << endl;
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
			chunk tempChunk;
			for (int y = 0; y < 16; ++y){
				for (int x = 0; x < 16; ++x){
					tempChunk.tile[x][y] = chunkstream.get();
				}
				getline(chunkstream, trash);
			}


			log << "Loaded chunk " << chunkId << endl;

			//Insert into a map
			chunks[chunkId] = tempChunk;
		}

		chunkstream.close();
	}
	else
	{
		log << "Failed to open chunks" << endl;
	}

	// Load world map

	ifstream mapstream;
	mapstream.open(mapFolder + "world.gmap");
	if (mapstream.is_open()){
		string trash;
		char tempChar;
		int x = 0, y = 0;
		while (!mapstream.eof()){

			tempChar = mapstream.get();
			if (tempChar == '\n') y++;
			else if (tempChar != EOF){ // not EOF character
				worldMap[x++][y] = tempChar;
			}
		}

		log << "Loaded world map" << endl;
		mapstream.close();
	}
	else
	{
		log << "Failed to open worldmap" << endl;
	}

	log.close();

}