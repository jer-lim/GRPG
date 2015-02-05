#include "game.h"
#include <thread>

namespace SoundManagerNS {
	const LPCSTR rocket = TEXT("assets/sound/rocket.wav");
}

class SoundManager {
public:
	static void playSound(LPCSTR filename){
		std::thread sound (threadSound, filename);
		sound.detach();
	}

	static void threadSound(LPCSTR filename){
		PlaySound(filename, NULL, SND_ALIAS);
	}
};