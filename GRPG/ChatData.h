#ifndef _CHATDATA_H               // Prevent multiple definitions if this 
#define _CHATDATA_H               // file is included in more than one place

#include <string>
#include "textDX.h"
#include <vector>
#include "Button.h"
using namespace std;

class ChatDecision;

class ChatData
{
public:
	ChatData() {}
	virtual ~ChatData() {}
	virtual int getType() = 0;

	//Returns the height the text inside will require in order to be printed
	//Starts off at -1, unknown, requires calling of calculateHeightTAken to be set
	virtual float getHeightTaken() = 0;

	//Calculates the height the text embedded in this chatdata will take,
	//given the specified font and the width allowed for the text
	//You can tehn call getHeightTaken() to retrieve the result anytime.
	virtual void calculateHeightTaken(TextDX* font, float widthGiven) = 0;

	//Draws the text into the specificed rectangle with the specified font
	//Note that textRect will change, and will likely end up being the RECT that was used to draw
	//the last line of text of the chat
	virtual void draw(RECT* textRect, TextDX* font) = 0;
};

namespace chatNS
{
	const int LEFT = 1;
	const int MIDDLE = 2;
	const int RIGHT = 3;

	const int INFORMATIONTYPE = 1;
	const int DECISIONTYPE = 2;

	const int VERTICALLY = 1;
	const int HORIZONTALLY = 2;

	extern ChatDecision YESNO;

	const DWORD optionBackground = graphicsNS::BLUE;
}

class ChatInformation : public ChatData
{
private:
	string chatText;
	int side;
	//The height the text inside will require in order to be printed
	//Starts off at -1, unknown, requires calling of calculateHeightTAken to be set
	float heightTaken;
public:
	ChatInformation(){}
	ChatInformation(string ct, int s)
	{
		chatText = ct;
		side = s;
		heightTaken = -1;
	}

	virtual int getType()
	{
		return chatNS::INFORMATIONTYPE;
	}

	virtual string getText()
	{
		return chatText;
	}

	virtual int getSide()
	{
		return side;
	}

	//Returns the height the text inside will require in order to be printed
	//Starts off at -1, unknown, requires calling of calculateHeightTAken to be set
	virtual float getHeightTaken()
	{
		return heightTaken;
	}

	//Calculates the height the text embedded in this chatdata will take,
	//given the specified font and the width allowed for the text
	//You can tehn call getHeightTaken() to retrieve the result anytime.
	virtual void calculateHeightTaken(TextDX* font, float widthGiven)
	{
		//Calculate how much space each text requires
		RECT* textRect = new RECT();
		textRect->top = 0;
		textRect->left = 0;
		textRect->right = widthGiven;

		font->print(chatText, *textRect, DT_CALCRECT | DT_WORDBREAK);

		heightTaken = textRect->bottom;

		delete textRect;
	}

	virtual void draw(RECT* textRect, TextDX* font)
	{
		UINT formatSpecifier = DT_LEFT;
		if (side == chatNS::LEFT)
		{
			formatSpecifier = DT_LEFT;
		}
		else if (side == chatNS::MIDDLE)
		{
			formatSpecifier = DT_CENTER;
		}
		else if (side == chatNS::RIGHT)
		{
			formatSpecifier = DT_RIGHT;
		}

		font->print(chatText, *textRect, formatSpecifier | DT_WORDBREAK);
	}
};

struct ChatOption
{
	int id;
	string text;
	Button background;
};

class ChatDecision : public ChatData
{
private:
	float heightTaken;
	int displayType;
	vector<ChatOption> options;
	Graphics* graphics; // Reference required for drawing the background of every option
public:
	ChatDecision() {}
	ChatDecision(int dt)
	{
		displayType = dt;
	}

	virtual ~ChatDecision() {
		for (int i = 0; i < options.size(); i++)
		{
			options[i].background.destroy();
		}
	}

	virtual int getType() { return chatNS::DECISIONTYPE; }

	virtual void addOption(ChatOption op)
	{
		options.push_back(op);
	}

	virtual void removeAllOptions()
	{
		options.clear();
	}

	virtual int getDisplayType() { return displayType; }
	virtual void setDisplayType(int dt) { displayType = dt; }

	virtual void setGraphics(Graphics* g) { graphics = g; }

	//Returns the height the text inside will require in order to be printed
	//Starts off at -1, unknown, requires calling of calculateHeightTaken to be set
	virtual float getHeightTaken()
	{
		return heightTaken;
	}

	//Calculates the height the text embedded in this chatdata will take,
	//given the specified font and the width allowed for the text
	//You can tehn call getHeightTaken() to retrieve the result anytime.
	virtual void calculateHeightTaken(TextDX* font, float widthGiven)
	{
		//Calculate how much space each text requires
		RECT* textRect = new RECT();
		textRect->top = 0;
		textRect->left = 0;
		textRect->right = widthGiven;

		if (displayType == chatNS::HORIZONTALLY)
		{
			int maximum = 0;
			for (int i = 0; i < options.size(); i++)
			{
				font->print(options[i].text, *textRect, DT_CALCRECT);
				if (maximum < textRect->bottom)
				{
					maximum = textRect->bottom;
				}
			}
			heightTaken = maximum;
		}
		else if (displayType == chatNS::VERTICALLY)
		{
			heightTaken = 0;
			for (int i = 0; i < options.size(); i++)
			{
				font->print(options[i].text, *textRect, DT_CALCRECT);
				heightTaken += textRect->bottom;
			}
		}

		delete textRect;
	}

	//Copies all properties of this ChatDecision, and return a reference to that copy.
	//For use with the prebuilt ChatDecision at chatNS.
	virtual ChatDecision* generateCloneReference()
	{
		ChatDecision* cd = new ChatDecision(displayType);
		for (int i = 0; i < options.size(); i++)
		{
			cd->addOption(options[i]);
		}
		return cd;

	}

	virtual void draw(RECT* locationToDraw, TextDX* font)
	{
		if (displayType == chatNS::HORIZONTALLY)
		{
			float spaceGiven = (locationToDraw->right - locationToDraw->left) / options.size();
			locationToDraw->right = locationToDraw->left + spaceGiven;
			RECT* tempRect = new RECT();
			tempRect->left = 0;
			tempRect->top = 0;
			for (int i = 0; i < options.size(); i++)
			{
				//Calculate how much space the font would take
				font->print(options[i].text, *tempRect, DT_CALCRECT);
				//Calculate all 4 points for button
				if (!options[i].background.initialize(graphics,
					//X and Y: Calculate middle then reduce by half of text width and height
					locationToDraw->left + ((locationToDraw->right - locationToDraw->left)/2) - (tempRect->right / 2),
					locationToDraw->top + ((locationToDraw->bottom - locationToDraw->top)/2) - (tempRect->bottom / 2),
					tempRect->right, tempRect->bottom, SETCOLOR_ARGB(180, 255, 0, 0), ""))
				{
					throw new GameError(gameErrorNS::FATAL_ERROR, "Options background could not be initalized");
				}
				graphics->spriteEnd();
				graphics->spriteBegin();
				options[i].background.draw();
				font->print(options[i].text, *locationToDraw, DT_CENTER);
				//Move the rectangle right by a little bit
				locationToDraw->left = locationToDraw->right;
				locationToDraw->right = locationToDraw->left + spaceGiven;
			}
			delete tempRect;
		}
	}
};

#endif