//  Module:             Games Programming
//  Assignment2:        GRPG
//  Student Name:       Jeremy Choo
//  Student Number:     S10132161H
//  Student Name:       Matthew Lee
//  Student Number:     s10128677

#include "player.h"
#include "grpg.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    image.spriteData.width = playerNS::WIDTH;           // size of player
	image.spriteData.height = playerNS::HEIGHT;
	//image.spriteData.x = playerNS::X;                   // location on screen
	//image.spriteData.y = playerNS::Y;
	image.spriteData.rect.bottom = playerNS::HEIGHT;    // rectangle to select parts of an image
	image.spriteData.rect.right = playerNS::WIDTH;
    radius = playerNS::WIDTH/2.0;
    collisionType = entityNS::BOX;
	health = 10;
	regenerationDelay = playerNS::startingRegnerationTime;

	//skills
	skills[skillNS::ID_SKILL_ATTACK] = PlayerSkill(this, Skill::ATTACK);
	skills[skillNS::ID_SKILL_STRENGTH] = PlayerSkill(this, Skill::STRENGTH);
	skills[skillNS::ID_SKILL_DEFENSE] = PlayerSkill(this, Skill::DEFENSE);
	skills[skillNS::ID_SKILL_TOUGHNESS] = PlayerSkill(this, Skill::TOUGHNESS);
	skills[skillNS::ID_SKILL_FISHING] = PlayerSkill(this, Skill::FISHING);
	skills[skillNS::ID_SKILL_COOKING] = PlayerSkill(this, Skill::COOKING);
	skills[skillNS::ID_SKILL_MINING] = PlayerSkill(this, Skill::MINING);
	
	//Start off toughness at a good level
	skills[skillNS::ID_SKILL_TOUGHNESS].gainXP(Skill::calculateXPRequired(11), true);

	nearStove = false;
}
//=============================================================================
// sayMessage
// Causes the message to appear right above the player, using the specified font
//=============================================================================
void Player::sayMessage(std::string message, TextDX* font)
{
	textMessage = message;
	fontToUse = font;
	timeLeft = playerNS::textTimeDisplay;
	// Calculate the text side
	RECT* textRect = new RECT();
	textRect->left = 0;
	textRect->top = 0;
	//Note: DT_CALCRECT only sets the rectangle size but does not end up actually drawing the text
	font->print(textMessage, *textRect, DT_CALCRECT);
	textSize.x = textRect->right;
	textSize.y = textRect->bottom;
	delete textRect;
	//https://msdn.microsoft.com/en-us/library/windows/desktop/dd162498%28v=vs.85%29.aspx
}

//=============================================================================
// Initialize the Player.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr)
{
	game = (Grpg*) gamePtr;
	tm = new TextureManager();
	mm = new TextureManager();
	if (!tm->initialize(gamePtr->getGraphics(), FISHING_IMAGE))
		throw new GameError(gameErrorNS::FATAL_ERROR, "Error initalizing Fishing texture");

	if (!fishingImage.initialize(gamePtr->getGraphics(), 0, 0, 1, tm))
		throw new GameError(gameErrorNS::FATAL_ERROR, "Error initalizing fishing image");

	if (!mm->initialize(gamePtr->getGraphics(), MINING_IMAGE))
		throw new GameError(gameErrorNS::FATAL_ERROR, "Error initalizing Mining texture");

	if (!miningImage.initialize(gamePtr->getGraphics(), 0, 0, 1, mm))
		throw new GameError(gameErrorNS::FATAL_ERROR, "Error initalizing mining image");
	
    return(Entity::initialize(gamePtr, Person::thePlayer));
}

//=============================================================================
// draw the player
//=============================================================================
void Player::draw(Viewport* viewport)
{
	Entity::draw(viewport);

	//Draw the text right above it
	if (timeLeft > 0)
	{
		VECTOR2 vpCoords = viewport->translate(getX(), getY());

		//Save the old font colour, and print in black
		DWORD oldColor = fontToUse->getFontColor();
		fontToUse->setFontColor(graphicsNS::BLACK);

		fontToUse->print(textMessage, 
			vpCoords.x - textSize.x/2,		//Make text center on top of player
			vpCoords.y - playerNS::HEIGHT / 2);

		fontToUse->setFontColor(oldColor);
	}

	//Show that the player is fishing 
	if (actionDelay > 0)
	{
		//Draw the relevant image above the player
		if (currentAction == resourceNS::FISHING)
		{
			fishingImage.setX(getX());
			fishingImage.setY(getY() - edge.top / 2 - fishingImage.getHeight() / 2);
			fishingImage.draw(viewport);
		}
		else if (currentAction == resourceNS::MINING)
		{
			miningImage.setX(getX());
			miningImage.setY(getY() - edge.top / 2 - miningImage.getHeight() / 2);
			miningImage.draw(viewport);
		}
	}
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime, Game* gamePtr)
{
	regenerationDelay -= frameTime;
	if (regenerationDelay <= 0)
	{
		if (health < skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel())
			health++;
		regenerationDelay += playerNS::startingRegnerationTime;
	}

	Entity::update(frameTime, gamePtr);
	//Stop fishing, mining and cooking if you're doing something else
	if (destination != nullptr || victim != nullptr)
	{
		actionDelay = 0;
		nearStove = false;
		//NO shopping
		game->getUI()->removeWindow();
	}

	timeLeft -= frameTime;
	if (actionDelay > 0)
	{
		actionDelay -= frameTime;
		if (actionDelay <= 0)
		{
			if (inventory->hasSpaceInInventory())
			{
				if (currentAction == resourceNS::FISHING)
				{
					InventoryItem* fish = new InventoryFood(game->getItemLoader()->getItem(5), 1,RAW);
					Entity* e = new Entity();
					e->initialize(game, fish, false);//anchored if its in an inventory (like it is now)
					//inventory->addEntityInventoryItem(e);
					e->setX(x);
					e->setY(y);
					theGame->getDrawManager()->addObject(e, 2);
					e->getTopMostBehavior()->action();
					//game->getDrawManager()->addObject(e);
					//ITEM_ADD result = inventory->addEntityInventoryItem(e);
					/*
					if (result != ITEM_ADD::ADDED && result != ITEM_ADD::MERGED)
					{
						//halp what should I do here
						delete e;
					}
					else
					{*/
					skills[skillNS::ID_SKILL_FISHING].gainXP(30);

					//Check if activity can continue
					if (inventory->hasSpaceInInventory())
					{
						restartCounter(playerNS::fishingWaitTime, skills[skillNS::ID_SKILL_FISHING].getSkillLevel());
					}
				}
				else if (currentAction == resourceNS::MINING)
				{
					//Decide which ore you get based on your mining level
					int currentMiningLevel = skills[skillNS::ID_SKILL_MINING].getSkillLevel();
					int itemToGet = 7; // 7 = copper, 8 = iron, 10 = silver, 11 = mystic stone, 12 = mithril, 13 = Diamond
					//Iron check
					if (currentMiningLevel > 2 && getRandomNumber() < 0.6 + (0.2*(currentMiningLevel - 2)))
					{
						itemToGet = 8;
					}
					//Silver check
					if (currentMiningLevel > 6 && getRandomNumber() < 0.6 + (0.2*(currentMiningLevel - 6)))
					{
						itemToGet = 10;
					}
					//Mystic stone
					if (currentMiningLevel > 10 && getRandomNumber() < 0.6 + (0.2*(currentMiningLevel - 10)))
					{
						itemToGet = 11;
					}
					//Mithril
					if (currentMiningLevel > 14 && getRandomNumber() < 0.6 + (0.2*(currentMiningLevel - 14)))
					{
						itemToGet = 12;
					}
					//Diamond check
					if (currentMiningLevel > 18 && getRandomNumber() < 0.6 + (0.2*(currentMiningLevel-18)))
					{
						itemToGet = 13;
					}
					InventoryItem* ore = new InventoryItem(game->getItemLoader()->getItem(itemToGet), 1);
					Entity* e = new Entity();
					e->initialize(game, ore, false);//anchored if its in an inventory (like it is now)
					//inventory->addEntityInventoryItem(e);
					e->setX(x);
					e->setY(y);
					theGame->getDrawManager()->addObject(e, 2);
					e->getTopMostBehavior()->action();

					skills[skillNS::ID_SKILL_MINING].gainXP((itemToGet -  7) * 30);

					//Check if activity can continue
					if (inventory->hasSpaceInInventory())
					{
						restartCounter(playerNS::miningWaitTime, skills[skillNS::ID_SKILL_MINING].getSkillLevel());
					}
				}
			}
		}
	}
}
//=============================================================================
// damage
// This entity has been damaged by another entity
// Pass in the other entity's attack and strength.
// Returns the amount of damage dealt.
//=============================================================================
int Player::damage(int atk, int str)
{
	str *= getDamageReduction();
	takeDamage(atk, str, skills[skillNS::ID_SKILL_DEFENSE].getSkillLevel());
	if (health <= 0)
	{
		game->getUI()->addChatText("You died!");
		setX(game->getStartLocation().x);
		setY(game->getStartLocation().y);
		health = skills[skillNS::ID_SKILL_TOUGHNESS].getSkillLevel();
		//TODO: Strip all items from player
	}
	return damageTaken;
}

//=============================================================================
//Starts the player fishing
//flip defines if the player's avatar should be flipped horizontally
//If true, player is facing left, otherwise facing right
//=============================================================================
void Player::startFishing(bool flip)
{
	image.flipHorizontal(flip);

	//Check if activity can continue
	if (inventory->hasSpaceInInventory())
	{
		currentAction = resourceNS::FISHING;
		restartCounter(playerNS::fishingWaitTime, skills[skillNS::ID_SKILL_FISHING].getSkillLevel());
	}
	else
	{
		game->getUI()->addChatText("Your inventory is full!");
	}
}

//=============================================================================
//Starts the player mining
//flip defines if the player's avatar should be flipped horizontally
//If true, player is facing left, otherwise facing right
//=============================================================================
void Player::startMining(bool flip)
{
	image.flipHorizontal(flip);

	//Check if activity can continue
	if (inventory->hasSpaceInInventory())
	{
		currentAction = resourceNS::MINING;
		restartCounter(playerNS::miningWaitTime, skills[skillNS::ID_SKILL_MINING].getSkillLevel());
	}
	else
	{
		game->getUI()->addChatText("Your inventory is full!");
	}
}

//=============================================================================
//Restarts the player internals's current item
//The player starts gaining a resource once the counter reaches 0
//Takes in the starting time it is originally and the skill level used for that resource
//=============================================================================
void Player::restartCounter(int startingTime, int skilLevel)
{
	actionDelay += startingTime - (skilLevel * 0.1);
}

UI* Player::getUI()
{
	return game->getUI();
}