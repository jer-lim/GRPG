#ifndef _BOOST_H			         // Prevent multiple definitions if this 
#define _BOOST_H		             // file is included in more than one place

#include "item.h"
#include <stdlib.h>     /* srand, rand */

class Boost : public Item
{
private:
	int xpGain;

public:
	Boost() : Item() {}
	Boost(int id, string n, string d, string i, int m, int c, int xp)
		: Item(id, n, d, i, m, c, itemNS::spriteColForMisc)
	{
		xpGain = xp;
	}
	int getXPGain() { return xpGain; }

	virtual string getType(){ return "BOOST"; }
};
#endif