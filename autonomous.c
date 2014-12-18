
#include "drivers/hitechnic-irseeker-v2.h"
//#include "JoystickDriver.c"

float xCoor = -1;
float yCoor = -1;
int direction = 0;

int north = 0;
int east = 1;
int south = 2;
int west = 3;

bool badPoint [7][7];

bool validPoint (int X, int Y){
	return badPoint[X][Y];
}

float convertInches(){
}

void moveForward(float difference, bool coor){

	for(int i = 0; i < abs(difference); i++){
		if(direction == north || direction == east){
			if(coor){
				if(validPoint(xCoor + 1, yCoor)){
					xCoor++;
				}
			}
			else{
				if(validPoint(xCoor, yCoor + 1)){
					yCoor++;
				}
			}
		}
		else{
			if(coor){
				if(validPoint(xCoor - 1, yCoor)){
					xCoor--;
				}
			}
			else{
				if(validPoint(xCoor, yCoor - 1)){
					yCoor--;
				}
			}
		}
		nxtDisplayString(0,"X: %i",xCoor);
		nxtDisplayString(1,"Y: %i",yCoor);
		wait10Msec(100);
	}
	wait10Msec(100);
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

void handleCoor(float newCoor, float speed, bool axis){
	float difference;

	if(axis){
		difference = newCoor - xCoor;
  }
	else{
 		difference = newCoor - yCoor;
	}
	if(difference != 0){
		//North Direction
		if(direction == north){
			//Positive
			if((difference/abs(difference)) == 1){
				//x axis
				if(axis){
					turnRight();
				}
				//y axis
				else{
				}
			}
			//Negative
			else{
				//x axis
				if(axis){
					turnLeft();
				}
				//y axis
				else{
					turnLeft();
					turnLeft();
				}
			}
		}
		//South Direction
		else if(direction == south){
			//Positive
			if((difference/abs(difference)) == 1){
				//x axis
				if(axis){
					turnLeft();
				}
				//y axis
				else{
					turnLeft();
					turnLeft();
				}
			}
			//Negative
			else{
				//x axis
				if(axis){
					turnRight();
				}
				//y axis
				else{
				}
			}
		}
		//East Direction
		else if(direction == east){
			//Positive
			if((difference/abs(difference)) == 1){
				//x axis
				if(axis){
			}
			//y axis
				else{
					turnLeft();
				}
			}
			//Negative
			else{
				//x axis
				if(axis){
					turnLeft();
					turnLeft();
				}
				//y axis
				else{
					turnRight();
				}
			}
		}
		//West Direction
		else if(direction == west){
			//Positive
			if((difference/abs(difference)) == 1){
				//x axis
				if(axis){
					turnLeft();
					turnLeft();
				}
				//y axis
				else{
					turnRight();
				}
			}
			//Negative
			else{
				//x axis
				if(axis){
				}
				//y axis
				else{
					turnLeft();
				}
			}
		}
		moveForward(difference, axis);
	}
}

void simulateRobot(float newXCoor, float newYCoor, float speed){
	if(speed != 0){
		handleCoor(newXCoor, speed, true);
		handleCoor(newYCoor, speed, false);
	}
	xCoor = newXCoor;
	yCoor = newYCoor;
	nxtDisplayString(0,"X: %i",xCoor);
	nxtDisplayString(1,"Y: %i",yCoor);
	nxtDisplayString(2,"D: %i",direction);
	nxtDisplayString(3,"Move done");
	wait10Msec(500);
	nxtDisplayClearTextLine(3);
}

task main()
{
	badPoint[3][3] = false;
	badPoint [3][4] = false;
	badPoint [4][3] = false;
	badPoint [4][4] = false;
	badPoint [3][1] = false;
	badPoint [4][1] = false;

	simulateRobot(3, 2, 0);
	simulateRobot(2, 2, 100);
	simulateRobot(4, 2, 100);
	simulateRobot(4, 4, 100);
	simulateRobot(4, 2, 100);
	simulateRobot(1, 5, 100);
	simulateRobot(3, 2, 100);
	simulateRobot(4, 2, 100);
	simulateRobot(2, 1, 100);
	simulateRobot(3, 3, 100);
	simulateRobot(3, 5, 100);
	simulateRobot(3, 3, 100);
	simulateRobot(3, 1, 100);
	simulateRobot(3, 5, 100);
	simulateRobot(4, 5, 100);
	simulateRobot(5, 5, 100);
	simulateRobot(4, 5, 100);

}
