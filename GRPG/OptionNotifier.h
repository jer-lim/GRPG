#ifndef _OPTIONNOTIFIER_H               // Prevent multiple definitions if this 
#define _OPTIONNOTIFIER_H               // file is included in more than one place

class ChatOption;

class OptionNotifier
{
public:
	virtual void optionSelected(ChatOption co) = 0;
};

#endif