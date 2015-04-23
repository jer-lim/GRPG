#include "ChatData.h"
#include "UI.h"

namespace chatNS
{
	ChatDecision YESNO = ChatDecision();
}

bool ChatDecision::checkMouseClick(float mouseX, float mouseY, UI* ui)
{
	if (optionChosen != -1)
	{
		return false;
	}
	for (int i = 0; i < options.size(); i++)
	{
		if (options[i].background.mouseOver(mouseX, mouseY))
		{
			//Mouse is over this option!
			optionChosen = i;
			break;
		}
	}

	if (optionChosen == -1)
	{
		return false;
	}

	if (caller != nullptr)
	{
		//We have to clone this
		//If we don't do this and simply pass the object in, then there is a possiblity that the following happens:
		//1) The caller attempts to stop the conversation (calling ui->removeWindow())
		//2) The UI removes the window, deleting all theChatOptions
		//3) The ChatOption PASSED IN, at the end of the method, attempts to delete iteself (as it has gone out of scope)
		//4) Since the Button* pointer is still pointing to the same object, that Button* is deleted twice
		//resulting in a crash
		//As such, a clone is created with all the necessary information for the caller, minus the Button.
		ChatOption clone;
		clone.id = options[optionChosen].id;
		clone.text = options[optionChosen].text;
		caller->optionSelected(clone);
		//ui->removeWindow();
	}

	return true;
}