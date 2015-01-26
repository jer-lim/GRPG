#ifndef _UI_H                 // Prevent multiple definitions if this 
#define _UI_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"

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

	const UINT chatHeight = 400;
	const UINT chatWidth = 500;
	const COLOR_ARGB chatColour = SETCOLOR_ARGB(192, 0, 0, 0);
}

// inherits from Entity class
class UI : public Entity
{
private:
	TextDX* uiText;
	Player* player; //Pointer to player to retrieve information about them
	Input* input; //The input system
	//The images that will draw the tab background onto the screen
	Image tabImage;
	TextureManager* tabTexture; // The texture of the tabs
	//The current active tab. From uiNS, either COMBATSTYLE, SKILLS or INVENTORY
	int activeTab;
	VertexC vtx[4];			//Vertex data for background
	LP_VERTEXBUFFER vertexBuffer;       // buffer to hold vertex data
	RECT textRect;               // text rectangle, used to specify and draw the chat box
	std::deque<std::string> text;       // UI text
	
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
	virtual void draw();
	virtual bool initialize(Game* gamePtr, Player* player, Input *in);

	// Checks if the mouse is currently over any part of the UI. Returns true if so
	virtual bool mouseInside();

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	void update(float frameTime);
};
#endif