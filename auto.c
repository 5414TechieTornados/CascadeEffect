#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     IRRight,        sensorI2CCustom)
#pragma config(Sensor, S3,     gyro,           sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     launcher,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     belts,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     left,          tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     right,         tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     collector,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     lift,          tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    bottomGate,           tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    topGate,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    tube,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    auto4,                tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "drivers/hitechnic-irseeker-v2.h"
#include "JoystickDriver.c"
#include "drivers/hitechnic-gyro.h"

//Robot directional info
float xCoor = -1;
float yCoor = -1;
int direction = 0;

//directions
int north = 0;
int east = 1;
int south = 2;
int west = 3;

//string version of directions
string forward = "forward";
string backwards = "backwards";
string rightDirection = "right";
string leftDirection = "left";

//movement information
float maxSpeed = 60;
float turnSpeed = 50;
float tolerance = 0;
float blockDistance = 12;
float leftTurnDistance = 10;
float turnDistance = 11.4;

const float TOP_GATE_UP = 180;
const float TOP_GATE_DOWN = 60;
const float SPIN_UP_TIME = 2500;
const float TURN_TIME = 1.0;
const float RIGHT_TURN_TIME = 1.1;

//Bad points on the field
bool badPoint [7][7];

void turnRight(){
	motor[left] = 78;
	motor[right] = -78;
}

void turnLeft(){
	motor[left] = -78;
	motor[right] = 78;
}

void gyroTurn(){
	HTGYROstartCal(gyro);
	float heading = 0.0;
	float currTime;
	float prevTime = nPgmTime;
	while (true)
	{
		currTime = nPgmTime;
		heading += ((float)HTGYROreadRot(gyro)) * (currTime - prevTime) / 1000;
		prevTime = currTime;
	}
}

void initializeRobot()
{

  // Place code here to initialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

	servoTarget[topGate] = TOP_GATE_DOWN;
	servoTarget[tube] = 255;

  	return;
}

void startSpin(bool up){
	ClearTimer(T1);
	if(up){
		motor[lift] = -100;
	}
	else{
		motor[lift] = -00;
	}
}

bool checkSpinTimer(){
	if(time1(T1) > SPIN_UP_TIME){
		motor[lift] = 0;
		return true;
	}
	else
		return false;
}

bool validPoint (int X, int Y){
	return badPoint[X][Y];
}

float convertInches(float inches){
	return (1120/6) * inches;
}

void driveRobot(float distance, float speed, string direction){
		//reset encoder values
	nMotorEncoder[right] = 0;
	nMotorEncoder[left] = 0;

	wait10Msec(100);

	float target = convertInches(distance);

	//setting the encoder target
	//nMotorEncoderTarget[right] = target;
	//nMotorEncoderTarget[left] = target;
	//blocks used to set correct motor speeds
	if(forward == direction){
		motor[right] = speed + 1;
		motor[left] = speed;
	}
	else if(backwards == direction){
		motor[right] = -speed - 1;
		motor[left] = -speed;
	}
	else if(rightDirection == direction){
		motor[right] = -speed - 1;
		motor[left] = speed;
	}
	else if(leftDirection == direction){
		motor[right] = speed + 1;
		motor[left] = -speed;
	}

		//Move the robot until encoder target is reached
	while(abs(nMotorEncoder[right]) < target){
	//while(nMotorRunState[left] != runStateIdle){

		checkSpinTimer();
		nxtDisplayTextLine(1, "target: %d", target);
		nxtDisplayTextLine(2, "Right: %d", nMotorEncoder[right]);
		nxtDisplayTextLine(3, "Left: %d", nMotorEncoder[left]);
		if(abs(target - abs(nMotorEncoder[right])) < tolerance || abs(target - abs(nMotorEncoder[left])) < tolerance){
			break;
		}
	}
	motor[right] = 0;
	motor[left] = 0;
}

void turnLeftTime(int full){

	motor[left] = -100;
	motor[right] = 100;
	if(full == 0){
		wait10Msec(100 * TURN_TIME);
	}
	else if(full == 1){
		wait10Msec(100 * (TURN_TIME/1.8));
	}
	else{
		wait10Msec(100 * (TURN_TIME * 1.2));
	}
	motor[left] = 0;
	motor[right] = 0;
}

void turnRightTime(int full){
	motor[left] = 100;
	motor[right] = -100;
		if(full == 0){
		wait10Msec(100 * RIGHT_TURN_TIME);
	}
	else{
		wait10Msec(100 * (RIGHT_TURN_TIME/1.8));
	}
	motor[left] = 0;
	motor[right] = 0;
}
void turnLeft(float distance){
	driveRobot(distance, turnSpeed, leftDirection);
}

void turnRight(float distance){
	driveRobot(distance, turnSpeed, rightDirection);
}

void scoreBall(){

	driveRobot(blockDistance / 4, maxSpeed, backwards);

	wait10Msec(100);
	servoTarget(topGate) = TOP_GATE_UP;
	wait10Msec(100);
	driveRobot(blockDistance / 4, maxSpeed, forward);
}


void hitPeg(){
	turnRight(turnDistance - 1);
	driveRobot(blockDistance/.9, 78, backwards);
	turnLeft(turnDistance + 1.);
	driveRobot(4 * blockDistance, 78, backwards);

}

void firstPosition(){
	startSpin(true);
	driveRobot(1.85 * blockDistance, 78, backwards);
	while(!checkSpinTimer()){
	}
	scoreBall();
	//startSpin(false);
	hitPeg();
}

void diagonalMove(){
	startSpin(true);
	driveRobot(blockDistance/2, 78, backwards);
	turnLeft(turnDistance / 2.35);
	driveRobot(blockDistance * 2, 78, backwards);
}

void secondPosition(){
	diagonalMove();
	turnRight(turnDistance);
	driveRobot(blockDistance/2.5, maxSpeed, backwards);
	while(!checkSpinTimer()){
	}
	scoreBall();
	hitPeg();
}


void thirdPosition(){
	diagonalMove();
	turnRight(turnDistance/1.8);
	driveRobot(blockDistance * 1.5, maxSpeed, backwards);
	turnRight(turnDistance);
	while(!checkSpinTimer()){
	}
	scoreBall();
	hitPeg();
}

void continueHitting(){
	driveRobot(blockDistance * 2, 78, forward);
	driveRobot(blockDistance * 2, 78, backwards);
}

task main()
{
	initializeRobot();
	waitForStart();

	tHTIRS2DSPMode _mode = DSP_1200;
	int seekerValue = -5;

	wait10Msec(50);
	seekerValue = HTIRS2readACDir(IRRight);
	nxtDisplayString(2,"IR: %i",seekerValue);

	ClearTimer(T1);
	startSpin(true);
	if(seekerValue > 4){
		firstPosition();
	}
	else if(seekerValue > 2){
		secondPosition();
	}
	else{
		thirdPosition();
	}
	while(true){
		continueHitting();
	}
}
