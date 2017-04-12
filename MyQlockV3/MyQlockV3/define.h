#include "Arduino.h"

#ifndef define_h
#define define_h

#define FANCY_START
//#define TOUCH_ENABLED

#define NUM_LEDS 114
#define DATA_PIN 13
#define CLOCK_PIN 14
#define LED_TYPE APA102

#define REFRESH_RATE 20    // in ms
#define COLOR_CHANGE_RATE 25
#define INPUT_REFRESH_RATE 20
#define BLINK_RATE 500

#define R_SENSOR_THRESHOLD 600
#define L_SENSOR_THRESHOLD 600
#define DEBOUNCE_DELAY 200
#define R_FLAG_MASK 0b00000001
#define L_FLAG_MASK 0b00000010

#define NUMBER_OF_STATIC_COLORS 1

#define TIME_HEADER  "T"   // Header tag for serial time sync message





#endif
