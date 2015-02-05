#ifndef _GRPG_H			         // Prevent multiple definitions if this 
#define _GRPG_H		             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "itemLoader.h"
#include "QuestLoader.h"
#include "player.h"
#include "image.h"
#include "UI.h"
#include "textDX.h"
#include "Enemy.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//=============================================================================
// This class is the core of the game
//=============================================================================
class Grpg : public Game
{
private:
    // game items
	Player* player;
	UI* ui;
	ItemLoader* itemLoader;
	QuestLoader* questLoader;
	GameEventManager* gameEventManager;
	TextDX* uiFont;

	TextureManager* missSplat;
	TextureManager* hitSplat;

	bool leftMouseWasDown;
	bool rightMouseWasDown;
	bool pro;//for cheaters

public:
    // Constructor
    Grpg();

    // Destructor
    virtual ~Grpg();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();

	bool processCommand(std::string command);

	UI* getUI() { return ui; }
	Player* getPlayer() { return player; }
	ItemLoader* getItemLoader() { return itemLoader; }
};

#endif
