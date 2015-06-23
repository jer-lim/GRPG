//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
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
Skill* Skill::THIEVING = nullptr;
Skill* Skill::RIFT_SEALING = nullptr;

void Skill::setupAllSkills(){
	ATTACK = new Skill("Accuracy", "Influences hit chance", 10);
	STRENGTH = new Skill("Power", "Influences Strength", 10);
	DEFENSE = new Skill("Evasion", "Influences Defense", 10);
	TOUGHNESS = new Skill("Toughness", "Influences Toughness", 10);
	FISHING = new Skill("Fishing", "Influences Fishing", 10);
	COOKING = new Skill("Cooking", "Influences Cooking", 30);
	MINING = new Skill("Mining", "Influences Mining", 10);
	THIEVING = new Skill("Thieving", "Be quick enough to pilfer small items from others, whether friendly or hostile, so long as they are not too alert.", 30);
	RIFT_SEALING = new Skill("Rift Sealing", "The ability to seal rifts", 1000);
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