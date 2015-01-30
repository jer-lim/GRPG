#include "skill.h"

Skill::Skill(){}
Skill::Skill(std::string n, std::string d, int exp_g){
	name = n;
	description = d;
	exp_gain = exp_g;
}
Skill::~Skill()
{

}

const Skill* Skill::ATTACK = new Skill("Attack", "Influences hit chance", 10);
const Skill* Skill::STRENGTH = new Skill("Strength", "Influences Strength", 10);
const Skill* Skill::DEFENSE = new Skill("Defense", "Influences Defense", 10);
const Skill* Skill::TOUGHNESS = new Skill("Toughness", "Influences Toughness", 10);
const Skill* Skill::FISHING = new Skill("Fishing", "Influences Fishing", 10);
const Skill* Skill::COOKING = new Skill("Cooking", "Influences Cooking", 10);
const Skill* Skill::MINING = new Skill("Mining", "Influences Mining", 10);