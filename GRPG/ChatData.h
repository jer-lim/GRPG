#ifndef _CHATDATA_H               // Prevent multiple definitions if this 
#define _CHATDATA_H               // file is included in more than one place

#include <string>
using namespace std;

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
};

namespace chatNS
{
	const int LEFT = 1;
	const int MIDDLE = 2;
	const int RIGHT = 3;

	const int INFORMATIONTYPE = 1;
	const int DECISIONTYPE = 2;
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
};

#endif