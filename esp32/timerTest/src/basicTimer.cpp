#include "basicTimer.h"
#include <arduino.h>

basicTimer::basicTimer(){

}

void basicTimer::init(){
	_active = 0;
	_interval = 10*1000; //10 seconds is the default interval
	_expired = 0;
}

void basicTimer::start(unsigned int interval){
	if(!_active){
		_startTime = millis();
		_interval = interval;
		_active = 1;
		_expired = 0;
	}
	//if the timer is active- do nothing
}

void basicTimer::stop()
{
	check(); //check to see if timer is expired before stopping
	_active = 0;
}

bool basicTimer::expired(){
	check();
	return _expired;
}

void basicTimer::clear(){
	_expired = 0;
}

void basicTimer::check(){
	if(millis()-_startTime > _interval && _active)
	{
		_expired = 1;
		_active = 0;
	}
}