#include <arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "screen.h"

#define SCREEN_WIDTH 64 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

screen::screen()
{
	//n/a
}

void screen::begin(){
	pinMode(A2, OUTPUT);
	digitalWrite(A2, HIGH);
	log_v("starting screen");
	oled = new Adafruit_SSD1306(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
	if(!(oled->begin(SSD1306_SWITCHCAPVCC, 0x3C))){
		log_e("screen failed to start");
	}
	oled->setTextSize(1);
	oled->setTextColor(SSD1306_WHITE);
	oled->cp437(true);
	oled->clearDisplay();
	oled->drawPixel(63,31,SSD1306_WHITE);
	oled->display();
	log_v("screen started");
}
void screen::display_surf_mode(int heading, float depthFeet, bool isCal)
{
	log_v("screen: surf");
	oled->clearDisplay();
	oled->setCursor(58,24);
	oled->print("S");
	oled->setCursor(16,4);
	oled->print(heading);
	oled->setCursor(16,14);
	oled->println(depthFeet);
	oled->display();
}

void screen::display_wake_mode()
{
	log_v("screen: wake");
	oled->clearDisplay();
	oled->setCursor(0,0);
	oled->print("Wake");
	oled->display();
}

void screen::display_dive_mode(int heading, float depthFeet, bool isCal)
{
	log_v("screen: dive");
	oled->clearDisplay();
	oled->setCursor(58,24);
	oled->print("D");
	oled->setCursor(16,4);
	oled->print(heading);
	oled->setCursor(16,14);
	oled->println(depthFeet);
	oled->display();
}

void screen::display_ep_mode(bool charging, int ap)
{
	log_v("screen: extpow");
	oled->clearDisplay();
	oled->setCursor(52,24);
	oled->print("EXTPOW");
	oled->setCursor(16,4);
	if(charging){
	oled->print("chg");
	}
	else{
	oled->print("done");
	}

	oled->setCursor(16,14);
	if(ap==1)
	{
	oled->print("AP ON");
	}
	if(ap==2)
	{
	oled->print("CONN");	
	}
	
	oled->display();
}


void screen::powerDown()
{
	oled->ssd1306_command(SSD1306_DISPLAYOFF);
	pinMode(A2, OUTPUT);
	digitalWrite(A2, LOW);
}