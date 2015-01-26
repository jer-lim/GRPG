// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// textureManager.cpp v1.0
// A TextureManager object loads and maintains one texture file. 
// Create a TextureManager object for each texture file in the game.

#include "textureManager.h"
#include "globals.h"

//=============================================================================
// default constructor
//=============================================================================
TextureManager::TextureManager()
{
    texture = NULL;
    width = 0;
    height = 0;
    //file = NULL;
	file_string;
    graphics = NULL;
    initialized = false;            // set true when successfully initialized
	resetting = false;
}

//=============================================================================
// destructor
//=============================================================================
TextureManager::~TextureManager()
{
    SAFE_RELEASE(texture);
}

//=============================================================================
// Loads the texture file from disk.
// Post: returns true if successful, false if failed
//=============================================================================
bool TextureManager::initialize(Graphics *g, const char *f)
{
    try{
        graphics = g;                       // the graphics object
        //file = f;                           // the texture file
		file_string = string(f, strlen(f));

		hr = graphics->loadTexture(file_string.c_str(), TRANSCOLOR, width, height, texture);
        if (FAILED(hr))
        {
            SAFE_RELEASE(texture);
            return false;
        }
    }
    catch(...) {return false;}
    initialized = true;                    // set true when successfully initialized
    return true;
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void TextureManager::onLostDevice()
{
    if (!initialized)
        return;
	else if (!resetting)
	{
		SAFE_RELEASE(texture);
		resetting = true;
	}
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void TextureManager::onResetDevice()
{
    if (!initialized)
        return;
	else if (resetting)
	{
		//if (file && file[0] == '\0') {//if file is empty
		//file = file_string.c_str();
		//}
		graphics->loadTexture(file_string.c_str(), TRANSCOLOR, width, height, texture);
		resetting = false;
	}
}


