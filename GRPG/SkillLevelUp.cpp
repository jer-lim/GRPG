#include "SkillLevelUp.h"
#include "UI.h"

SkillLevelUp::SkillLevelUp(Image* upArrow, SkillLevelUp* previousLU, string sn, TextDX* theFont, UI* u)
{
	previousLevelUp = previousLU;
	skillName = sn;
	font = theFont;
	deathTimer = skillLevelUpNS::timeDisplay;
	image = *upArrow;
	ui = u;

	//Calculate correct locations
	RECT* textRect = new RECT();
	textRect->left = 0;
	textRect->right = 0;
	font->print(skillName, *textRect, DT_CALCRECT);
	int totalWidth = textRect->right + skillLevelUpNS::imageWidth;
	x = GAME_WIDTH / 2 - totalWidth / 2 + skillLevelUpNS::imageWidth / 2;
	y = GAME_HEIGHT / 2;
	fontHeightHalf = textRect->bottom / 2;
	delete textRect;

	performRepositioning();
}

void SkillLevelUp::draw(Viewport* viewport)
{
	//Always visible regardless of realm
	//setIsInDarkRealm(thePlayer->inDarkRealm());
	//thePlayer is null, will always draw anyway.

	Entity::draw(viewport);
	font->print(skillName, x + skillLevelUpNS::imageWidth / 2, y - fontHeightHalf);

	if (previousLevelUp != nullptr)
	{
		previousLevelUp->draw(viewport);
	}
}

void SkillLevelUp::update(float frameTime, Game* gamePtr)
{
	if (previousLevelUp != nullptr)
	{
		previousLevelUp->update(frameTime, gamePtr);
	}

	deathTimer -= frameTime;
	//Move up
	y -= skillLevelUpNS::upSpeed * frameTime;
	if (deathTimer <= 0)
	{
		if (nextLevelUp != nullptr)
		{
			nextLevelUp->setPreviousLevelUp(nullptr);
		}
		else
		{
			ui->removeLastSkillLevelUp();
		}
	}
}

void SkillLevelUp::performRepositioning()
{
	//See if the above one needs to be moved
	if (previousLevelUp != nullptr)
	{
		if (getY() - previousLevelUp->getY() < skillLevelUpNS::imageHeight + skillLevelUpNS::collideMargin)
		{
			previousLevelUp->setY(getY() - skillLevelUpNS::imageHeight - skillLevelUpNS::collideMargin);
			previousLevelUp->performRepositioning();
		}
	}
}