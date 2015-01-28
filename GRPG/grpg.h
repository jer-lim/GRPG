#ifndef _GRPG_H			         // Prevent multiple definitions if this 
#define _GRPG_H		             // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "itemLoader.h"
#include "player.h"
#include "image.h"
#include "UI.h"
#include "textDX.h"
#include "Enemy.h"

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
	TextDX* uiFont;
	Entity* mouseOverEntity = nullptr;

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

	void setMouseOverEntity(Entity* ent){ mouseOverEntity = ent; }
	Entity* getMouseOverEntity(){ return mouseOverEntity; }

	bool processCommand(std::string command);
};

#endif
