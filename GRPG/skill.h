#ifndef _SKILL_H                 // Prevent multiple definitions if this 
#define _SKILL_H                 // file is included in more than one place

#include "constants.h"

namespace skillNS
{
    const int strength = 1; // The strength skill, which modifies damage dealt
}

class Skill
{
private:
	long experience;
public:
    // constructor
    Skill();

	int getSkillLevel();

    // inherited member functions
    void gainXP(long xp);
};
#endif

