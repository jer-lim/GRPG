#ifndef _INTERACTBLE_H
#define _INTERACTBLE_H

class Interactable
{
private:
	DWORD player_actions = 0x0000;
	/*
		000000000
		IHGFEDCBA, where:
		I: Can Smith (Blacksmith system)
		H: Can Trade (Trade with interactable - store system)
		G: Can Talk (Talk with interactable)
		F: Can Mine (Can mine from interactable)
		E: Can Fish (Can fish from Interactable)
		D: Can Attack (Can attack interactable)
		C: Can Cook (Can cook if nearby a fire or stove)
		B: Can View (Can see description)
		A: Can See Name (Shows name on mouseover, toggles mouseover) (1)
	*/
public:
	virtual void smith(){}
	virtual void trade(){}
	virtual void talk(){}
	virtual void mine(){}
	virtual void fish(){}
	virtual void attack(){}
	virtual void cook(){}
	virtual string view(){ return "Move Here"; }//returns a string tht is displayed when mouse overed
};

#endif