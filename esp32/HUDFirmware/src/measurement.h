#include <Adafruit_BNO055.h>
#include <MS5837.h>
// #include depth

class measure{
	public:
		measure();
		void begin();
		void takeMeasurement();// manually take a measurement, store data in class
		void exportMeasurement(int* heading, float* depth, bool* isCalibrated);//send the data to an external pointer
		void setMeasurementTaskFrequency(int mils); //set how often we really measure
		void stopMeasurementTask();//stops the measurement task
		void startMeasurementTask();
		void measurementTask(); //measurement task will take a new measurement if it is time
		// void powerDown();
		Adafruit_BNO055* orient;
		MS5837* depth;
		int lastHeading;
		float lastDepth;
		uint lastTime;
		int intervalMs;
		bool started;
		bool newDepthReading;
		// depth* ds;
};