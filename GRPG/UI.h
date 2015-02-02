#ifndef _UI_H                 // Prevent multiple definitions if this 
#define _UI_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Button.h"
#include "Behavior.h"

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
	const int EQUIPS = 4;
	const int QUESTS = 5;
	const int totalTabs = 5;

	const int tabWIDTH = 34;
	const int tabHEIGHT = 40;
	const int tabLMargin = 7;
	const int tabMargin = 2;

	const UINT chatHeight = 200;
	const UINT chatWidth = 500;
	const COLOR_ARGB chatColour = SETCOLOR_ARGB(50, 0, 0, 0);

	const UINT healthHeight = 40;
	// Calculate the width of the health bar
	// Health bar centralizes in the middle, left maximum chat, right as much as left
	const float healthWidth = (GAME_WIDTH / 2 - uiNS::chatWidth) * 2;
	const COLOR_ARGB healthColor = SETCOLOR_ARGB(180, 255, 0, 0);
	const COLOR_ARGB noHealthColor = SETCOLOR_ARGB(180, 0, 0, 0);

	const COLOR_ARGB rightClickBG = SETCOLOR_ARGB(255, 0, 0, 0);

	// Coordindates referring to the height and width of the X icon at the top left
	// of the window texture
	const UINT windowXWidth = 28;
	const UINT windowXHeight = 17;

	//Coordindates for shop
	const int shopColMax = 9;
	//Margins from the edges of the image
	const int shopLMargin = 20;
	const int shopTMargin = 41;
	// Margins between shop items
	const int shopWMargin = 6;
	const int shopHMargin = 15;
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
	TextureManager* uiImgTexture;
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

	// Right click menu
	vector<Behavior*> menus;
	VECTOR2 menuTop;
	Button rightClickBackground;	// Vertex data for drawing the background

	// WINDOW SYSTEM

	// The background of the big window that appears in the middle of the game
	//Used for store, quests etc.
	TextureManager* windowTexture;
	Image windowImage; // The window image that is drawn in the middle
	// The header text in the window. If empty, signifies no window and no window will be drawn
	string windowHeader;
	RECT* shopRect; // The rectangle used for drawing the shop text
	vector<Entity* > items; //The list of items that will be sold in the shop
	
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
	virtual bool mouseInside(Viewport* vp);

	// Add text to console
	// Only the first line of text in str will be displayed.
	virtual void addChatText(const std::string &str);     // add text to console

	//Draws a right click 
	//See removeRightClickMenu
	virtual void setRightClickMenu(vector<Behavior*> behaviors);

	//Stop drawing the right click menu
	//See setRightClickMenu
	virtual void removeRightClickMenu() { rightClickBackground.setVisible(false); }

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	void update(float frameTime, Game* gamePtr);

	//Causes the UI to update as if a click had been performed
	void performClick();

	float getTopLeftX(){ return getX() - uiNS::WIDTH / 2; }
	float getTopLeftY(){ return getY() - uiNS::HEIGHT / 2; }

	virtual string getType(){ return "UI"; }

	// Start drawing the window in the middle of the screen, with the specified header
	// that is shown at the top of the window
	// Warning: You MUST specify a header, or the window won't be drawn
	virtual void drawWindow(string header)
	{
		windowHeader = header;
		//Make the player's active tab the inventory
		activeTab = uiNS::INVENTORY;
	}

	// Removes the shop window, if any, is being drawn on the screen
	virtual void removeWindow() 
	{
		windowHeader = ""; 
		items.clear();
	}

	virtual void setShopItems(vector<Entity* > i)
	{
		items = i;
	}

	//Gets the items shown in the shop. If empty, no items are shown
	virtual vector<Entity*> getShopItems() { return items; }

	// Gets the string shown in the window header
	// If string is an empty string ("") no window is currently open
	virtual string getWindowHeader() { return windowHeader; }

	int getActiveTab(){ return activeTab; }
	void setActiveTab(int i)
	{
		if (activeTab != i)
		{
			
			switch (activeTab)
			{
			case uiNS::INVENTORY:
			map<int, Entity*>* slotList = player->getInventory()->getSlotList();
			for (std::map<int, Entity*>::iterator it = slotList->begin(); it != slotList->end(); ++it)
			{
				this->game->getDrawManager()->removeObject(it->second);
				player->getInventory()->setByDrawnManager(false);
			}
			break;
			}

			switch (i)
			{
			case uiNS::INVENTORY:
			map<int, Entity*>* slotList = player->getInventory()->getSlotList();
			for (std::map<int, Entity*>::iterator it = slotList->begin(); it != slotList->end(); ++it)
			{
				this->game->getDrawManager()->addObject(it->second);
				player->getInventory()->setByDrawnManager(true);
			}
			break;
			}
			
			activeTab = i;
		}
	}
};
#endif