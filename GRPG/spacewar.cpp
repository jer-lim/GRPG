// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Draw animated spaceships with collision and shield
// Chapter 6 spacewar.cpp v1.0
// This class is the core of the game

#include "spaceWar.h"

//=============================================================================
// Constructor
//=============================================================================
Spacewar::Spacewar()
{}

//=============================================================================
// Destructor
//=============================================================================
Spacewar::~Spacewar()
{
    releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void Spacewar::initialize(HWND hwnd)
{
    Game::initialize(hwnd); // throws GameError

    // nebula texture
    if (!nebulaTexture.initialize(graphics,NEBULA_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula texture"));

    // main game textures
    if (!gameTextures.initialize(graphics,TEXTURES_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));

    // nebula image
    if (!nebula.initialize(graphics,0,0,0,&nebulaTexture))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing nebula"));

    // planet
    if (!planet.initialize(this, planetNS::WIDTH, planetNS::HEIGHT, 2, &gameTextures))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing planet"));

    // ship
    if (!ship1.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures, true))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship1"));
    ship1.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
    ship1.setCurrentFrame(shipNS::SHIP1_START_FRAME);
    ship1.setX(GAME_WIDTH/4);
    ship1.setY(GAME_HEIGHT/4);
    ship1.setVelocity(VECTOR2(shipNS::SPEED,-shipNS::SPEED)); // VECTOR2(X, Y)
    // ship2
    if (!ship2.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures, true))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship2"));
    ship2.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
    ship2.setCurrentFrame(shipNS::SHIP2_START_FRAME);
    ship2.setX(GAME_WIDTH - GAME_WIDTH/4);
    ship2.setY(GAME_HEIGHT/4);
    ship2.setVelocity(VECTOR2(-shipNS::SPEED,-shipNS::SPEED)); // VECTOR2(X, Y)
	//ship3 initalization. Do not rotate, start pointing north. This ship is controlled by the arrow keys.
	//ship3 ignores direction and will move in the direction of the key
	if (!ship3.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures, false))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship3"));
	ship3.setFrames(shipNS::SHIP1_START_FRAME, shipNS::SHIP1_END_FRAME);
    ship3.setCurrentFrame(shipNS::SHIP1_START_FRAME);
    ship3.setX(GAME_WIDTH - GAME_WIDTH/4);
    ship3.setY(GAME_HEIGHT - GAME_HEIGHT/4);
	ship3.setDegrees(270);
	//ship3.setVelocity(VECTOR2(shipNS::SPEED,-shipNS::SPEED)); // VECTOR2(X, Y)
	//ship4 initalization. Do not rotate, start pointing north. This ship is controlled by WASD
	//ship4 will rotate with A and D, and will move forward based on direction on W, and move backward based on direction on S.
	if (!ship4.initialize(this, shipNS::WIDTH, shipNS::HEIGHT, shipNS::TEXTURE_COLS, &gameTextures, false))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing ship4"));
	ship4.setFrames(shipNS::SHIP2_START_FRAME, shipNS::SHIP2_END_FRAME);
    ship4.setCurrentFrame(shipNS::SHIP2_START_FRAME);
    ship4.setX(GAME_WIDTH/4);
    ship4.setY(GAME_HEIGHT - GAME_HEIGHT/4);
	ship4.setDegrees(270);
	//ship4.setVelocity(VECTOR2(shipNS::SPEED,-shipNS::SPEED)); // VECTOR2(X, Y)

    return;
}

//=============================================================================
// Update all game items
//=============================================================================
void Spacewar::update()
{
	//Ship 3 movement. Adds the required part to the velocity
	if(input->isKeyDown(SHIP_RIGHT_KEY))            // if move right
    {
		ship3.setVelocity(ship3.getVelocity() + VECTOR2(frameTime * shipNS::SPEED, 0));
    }
    if(input->isKeyDown(SHIP_LEFT_KEY))             // if move left
    {
		ship3.setVelocity(ship3.getVelocity() - VECTOR2(frameTime * shipNS::SPEED, 0));
    }
	if(input->isKeyDown(SHIP_DOWN_KEY))            // if move down
    {
		ship3.setVelocity(ship3.getVelocity() + VECTOR2(0, frameTime * shipNS::SPEED));
    }
    if(input->isKeyDown(SHIP_UP_KEY))             // if move top
    {
		ship3.setVelocity(ship3.getVelocity() - VECTOR2(0, frameTime * shipNS::SPEED));
    }

	//Ship4 controls.
	//Rotations
	if(input->isKeyDown(0x44))				  // if turn right, D key
    {
		ship4.setRadians(ship4.getRadians() + frameTime * shipNS::ROTATION_RATE);
    }
    if(input->isKeyDown(0x41))			       // if turn left, A key
    {
		ship4.setRadians(ship4.getRadians() - frameTime * shipNS::ROTATION_RATE);
    }
	//Movement forward and backwards
    if(input->isKeyDown(0x57))               // if move up, W key
    {
		//Calculate how much to move
		float currentRotation = ship4.getRadians();
		
		double xChange = cos(currentRotation);
		double yChange = sin(currentRotation);

		//Increase velocity (moving forward) based on direction.
		ship4.setVelocity(ship4.getVelocity() + VECTOR2(frameTime * shipNS::SPEED * xChange, frameTime * shipNS::SPEED * yChange));
    }
	if(input->isKeyDown(0x53))             // if move down, S Key
    {
        //Calculate how much to move
		float currentRotation = ship4.getRadians();
		
		double xChange = cos(currentRotation);
		double yChange = sin(currentRotation);

		//Decrease velocity (moving backward) based on direction.
		ship4.setVelocity(ship4.getVelocity() - VECTOR2(frameTime * shipNS::SPEED * xChange, frameTime * shipNS::SPEED * yChange));
    }

	//Update everything
    planet.update(frameTime);
    ship1.update(frameTime);
    ship2.update(frameTime);
	ship3.update(frameTime);
	ship4.update(frameTime);
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void Spacewar::ai()
{}

//=============================================================================
// Handle collisions
//=============================================================================
void Spacewar::collisions()
{
    VECTOR2 collisionVector;
    // if collision between ship and planet
    if(ship1.collidesWith(planet, collisionVector))
    {
        // bounce off planet
        ship1.bounce(collisionVector, planet);
        ship1.damage(PLANET);
    }
    if(ship2.collidesWith(planet, collisionVector))
    {
        // bounce off planet
        ship2.bounce(collisionVector, planet);
        ship2.damage(PLANET);
    }
	if(ship3.collidesWith(planet, collisionVector))
    {
        // bounce off planet
        ship3.bounce(collisionVector, planet);
        ship3.damage(PLANET);
    }
	if(ship4.collidesWith(planet, collisionVector))
    {
        // bounce off planet
        ship4.bounce(collisionVector, planet);
        ship4.damage(PLANET);
    }
    // if collision between ships
    if(ship1.collidesWith(ship2, collisionVector))
    {
        // bounce off ship
        ship1.bounce(collisionVector, ship2);
        ship1.damage(SHIP);
        // change the direction of the collisionVector for ship2
        ship2.bounce(collisionVector*-1, ship1);
        ship2.damage(SHIP);
    }
	if(ship1.collidesWith(ship3, collisionVector))
    {
        // bounce off ship
        ship1.bounce(collisionVector, ship3);
        ship1.damage(SHIP);
        // change the direction of the collisionVector for ship3
        ship3.bounce(collisionVector*-1, ship1);
        ship3.damage(SHIP);
    }
	if(ship1.collidesWith(ship4, collisionVector))
    {
        // bounce off ship
        ship1.bounce(collisionVector, ship4);
        ship1.damage(SHIP);
        // change the direction of the collisionVector for ship4
        ship4.bounce(collisionVector*-1, ship1);
        ship4.damage(SHIP);
    }
	if(ship2.collidesWith(ship3, collisionVector))
    {
        // bounce off ship
        ship2.bounce(collisionVector, ship3);
        ship2.damage(SHIP);
        // change the direction of the collisionVector for ship3
        ship3.bounce(collisionVector*-1, ship2);
        ship3.damage(SHIP);
    }
	if(ship2.collidesWith(ship4, collisionVector))
    {
        // bounce off ship
        ship2.bounce(collisionVector, ship4);
        ship2.damage(SHIP);
        // change the direction of the collisionVector for ship4
        ship4.bounce(collisionVector*-1, ship2);
        ship4.damage(SHIP);
    }
	if(ship3.collidesWith(ship4, collisionVector))
    {
        // bounce off ship
        ship3.bounce(collisionVector, ship4);
        ship3.damage(SHIP);
        // change the direction of the collisionVector for ship3
        ship3.bounce(collisionVector*-1, ship4);
        ship3.damage(SHIP);
    }
}

//=============================================================================
// Render game items
//=============================================================================
void Spacewar::render()
{
    graphics->spriteBegin();                // begin drawing sprites

    nebula.draw();                          // add the orion nebula to the scene
    planet.draw();                          // add the planet to the scene
    ship1.draw();                           // add the spaceship to the scene
    ship2.draw();                           // add the spaceship to the scene
	ship3.draw();                           // add the spaceship to the scene
	ship4.draw();                           // add the spaceship to the scene

    graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void Spacewar::releaseAll()
{
    nebulaTexture.onLostDevice();
    gameTextures.onLostDevice();
    Game::releaseAll();
    return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void Spacewar::resetAll()
{
    gameTextures.onResetDevice();
    nebulaTexture.onResetDevice();
    Game::resetAll();
    return;
}
