#define CUSTOM_SETTINGS
#include <stdio.h>
#include "Arduino.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "DabbleESP32.h"

#define TOTAL_IR    4
#define IR_SAMSUNG  1
#define IR_LG       2
#define IR_ANDROID  3
#define IR_NEC      4

#define NAV_KEY_LEFT  0
#define NAV_KEY_RIGHT 1
#define NAV_KEY_UP    2 
#define NAV_KEY_DOWN  3
#define NAV_KEY_OK    4
#define NAV_KEY_BACK  5
	    
extern u_int32_t samsung_nav_key[];
extern u_int32_t      lg_nav_key[];
extern u_int32_t android_nav_key[];

void ir_send(IRsend, u_int8_t, int16_t);

void scan_terminal(u_int8_t *, int16_t *);
void scan_gamepad(u_int8_t *, int16_t *);
void notify(u_int8_t *);
