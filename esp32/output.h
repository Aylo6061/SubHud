#include <arduino.h>
typedef enum
{
modeWake = 0,
modeSleep = 1,
modeChg = 2,
modeSurf = 3,
modeStartDive = 4,
modeDiveA = 5,
modeDiveB = 6,
modeFinishDive = 7,
modeAP = 8,
modeServer = 9,
} state_t;

typedef enum
{
onExit = 0,
onTimeout = 1,
onExtpowConn = 2,
onExtpowDC = 3,
onButton = 4,
onWifiConnect = 5,
onWifiDisconnect = 6,
onDive = 7,
onSurface = 8,
} event_t;

state_t stateTable [10][9] =
{
{modeWake,modeSleep,modeChg,modeWake,modeSurf,modeWake,modeWake,modeWake,modeWake,},
{modeWake,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,},
{modeChg,modeSleep,modeChg,modeSleep,modeAP,modeChg,modeChg,modeChg,modeChg,},
{modeSurf,modeSleep,modeAP,modeSurf,modeSurf,modeSurf,modeSurf,modeStartDive,modeSurf,},
{modeDiveA,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,},
{modeDiveA,modeDiveA,modeDiveA,modeDiveA,modeDiveB,modeDiveA,modeDiveA,modeDiveA,modeFinishDive,},
{modeDiveB,modeDiveB,modeDiveB,modeDiveB,modeDiveA,modeDiveB,modeDiveB,modeDiveB,modeFinishDive,},
{modeSurf,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,},
{modeAP,modeChg,modeAP,modeSleep,modeAP,modeServer,modeAP,modeAP,modeAP,},
{modeServer,modeAP,modeServer,modeSleep,modeServer,modeServer,modeAP,modeServer,modeServer,},
};

state_t currentState = modeWake;

bool isStateNew = 1;

	void stateMachineUpdate(event_t event)
	{
		state_t prevState;
		prevState = currentState;
		currentState = stateTable[currentState][event];

		if(currentState!= prevState){
			isStateNew = 1;
		}

	}

	bool newState(){
		if(isStateNew){
			isStateNew = 0;
			return 1;
		}
		else{
			return 0;
		}
	}