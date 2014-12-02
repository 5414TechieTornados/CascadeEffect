
#include "drivers/hitechnic-irseeker-v2.h"
//#include "JoystickDriver.c"

float xCoor = -1;
float yCoor = -1;
int direction = 0;

int north = 0;
int east = 1;
int south = 2;
int west = 3;

float convertInches(){
}

void moveForward(float difference, bool coor){

	for(int i = 0; i < abs(difference); i++){
		if(direction == north || direction == east){
			if(coor){
				xCoor++;
			}
			else{
				yCoor++;
			}
		}
		else{
			if(coor){
				xCoor--;
			}
			else{
				yCoor--;
			}
		}
		nxtDisplayString(0,"X: %i",xCoor);
		nxtDisplayString(1,"Y: %i",yCoor);
		wait10Msec(100);
	}
}

void moveBackward(float difference){
	wait10Msec(50);
}

void turnLeft(){

	direction = ((direction - 1) + 4) % 4;
	nxtDisplayString(2,"D: %i",direction);
	wait10Msec(100);
}

void turnRight(){
	direction = ((direction + 1) + 4) % 4;
	nxtDisplayString(2,"D: %i",direction);
	wait10Msec(100);
}

void handleXCoor(float newXCoor, float speed){

	float difference = newXCoor - xCoor;
	if(difference != 0){
	if(direction == north){
		if((difference/abs(difference)) == 1){
			turnRight();
		}
		else{
			turnLeft();
		}
	}
	else if(direction == south){
		if((difference/abs(difference)) == 1){
			turnLeft();
		}
		else{
			turnRight();
		}
	}
	else if(direction == east){
		if((difference/abs(difference)) == 1){
		}
		else{
			turnLeft();
			turnLeft();
		}
	}
	else if(direction == west){
		if((difference/abs(difference)) == 1){
			turnLeft();
			turnLeft();
		}
	}
	moveForward(difference, true);
}
}

void handleYCoor(float newYCoor, float speed){
float difference = newYCoor - yCoor;
if(difference != 0){
	if(direction == north){
		if((difference/abs(difference)) == 1){
		}
		else{
			turnLeft();
			turnLeft();
		}
	}
	else if(direction == south){
		if((difference/abs(difference)) == 1){
			turnLeft();
			turnLeft();
		}
		else{
		}
	}
	else if(direction == east){
		if((difference/abs(difference)) == 1){
		  turnLeft();
		}
		else{
			turnRight();
		}
	}
	else if(direction == west){
		if((difference/abs(difference)) == 1){
			turnRight();
		}
		else{
			turnLeft();
		}
	}
	moveForward(difference, false);
}
}

void simulateRobot(float newXCoor, float newYCoor, float speed){
	if(speed != 0){
		handleXCoor(newXCoor, speed);
		handleYCoor(newYCoor, speed);
	}
	xCoor = newXCoor;
	yCoor = newYCoor;
	nxtDisplayString(0,"X: %i",xCoor);
	nxtDisplayString(1,"Y: %i",yCoor);
	nxtDisplayString(2,"D: %i",direction);
	wait10Msec(50);
}

task main()
{

	simulateRobot(3, 2, 0);
	simulateRobot(2, 2, 100);
	simulateRobot(4, 2, 100);
	simulateRobot(4, 4, 100);
	simulateRobot(4, 2, 100);
	simulateRobot(1, 5, 100);
	simulateRobot(3, 2, 100);
}
