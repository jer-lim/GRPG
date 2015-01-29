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
	bool initialize(Graphics *g, float x, float y, float WIDTH, float HEIGHT, COLOR_ARGB backColor, std::string txt)
	{
		visible = true;
		initialized = false;
		text = txt;
		vertexBuffer = NULL;
		sides = 4;
		//sidesOfCircle = 16;
		try {
			graphics = g;                    // the graphics system


			/*float radius = WIDTH;
			DWORD backColor = SETCOLOR_ARGB(255,255,255,255);
			for(int a=0;a<sidesOfCircle;++a)
			{
			//float angle = 360/sidesOfCircle*a;//get next point of circle
			//angle = angle*PI/180;//angle to rad
			float angle =(float)(2*PI/sidesOfCircle*a);
			vtx[a].x = x - cos(angle)*radius;
			vtx[a].y = y - sin(angle)*radius;
			vtx[a].z = 0.0f;	//keep this at 0.0f
			vtx[a].rhw = 1.0f;	//idk wtf rhw is, just keep as 1.0f
			vtx[a].color = SETCOLOR_ARGB(192,rand() % 255,rand() % 255,rand() % 255);
			}*/

			// top left

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
		catch (...) {
			return false;
		}
		initialized = true;
		return true;
	}
	void destroy(){ graphics = nullptr; }//i don't want to destroy the graphics object
	~Button(){ destroy(); }

	bool mouseOver(int mouseX, int mouseY)
	{//rectangle collision
		return mouseX >= vtx[0].x && mouseX <= vtx[2].x && mouseY >= vtx[0].y && mouseY <= vtx[2].y;
	}

	bool getVisible(){ return visible; }
	bool setVisible(bool b){ visible = b; }
	string getText(){ return text; }
	bool setText(string b){ text = b; }

	void draw()
	{
		if (!visible || !initialized)
		{//DIIE
		}
		else
		{
			graphics->drawQuad(vertexBuffer, sides - 2);       // draw circle, - 2 because rectangle takes 2 primitives, so just - 2
			graphics->spriteBegin();                // begin drawing sprites
			graphics->spriteEnd();                // begin drawing sprites
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
			graphics->spriteBegin();                // begin drawing sprites
			if (text != "")
			{
				font->print(text, (int)vtx[0].x, (int)vtx[0].y);
			}
			graphics->spriteEnd();                // begin drawing sprites
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
};

#endif