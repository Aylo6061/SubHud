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
onCharge = 2,
onChargeEnd = 3,
onButton = 4,
onWifiConnect = 5,
onWifiDisconnect = 6,
} event_t;

state_t stateTable [10][7] =
{
{modeWake,modeSleep,modeChg,modeWake,modeSurf,modeWake,modeWake,},
{modeWake,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,modeSleep,},
{modeChg,modeChg,modeChg,modeSleep,modeAP,modeChg,modeChg,},
{modeSurf,modeSleep,modeAP,modeSurf,modeSurf,modeSurf,modeSurf,},
{modeDiveA,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,modeStartDive,},
{modeDiveA,modeDiveA,modeDiveA,modeDiveA,modeDiveB,modeDiveA,modeDiveA,},
{modeDiveB,modeDiveB,modeDiveB,modeDiveB,modeDiveA,modeDiveB,modeDiveB,},
{modeSurf,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,modeFinishDive,},
{modeAP,modeChg,modeAP,modeAP,modeAP,modeServer,modeAP,},
{modeServer,modeChg,modeServer,modeServer,modeServer,modeServer,modeAP,},
};

state_t currentState = modeWake;

	void stateMachineUpdate(event_t event)
	{
		currentState = stateTable[currentState][event];
	}