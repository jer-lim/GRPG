#include "skill.h"

Skill::Skill(){}
Skill::Skill(std::string n, std::string d, int exp_g){
	name = n;
	description = d;
	exp_gain = exp_g;
}

const Skill Skill::ATTACK = Skill("Attack", "Influences hit chance", 10);
const Skill Skill::STRENGTH = Skill("Strength", "Influences Strength", 10);
const Skill Skill::DEFENSE = Skill("Defense", "Influences Defense", 10);
const Skill Skill::TOUGHNESS = Skill("Toughness", "Influences Toughness", 10);
const Skill Skill::FISHING = Skill("Fishing", "Influences Fishing", 10);
const Skill Skill::COOKING = Skill("Cooking", "Influences Cooking", 10);
const Skill Skill::MINING = Skill("Mining", "Influences Mining", 10);