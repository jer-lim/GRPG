#ifndef _CHATDATA_H               // Prevent multiple definitions if this 
#define _CHATDATA_H               // file is included in more than one place

#include <string>
#include "textDX.h"
#include <vector>
#include "Button.h"
#include "OptionNotifier.h"

using namespace std;

class ChatDecision;
class UI;

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

	//Originally intended to be a simple yes / no decision split that can be put into any chat decision
	//for ease of use instead of having to constantly recreate a YES/NO ChatDecision whenever you need one.
	//But does not work well, don't use. Problems:
	// 1) Lots of special code need to be placed into the talking phrase to handle this object
	// and ensure it is not deleted
	// 2) After used, it needs to be reset otherwise the unselected option will remain dark blue and cannot
	// be selected, and the object will ignore all further commands.
	// 3) Cannot customize the yes / no id, they are affixed to 1 and 2, which means you must reserve those
	// just for the use of this Yes / No object.
	extern ChatDecision YESNO;

	const COLOR_ARGB optionBackground = graphicsNS::BLUE;
	const COLOR_ARGB optionDismissedBackground = SETCOLOR_ARGB(160, 0, 0, 160);
	const int optionMargin = 5;
}

class ChatInformation : public ChatData
{
private:
	string chatText;
	int side;
	//The height the text inside will require in order to be printed
	//Starts off at -1, unknown, requires calling of calculateHeightTaken to be set
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
	int optionChosen;
	OptionNotifier* caller;
public:
	ChatDecision() {
		optionChosen = -1;
		caller = nullptr;
	}
	ChatDecision(int dt)
	{
		displayType = dt;
		optionChosen = -1;
		caller = nullptr;
	}

	virtual ~ChatDecision() {
		for (int i = 0; i < options.size(); i++)
		{
		//	options[i].background->destroy();
		}
	}

	virtual int getType() { return chatNS::DECISIONTYPE; }

	virtual void addOption(ChatOption op)
	{
		options.push_back(op);
	}

	virtual void addOption(int i, string s)
	{
		ChatOption co;
		co.id = i;
		co.text = s;
		options.push_back(co);
	}

	virtual void removeAllOptions()
	{
		options.clear();
	}

	virtual int getDisplayType() { return displayType; }
	virtual void setDisplayType(int dt) { displayType = dt; }

	virtual void setGraphics(Graphics* g) { graphics = g; }

	virtual void setCaller(OptionNotifier* whoToTell) { caller = whoToTell; }

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
				heightTaken += textRect->bottom + chatNS::optionMargin;
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
			COLOR_ARGB chosenColour;
			for (int i = 0; i < options.size(); i++)
			{
				//Calculate how much space the font would take
				font->print(options[i].text, *tempRect, DT_CALCRECT);
				
				//Sets colour correctly
				if (optionChosen == -1 || optionChosen == i)
				{
					chosenColour = chatNS::optionBackground;
				}
				else
				{
					chosenColour = chatNS::optionDismissedBackground; 
				}

				//Calculate all 4 points for button
				if (!options[i].background.initialize(graphics,
					//X and Y: Calculate middle then reduce by half of text width and height
					locationToDraw->left + ((locationToDraw->right - locationToDraw->left)/2) - (tempRect->right / 2) - chatNS::optionMargin,
					locationToDraw->top + ((locationToDraw->bottom - locationToDraw->top)/2) - (tempRect->bottom / 2),
					tempRect->right + (chatNS::optionMargin*2) /* *2 for margin on both left and right */, tempRect->bottom, chosenColour, ""))
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
		else if (displayType == chatNS::VERTICALLY)
		{
			//Minus margin because the first time we don't want an extra margin, and it's added on the
			//first loop already.
			locationToDraw->bottom = locationToDraw->top - chatNS::optionMargin;
			RECT* tempRect = new RECT();
			tempRect->left = 0;
			tempRect->top = 0;
			COLOR_ARGB chosenColour;
			for (int i = 0; i < options.size(); i++)
			{
				//Calculate how much space the font would take
				font->print(options[i].text, *tempRect, DT_CALCRECT);

				locationToDraw->top = locationToDraw->bottom + chatNS::optionMargin;
				locationToDraw->bottom = locationToDraw->top + tempRect->bottom;

				//Sets colour correctly
				if (optionChosen == -1 || optionChosen == i)
				{
					chosenColour = chatNS::optionBackground;
				}
				else
				{
					chosenColour = chatNS::optionDismissedBackground;
				}

				//Calculate all 4 points for button
				if (!options[i].background.initialize(graphics,
					//X and Y: Calculate middle then reduce by half of text width and height
					locationToDraw->left + ((locationToDraw->right - locationToDraw->left) / 2) - (tempRect->right / 2) - chatNS::optionMargin,
					locationToDraw->top + ((locationToDraw->bottom - locationToDraw->top) / 2) - (tempRect->bottom / 2),
					tempRect->right + (chatNS::optionMargin * 2) /* *2 for margin on both left and right */, tempRect->bottom, chosenColour, ""))
				{
					throw new GameError(gameErrorNS::FATAL_ERROR, "Options background could not be initalized");
				}
				graphics->spriteEnd();
				graphics->spriteBegin();
				options[i].background.draw();
				font->print(options[i].text, *locationToDraw, DT_CENTER);
			}
			delete tempRect;
		}
	}

	//Checks if the player's mouse is currently over any option in this decision tree.
	//If the player's mouse is over an option, then checks that option, dimming all other options.
	//It will also remember this option, and return true.
	//All further calls to this method once an option has been chosen will ALWAYS return false, no matter where the player's mouse currently is.
	//If the player's mouse is currently not over an option then false will be returned.
	//If an option was clicked, then the appropriate notifier will also be informed with the option.
	virtual bool checkMouseClick(float mouseX, float mouseY, UI* ui);
};

#endif