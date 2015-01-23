#ifndef _SKILLLOADER_H                 // Prevent multiple definitions if this 
#define _SKILLLOADER_H                 // file is included in more than one place

#include "constants.h"
#include "skill.h"
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

class SkillLoader
{
private:
	unordered_map<int, Skill> map_skills;
public:
	void loadAllSkills();
};
#endif