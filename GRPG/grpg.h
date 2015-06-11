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

namespace GrpgNS
{
	////To keep note of spawned rifts and see if they are dead or not, Grpg uses the spawnlinks
	//in game, instead of directly accessing the memory, which may cause a crash if the entity is 
	//already dead and deleted. To ensure that spawnlinks added by Grpg does not collide with spawnlinks
	//added by other stuff, (i.e. the legitimate, planned uses like Spawners), a phrase is added in front of
	//the spawn links used by Grpg.
	const string riftSpawnPhrase = "Rifts";
	const int riftLimit = 1;
}

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
	RiftData* riftData;
	TextDX* uiFont;

	TextureManager* missSplat;
	TextureManager* hitSplat;
	//Aidil's dragonfire. Used to be stored in the Aidil class, but moved here as dragonfire
	//can continue to exist once Aidil has died and been deleted.
	TextureManager* dragonfireTexture;

	bool leftMouseWasDown;
	bool rightMouseWasDown;
	bool pro;//for cheaters
	//For easter egg dropping. Every 5 easter eggs that drop, at least 1 is guaranteed to be rare or better.
	int easterEggCounter;

	//Tracks the total number of rifts in game. This GRPG class will handle spawning.
	vector<Rift*> riftsInGame;

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
	TextureManager* getDragonfireTexture() { return dragonfireTexture; }
	RiftData* getRiftData() { return riftData; }

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
