#ifndef _NPC_H                 // Prevent multiple definitions if this 
#define _NPC_H				// file is included in more than one place

#include "constants.h"
#include "Person.h"
#include "Interactable.h"
#include "graphics.h"
#include "entity.h"
#include <string>

class PersonLoader;
class DrawManager;

class NPC : public Person, Interactable
{
private:
	string name;
	string description;
	int maxHealth;

public:

	NPC() : Person() {}
	NPC(string i, float mov, float atk, float h, float w, int cols, int colHeight, int colWidth, string nama,string desc,int maxhp)
		: Person(i, mov, atk, h, w, cols, colHeight, colWidth)
	{
		name = nama;
		description = desc;
		maxHealth = maxhp;
	}

	static void spawn(Game* gamePtr, PersonLoader* pl, int npcId, VECTOR2 coords, Entity* victim = nullptr);

	string getname() { return name; }
	string getdescription() { return description; }
	int getmaxhealth() { return maxHealth; }
};

#endif