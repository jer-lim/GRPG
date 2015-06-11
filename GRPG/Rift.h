#ifndef _RIFT_H                 // Prevent multiple definitions if this 
#define _RIFT_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "RiftData.h"

class GameEvent;
class UI;

namespace riftNS
{
	const char location[] = "assets/misc/rift.png";
	const int imageWidth = 32;
	const int imageHeight = 32;
	const int imageCols = 1;

	//in seconds
	const int timeForFullRotation = 5;
	//In pixels
	const int maximumWanderRadiusFromRift = 500;

	//Wave status
	//Wave is just starting, monsters are arriving from off the map.
	const int STARTING = 0;
	//Wave has started, monsters are in position, although none are attacking the player at the moment.
	const int STARTED = 1;
	//Wave is in progress, player is currently battling the monsters.
	const int PROGRESSING = 2;
	//Rift has completed
	const int COMPLETED = 3;

	//How far away can the player move from the rift?
	const float maximumDistanceFromRift = 300;
	const float minimumSpawnFromRift = 50;
	const float maximumSpawnFromRift = 80;
	const float enemyEntryDistance = 300;

	//To keep note of spawned enemies and see if they are dead or not, Rift uses the spawnlinks
	//in game, instead of directly accessing the memory, which may cause a crash if the entity is 
	//already dead and deleted. To ensure that spawnlinks added by Rift does not collide with spawnlinks
	//added by other stuff, (i.e. the legitimate, planned uses like Spawners), a phrase is added in front of
	//the spawn links used by Rift.
	const string spawnLinkPhrase = "Rift";

	const int bonusXpForFullComp = 2000;
}

//The rift, which start appearing during the Mysterious Artifact quest, and appears everywhere from now on.
class Rift : public Entity
{
private:
	TextureManager* riftTexture;
	UI* ui;
	int currentWave;
	int waveStatus;
	RiftData* riftData;
	int remainingDifficulty;
	vector<Entity*> enemiesSpawned;
	//Whether this entity will be deleted on the next update step.
	bool allocatedForDeletion;
	int totalWaves;
protected:
	Behavior* enterBehavior;
	Behavior* exitBehavior;
	
	//Adds up all the probabilities for all the possible minions that can be created,
	//picks a random one and then returns the id, reducing the reaminingDifficulty amount
	//by the difficulty of the selected NPC.
	virtual int getNewNPC();

	//Calculate the final location of a specific point,
	//given that, from the passed in x and y co-ordinates, a person travels a certain
	//distance at a specific angle.
	//Used to pick a random point for mobs to spawn
	VECTOR2 getFinalLocation(float startX, float startY, float angle, float distance);
public:
	// constructor
	Rift();

	// Destructor
	virtual ~Rift();

	virtual int getCurrentWave() { return currentWave; }
	virtual int getWaveStatus() { return waveStatus; }

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, NPC* character);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual void setupVectorActiveBehaviors()
	{
		vectorActiveBehaviors.clear();
		if (exitBehavior)
			vectorActiveBehaviors.push_back(exitBehavior);
		if (enterBehavior)
			vectorActiveBehaviors.push_back(enterBehavior);
		if (viewBehavior)
			vectorActiveBehaviors.push_back(viewBehavior);
	}

	virtual void setupBehaviors();

	virtual string getType(){ return "RIFT"; }

	//Begins the rift wave. Starts by spawning the default amount of mobs around the rift.
	//Once those are killed, the real wave starts.
	//Require walking: If true, enemies will walk to their planned spawn points, otherwise, they just appear there
	virtual void begin(bool requireWalking);

	//Game event needs to tell rift when the player gets damaged or is damaged
	virtual void eventOccured(GameEvent* e, UI* ui);

	//Closes the rift
	virtual void close();

	//Awards xp based on how many waves are cleared
	virtual void awardXP();

	//Deals damage to the player based on how many enemies there are left
	//If the rift is safe to exit, will return without dealing any damage.
	virtual void preMatureExit();
};
#endif