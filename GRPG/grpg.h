//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Jeremy Lim
//  Student Number:     s10122326F
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

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
#include "ChatData.h"
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
	//For easter egg dropping. Every 5 easter eggs that drop, at least 1 is guaranteed to be rare or better.
	int easterEggCounter;

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
	QuestLoader* getQuestLoader() { return questLoader; }
	GameEventManager* getGameEventManager() { return gameEventManager; }

	//Attempt to finish all the quests that have all their required conditions completed, as long as they
	//have not had their reward given already
	//Will display the quest complete popup if required, as well as awarding any rewards that should be given
	//to the player's inventory. Since the quest complete popup is shown, any window headers already displayed
	//will be removed.
	void attemptQuestCompletions();

	//Attempt to drop an easter egg for an entity. Generates a new easter egg object and returns it.
	Entity* dropEasterEgg();
};

#endif
