
#ifndef _GAME_H                 // Prevent multiple definitions if this 
#define _GAME_H                 // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <Mmsystem.h>
#include "graphics.h"
#include "input.h"
#include "constants.h"
#include "globals.h"
#include "gameError.h"

class Entity;
class DrawManager;
class Viewport;
class MapLoader;
class PersonLoader;

class Game
{
protected:
    // common game properties
    Graphics *graphics;         // pointer to Graphics
    Input   *input;             // pointer to Input
    HWND    hwnd;               // window handle
    HRESULT hr;                 // standard return type
    LARGE_INTEGER timeStart;    // Performance Counter start value
    LARGE_INTEGER timeEnd;      // Performance Counter end value
    LARGE_INTEGER timerFreq;    // Performance Counter frequency
    float   frameTime;          // time required for last frame
    float   fps;                // frames per second
    DWORD   sleepTime;          // number of milli-seconds to sleep between frames
    bool    paused;             // true if game is paused
    bool    initialized;

	DrawManager* drawManager;
	Viewport* viewport;
	MapLoader* mapLoader;
	PersonLoader* personLoader;
	Entity* mouseOverEntity = nullptr;

public:
    // Constructor
    Game();
    // Destructor
    virtual ~Game();

    // Member functions

    // Window message handler
    LRESULT messageHandler( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

    // Initialize the game
    // Pre: hwnd is handle to window
    virtual void initialize(HWND hwnd);

    // Call run repeatedly by the main message loop in WinMain
    virtual void run(HWND);

    // Call when the graphics device was lost.
    // Release all reserved video memory so graphics device may be reset.
    virtual void releaseAll();

    // Recreate all surfaces and reset all entities.
    virtual void resetAll();

    // Delete all reserved memory.
    virtual void deleteAll();

    // Render game items.
    virtual void renderGame();

    // Handle lost graphics device
    virtual void handleLostGraphicsDevice();

    // Set display mode (fullscreen, window or toggle)
    void setDisplayMode(graphicsNS::DISPLAY_MODE mode = graphicsNS::TOGGLE);

    // Return pointer to Graphics.
    Graphics* getGraphics() {return graphics;}

    // Return pointer to Input.
    Input* getInput()       {return input;}

    // Exit the game
    void exitGame()         {PostMessage(hwnd, WM_DESTROY, 0, 0);}

    // Pure virtual function declarations
    // These functions MUST be written in any class that inherits from Game

    // Update game items.
    virtual void update() = 0;

    // Perform AI calculations.
    virtual void ai() = 0;

    // Check for collisions.
    virtual void collisions() = 0;

    // Render graphics.
    // Call graphics->spriteBegin();
    //   draw sprites
    // Call graphics->spriteEnd();
    //   draw non-sprites
    virtual void render() = 0;

	// Process commands that the user has entered into the console
	// or the chat (i.e. kill the player, spawn enemies etc.)
	// Returns true if command was processed, false on failure
	virtual bool processCommand(std::string command) = 0;

	DrawManager* getDrawManager(){ return drawManager; }
	Viewport* getViewport(){ return viewport; }
	PersonLoader* getPersonLoader(){ return personLoader; }

	void setMouseOverEntity(Entity* ent){ mouseOverEntity = ent; }
	Entity* getMouseOverEntity(){ return mouseOverEntity; }
};

#endif
