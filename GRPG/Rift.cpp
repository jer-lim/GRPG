#include "Rift.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "RiftEnterBehavior.h"
#include "AnimatableTile.h"
#include "RiftExitBehavior.h"
#include "NPC.h"
#include "player.h"
#include "GameEventManager.h"
#include "NoiseManager.h"

Rift::Rift() : Entity()
{
	riftTexture = new TextureManager();
	currentWave = 0;
	totalWaves = 2 + rand() % 4; //2-5 (2+(0to3))
}

Rift::~Rift()
{
	//Rifts that are outright deleted without calling close is likely due to the game being closed;
	//which means that any entity created will be killed in drawmanager already.
	//close();
	SAFE_DELETE(riftTexture);
	SAFE_DELETE(enterBehavior);
	SAFE_DELETE(exitBehavior);
	SAFE_DELETE(viewBehavior);
}

bool Rift::initialize(Game* gamePtr, Player* p, NPC* character)
{
	graphics = gamePtr->getGraphics();
	ui = ((Grpg*)gamePtr)->getUI();
	thePlayer = p;
	person = character;
	riftData = ((Grpg*)gamePtr)->getRiftData();

	if (!riftTexture->initialize(graphics, riftNS::location))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize rift texture.");
	}
	bool result = Entity::initialize(gamePtr, riftNS::imageWidth, riftNS::imageHeight, 0, riftTexture);
	setupBehaviors();

	((Grpg*)gamePtr)->getGameEventManager()->addListener(this);
	playerInRift = false;

	return result;
}

void Rift::draw(Viewport* viewport)
{
	//The rift must always be visible for the player to change between realms,
	//so it must always be drawn and thus be in the same realm as the player
	setIsInDarkRealm(thePlayer->inDarkRealm());
	Entity::draw(viewport);
}

void Rift::update(float frameTime, Game* gamePtr)
{
	//Rift never moves, attacks ... or get attacked, so no use for entity update
	//Entity::update(frameTime, gamePtr);

	//For talking text
	timeLeft -= frameTime;

	//Rotate accordingly
	image.setDegrees(image.getDegrees() + (frameTime * 360) / riftNS::timeForFullRotation);

	//Prevent players from walking too far away from the rift
	if (thePlayer->getRiftPortal() == this)
	{
		VECTOR2 direction = thePlayer->getVector() - getVector();
		float distanceToPlayer = D3DXVec2Length(&direction);
		if (distanceToPlayer >= riftNS::maximumDistanceFromRift)
		{
			//Too far
			//Find angle player is from rift
			VECTOR2 *normalizedDirection = &VECTOR2();
			D3DXVec2Normalize(normalizedDirection, &direction);
			/*
			Dot Product of 2 unit vectors gives the cosine between the vectors.
			This can be used to determine angles for trajectory and light reflection.
			*/
			float anglePlayerFromRift = acos(normalizedDirection->x / D3DXVec2Length(normalizedDirection));

			float fullCircle = 2 * PI;
			if (thePlayer->getY() > getY())
			{
				anglePlayerFromRift = fullCircle - anglePlayerFromRift;
			}
			anglePlayerFromRift += PI / 2;
			//Move player to as far away as possible from rift
			VECTOR2 newLocation = getFinalLocation(getX(), getY(), anglePlayerFromRift, riftNS::maximumDistanceFromRift);
			thePlayer->setX(newLocation.x);
			thePlayer->setY(newLocation.y);
			ui->addChatText("Vangel's words echo in your head...");
			ui->addChatText("Don't stray too far from the Rift in case it closes!");
			thePlayer->releaseDestination();
			thePlayer->setVictim(nullptr);
		}
	}

	//Track the monsters spawned so that you know when they are all killed
	if (enemiesSpawned.size() > 0)
	{
		stringstream ss;
		int enemiesFound = 0;
		for (int i = 0; i < enemiesSpawned.size(); i++)
		{
			if (enemiesSpawned[i] != nullptr)
			{
				ss << riftNS::spawnLinkPhrase << i;
				if (theGame->getSpawnLink(ss.str()) == NULL)
				{
					enemiesSpawned[i] = nullptr;
				}
				else
				{
					enemiesFound++;
				}
				ss.str("");
			}
		}
		if (enemiesFound == 0)
		{
			currentWave++;
			if (currentWave <= totalWaves)
			{
				waveStatus = riftNS::STARTING;
				begin(true);
			}
			else
			{
				thePlayer->sayMessage("That seems to be all of them.");
				waveStatus = riftNS::COMPLETED;
				enemiesSpawned.clear();
				SoundManager::playMusic(soundManagerNS::generalMusicID);
			}
		}
		//We also better make sure the monsters don't walk outside the zone the player can go
		for (int i = 0; i < enemiesSpawned.size(); i++)
		{
			if (enemiesSpawned[i] != nullptr)
			{
				if (enemiesSpawned[i]->getDestination() != nullptr)
				{
					VECTOR2 direction2 = enemiesSpawned[i]->getDestination()->getVector() - getVector();
					float distanceToDestFromRift = D3DXVec2Length(&direction2);
					if (distanceToDestFromRift > riftNS::maximumDistanceFromRift)
					{
						//Stop going there and find a new place!
						enemiesSpawned[i]->releaseDestination();
					}
				}
			}
		}
	}

	//If the player died while in this rift
	//Close it, award no xp
	if (playerInRift && playerLastTotalDeaths < thePlayer->getTotalDeaths())
	{
		close();
	}

	if (allocatedForDeletion)
	{
		((Grpg*)theGame)->deleteEntity(this);
	}
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void Rift::onLostDevice()
{
	riftTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Rift::onResetDevice()
{
	riftTexture->onResetDevice();
}

void Rift::setupBehaviors()
{
	SAFE_DELETE(enterBehavior);
	SAFE_DELETE(exitBehavior);
	SAFE_DELETE(viewBehavior);
	if (thePlayer->inDarkRealm())
	{
		exitBehavior = new RiftExitBehavior(thePlayer, this, ui, (Grpg*)theGame);
	}
	else
	{
		enterBehavior = new RiftEnterBehavior(thePlayer, this, ui, (Grpg*)theGame);
	}
	viewBehavior = new ViewBehavior("Rift", ((NPC*)person)->getdescription(), ui);
	setupVectorActiveBehaviors();
}

void Rift::begin(bool requireWalking)
{
	//Start tracking deaths, so that we know if the player dies
	playerLastTotalDeaths = thePlayer->getTotalDeaths();
	playerInRift = true;

	//Mob spawning time!
	if (requireWalking)
	{
		waveStatus = riftNS::STARTING;
	}
	else
	{
		waveStatus = riftNS::STARTED;
	}
	remainingDifficulty = riftData->getRandomDifficulty(currentWave);
	stringstream ss;
	while (remainingDifficulty >= 2)
	{
		//Find an appropriate location for the enemy to spawn
		//Pick a random angle
		float randAngle = (rand() * 1.0 / RAND_MAX) * 2 * PI;
		float randDistance = (rand() * 1.0 / RAND_MAX) * (riftNS::maximumDistanceFromRift - riftNS::minimumSpawnFromRift) + riftNS::minimumSpawnFromRift;
		VECTOR2 location = getFinalLocation(getX(), getY(), randAngle, randDistance);

		Entity* newEnemy = (NPC::spawn(theGame, getNewNPC(), location, thePlayer)); //Alternatively, don't pass in thePlayer for no automatic aggro.
		newEnemy->setIsInDarkRealm(true);
		newEnemy->setSpawnPoint(location);
		if (requireWalking)
		{
			//Spawn the guy a bit further away, have him walk to the planned location instead.
			Point* p = new Point(location.x, location.y);
			location = getFinalLocation(getX(), getY(), randAngle, riftNS::enemyEntryDistance);
			newEnemy->setX(location.x);
			newEnemy->setY(location.y);
			newEnemy->setDestination(p);
		}
		//We need to track the enemies and find out if they are killed or not,
		//without actually accessing the memory (cause if we do and it's been killed & deleted we'll crash the game)
		//The easiest way to do so is to use spawnlinks.
		ss << riftNS::spawnLinkPhrase << enemiesSpawned.size();
		theGame->addSpawnLink(ss.str(), newEnemy);
		ss.str("");
		enemiesSpawned.push_back(newEnemy);
	}
	remainingDifficulty = 0;

	//Start music
	SoundManager::playMusic(riftData->getMusicForWave(currentWave));
}

int Rift::getNewNPC()
{
	vector<RiftNPC> allNPCs = riftData->getRiftNPC();
	vector<RiftNPC> selectedNPCs;
	int totalProbability = 0;
	for (vector<RiftNPC>::iterator i = allNPCs.begin(); i != allNPCs.end(); i++)
	{
		if (i->difficulty <= remainingDifficulty)
		{
			selectedNPCs.push_back(*i);
			totalProbability += i->probability;
		}
	}
	//Roll a random number between 0 and probability.
	float random = rand() * 1.0 / RAND_MAX;
	random = random * totalProbability;
	//Pick the NPC that corresponds
	for (vector<RiftNPC>::iterator i = selectedNPCs.begin(); i != selectedNPCs.end(); i++)
	{
		random -= i->probability;
		if (random <= 0)
		{
			remainingDifficulty -= i->difficulty;
			return i->NPCId;
		}
	}
}

VECTOR2 Rift::getFinalLocation(float startX, float startY, float angle, float distance)
{
	VECTOR2 result;
	result.x = startX + sin(angle)*distance;
	result.y = startY + cos(angle)*distance;
	return result;
}

void Rift::eventOccured(GameEvent* e, UI* ui)
{
	if (e->getType() == "GAMEEVENT_DAMAGE")
	{
		waveStatus = riftNS::PROGRESSING;
	}
}

void Rift::close()
{
	for (int i = 0; i < enemiesSpawned.size(); i++)
	{
		if (enemiesSpawned[i] != nullptr)
		{
			((Grpg*)theGame)->deleteEntity(enemiesSpawned[i]);
		}
	}
	enemiesSpawned.clear();
	allocatedForDeletion = true;
	image.setVisible(false);
}

//Awards xp based on how many waves are cleared
void Rift::awardXP()
{
	int totalXP = 0;
	bool waveCleared = currentWave > totalWaves;
	for (int i = 0; i < currentWave; i++)
	{
		totalXP += riftData->getXPRewardForWave(i);
	}
	if (waveCleared)
	{
		thePlayer->getSkills()->at(skillNS::ID_SKILL_RIFTSEALING).gainXP(totalXP + riftNS::bonusXpForFullComp);
	}
	else
	{
		thePlayer->getSkills()->at(skillNS::ID_SKILL_RIFTSEALING).gainXP(totalXP / 2);
	}
}

//Deals damage to the player based on how many enemies there are left
void Rift::preMatureExit()
{
	if (waveStatus != riftNS::PROGRESSING)
	{
		return;
	}
	else
	{
		int totalDamage = 0;
		for (int i = 0; i < enemiesSpawned.size(); i++)
		{
			if (enemiesSpawned[i] != nullptr)
			{
				totalDamage += rand() % 10 + 1;
			}
		}
		thePlayer->damage(totalDamage);
		ui->addChatText("The enemies attack you as you attempt to leave early.");
		thePlayer->sayRandomHurtPhrase();
	}
}