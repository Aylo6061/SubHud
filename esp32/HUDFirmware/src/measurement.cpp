#include <Adafruit_BNO055.h>
#include <MS5837.h>
#include <Wire.h>
#include <Arduino.h>
#include "measurement.h"


measure::measure(){

}
void measure::begin(){
	Wire.begin();
	depth = new MS5837();
	orient = new Adafruit_BNO055(55, 0x28);
	if(!(orient->begin()))
	{
		log_e("ahrs init failed");
	}
	orient->setExtCrystalUse(true);

	if(!(depth->init()))
	{
		log_e("depth init failed");
	}
	depth->setModel(MS5837::MS5837_30BA);
    depth->setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
	depth->setOsr(4);
	depth->setModel(MS5837::MS5837_02BA);

	newDepthReading=0;
	lastDepth = 0;
	lastHeading = 0;
	started = 0;
	intervalMs = 100;
	log_v("measurement system started");
}
//actually takes a measurement, store it in local vars
void measure::takeMeasurement()
{
	sensors_event_t event;
	//cie - clear int enable?
	orient->getEvent(&event);
	//sei - set int enable?
	lastHeading = event.orientation.x;
	log_v("got a heading");
}

//send the measurement to a variable to be read by someone else
void measure::exportMeasurement(int* heading, float* depth, bool* isCalibrated)
{
	*heading=lastHeading;
	*depth = lastDepth;
}
void measure::setMeasurementTaskFrequency(int mils)
{
	intervalMs = mils;
}
void measure::stopMeasurementTask()
{
	started = 0;
	log_v("stopped meas sys");
}

void measure::startMeasurementTask()
{
	started = 1;
	lastTime = millis();
	depth->startMeasurement();
	depth->doMeasurement();
	newDepthReading = 1;
	log_v("started meas sys");
}

void measure::measurementTask()
{
	if(started){
		uint timeNow;
		timeNow = millis();

		//kick off or take measurements
		if(timeNow-lastTime>intervalMs)
		{
			log_v("measurement tick");
			lastTime = timeNow;
			takeMeasurement();

			//kick off a new depth meas
			depth->startMeasurement();
			newDepthReading = 1;
			depth->doMeasurement();
		}
		//if the depth measurement is in progress, keep going
		depth->doMeasurement();

		//if depth measurement is ready and new, take a reading and store it
		if((depth->checkMeasurement() == done) && newDepthReading)
		{
			log_v("got depth");
			newDepthReading=0;
			lastDepth = depth->depth();
		}
	}
}

void measure::powerDown()
{
	orient->enterSuspendMode();
}