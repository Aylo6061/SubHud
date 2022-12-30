#include <Adafruit_SSD1306.h>

class  screen{
public:
	screen();
	void begin();
	void display_surf_mode(int heading, float depthFeet, bool isCal);
	void display_wake_mode();
	void display_dive_mode(int heading, float depthFeet, bool isCal);
	// void powerDown();
	Adafruit_SSD1306* oled;
};