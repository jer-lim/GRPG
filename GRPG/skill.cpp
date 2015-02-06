//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     s10132161
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "skill.h"

Skill* Skill::ATTACK = nullptr;
Skill* Skill::STRENGTH = nullptr;
Skill* Skill::DEFENSE = nullptr;
Skill* Skill::TOUGHNESS = nullptr;
Skill* Skill::FISHING = nullptr;
Skill* Skill::COOKING = nullptr;
Skill* Skill::MINING = nullptr;


void Skill::setupAllSkills(){
	ATTACK = new Skill("Attack", "Influences hit chance", 10);
	STRENGTH = new Skill("Strength", "Influences Strength", 10);
	DEFENSE = new Skill("Defense", "Influences Defense", 10);
	TOUGHNESS = new Skill("Toughness", "Influences Toughness", 10);
	FISHING = new Skill("Fishing", "Influences Fishing", 10);
	COOKING = new Skill("Cooking", "Influences Cooking", 30);
	MINING = new Skill("Mining", "Influences Mining", 10);
}

Skill::Skill(){}
Skill::Skill(std::string n, std::string d, int exp_g){
	name = n;
	description = d;
	exp_gain = exp_g;
}
Skill::~Skill()
{

}