#include "Aidil.h"
#include "grpg.h"
#include "mapLoader.h"
#include "destination.h"
#include "AnimatableTile.h"

Aidil::Aidil()
{
	dragonfireTexture = new TextureManager();
	aidilFlyTexture = new TextureManager();
	dragonfireCooldownTimer = -1;
	currentDragonfireAngle = -1;
	currentPhase = 1;
	combatBegun = false;
}

Aidil::~Aidil()
{
	SAFE_DELETE(dragonfireTexture);
	SAFE_DELETE(dragonfireImage);
	SAFE_DELETE(aidilFlyingImage);
	SAFE_DELETE(aidilFlyTexture);
	SAFE_DELETE(oldAttackBehavior);
	SAFE_DELETE(oldExamineBehavior);
}

bool Aidil::initialize(Game* gamePtr, Player* p, NPC* aidilInfo)
{
	ui = ((Grpg*)gamePtr)->getUI();

	if (!Entity::initialize(gamePtr, aidilInfo, false))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize aidil basic entity");
	}
	thePlayer = p;
	if (!dragonfireTexture->initialize(graphics, aidilNS::dragonfireLocation))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize dragonfire texture.");
	}
	dragonfireImage = new Image();
	if (!dragonfireImage->initialize(graphics, aidilNS::dragonfireWidth, aidilNS::dragonfireHeight, 0, dragonfireTexture))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize dragonfire image.");
	}
	
	if (!aidilFlyTexture->initialize(graphics, aidilNS::aidilFlyLocation))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize aidil's flying texture.");
	}
	aidilFlyingImage = new Image();
	if (!aidilFlyingImage->initialize(graphics, aidilNS::aidilFlyWidth, aidilNS::aidilFlyHeight, aidilNS::aidilFlyCols, aidilFlyTexture))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Failed to initialize aidil's flying image.");
	}

	//Set essentials
	//Note that x and y will be changed later.
	//Set to 
	for (int i = 0; i < 3; i++)
	{
		vtx[i].x = 0;
		vtx[i].y = 0;
		vtx[i].z = 0.0f;
		vtx[i].rhw = 1.0f;
		vtx[i].color = aidilNS::DRAGONFIRE_WARN_COLOUR;
	}
	//Set for debugging purposes in case it ends up non-initalized or wahtever.
	vtx[1].x = x;
	vtx[2].y = y;

	//Will be deleted later on every draw step, created once at start for simplicity to not 
	//code an exception for the first time.
	graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
}

void Aidil::draw(Viewport* viewport)
{
	Entity::draw(viewport);
	if (dragonfireStatus == aidilNS::DRAGONFIRE_WARNING || dragonfireStatus == aidilNS::DRAGONFIRE_ACTIVE)
	{
		graphics->spriteEnd();
		graphics->spriteBegin();
		vertexBuffer->Release();
		//Generate new vertex buffer
		//don't bother optimizing to only draw when viewport changes, not needed - (c) Oon 2015
		for (int i = 0; i < 3; i++)
		{
			vtx[i].x = dragonfireLocations[i].x - viewport->getTopLeft().x;
			vtx[i].y = dragonfireLocations[i].y - viewport->getTopLeft().y;
		}
		graphics->createVertexBuffer(vtx, sizeof vtx, vertexBuffer);
		graphics->drawQuad(vertexBuffer, 1);
	}
}

void Aidil::update(float frameTime, Game* gamePtr)
{
	Entity::update(frameTime, gamePtr);
	
	//If combat has begun against the player
	if (!combatBegun && victim == thePlayer)
	{
		combatBegun = true;
		dragonfireCooldownTimer = aidilNS::dragonfireStartDelay;
		dragonfireStatus = aidilNS::DRAGONFIRE_COOLDOWN;
	}
	if (combatBegun)
	{
		dragonfireCooldownTimer -= frameTime;
		dragonfireWarningTimer -= frameTime;
		dragonfireActiveTimer -= frameTime;

		if (dragonfireStatus == aidilNS::DRAGONFIRE_COOLDOWN)
		{
			//Not while loop cause we don't want to overwhelm the player in case of a huge lag.
			if (dragonfireCooldownTimer < 0)
			{
				//Effectively a random number from 0 to 1.
				float random = (rand() % 1000) / 1000;
				float increase = aidilNS::dragonfireDelayDeviation * 2 * random - aidilNS::dragonfireDelayDeviation;
				dragonfireCooldownTimer += aidilNS::dragonfireNormalDelay + increase;

				dragonfireWarningTimer = aidilNS::dragonfireWarningTime;
				dragonfireStatus = aidilNS::DRAGONFIRE_WARNING;

				//calculate angle
				VECTOR2 direction = thePlayer->getVector() - getVector();
				VECTOR2 *normalizedDirection = &VECTOR2();
				D3DXVec2Normalize(normalizedDirection, &direction);
				/*
				Dot Product of 2 unit vectors gives the cosine between the vectors.
				This can be used to determine angles for trajectory and light reflection.
				*/
				currentDragonfireAngle = acos(normalizedDirection->x / D3DXVec2Length(normalizedDirection));

				float fullCircle = 2 * PI;
				if (thePlayer->getY() > getY())
				{
					currentDragonfireAngle = fullCircle - currentDragonfireAngle;
				}
				currentDragonfireAngle += PI / 2;
				stringstream ss;
				ss << "Current angle: " << currentDragonfireAngle;

				((Grpg*)theGame)->getUI()->addChatText(ss.str());
				//Generate vtx
				topDragonfireAngle = currentDragonfireAngle - aidilNS::dragonfireSpread / 2.0;
				if (topDragonfireAngle < 0)
				{
					topDragonfireAngle += fullCircle;
				}
				botDragonfireAngle = currentDragonfireAngle + aidilNS::dragonfireSpread / 2.0;
				botDragonfireAngle = botDragonfireAngle - fullCircle;

				//Set the actual parameters
				dragonfireLocations[0].x = x;
				dragonfireLocations[0].y = y;

				dragonfireLocations[1] = getFinalLocation(getX(), getY(), topDragonfireAngle, aidilNS::dragonfireDistance);
				dragonfireLocations[2] = getFinalLocation(getX(), getY(), botDragonfireAngle, aidilNS::dragonfireDistance);
			}
		}
		else if (dragonfireStatus == aidilNS::DRAGONFIRE_WARNING)
		{
			if (dragonfireWarningTimer <= 0)
			{
				dragonfireActiveTimer = aidilNS::dragonfireActiveTime;
				dragonfireStatus = aidilNS::DRAGONFIRE_ACTIVE;
				dragonfireCounter = 0;
			}
		}
		else if (dragonfireStatus == aidilNS::DRAGONFIRE_ACTIVE)
		{
			//Spawn dragonfire!
			dragonfireCounter -= frameTime;
			for (; dragonfireCounter < 0; dragonfireCounter += FRAME_RATE/aidilNS::dragonfireSpawnedPerSecond)
			{
				//Generate a random angle
				float r = (rand() / (float)RAND_MAX * aidilNS::dragonfireSpread) + topDragonfireAngle;
				if (r > 2 * PI)
				{
					r -= 2 * PI;
				}
				//Calculate the final location based off the new angle
				VECTOR2 finalLocation = getFinalLocation(dragonfireLocations[0].x, dragonfireLocations[0].y, r, aidilNS::dragonfireDistance);
				Point* d = new Point(finalLocation);

				//Summon dragonfire!
				Dragonfire* df = new Dragonfire();
				df->initialize(theGame, dragonfireTexture, d, r);
				df->setX(dragonfireLocations[0].x);
				df->setY(dragonfireLocations[0].y);
				theGame->getDrawManager()->addObject(df);
				if (dragonfireActiveTimer <= 0)
				{
					currentDragonfireAngle = -1;
					dragonfireStatus = aidilNS::DRAGONFIRE_COOLDOWN;
				}
				/*
				//Spawn a dragonfire at destination
				Dragonfire* df2 = new Dragonfire();
				df2->initialize(theGame, dragonfireTexture, d, r);
				df2->setX(d->getX());
				df2->setY(d->getY());
				theGame->getDrawManager()->addObject(df2);*/
			}
		}

		//Handle blocking of the wall of aidil's cave.
		if (currentPhase == 2 || currentPhase == 3)
		{
		}

		if (currentPhase == 1 && health < aidilNS::healthThresholdForPhase2)
		{
			currentPhase = 2;
			normalImage = &image;
			image = *aidilFlyingImage;
			dragonfireStatus = aidilNS::DRAGONFIRE_COOLDOWN;
			dragonfireCooldownTimer = aidilNS::phaseTwoTime + aidilNS::dragonfireStartDelay + aidilNS::flyAnimationTime * 2;
			image.setLoop(true);
			image.setFrames(0, aidilNS::aidilFlyCols - 1);
			releaseDestination();
			victim = nullptr;
			thePlayer->setVictim(nullptr);
			thePlayer->releaseDestination();
			ui->addChatText("Aidil does a quick swipe at you, stunning you!");
			thePlayer->failThieving();
			scaleChange = -1;
			ui->addChatText("Aidil takes to the skies!");
			oldAttackBehavior = attackBehavior;
			attackBehavior = nullptr;
			setupVectorActiveBehaviors();
			phase2Timer = aidilNS::phaseTwoTime + aidilNS::flyAnimationTime;
			//Have aidil fly up
			VECTOR2 flyLocation = ((Grpg*)theGame)->getMapLoader()->translateIdToCoords('/');
			destination = new Point(flyLocation);
			//Set speed relative to how long it will take
			VECTOR2 direction = destination->getVector() - getVector();
			float distance = D3DXVec2Length(&direction);
			oldMovementSpeed = person->getMovementSpeed();
			person->setMovementSpeed(distance / aidilNS::flyAnimationTime);
		}
		else if (currentPhase == 2)
		{
			if (scaleChange != 0)
			{
				float scaleToChangeThisFrame = ((100 / aidilNS::flyAnimationTime) * frameTime * scaleChange)/100;
				image.setScale(image.getScale() + scaleToChangeThisFrame);
				if (image.getScale() <= 0)
				{
					image.setVisible(false);
					image.setScale(0);
					//Aidil is now in the air!
					scaleChange = 0;
					//Remove examine behavior, otherwise aidil (that is still there) can be moused over and examined,
					//which will override the default "walk here" command.
					oldExamineBehavior = viewBehavior;
					viewBehavior = nullptr;
					setupVectorActiveBehaviors();
					//Also prevent aidil from moving and hitting the player
					person->setMovementSpeed(0);
					//Have aidil swipe the entrance, blocking off the one and only escape route.
					//Tile* t = ((Grpg*)theGame)->getMapLoader()->translateIdToCoords('/');
				}
				else if (image.getScale() >= 1)
				{
					scaleChange = 0;
					image.setScale(1);
					image = *normalImage;
					currentPhase = 3;
					attackBehavior = oldAttackBehavior;
					oldAttackBehavior = nullptr;
					setupVectorActiveBehaviors();
					person->setMovementSpeed(oldMovementSpeed);
				}
			}
			else
			{
				//Aidil rains fireballs down from above!
				phase2Timer -= frameTime;
				if (phase2Timer <= 0)
				{
					scaleChange = 1;
					image.setVisible(true);
					viewBehavior = oldExamineBehavior;
					setupVectorActiveBehaviors();
				}
			}
		}
	}
}

//=============================================================================
// called when graphics device is lost
//=============================================================================
void Aidil::onLostDevice()
{
	dragonfireTexture->onLostDevice();
}

//=============================================================================
// called when graphics device is reset
//=============================================================================
void Aidil::onResetDevice()
{
	dragonfireTexture->onResetDevice();
}


VECTOR2 Aidil::getFinalLocation(float startX, float startY, float angle, float distance)
{
	VECTOR2 result;
	result.x = startX + sin(angle)*distance;
	result.y = startY + cos(angle)*distance;
	return result;
}