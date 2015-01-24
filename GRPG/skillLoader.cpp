#include "skillLoader.h"


void SkillLoader::loadAllSkills()
{
	ifstream skillstream;
	skillstream.open(skillDataLocation);
	if (skillstream.is_open()){
		string name, descript;
		int id, exp_gain;
		while (!skillstream.eof()){

			skillstream >> id >> name >> descript >> exp_gain;
			Skill skill = Skill(name, descript, exp_gain);
			//Insert into the map
			map_skills[id] = skill;
		}
		skillstream.close();
	}
}