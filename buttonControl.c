#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     IRLeft,         sensorI2CCustom)
#pragma config(Sensor, S3,     IRRight,        sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     lift,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     left,          tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     right,         tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C4_1,     topCollector,  tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_2,     bottomCollector, tmotorTetrix, openLoop, reversed)
#pragma config(Servo,  srvo_S1_C3_2,    topGate,              tServoStandard)
#pragma config(Servo,  srvo_S1_C3_3,    tube,                 tServoStandard)
#pragma config(Servo,  srvo_S1_C3_4,    auto4,                tServoStandard)
#pragma config(Servo,  srvo_S1_C3_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_6,    servo6,               tServoNone)


int rightSpeedDefault = 3;
int leftSpeedDefault = -3;
int rightSpeed = rightSpeedDefault;
int leftSpeed = leftSpeedDefault;

task ButtonTask()
{
 nSchedulePriority	 = kHighPriority;

  //
  // Update count of times each button is pushed. If using this program as a template, replace these
  // statements with your own custom action.
  //
  switch (nNxtButtonPressed)
  {
  	case kExitButton:
			motor[lift] = 0;
			leftSpeed = leftSpeedDefault;
			rightSpeed = rightSpeedDefault;
			break;

  	case kLeftButton:
		  motor[lift] = 50;
		  rightSpeed = rightSpeed + rightSpeed;
			break;

		case kRightButton:
		  motor[lift] = -50;
			break;

		case kEnterButton:
			break;

	}
}

task main()
{
	//
	// "Hi-jack" buttons for user program control.
	//
	// Note: program cannot be terminated if we hijack the 'exit' button. So there has to be an escape sequence
	//       that will return buttons to system control!
	//
	nNxtExitClicks    = 2;
	nNxtButtonTask    = ButtonTask;

	nxtDisplayTextLine(2, "  < to Decrease");
  nxtDisplayTextLine(3, "  > to Increase");
  nxtDisplayTextLine(4, "  Enter to Stop");
  nxtDisplayTextLine(5, "  ExitX2 to Exit");
	//
	// Do nothing. Just keep waiting
	//
	while (true)
	{
		//
		// Some dummy code to do some work.
		//
	  wait1Msec(50);
  }
	return;
}
