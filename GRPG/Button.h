//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

//MATTSCAN: LIKELIHOOD OF MEMORY LEAK: VERY UNLIKELY
/*
	MEMORY LEAK POSSIBILITIES:
		GRAPHICS PTR: DELETE CALLED IN GAME DESTRUCTOR
		VERTEXBUFFER: SAFE_RELEASE CALLED IN DESTRUCTOR AND RECREATE
*/

#ifndef _BUTTON_H              // Prevent multiple definitions if this 
#define _BUTTON_H              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "constants.h"
#include "graphics.h"
#include "textDX.h"

class Button
{
private:
	Graphics    *graphics;              // graphics system
	VertexC		vtx[4];
	int			sides;
	LP_VERTEXBUFFER vertexBuffer;
	std::string text;
	bool visible;
	bool initialized;

public:
	Button() {
		initialized = false;
	}

	//Destructor
	void destroy(){
		graphics = nullptr;
		deleteVertexBuffer();
	}
	virtual ~Button()
	{
		destroy();
	}

	//wrap this with a try catch
	void initializeRectangle(Graphics *g, float x, float y, float WIDTH, float HEIGHT, COLOR_ARGB backColor)
	{
		deleteVertexBuffer();
		graphics = g;                    // the graphics system

		vtx[0].x = x;
		vtx[0].y = y;
		vtx[0].z = 0.0f;
		vtx[0].rhw = 1.0f;
		vtx[0].color = backColor;
		// top right
		vtx[1].x = x + WIDTH;
		vtx[1].y = y;
		vtx[1].z = 0.0f;
		vtx[1].rhw = 1.0f;
		vtx[1].color = backColor;
		// bottom right
		vtx[2].x = x + WIDTH;
		vtx[2].y = y + HEIGHT;
		vtx[2].z = 0.0f;
		vtx[2].rhw = 1.0f;
		vtx[2].color = backColor;
		// bottom left
		vtx[3].x = x;
		vtx[3].y = y + HEIGHT;
		vtx[3].z = 0.0f;
		vtx[3].rhw = 1.0f;
		vtx[3].color = backColor;

		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	}
	bool initialize(Graphics *g, float x, float y, float WIDTH, float HEIGHT, COLOR_ARGB backColor, std::string txt)
	{
		visible = true;
		initialized = false;
		text = txt;
		vertexBuffer = NULL;
		sides = 4;
		//sidesOfCircle = 16;
		try {
			initializeRectangle(g, x, y, WIDTH, HEIGHT, backColor);
		}
		catch (...) {
			return false;
		}
		initialized = true;
		return true;
	}

	bool mouseOver(int mouseX, int mouseY)
	{//rectangle collision
		return mouseX >= vtx[0].x && mouseX <= vtx[2].x && mouseY >= vtx[0].y && mouseY <= vtx[2].y;
	}

	bool getVisible(){ return visible; }
	void setVisible(bool b){ visible = b; }
	string getText(){ return text; }
	void setText(string b){ text = b; }

	VertexC* getVtx() { return vtx; }

	void draw()
	{
		if (!visible || !initialized)
		{//DIIE
		}
		else
		{
			graphics->drawQuad(vertexBuffer, sides - 2);       // draw circle, - 2 because rectangle takes 2 primitives, so just - 2
		}
	}
	void draw(TextDX* font)
	{
		if (!visible || !initialized)
		{//DIIE
		}
		else
		{
			graphics->drawQuad(vertexBuffer, sides - 2);       // draw circle, - 2 because rectangle takes 2 primitives, so just - 2
			if (text != "")
			{
				font->print(text, (int)vtx[0].x, (int)vtx[0].y);
			}
		}
	}

	// Call when graphics device is lost.
	void onLostDevice()
	{
		SAFE_RELEASE(vertexBuffer);
	}

	// Call when graphics device is reset.
	void onResetDevice()
	{
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
	}

	void deleteVertexBuffer()
	{
		if (initialized)
			//You can't call delete, must call release for vertex buffers
			//vertexBuffer->Release();
			SAFE_RELEASE(vertexBuffer);
	}
};

#endif