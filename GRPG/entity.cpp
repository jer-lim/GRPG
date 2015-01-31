#include "entity.h"
#include "game.h"
#include "mapLoader.h"
#include <cmath>
#include <sstream>
#include "UI.h"
#include "Enemy.h"
#include "AttackBehavior.h"
#include "PickupBehavior.h"
#include "DropBehavior.h"

namespace entityNS
{
	Image miss = Image();
	Image hit = Image();
	TextDX splatText = TextDX();
}

//=============================================================================
// constructor
//=============================================================================
Entity::Entity()
{
    radius = 1.0;
    edge.left = -1;
    edge.top = -1;
    edge.right = 1;
    edge.bottom = 1;
    active = true;                  // the entity is active
    rotatedBoxReady = false;
    collisionType = entityNS::BOX;
    health = 100;
	image = Image();
	destination = 0;
	attackCooldown = 0;
	image.setFrameDelay(entityNS::animationWait);

	lastPathfindTime.QuadPart = 0;
	currentDestination = VECTOR2(-1, -1);

	person = nullptr;
	inventoryItem = nullptr;
	backHealth = nullptr;
	availableHealth = nullptr;

	oldViewport = VECTOR2(-1, -1);
	oldLocation = VECTOR2(-1, -1);
}

//=============================================================================
// Destructor
//=============================================================================
Entity::~Entity()
{
	//Destroy all behaviors
	SAFE_DELETE(viewBehavior);//View name -> display description
	SAFE_DELETE(blacksmithBehavior);//Blacksmith popup
	SAFE_DELETE(tradeBehavior);//store popup
	SAFE_DELETE(attackBehavior);//Attack name -> perform attack
	SAFE_DELETE(pickupBehavior);//Pickup name -> pickup obj
	SAFE_DELETE(dropBehavior);//Drop name -> drop obj
	SAFE_DELETE(cookBehavior);//Cook name -> cook obj if fire nearby
	vectorActiveBehaviors.clear();
	if (backHealth != nullptr)
	{
		backHealth->deleteVertexBuffer();
		backHealth = nullptr;
	}
	if (availableHealth != nullptr)
	{
		availableHealth->deleteVertexBuffer();
		availableHealth = nullptr;
	}
	if (person != nullptr)
	{
		SAFE_RELEASE(person);
	}
	if (inventoryItem != nullptr)
	{
		SAFE_DELETE(inventoryItem);
	}
}

//=============================================================================
// Initialize the Entity.
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//		whichTexture = the texture that this entity reads from
//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
// Post: returns true if successful, false if failed
//=============================================================================
/*
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols, const char* whichTexture, bool anc)
{
	anchored = anc;

	input = gamePtr->getInput();                // the input system
	graphics = gamePtr->getGraphics();

	textureM = new TextureManager();

	//init texture
	if (!textureM->initialize(graphics, whichTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing " + *whichTexture));

	return image.initialize(gamePtr->getGraphics(), width, height, ncols, textureM, anc);
}*/

//=============================================================================
// Initialize the Entity.
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//		whichCharacter = the character that this entity refers to
//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
// Post: returns true if successful, false if failed
//=============================================================================
bool Entity::initialize(Game *gamePtr, Person* whichCharacter, bool anc)
{
	//setup behaviors
	if (whichCharacter->getType() == "NPC" || whichCharacter->getType() == "ENEMY")
	{
		if (whichCharacter->getType() == "ENEMY")//Attack enemy
			attackBehavior = new AttackBehavior(this);
		viewBehavior = new ViewBehaviorNPC((NPC*)whichCharacter);
	}
	setupVectorActiveBehaviors();

	anchored = anc;

    input = gamePtr->getInput();                // the input system
	graphics = gamePtr->getGraphics();

	//textureM = new TextureManager();
	//textureM = whichCharacter->getTextureManager();
	textureM = whichCharacter->initializeTexture(gamePtr);
	person = whichCharacter;

	//Set the health if this is not the player
	if (whichCharacter != Person::thePlayer)
	{
		health = ((NPC*)whichCharacter)->getmaxhealth();
		//Health bars above the player
		backHealth = new Button();
		availableHealth = new Button();
		backHealth->setVisible(false);
		availableHealth->setVisible(false);
	}
	
	edge.top = whichCharacter->getColliHeight() / 2;
	edge.bottom = whichCharacter->getColliHeight() / 2;
	edge.left = whichCharacter->getColliWidth() / 2;
	edge.right = whichCharacter->getColliWidth() / 2;

	string path = PersonNS::spriteDirectory + person->getImgFileName();
	//init texture
	if (!textureM->initialize(graphics, path.c_str()))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initalizing " + *path.c_str()));

    return image.initialize(gamePtr->getGraphics(), whichCharacter->getWidth(), whichCharacter->getHeight(), whichCharacter->getNumOfCols(), textureM, anc);
}

//Initialize entity via inventoryitem
bool Entity::initialize(Game *gamePtr, InventoryItem* invItem, bool inInventory)
{
	//setup behaviors
	viewBehavior = new ViewBehaviorItem(invItem);
	if (inInventory)
		dropBehavior = new DropBehavior(this);
	else
		pickupBehavior = new PickupBehavior(this);
	setupVectorActiveBehaviors();

	anchored = inInventory;//anchored = true = inInventory

	input = gamePtr->getInput();                // the input system
	graphics = gamePtr->getGraphics();
	invItem->initialize(gamePtr);
	Item* item = invItem->getItem();
	textureM = item->getTextureManager();
	inventoryItem = invItem;

	return image.initialize(gamePtr->getGraphics(), item->getSpriteWidth(), item->getSpriteHeight(), item->getSpriteColumns(), item->getTextureManager(), anchored);
}


//=============================================================================
// Initialize entity using a pre-initialized TextureManager
// Pre: *gamePtr = pointer to Game object
//      width = width of Image in pixels  (0 = use full texture width)
//      height = height of Image in pixels (0 = use full texture height)
//      ncols = number of columns in texture (1 to n) (0 same as 1)
//		tm = the texture that has already been initalized (tm->initalize(...);)
//		anc = twhether this entity is anchored or not (Changes drawing x and y location based on viewport)
// Post: returns true if successful, false if failed
//=============================================================================
bool Entity::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager* tm, bool anc)
{
	anchored = anc;

	input = gamePtr->getInput();                // the input system
	graphics = gamePtr->getGraphics();
	textureM = tm;

	return image.initialize(gamePtr->getGraphics(), width, height, ncols, textureM, anc);
}

//=============================================================================
// activate the entity
//=============================================================================
void Entity::activate()
{
    active = true;
}

//=============================================================================
// Draws the entity onto the screen based on it's x and y position
//=============================================================================
void Entity::draw(Viewport* viewport)
{
	if (backHealth != nullptr)
	{
		//Perform a viewport check
		//Only reset graphics if need to draw the health bar
		if (availableHealth->getVisible())
		{
			//Check if changes are required
			if (viewport->getTopLeft() != oldViewport || getVector() != oldLocation)
			{
				//Calculate drawing requirements
				resetAvailableHealth(viewport->getTopLeft());
				resetHealth(viewport->getTopLeft());
				//delete oldViewport;
				//delete oldLocation;
				oldViewport = viewport->getTopLeft();
				oldLocation = getVector();
			}

			graphics->spriteEnd();
			graphics->spriteBegin();

			backHealth->draw();
			availableHealth->draw();
		}
	}

	image.setX(getX());
	image.setY(getY());
	if (anchored || viewport == nullptr) image.draw();
	else image.draw(viewport);

	//Damage splats
	if (splatTime > 0)
	{
		if (damageTaken == 0)
		{
			entityNS::miss.setX(getX());
			entityNS::miss.setY(getY());
			entityNS::miss.draw(viewport);
		}
		else
		{
			VECTOR2 vpCoord = viewport->getTopLeft();
			float newX = getX() - vpCoord.x;
			float newY = getY() - vpCoord.y;
			entityNS::hit.setX(newX);
			entityNS::hit.setY(newY);
			entityNS::hit.draw();

			stringstream ss;
			ss << damageTaken;

			// Calculate the text side
			RECT* textRect = new RECT();
			textRect->left = 0;
			textRect->top = 0;
			//Note: DT_CALCRECT only sets the rectangle size but does not end up actually drawing the text
			entityNS::splatText.print(ss.str(), *textRect, DT_CALCRECT);
			newX -= textRect->right/2;
			newY -= textRect->bottom/2;

			entityNS::splatText.print(ss.str(), newX, newY);
		}
	}
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Entity::update(float frameTime, Game* gamePtr)
{
	//temp fix
	if (person != nullptr)
	{
	VECTOR2 collisionVector;

	// Is there a victim? If so, set as destination
	if (victim != 0)
	{
		if (!this->collidesWith(*victim, collisionVector))
		{
			destination = victim;
		}
		else
		{
			destination = 0;
		}
	}

	//Don't change the animation or move if the entity's animation
	//is still at an attacking state
	if (attackCooldown > person->getAttackCooldown() - ((person->getNumOfCols() - 1) * entityNS::animationWait))
	{
		//Do nothing, plasyer is effectively "stunned" for this duration
	}
	else
	{
		if (destination != 0)
		{
			//Handle animation
			//If the animation was not already set to moving...
			if (!image.getLoop())
			{
				//Set it to moving
				image.setLoop(true);
				image.setFrames(0, 1);
			}

			VECTOR2 destinationVector = destination->getVector();
			VECTOR2 immediateVector = destinationVector;
			
			LARGE_INTEGER currentTime;
			LARGE_INTEGER timerFreq;
			QueryPerformanceCounter(&currentTime);
			QueryPerformanceFrequency(&timerFreq);

			// Find a path if destination is changed and time since last pathfinding is long enough
			bool requestPath = false;
			if (currentDestination == VECTOR2(-1, -1)){
				requestPath = true;
			}
			else if (gamePtr != nullptr && currentDestination != destinationVector && (currentTime.QuadPart - lastPathfindTime.QuadPart) / timerFreq.QuadPart > 0.5){
				requestPath = true;
			}

			// Uncomment this and comment the next block to not use pathfinding
			//path.push(immediateVector);

			if (requestPath){
				// Awesome pathfinding here
				if (gamePtr->getMapLoader()->canRequestPath()){
					path = gamePtr->getMapLoader()->path(getVector(), destinationVector);
					currentDestination = destinationVector;
					QueryPerformanceCounter(&lastPathfindTime);
				}
			}

			// While entity has a path to follow, follow path
			if (!path.empty()){
				immediateVector = path.front();
				if (immediateVector == getVector()){
					path.pop();
					if (!path.empty()){
						immediateVector = path.front();
						//runtimeLog << "Travelling to " << immediateVector.x << ", " << immediateVector.y << endl;
					}
				}
				
			}
			else{
				// Can't go there
				destination = 0;
				return;
			}
			
			float speed = person->getMovementSpeed();
			
			VECTOR2 direction = immediateVector - getVector();
			VECTOR2 *normalizedDirection = &VECTOR2();
			D3DXVec2Normalize(normalizedDirection, &direction);
			setX(getX() + normalizedDirection->x * speed * frameTime);
			setY(getY() + normalizedDirection->y * speed * frameTime);
			image.flipHorizontal(normalizedDirection->x < 0);

			//Is it close enough?
			float distanceToDest = D3DXVec2Length(&direction);
			if (distanceToDest < speed * frameTime)
			{
				setX(immediateVector.x);
				setY(immediateVector.y);
				// delete destination; // Sometimes a destination might be re-used or be an actual entity

				// If arrived at final destination
				if (destination->getVector() == getVector()){
					destination->release();
					destination = 0;
				}
			}
		}
		else
		{
			image.setFrames(1, 1);
			image.setCurrentFrame(1);
			image.setLoop(false);
		}
	}

	//Are we currently colliding with the entity? If so, attack!
	if (victim != 0)
	{
		//Can't attack yet, it's on cooldown!
		if (attackCooldown <= 0)
		{
			//We can attack!
			VECTOR2 collisionVector;
			if (this->collidesWith(*victim, collisionVector))
			{
				// Check if the entity needs to be flipped
				VECTOR2 destinationVector = victim->getVector();
				VECTOR2 direction = destinationVector - getVector();
				image.flipHorizontal(direction.x < 0);

				if (person != Person::thePlayer)
				{
					victim->damage(((Enemy*)person)->getattackLv(), ((Enemy*)person)->getstrengthLv());
				}
				else
				{
					map <int, PlayerSkill>* skills = ((Player*)this)->getSkills();
					int damageDealt = victim->damage(skills->at(skillNS::ID_SKILL_ATTACK).getSkillLevel(), skills->at(skillNS::ID_SKILL_STRENGTH).getSkillLevel());
					//We're obviously not going to implement combat styles so I'll just pump everything.
					skills->at(skillNS::ID_SKILL_ATTACK).gainXP(damageDealt * 4);
					skills->at(skillNS::ID_SKILL_DEFENSE).gainXP(damageDealt * 4);
					skills->at(skillNS::ID_SKILL_STRENGTH).gainXP(damageDealt * 4);
					skills->at(skillNS::ID_SKILL_TOUGHNESS).gainXP(damageDealt * 4);
				}
				attackCooldown = person->getAttackCooldown();
				image.setFrames(1, person->getNumOfCols()-1);
				image.setLoop(false);
			}
		}
	}

	// Reduce attackCooldown if required
	if (attackCooldown > 0)
	{
		attackCooldown -= frameTime;
	}

    image.update(frameTime);
    rotatedBoxReady = false;    // for rotatedBox collision detection
	}

	//health bar display
	if (displayTime > 0)
	{
		displayTime -= frameTime;
		if (displayTime <= 0)
		{
			availableHealth->setVisible(false);
			backHealth->setVisible(false);
		}
	}

	if (splatTime > 0)
	{
		splatTime -= frameTime;
	}
}

//=============================================================================
// ai (artificial intelligence)
// typically called once per frame
// performs ai calculations, ent is passed for interaction
//=============================================================================
void Entity::ai(float frameTime, Entity &ent)
{}

//=============================================================================
// Perform collision detection between this entity and the other Entity.
// Each entity must use a single collision type. Complex shapes that require
// multiple collision types may be done by treating each part as a separate
// entity.
// Typically called once per frame.
// The collision types: CIRCLE, BOX, or ROTATED_BOX.
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collidesWith(Entity &ent, VECTOR2 &collisionVector)
{ 
    // if either entity is not active then no collision may occcur
    if (!active || !ent.getActive())    
        return false;

    // If both entities are CIRCLE collision
    if (collisionType == entityNS::CIRCLE && ent.getCollisionType() == entityNS::CIRCLE)
        return collideCircle(ent, collisionVector);
    // If both entities are BOX collision
    if (collisionType == entityNS::BOX && ent.getCollisionType() == entityNS::BOX)
        return collideBox(ent, collisionVector);
    // All other combinations use separating axis test
    // If neither entity uses CIRCLE collision
    if (collisionType != entityNS::CIRCLE && ent.getCollisionType() != entityNS::CIRCLE)
        return collideRotatedBox(ent, collisionVector);
    else    // one of the entities is a circle
        if (collisionType == entityNS::CIRCLE)  // if this entity uses CIRCLE collision
        {
            // Check for collision from other box with our circle
            bool collide = ent.collideRotatedBoxCircle(*this, collisionVector); 
            // Put the collision vector in the proper direction
            collisionVector *= -1;              // reverse collision vector
            return collide;
        }
        else    // the other entity uses CIRCLE collision
            return collideRotatedBoxCircle(ent, collisionVector);
    return false;
}

//=============================================================================
// Circular collision detection method
// Called by collision(), default collision detection method
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideCircle(Entity &ent, VECTOR2 &collisionVector)
{
    // difference between centers
    distSquared = *getCenter() - *ent.getCenter();
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // Calculate the sum of the radii (adjusted for scale)
	sumRadiiSquared = (radius*image.getScale()) + (ent.radius*ent.image.getScale());
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // if entities are colliding
    if(distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;   // not colliding
}

//=============================================================================
// Axis aligned bounding box collision detection method
// Called by collision()
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideBox(Entity &ent, VECTOR2 &collisionVector)
{
    // if either entity is not active then no collision may occcur
    if (!active || !ent.getActive())
        return false;

    // Check for collision using Axis Aligned Bounding Box.
	if ((getX() + edge.right*image.getScale() >= ent.getX() - ent.getEdge().left*ent.image.getScale()) &&
		(getX() - edge.left*image.getScale() <= ent.getX() + ent.getEdge().right*ent.image.getScale()) &&
		(getY() + edge.bottom*image.getScale() >= ent.getY() - ent.getEdge().top*ent.image.getScale()) &&
		(getY() - edge.top*image.getScale() <= ent.getY() + ent.getEdge().bottom*ent.image.getScale()))
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// Rotated Box collision detection method
// Called by collision()
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
// Uses Separating Axis Test to detect collision. 
// The separating axis test:
//   Two boxes are not colliding if their projections onto a line do not overlap.
//=============================================================================
bool Entity::collideRotatedBox(Entity &ent, VECTOR2 &collisionVector)
{
    computeRotatedBox();                    // prepare rotated box
    ent.computeRotatedBox();                // prepare rotated box
    if (projectionsOverlap(ent) && ent.projectionsOverlap(*this))
    {
        // set collision vector
        collisionVector = *ent.getCenter() - *getCenter();
        return true;
    }
    return false;
}

//=============================================================================
// Projects other box onto this edge01 and edge03.
// Called by collideRotatedBox()
// Post: returns true if projections overlap, false otherwise
//=============================================================================
bool Entity::projectionsOverlap(Entity &ent)
{
    float projection, min01, max01, min03, max03;

    // project other box onto edge01
	projection = graphics->Vector2Dot(&edge01, ent.getCorner(0)); // project corner 0
    min01 = projection;
    max01 = projection;
    // for each remaining corner
    for(int c=1; c<4; c++)
    {
        // project corner onto edge01
        projection = graphics->Vector2Dot(&edge01, ent.getCorner(c));
        if (projection < min01)
            min01 = projection;
        else if (projection > max01)
            max01 = projection;
    }
    if (min01 > edge01Max || max01 < edge01Min) // if projections do not overlap
        return false;                       // no collision is possible

    // project other box onto edge03
    projection = graphics->Vector2Dot(&edge03, ent.getCorner(0)); // project corner 0
    min03 = projection;
    max03 = projection;
    // for each remaining corner
    for(int c=1; c<4; c++)
    {
        // project corner onto edge03
        projection = graphics->Vector2Dot(&edge03, ent.getCorner(c));
        if (projection < min03)
            min03 = projection;
        else if (projection > max03)
            max03 = projection;
    }
    if (min03 > edge03Max || max03 < edge03Min) // if projections do not overlap
        return false;                       // no collision is possible

    return true;                            // projections overlap
}

//=============================================================================
// Rotated Box and Circle collision detection method
// Called by collision()
// Uses separating axis test on edges of box and radius of circle.
// If the circle center is outside the lines extended from the collision box
// edges (also known as the Voronoi region) then the nearest box corner is checked
// for collision using a distance check.
// The nearest corner is determined from the overlap tests.
//
//   Voronoi0 |   | Voronoi1
//         ---0---1---
//            |   |
//         ---3---2---
//   Voronoi3 |   | Voronoi2
//
// Pre: This entity must be box and other entity (ent) must be circle.
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideRotatedBoxCircle(Entity &ent, VECTOR2 &collisionVector)
{
    float min01, min03, max01, max03, center01, center03;

    computeRotatedBox();                    // prepare rotated box

    // project circle center onto edge01
    center01 = graphics->Vector2Dot(&edge01, ent.getCenter());
	min01 = center01 - ent.getRadius()*ent.image.getScale(); // min and max are Radius from center
	max01 = center01 + ent.getRadius()*ent.image.getScale();
    if (min01 > edge01Max || max01 < edge01Min) // if projections do not overlap
        return false;                       // no collision is possible
        
    // project circle center onto edge03
    center03 = graphics->Vector2Dot(&edge03, ent.getCenter());
	min03 = center03 - ent.getRadius()*ent.image.getScale(); // min and max are Radius from center
	max03 = center03 + ent.getRadius()*ent.image.getScale();
    if (min03 > edge03Max || max03 < edge03Min) // if projections do not overlap
        return false;                       // no collision is possible

    // circle projection overlaps box projection
    // check to see if circle is in voronoi region of collision box
    if(center01 < edge01Min && center03 < edge03Min)    // if circle in Voronoi0
        return collideCornerCircle(corners[0], ent, collisionVector);
    if(center01 > edge01Max && center03 < edge03Min)    // if circle in Voronoi1
        return collideCornerCircle(corners[1], ent, collisionVector);
    if(center01 > edge01Max && center03 > edge03Max)    // if circle in Voronoi2
        return collideCornerCircle(corners[2], ent, collisionVector);
    if(center01 < edge01Min && center03 > edge03Max)    // if circle in Voronoi3
        return collideCornerCircle(corners[3], ent, collisionVector);

    // circle not in voronoi region so it is colliding with edge of box
    // set collision vector, uses simple center of circle to center of box
    collisionVector = *ent.getCenter() - *getCenter();
    return true;
}

//=============================================================================
// The box corner is checked for collision with circle using a distance check.
// Called by collideRotatedBoxCircle()
// Post: returns true if collision, false otherwise
//       sets collisionVector if collision
//=============================================================================
bool Entity::collideCornerCircle(VECTOR2 corner, Entity &ent, VECTOR2 &collisionVector)
{
    distSquared = corner - *ent.getCenter();            // corner - circle
    distSquared.x = distSquared.x * distSquared.x;      // difference squared
    distSquared.y = distSquared.y * distSquared.y;

    // Calculate the sum of the radii, then square it
	sumRadiiSquared = ent.getRadius()*ent.image.getScale();   // (0 + circleR)
    sumRadiiSquared *= sumRadiiSquared;                 // square it

    // if corner and circle are colliding
    if(distSquared.x + distSquared.y <= sumRadiiSquared)
    {
        // set collision vector
        collisionVector = *ent.getCenter() - corner;
        return true;
    }
    return false;
}

//=============================================================================
// Compute corners of rotated box, projection edges and min and max projections
// 0---1  corner numbers
// |   |
// 3---2
//=============================================================================
void Entity::computeRotatedBox()
{
    if(rotatedBoxReady)
        return;
    float projection;

	VECTOR2 rotatedX(cos(image.spriteData.angle), sin(image.spriteData.angle));
	VECTOR2 rotatedY(-sin(image.spriteData.angle), cos(image.spriteData.angle));

    const VECTOR2 *center = getCenter();
	corners[0] = *center + rotatedX * ((float)edge.left*image.getScale()) +
		rotatedY * ((float)edge.top*image.getScale());
	corners[1] = *center + rotatedX * ((float)edge.right*image.getScale()) +
		rotatedY * ((float)edge.top*image.getScale());
	corners[2] = *center + rotatedX * ((float)edge.right*image.getScale()) +
		rotatedY * ((float)edge.bottom*image.getScale());
	corners[3] = *center + rotatedX * ((float)edge.left*image.getScale()) +
		rotatedY * ((float)edge.bottom*image.getScale());

    // corners[0] is used as origin
    // The two edges connected to corners[0] are used as the projection lines
    edge01 = VECTOR2(corners[1].x - corners[0].x, corners[1].y - corners[0].y);
    graphics->Vector2Normalize(&edge01);
    edge03 = VECTOR2(corners[3].x - corners[0].x, corners[3].y - corners[0].y);
    graphics->Vector2Normalize(&edge03);

    // this entities min and max projection onto edges
    projection = graphics->Vector2Dot(&edge01, &corners[0]);
    edge01Min = projection;
    edge01Max = projection;
    // project onto edge01
    projection = graphics->Vector2Dot(&edge01, &corners[1]);
    if (projection < edge01Min)
        edge01Min = projection;
    else if (projection > edge01Max)
        edge01Max = projection;
    // project onto edge03
    projection = graphics->Vector2Dot(&edge03, &corners[0]);
    edge03Min = projection;
    edge03Max = projection;
    projection = graphics->Vector2Dot(&edge03, &corners[3]);
    if (projection < edge03Min)
        edge03Min = projection;
    else if (projection > edge03Max)
        edge03Max = projection;

    rotatedBoxReady = true;
}

//=============================================================================
// Is this Entity outside the specified rectangle
// Post: returns true if outside rect, false otherwise
//=============================================================================
bool Entity::outsideRect(RECT rect)
{
	if (image.spriteData.x + image.spriteData.width*image.getScale() < rect.left ||
		image.spriteData.x > rect.right ||
		image.spriteData.y + image.spriteData.height*image.getScale() < rect.top ||
		image.spriteData.y > rect.bottom)
        return true;
    return false;
}

void Entity::takeDamage(int atk, int str, int def)
{
	float attackerR = getRandomNumber();
	float defenderR = getRandomNumber();
	float chanceToHit = (ceil((0.5*attackerR + 0.5)*atk) - (0.5*defenderR + 0.5)*def)* 0.8;
	if (getRandomNumber() < chanceToHit)
	{
		damageTaken = ceil((0.5*getRandomNumber() + 0.5)*str);
	}
	else
	{
		damageTaken = 0;
	}
	splatTime = entityNS::splatTime;
	health -= damageTaken;
}

//=============================================================================
// damage
// This entity has been damaged by another entity
// Pass in the other entity's attack and strength.
// Returns the amount of damage dealt.
//=============================================================================
int Entity::damage(int atk, int str)
{
	takeDamage(atk, str, ((Enemy*)person)->getdefenseLv());
	resetAvailableHealth(oldViewport);
	displayTime = entityNS::healthDisplay;
	availableHealth->setVisible(true);
	backHealth->setVisible(true);
	if (health <= 0)
	{
		//TODO: tell draw manager that this is dead
		//TODO: Tell spawn manager to spawn another one of this dude
		delete this;
	}
	return damageTaken;
}

//=============================================================================
// Move
// Move this entity to another location.
// Location can be a single spot (Entity will move there then stop) or another entity (Entity will follow)
//=============================================================================
void Entity::move(Destination* d)
{
	//Remove last destination if any
	if (destination != nullptr)
	{
		destination->release();
	}
	destination = d;
}

//=============================================================================
// attack
// Causes this entity to attack another entity, moving towards that entity
// If both entites are colliding, perform an attack
//=============================================================================
void Entity::attack(Entity* e)
{
	victim = e;
}

//=============================================================================
// MouseInside
// Check if the mouse is inside this entity's sprite
// Returns true if it is inside, false if it is outside
// Uses input->getMouseX() and input->getMouseY()
//=============================================================================
bool Entity::mouseInside(Viewport* vp)
{
	VECTOR2 screenTopLeft;
	if (!anchored)
	{
		screenTopLeft = vp->getTopLeft();
	}
	else
	{
		screenTopLeft = VECTOR2(0, 0);
	}

	float imageTopLeftX = getX() - screenTopLeft.x - image.getWidth() / 2;
	float imageTopLeftY = getY() - screenTopLeft.y - image.getHeight() / 2;

	if (input->getMouseX() >= imageTopLeftX && input->getMouseX() <= imageTopLeftX + image.getWidth() &&
		input->getMouseY() >= imageTopLeftY && input->getMouseY() <= imageTopLeftY + image.getHeight())
		return true;

	return false;
}

/*
string Entity::view()
{
	if (person != nullptr)
	{
		return "Attack " + (((NPC*)person)->getname());
	}
	else
	{
		return "Move here";
	}
}*/

bool Entity::isEnemy()
{
	if (person != nullptr)
	{
		return (((NPC*)person)->getIsEnemy());
	}
	else
	{
		return false;
	}
}

// Resets the coordinates of the available health portion of this entity
void Entity::resetAvailableHealth(VECTOR2 topLeftViewport)
{
	availableHealth->deleteVertexBuffer();
	float healthWidth = (health / ((NPC*)person)->getmaxhealth()) * entityNS::healthBarWidth;

	if (!availableHealth->initialize(graphics,
		getX() - topLeftViewport.x - entityNS::healthBarWidth / 2,
		getY() - image.getHeight() / 2 - topLeftViewport.y - entityNS::healthBarHeight,
		healthWidth, entityNS::healthBarHeight, uiNS::healthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Available Health could not be initalized");
	}
}

// Resets the coordinates of the health portion of this entity
// Note that this only resets the health part, which is the background of the health
// To re-draw the red part as well, call resetAvailableHealth.
void Entity::resetHealth(VECTOR2 topLeftViewport)
{
	backHealth->deleteVertexBuffer();
	if (!backHealth->initialize(graphics,
		getX() - topLeftViewport.x - entityNS::healthBarWidth / 2,
		getY() - image.getHeight() / 2 - topLeftViewport.y - entityNS::healthBarHeight,
		entityNS::healthBarWidth, entityNS::healthBarHeight, uiNS::noHealthColor, ""))
	{
		throw new GameError(gameErrorNS::FATAL_ERROR, "Back Health could not be initalized");
	}
}