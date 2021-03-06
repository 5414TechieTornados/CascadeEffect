#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S2,     IRRight,        sensorI2CCustom)
#pragma config(Sensor, S3,     gyro,           sensorI2CCustom)
#pragma config(Sensor, S4,     SonarSensor,    sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     swing,         tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     right,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     topCollector,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     bottomCollector, tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoNone)
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
float heading = 0.0;

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
float turnSpeed = 100;
float tolerance = 0;
float blockDistance = 12;
float leftTurnDistance = 10;
float turnDistance = 11.4;
float badHeadingValue = 26;
float sonarDistance = 30;
//used to keep track of if we've been blocked yet
bool blocked = false;
int seekerValue;

const float TOP_GATE_UP = 140;
const float TOP_GATE_DOWN = 255;
const float SPIN_UP_TIME = 2700;
const float TURN_TIME = 1.0;
const float RIGHT_TURN_TIME = 1.1;
const float SWING_OUT_TIME = 950;

const float buffer = 6.0;


void initializeRobot()
{

  // Place code here to initialize servos to starting positions.
  // Sensors are automatically configured and setup by ROBOTC. They may need a brief time to stabilize.

	servoTarget[topGate] = TOP_GATE_DOWN;
	servoTarget[tube] = 255;

  	return;
}


/*Basic right turn
*/
void turnRight(float speed){
	motor[left] = speed;
	motor[right] = -speed;
}

/*Basic left turn
*/
void turnLeft(float speed){
	motor[left] = -speed;
	motor[right] = speed;
}

/*Stopping the motors
*/
void stopMotors(){
	motor[left] = 0;
	motor[right] = 0;
	motor[swing] = 0;
}


/*Multitasking method used to spin up our lift while we are driving.
  This is used in combination with startSpin to make sure we have fully raised
  the lift before moving towards the goal. This keeps us from potentially knocking the goal off.
*/
bool checkSpinTimer(){
	if(time1(T1) > SPIN_UP_TIME){
		motor[lift] = 0;
		return true;
	}
	else
		return false;
}

/*Multitasking method used to spin up our lift while we are driving.
  This is used in combination with checkSpinTimer to make sure we have fully raised
  the lift before moving towards the goal. This keeps us from potentially knocking the goal off.
*/
void startSpin(bool up){
	ClearTimer(T1);
	if(up){
		motor[lift] = -100;
	}
	else{
		motor[lift] = -00;
	}
}

/*The logic to use the gyroscope in a turn
*/
void gyroTurn(float target, float speed, bool direction){
	HTGYROstartCal(gyro);
	heading = 0.0;
	float currTime;
	float prevTime = nPgmTime;
	while (true)
	{
		checkSpinTimer();
		currTime = nPgmTime;
		//Our heading calculation logic
		heading += (abs((float)HTGYROreadRot(gyro))) * (currTime - prevTime) / 1000;
		prevTime = currTime;

		if(direction)
		{
			if(heading < (target - buffer))
			{
				turnRight(speed);
			//	nxtDisplayString(1,"gyro: %i",HTGYROreadRot(gyro));
			}
			else if(heading > (target + buffer))
			{
				turnLeft(speed);
			//	nxtDisplayString(1,"gyro: %i",HTGYROreadRot(gyro));
			}
			else if(heading <= (target + buffer) && heading >= (target - buffer))
			{
				stopMotors();
			//	wait1Msec(waitTime);
				return;
			}
		}
		else
		{
			if(heading < (target - buffer))
			{
				turnLeft(speed);
			//	nxtDisplayString(1,"gyro: %i",HTGYROreadRot(gyro));

			}
			else if(heading > (target + buffer))
			{
				turnRight(speed);
			//	nxtDisplayString(1,"gyro: %i",HTGYROreadRot(gyro));
			}
			else if(heading <= (target + buffer) && heading >= (target - buffer))
			{
				stopMotors();
			//	wait1Msec(waitTime);
				return;
			}
		}
	}
	checkSpinTimer();
}

/*Our inches conversion for the robot
*/
float convertInches(float inches){
	return (1120/6) * inches;
}

/*Stops our robot in the case of being knocked off course
*/
void offCourse(){
	while(true){
		motor[left] = 0;
		motor[right] = 0;
	}
}

/*Used as our backup plan to try and hit the kickstand if it appears we are blocked
*/
void backupKickstand(){
	if(seekerValue > 2){
//		hitPeg();
	}
	else{
//		hitPeg();
	}
	stopMotors();
}

/*Basic drive logic
*/
bool driveRobot(float distance, float speed, string direction, bool useSonar){
		//reset encoder values
	nMotorEncoder[right] = 0;
	nMotorEncoder[left] = 0;

	wait10Msec(50);

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

	HTGYROstartCal(gyro);
	heading = 0.0;
	float currTime;
	float prevTime = nPgmTime;

		//Move the robot until encoder target is reached
	while(abs(nMotorEncoder[right]) < target){
		checkSpinTimer();

		//Check to make sure we are still heading in the right direction
		currTime = nPgmTime;
		//Our heading calculation logic
		heading += (abs((float)HTGYROreadRot(gyro))) * (currTime - prevTime) / 1000;
		prevTime = currTime;
		if(heading >= badHeadingValue){
			offCourse();
		}


		//check to make sure we have nothing behind us (we drive backwards)
		if(SensorValue(sonarSensor) < sonarDistance && useSonar){
			motor[right] = 0;
			motor[left] = 0;
			return false;
		}


		if(abs(target - abs(nMotorEncoder[right])) < tolerance || abs(target - abs(nMotorEncoder[left])) < tolerance){
			break;
		}
	}
	motor[right] = 0;
	motor[left] = 0;
	return true;
}

/*This is our obstacle avoidance method. If we detect a robot along our path to the second or third
  position, we first will retreat back to the top of the tape. We then attempt to score again. If
  we still read we are blocked we will retreat to the tape and then just attempt to hit the kickstand */
void avoidObstacle(){
	if(!blocked){
		driveRobot(abs(nMotorEncoder[right]), 78, forward, false);
		wait10Msec(500);
		blocked = true;
	}
	else{
		driveRobot(abs(nMotorEncoder[right]), 78, forward, true);
		backupKickstand();
	}
}

/*Method that once the robot is in position in front of the goal proceeds to score the ball
*/
void scoreBall(){
	driveRobot(blockDistance / 3.4, maxSpeed, backwards, false);

	wait10Msec(100);
	servoTarget(topGate) = TOP_GATE_UP;
	wait10Msec(100);
	driveRobot(blockDistance / 4, maxSpeed, forward, false);
	motor[lift] = 75;
	wait10Msec(100);
	motor[lift] = 0;
}
/*Powers the swinging arm for the kickstand
*/
void swingOut(bool forward){
	if(forward){
		motor[swing] = 50;
	}
	else{
		motor[swing] = -50;
	}
}

/*Method to move the swinging arm
*/
void moveSwing(){

	swingOut(true);
	wait1Msec(SWING_OUT_TIME);
	gyroTurn(60, turnSpeed, true);
	swingOut(false);
	wait1Msec(SWING_OUT_TIME);
}

/*From the position of the center goal, we retreat and then try to hit the peg
*/
void hitPeg(){
	gyroTurn(92.0, turnSpeed, true);
	driveRobot(blockDistance * 1.5, 78, backwards, false);
	gyroTurn(92.0, turnSpeed, false);
	driveRobot(blockDistance * 1.35, 78, backwards, false);
}

/*Method used to position the robot to score in the first center goal position
*/
void firstPosition(){
	driveRobot(1.7 * blockDistance, 78, backwards, false);
	while(!checkSpinTimer()){
	}
	scoreBall();
	hitPeg();
}

/*Sets up the move towards the 2nd and 1st position
*/
void firstStepTowardsOuterTwo(){
	driveRobot(blockDistance/2, 78, backwards, false);
	gyroTurn(45.0, turnSpeed, false);
}

/*Moves the robot in a path common to the 2nd and 3rd positions
*/
void diagonalMove(){
	if(!driveRobot(blockDistance * 2.3, 78, backwards, false)){
		avoidObstacle();
		if(!blocked){
			diagonalMove();
		}
		else{
			backupKickstand();
		}
	}
	startSpin(true);
}

/*Method used to position the robot to score in the second center goal position
*/
void secondPosition(){
	firstStepTowardsOuterTwo();
	diagonalMove();
	gyroTurn(90.0, turnSpeed, true);
	driveRobot(blockDistance/2.6, maxSpeed, backwards, false);
	while(!checkSpinTimer()){
	}
	scoreBall();
	hitPeg();
}

/*Method used to position the robot to score in the third center goal position
*/
void thirdPosition(){
	firstStepTowardsOuterTwo();
	diagonalMove();
	gyroTurn(45.0, turnSpeed, true);
	driveRobot(blockDistance * 1.35, maxSpeed, backwards, false);
	gyroTurn(90.0, turnSpeed, true);
	while(!checkSpinTimer()){
	}
	scoreBall();
	hitPeg();
}

task main()
{
	initializeRobot();
	waitForStart();

	tHTIRS2DSPMode _mode = DSP_1200;
	seekerValue = -5;

	wait10Msec(50);

	seekerValue = HTIRS2readACDir(IRRight);
	ClearTimer(T1);

	//Our sensing logic, anything greater than 4 is straight ahead
	//Greater than 2 less than 4 is 2nd position
	//Default to third position if nothing is found because that position returns a 0 on the seeker
	if(seekerValue > 4){
		startSpin(true);
		firstPosition();
	}
	else if(seekerValue > 2){
		secondPosition();
	}
	else{
		thirdPosition();
	}
	moveSwing();
	while(true){
		stopMotors();
	}
}
