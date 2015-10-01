#define PI 3.14159265

#include "../include/KSATank.h"

KSATank::KSATank() // Construtor
{
	timeToFire = false;
	isWinning = false;
	forwards = true;
	currentState = 0;
	currentWayPoint = 0;


	wayPoint[0].set(700,100,0);
	wayPoint[1].set(100,100,0);
	wayPoint[2].set(400,350,0);
	wayPoint[3].set(100,500,0);
	wayPoint[4].set(700,500,0);
}

KSATank::~KSATank(){} // Destructor


void KSATank::reset() // When the tank is hit
{
	timeToFire = false;
	currentState = 0;
	forwards = true;
	stopTurret();
}

void KSATank::move()
{
	switch(currentState)
	{
	case roamingState: // Roaming state
		MoveToWaypoint(wayPoint[currentWayPoint]);	
		break;

	case offensiveState: // Offensive state
		aimTurret(buildingPos);
		break;

	case evadeState: // Evade state
		evade();
		break;

	case collisionState: // Collision state
		manouver();
			break;
	}

}

void KSATank::collided()
{
	currentState = 3;
	currentWayPoint = currentWayPoint + 1;
}


void KSATank::markTarget(Position p) // Enemy base
{	
	currentState = offensiveState; // Current state = offensive (building)
	buildingPos.set(p.getX(),p.getY(),p.getTh());// Store the buildings position
	
}

void KSATank::markEnemy(Position p) // Enemy tank
{
	currentState = offensiveState; // Current state = offensive (enemy)
	enemyPos.set(p.getX(),p.getY(),0); // Store the enemies position
}

void KSATank::markBase(Position p) // Own base
{

}

void KSATank::markShell(Position p)  // Dodge bullets
{
	currentState = evadeState;// Current state = evadeState
	enemyBullet.set(p.getX(),p.getY(),p.getTh()); // Get position of enemy bullet
}

void KSATank::score(int thisScore,int enemyScore) 
{
	if(enemyScore > thisScore)
	{
		isWinning = false;
	}else{isWinning = true;} // Returns if the AI is winning or not
}

bool KSATank::isFiring()
{
	bool FireNow = timeToFire;  // If it is time to fire
	timeToFire = false;			// Set time to fire to be false
	return FireNow;				// Return bool if tank can fire
} 

void KSATank::aimTurret(Position p)
{
	timeToFire = false;
	// Remember 90 degrees globally is 0 degrees in this co-ordinate system

	float x, y, angle, angleDifference;
	angleDifference = 0;
	x = p.getX() - pos.getX();
	y = p.getY() - pos.getY();

	angle = atan2(y,x) * 180.0 / PI;

	if(angle < 0)
	{
		angle = angle + 360;
	}
	if(angle > 360)
	{
		angle = angle - 360;
	}

	angleDifference = angle - turretTh;

	if(angle < turretTh-2)
	{
		if(angleDifference >= 0)
		{
			turretGoRight();
		}else{turretGoLeft();}
			
	}

	if(angle > turretTh+2)
	{
		if(angleDifference <= 0)
		{
			turretGoLeft();
		}else{turretGoRight();}
	}
	

	if(turretTh-4 < angle && angle < turretTh+4)
	{
		timeToFire = true;
	}
	
	currentState = roamingState;
}

bool KSATank::playerWinning()
{
	return isWinning; // Returns the isWinning bool
}

void KSATank::MoveToWaypoint(Position p)
{
	float x, y, angle, angleDifference; // Variables for calculating where to go
	angleDifference = 0;
	angle = 0;
	x = p.getX() - pos.getX();
	y = p.getY() - pos.getY();

	angle = atan2(y,x) * 180.0 / PI;

	angleDifference = angle - pos.getTh();

	// Sets the angle back to 360 if it goes below 0
	if(angle < 0)
	{
		angle = angle + 360;
	}
	// Sets the angle back to 0 if it goes above 360
	if(angle > 360)
	{
		angle = angle - 360;
	}

	// If the angle is less than the tanks rotation
	if(angle < pos.getTh()-5)
	{
		if(angleDifference >= 0) //if the angle difference is positive
		{
			goRight(); //turn right
		}
			
	}
	// If the angle is greater than the tanks rotation
	if(angle > pos.getTh()+5)
	{
		if(angleDifference <= 0) // If the angle difference is negative
		{
			goLeft(); // Turn left
		}
	}
	
	if(pos.getTh()-2 < angle && angle < pos.getTh()+2) // If the tank is turned in correct position
	{
		goForward(); // Go forwards
	}
	changeWayPoint(p); // Changed to a new waypoint
	
}

void KSATank::changeWayPoint(Position p)
{
	
	if(pos.getX() < p.getX() + 25 && pos.getX() > p.getX() - 25) // Once tank is within a perimeter of 25 of the waypoint
	{
		if(pos.getY() < p.getY() + 25 && pos.getY() > p.getY() - 25)
		{
		
			currentWayPoint = currentWayPoint + 1; // Change waypoint
			goLeft();	// Go left
		}

	}
	if(currentWayPoint > 3)currentWayPoint = 0; // If waypoint is above number of waypoints, reset back to 0

}

void KSATank::evade()
{
	// Get the angle difference between bullet and tank
	float angleDiff;
	angleDiff =pos.getTh() - enemyBullet.getTh();

	// If bullet is coming from the side, move forward or backwards
	Position originalPos;
	originalPos.set(enemyBullet.getX(),enemyBullet.getY(),enemyBullet.getTh());
	if(angleDiff >= 30 || angleDiff <= -30)
	{
		if(forward == true)goBackward();
		if(backward == true)goForward();
	}
	if(enemyBullet.getX() > originalPos.getX() + 50 || enemyBullet.getX() < originalPos.getX() - 50)
	{
		currentState = roamingState;
	}
	if(enemyBullet.getY() > originalPos.getY() + 50 || enemyBullet.getY() < originalPos.getY() - 50)
	{
		currentState = roamingState;
	}
}

void KSATank::manouver()
{
	// New angle is a 180degree turn for tank
	static float newAngle = pos.getTh() + 180;

	if(newAngle < 0)
	{
		newAngle = newAngle + 360;
	}
	if(newAngle > 360)
	{
		newAngle = newAngle - 360;
	}
	// If the tank has turned 180 degrees
	if(pos.getTh() < newAngle + 10 && pos.getTh() > newAngle - 10)
	{
		goForward(); // Go forward
		
		currentWayPoint = currentWayPoint + 1; // Increase waypoint by 1
	}else{goLeft();} // Else go left

	currentState = roamingState; // Change state back to roaming
}