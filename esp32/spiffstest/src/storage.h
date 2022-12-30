#include <SPIFFS.h>
#include <FS.h>

File dataLog;

#ifndef LOGGING_INTERVAL
//set logging interval to 20s
#define LOGGING_INTERVAL 20000
#endif

uint logTimer;

void readFile(fs::FS &fs, const char * path){
    log_v("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if(!file || file.isDirectory()){
        log_e("- failed to open file for reading");
    }

    log_v("- read from file:");
    while(file.available()){
        Serial.write(file.read());
    }
    file.close();
}

void writeFile(fs::FS &fs, const char * path, const char * message){
    log_v("Writing file: %s\r\n", path);

    File file = fs.open(path, FILE_WRITE);
    if(!file){
        log_e("- failed to open file for writing");
    }
    if(file.print(message)){
        log_v("- file written");
    } else {
        log_e("- write failed");
    }
    file.close();
}

void initLog(){
    log_v("initLog started");
	//init spiffs if it is not init
	if(!SPIFFS.begin(true))
	{
		log_e("SPIFFS INIT FAILED");
	}

	File file = SPIFFS.open("/logfile.csv",FILE_APPEND);

	//if file doesent exist, make one!
    if(!file){
        log_v("Initialize new log");
        writeFile(SPIFFS,"/logfile.csv","logfile\n");
        file.close();
    } //close the file we just opened
    else{
        file.close();
    }
    log_v("leave init log");
}

void startNewLog(){
	dataLog = SPIFFS.open("/logfile.csv",FILE_APPEND);
	if(!dataLog){
        log_e("new log failed- no logfile");
    }
    dataLog.print("START\n");
    logTimer = millis();
    //leave file open
}

void doLogTask(int depth, int temp){
    uint logTimeNow = millis();

    if(logTimeNow-logTimer > LOGGING_INTERVAL){
        if(dataLog.available()){
            dataLog.print(depth); //presumably, write to spiffs buffer
            dataLog.print(",");
            dataLog.print(temp);
            dataLog.print("\n");
            log_v("logged data");
        }
        else{
            log_e("failed to log data- no dataLog");
        }
        logTimer=logTimeNow;
    }
}

void endLog(){
    //end Log, close file - this frees up ram and
    //should also force a real write to the file
	dataLog.print("END\n");
	dataLog.close();
    log_v("closing log");
    if(dataLog){
        log_e("log still open!");
    }
}

void eraseLog()
{
	 if(SPIFFS.remove("/logfile.csv")){
        log_v("- file deleted");
    } else {
        log_e("- delete failed");
    }
}

//debug purposes only - dont try to do this in real code
void printLog(){
	readFile(SPIFFS, "/logfile.csv");
}