#ifndef KSA_TANK_H
#define KSA_TANK_H

#include "aitank.h"
#include <list>
#include <math.h>


class KSATank : public AITank
{
private:
	bool timeToFire; // Bool for firing bullets

	bool forwards; // Is player going forwards
	Position wayPoint[5]; // Waypoints on the map for the AI
	int currentWayPoint; // Current waypoint of AI 

	enum state{roamingState, offensiveState, evadeState, collisionState}; // AI's FSM states
	Position buildingPos;		// Position of a found building
	Position enemyPos;			// Position of a found enemy
	Position enemyBullet;		// Position of a found enemy bullet
	int currentState;			// Current state AI is in
	int lastState;				// Last state AI was in
	bool isWinning;				// Bool to check if AI is winning
	
public:
    KSATank();
    ~KSATank();

	void move();
	void reset();
	void collided();
	void markTarget(Position p);				// Mark position of enemy base
	void markEnemy(Position p);					// Mark position of enemy tank
	void markBase(Position p);					// Mark position of AI base
	void markShell(Position p);					// Mark position of shell
	void score(int thisScore,int enemyScore);	// Calculate score between player and ai
	bool isFiring();							// Is the AI firing?

	void aimTurret(Position p);					// Aim turret function
	bool playerWinning();						// If the ai is winning set to true
	void MoveToWaypoint(Position p);			// Move to a way point
	void changeWayPoint(Position p);			// Change way point once reached
	void evade();								// Evade function
	void manouver();							// Manouver function (for collisions)
};
#endif