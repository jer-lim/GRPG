#ifndef _FOOD_H			         // Prevent multiple definitions if this 
#define _FOOD_H		             // file is included in more than one place

#include "item.h"

class Food : public Item
{
private:
	int health_gain;
	int levelReq_for_50;
	float level_deviation;
	/*
	ABC, where:
	A: weapon , 0 for does not equip, 1 for equip
	B: offhand weapon, 0 for does not equip, 1 for equip
	C: body, 0 for does not equip, 1 for equip
	*/
public:
	Food(){ Item(); }
	Food(string n, string d, string i, int m, int c,int h,int lr,float ld)
	{
		Item(n, d, i, m, c);
		health_gain = h;
		levelReq_for_50 = lr;
		level_deviation = ld;
	}
	int gethealth_gain() { return health_gain; }
	void sethealth_gain(int i){ health_gain = i; }
	int getlevelReq_for_50() { return levelReq_for_50; }
	void setlevelReq_for_50(int i){ levelReq_for_50 = i; }
	float getlevel_deviation() { return level_deviation; }
	void setlevel_deviation(float i){ level_deviation = i; }
};
#endif