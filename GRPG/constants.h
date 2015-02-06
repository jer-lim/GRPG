//MATTSCAN: LIKELIHOOD OF MEMORY LEAK: NO WAY

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.0

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

using namespace std;

//-----------------------------------------------
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------

// window
const char CLASS_NAME[] = "GRPG";
const char GAME_TITLE[] = "GRPG";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1200;               // width of game in pixels
const UINT GAME_HEIGHT = 600;               // height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 60.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const float GRAVITY = 6.67428e-11f;             // gravitational constant

// graphic images
const char TEXTURES_IMAGE[] = "player.png";  // game textures, path is defined in personNS
const char UI_IMAGE[] = "assets/ui/bottomrightbackground.png"; //UI texture
const char TAB_IMAGE[] = "assets/ui/tabs.png"; //UI tabs texture
const char HIT_IMAGE[] = "assets/ui/hit.png"; //Hit splat hit texture
const char MISS_IMAGE[] = "assets/ui/miss.png"; //Hit splat miss texture
const char WINDOW_IMAGE[] = "assets/ui/windowBackground.png"; // Window background texture
const char SHOP_IMAGE[] = "assets/ui/shopNote.png"; // Window background texture
const char FISHING_IMAGE[] = "assets/actions/fishing.png"; //The image shown above the player on fishing
const char MINING_IMAGE[] = "assets/actions/mining.png"; //The image shown above the player on mining

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY      = VK_MENU;         // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;       // Enter key


// weapon types
enum WEAPON {TORPEDO, SHIP, PLANET};
enum ITEM_MERGE { IMPOSSIBLE, INCOMPLETE, SUCCESSFUL };
enum INVENTORY_CHANGE { SUCCESS, MERGED, PARTIAL_MERGE, FAILED };
enum FOOD_STATE { RAW, BURNT, COOKED, DELICIOUS };

#endif
