#ifndef _SHRIVELED_MAN_H                 // Prevent multiple definitions if this 
#define _SHRIVELED_MAN_H                 // file is included in more than one place

#include "entity.h"
#include <deque>
#include "constants.h"
#include "textDX.h"
#include "player.h"
#include "Dragonfire.h"

struct StolenArtifactRunPhrase
{
	//The phrase to say
	string phrase;
	//Time till the next phrase, in seconds.
	float time;
};

namespace shriveledManNS
{
	const char location[] = "assets/npcs/sprites/shriveled_man.png";
	const int imageWidth = 64;
	const int imageHeight = 64;

	//The location in which the man attempts to run towards at the end of the Stolen Artifact Quest.
	const char locationToRunTo = '\\';
}

//The shrieveld man, the main antagonist of the mysterious artifact questline
class ShriveledMan : public Entity
{
private:
	//Phrases the character will say at the end of the stolen artifact quest, as well as the time (in seconds).
	//Initalize in class initalizer.
	vector<StolenArtifactRunPhrase> stolenArtifactRunPhrases;

	TextureManager* manTexture;
	UI* ui;
	bool currentlyVisible;
	//Marks the status of the stolen artifact run, which this character does at the end of the quest.
	//Number represents current phrase being said, -1 means no phrase (i.e. no run is currently being performed)
	int stolenArtifactEndRunStatus;
	//Timer till the next phrase is said.
	float stolenArtifactTimer;
	//Whether to call fakeDelete() once this person is out of view
	bool disappearWhenOutOfView;
protected:

public:
	// constructor
	ShriveledMan();

	// Destructor
	virtual ~ShriveledMan();

	// inherited member functions
	virtual void draw(Viewport* viewport);
	virtual bool initialize(Game* gamePtr, Player* player, Destination* location, NPC* npc);

	virtual void update(float frameTime, Game* gamePtr);

	// Release resources
	virtual void onLostDevice();

	// Restore resources
	virtual void onResetDevice();

	virtual string getType(){ return "SHRIVELED_MAN"; }

	//Starts the run at the end of the stolen artifact quest.
	virtual void startStolenArtifactRun();

	//Starts the rift intro in the mysterious artifact quest
	virtual void startRiftIntro();

	//Continues the rift intro where monsters appear in the mysterious artifact quest
	virtual void continueRiftIntro();
};
#endif