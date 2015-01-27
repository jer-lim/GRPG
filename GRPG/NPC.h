#ifndef _NPC_H                 // Prevent multiple definitions if this 
#define _NPC_H				// file is included in more than one place

#include "constants.h"
#include "Person.h"
#include "Interactable.h"
#include <string>

class NPC : public Person, Interactable
{
private:
	string name;
	string description;
	int maxHealth;

public:

	NPC() : Person() {}
	NPC(string i, float mov, float atk,string nama,string desc,int maxhp)
		: Person(i, mov, atk)
	{
		name = nama;
		description = desc;
		maxHealth = maxhp;
	}

	string getname() { return name; }
	string getdescription() { return description; }
	int getmaxhealth() { return maxHealth; }
};

#endif