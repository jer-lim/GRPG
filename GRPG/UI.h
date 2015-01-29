#ifndef _UI_H                 // Prevent multiple definitions if this 
#define _UI_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Button.h"

namespace uiNS
{
	const int WIDTH = 190;                   // image width
	const int HEIGHT = 264;                  // image height
	const int X = GAME_WIDTH - WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT - HEIGHT / 2;
	const int textSize = 20;

	const int COMBATSTYLE = 1;
	const int SKILLS = 2;
	const int INVENTORY = 3;

	const int tabWIDTH = 53;
	const int tabHEIGHT = 40;
	const int tabLMargin = 15;
	const int tabMargin = 5;

	const UINT chatHeight = 200;
	const UINT chatWidth = 500;
	const COLOR_ARGB chatColour = SETCOLOR_ARGB(50, 0, 0, 0);

	const UINT healthHeight = 40;
	// Calculate the width of the health bar
	// Health bar centralizes in the middle, left maximum chat, right as much as left
	const float healthWidth = (GAME_WIDTH / 2 - uiNS::chatWidth) * 2;
	const COLOR_ARGB healthColor = SETCOLOR_ARGB(180, 255, 0, 0);
	const COLOR_ARGB noHealthColor = SETCOLOR_ARGB(180, 0, 0, 0);
}

// inherits from Entity class
class UI : public Entity
{
private:

	//GENERAL UI VARIABLES
	Game* game;
	TextDX* uiText;
	Player* player; //Pointer to player to retrieve information about them
	Input* input; //The input system

	// TABBING SYSTEM

	//The images that will draw the tab background onto the screen
	Image tabImage;
	TextureManager* tabTexture; // The texture of the tabs
	//The current active tab. From uiNS, either COMBATSTYLE, SKILLS or INVENTORY
	int activeTab;

	// CHAT VARIABLES

	Button chatRect;				// Vertex data to show the chat rectangle at the bottom left
	RECT textRect;               // text rectangle, used to specify and draw the chat box
	std::deque<std::string> text;       // UI text
	int rows;	//Total number of rows in the chat
	int rowHeight;	//Amount of pixels each height take in the chat box

	// HEALTH BAR
	Button health;				//Vertex data for the entirety of the health bar
	Button availableHealth;		// Vertex data for only the health that the player has
	
protected:
	//Draws the specified tab number onto the screen on the correct location
	//Also see drawTabContents
	void drawTab(int tabNumber);

	//Draws the specified tab contents onto the screen on the correct location
	//Also see drawTab
	void drawTabContents(int tabNumber);
public:
	// constructor
	UI();

	// Destructor
	~UI();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Input *in);

	// Process console command
	// Returns true if processing was done, false otherwise
	virtual bool UI::processCommand(const std::string commandStr);

	// Checks if the mouse is currently over any part of the UI. Returns true if so
	virtual bool mouseInside(Viewport vp);

	// Add text to console
	// Only the first line of text in str will be displayed.
	virtual void addChatText(const std::string &str);     // add text to console

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	void update(float frameTime, Game* gamePtr);

	//Causes the UI to update as if a click had been performed
	void performClick();

	string view();

	float getTopLeftX(){ return getX() - uiNS::WIDTH / 2; }
	float getTopLeftY(){ return getY() - uiNS::HEIGHT / 2; }
};
#endif