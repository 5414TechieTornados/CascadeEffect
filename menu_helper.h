TButtons NEXT_BTN = kRightButton;
TButtons PREV_BTN = kLeftButton;
TButtons CAT_BTN = kEnterButton;

void switchBool(bool *ptr, TButtons btn)
{
	if(btn == NEXT_BTN||btn == PREV_BTN)
	{
		*ptr=!*ptr;
	}
}

void switchInt(int *ptr, TButtons btn)
{
	if(btn == NEXT_BTN){
		*ptr=*ptr+1;
	} else if(btn == PREV_BTN) {
		*ptr=*ptr-1;
	}
}

bool right=false;
bool full=true;
int scoreTol=1;
int parkTol=2;
int delay=0;
bool blockRobot = false;
string dottedLine = "---------------------------------------------------------------";


void displayOpeningMenu(){

		nxtDisplayString(0,"New Task");
		nxtDisplayString(2,"Load Task");
		nxtDisplayString(4,"View Program");
		nxtDisplayString(1,dottedLine);
		nxtDisplayString(3,dottedLine);
		nxtDisplayString(5,dottedLine);
		nxtDisplayString(6,dottedLine);
		nxtDisplayString(7,"Lock   | Unlock");

}

void displayNewTaskMenu(){
		nxtDisplayString(0,"Task Name: ");
		nxtDisplayString(1,"Var 1:");
		nxtDisplayString(2,"Var 2:");
		nxtDisplayString(3,"Var 3:");
		nxtDisplayString(4,"Var 4:");
		nxtDisplayString(5,"Var 5:");
		nxtDisplayString(6,"Var 6:");
		nxtDisplayString(7,"Save   | Cancel");
}

void displayLoadTaskMenu(){
		nxtDisplayString(0,"Drive/Bridge");
		nxtDisplayString(1,"Shoot Ball");
		nxtDisplayString(2,"Grab Tube 1");
		nxtDisplayString(3,"Grab Tube 2");
		nxtDisplayString(4,"Grab Tube 3");
		nxtDisplayString(5,"Dep Ball");
		nxtDisplayString(6,"Shoot Ball");
		nxtDisplayString(7,"Save   | Cancel");
}

void displayViewTasksMenu(){
		nxtDisplayString(0,"Shoot Ball");
		nxtDisplayString(1,"Knock Peg");
		nxtDisplayString(2,"Wait");
		nxtDisplayString(3,"");
		nxtDisplayString(4,"");
		nxtDisplayString(5,"");
		nxtDisplayString(6,"");
		nxtDisplayString(7,"       | Cancel");
}

task runMenu()
{
	int menuPosition = 0;
	while(true){
		if(menuPosition == 0){
			displayOpeningMenu();
		}
		else if(menuPosition == 1){
			displayNewTaskMenu();
		}
		else if(menuPosition == 2){
			displayLoadTaskMenu();
		}
		else if(menuPosition == 3){
			displayViewTasksMenu();
		}

		if(nNxtButtonPressed==NEXT_BTN && menuPosition < 3){
			menuPosition = menuPosition + 1;
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
			eraseDisplay();
		}
		else if(nNxtButtonPressed==PREV_BTN){
			menuPosition = 0;
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
			eraseDisplay();
		}
	}
}

	/*
	void* currVar;
	char currType;

	currVar = &right;
	currType = 'b';

	while(true){
		if(delay<0){
			delay=15;
		} else if(delay>15){
			delay = 0;
		}
		if(scoreTol<0){
			scoreTol = 5;
		} else if(scoreTol>5){
			scoreTol = 0;
		}
		if(parkTol<0){
			parkTol = 5;
		} else if(parkTol>5){
			parkTol = 0;
		}



		nxtDisplayString(0,"Side     : %s",right?"right":"left ");
		nxtDisplayString(1,"Type     : %s",full?"full  ":"first");
		nxtDisplayString(2,"Score Tol: %i",scoreTol);
		nxtDisplayString(3,"Park Tol : %i",parkTol);
		nxtDisplayString(4,"Delay    : %i",delay);
		nxtDisplayString(5,"Block: %s",blockRobot?"Yes":"No ");
		nxtDisplayString(6,"---------------------------------------------------------------");
		nxtDisplayString(7,"Save   | Cancel");

		if(currVar == &right)
		{
			nxtDisplayStringAt(94,63,"*");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &full){
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55,"*");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &scoreTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47,"*");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &parkTol) {
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39,"*");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23," ");
		} else if(currVar == &delay){
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31,"*");
			nxtDisplayStringAt(94,23," ");
		}	else{
			nxtDisplayStringAt(94,63," ");
			nxtDisplayStringAt(94,55," ");
			nxtDisplayStringAt(94,47," ");
			nxtDisplayStringAt(94,39," ");
			nxtDisplayStringAt(94,31," ");
			nxtDisplayStringAt(94,23,"*");
		}

		if((nNxtButtonPressed==NEXT_BTN||nNxtButtonPressed==PREV_BTN) && (!blockRobot || currVar == &blockRobot)){
			if(currType=='b'){
				switchBool(currVar,nNxtButtonPressed);
			} else if (currType=='i') {
				if(currVar == &scoreTol){
					nxtDisplayString(2,"Score Tol:    ",scoreTol);
				}
				if(currVar == &parkTol){
					nxtDisplayString(3,"Park Tol :    ",parkTol);
				}
				if(currVar == &delay){
					nxtDisplayString(4,"Delay    :    ",delay);
				}
				switchInt(currVar,nNxtButtonPressed);
			}
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
		}

		if(nNxtButtonPressed==CAT_BTN){
			if(currVar == &right){
				currVar = &full;
				currType = 'b';
			} else if(currVar == &full){
				currVar = &scoreTol;
				currType = 'i';
			} else if(currVar == &scoreTol) {
				currVar = &parkTol;
				currType = 'i';
			} else if(currVar == &parkTol) {
				currVar = &delay;
				currType = 'i';
			} else if(currVar == &delay) {
				currVar = &blockRobot;
				currType = 'b';
			}else {
				currVar = &right;
				currType = 'b';
			}
			ClearTimer(T1);
			while(nNxtButtonPressed!=kNoButton&&time1[T1]<=400);
		}
	}
}

/*void displayAutoType()
{
	nxtDisplayTextLine(7,"%s,%s,%i,%i,%i",right?"R":"L",full?"Ful":"1st",scoreTol,parkTol,delay);
}*/
