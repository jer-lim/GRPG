#include "mapLoader.h"

using namespace std;

void MapLoader::load(){
	ofstream log;
	log.open("log.txt");
	log << "Test log";
	log.close();
}