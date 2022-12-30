//we are only going to have one button...at least for now
#include <arduino.h>
#define debounce_time 200

#ifndef BUTTONPIN
#define BUTTONPIN 4
#endif

typedef enum{
	none = 0,
	singleTap = 1,
} button_press_t;

typedef enum{
	disabled = 0,
	enabled = 1,
	wakeup = 2,
} button_system_status_t;

button_press_t current_button_state = none;

button_system_status_t button_stystem_status = disabled;

uint lastButtonTime=0;

void IRAM_ATTR button_ISR() {
	uint buttonTime = millis();
	if(buttonTime-lastButtonTime > debounce_time)
	{
		if(current_button_state == none)
		{
			current_button_state = singleTap;
		}
		lastButtonTime = buttonTime;
	}
}

void initButton()
{
	pinMode(BUTTONPIN, INPUT_PULLUP);
	attachInterrupt(BUTTONPIN, button_ISR, FALLING);
	log_v("button initialized");
	button_stystem_status = enabled;
}

void disableButton()
{
	detachInterrupt(BUTTONPIN);
	current_button_state = none;
	log_v("button disabled");
	button_stystem_status = disabled;
}

void setButtonAsWakeUp()
{	
	log_v("setting as wakeup");

	if(button_stystem_status == enabled){
		disableButton();
	}

	esp_deep_sleep_enable_gpio_wakeup(1ULL << BUTTONPIN, ESP_GPIO_WAKEUP_GPIO_LOW);
	button_stystem_status = wakeup;
	log_v("exit set as wakeup");
}

void enterSleep()
{
	if(button_stystem_status != wakeup){
		log_e("tried to sleep; button not set to wake");
		setButtonAsWakeUp(); //fix the problem
	}
	log_v("deep sleep start");
	esp_deep_sleep_start();
}
//read and clear button status
//this should be read regularly so as not to miss
//button presses
button_press_t readButton(){
	log_v("button read");
	button_press_t temp;

	temp = current_button_state;
	current_button_state = none;

	return temp;
}