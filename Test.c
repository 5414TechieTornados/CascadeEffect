#pragma config(Hubs,  S1, HTMotor,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorA,        tmotorTetrix, PIDControl, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	nMotorEncoder[motorA] = 0;
	float target = (1440/9.42)*12;
	motor[motorA] = 100;
	while (nMotorEncoder[motorA] < target)
	{
	}
	motor[motorA] = 0;

}
