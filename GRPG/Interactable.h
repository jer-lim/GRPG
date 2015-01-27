#ifndef _INTERACTBLE_H
#define _INTERACTBLE_H

class Interactable
{
private:
	DWORD player_actions = 0x0000;
	/*
		000000
		ABCDEF, where:
		A: Can (Talk with interactable)
		B: Can Mine (Can mine from interactable)
		C: Can Fish (Can fish from Interactable)
		D: Can Attack (Can attack interactable)
		E: Can View (Can see description)
		F: Can See Name (Shows name on mouseover) (1)
	*/
public:
	
};

#endif