#include "skill.h"

Skill::Skill(){}
Skill::Skill(std::string n, std::string d, int exp_g){
	name = n;
	description = d;
	exp_gain = exp_g;
}