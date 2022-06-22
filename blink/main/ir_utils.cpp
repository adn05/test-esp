#include "common.h"

//                                   LEFT,      RIGHT,         UP,       DOWN,         OK,     RETURN 		    	    
u_int32_t samsung_nav_key[] = {0xE0E0A659, 0xE0E046B9, 0xE0E006F9, 0xE0E08679, 0xE0E016E9, 0xE0E01AE5};
u_int32_t      lg_nav_key[] = {0x343415EA, 0x3434956A, 0x3434E51A, 0x3434659A, 0x343455AA, 0x343445BA};
u_int32_t android_nav_key[] = {0x80BF9966, 0x80BF837C, 0x80BF53AC, 0x80BF4BB4, 0x80BF738C, 0x80BFA35C};

void ir_send(IRsend irsend, u_int8_t brand, int16_t key_class)
{
    u_int32_t ir_code;

    if (brand == IR_SAMSUNG)
    {
        if (key_class >= 0 && key_class < 10)
        { // Class NAV_KEY
            ir_code = samsung_nav_key[key_class];
            Serial.printf("IR sending code %X\n", ir_code);
            irsend.sendSAMSUNG(ir_code, 32, 1);
        }
    }
    else if (brand == IR_LG)
    {
        if (key_class >= 0 && key_class < 10)
        { // Class NAV_KEY
            ir_code = lg_nav_key[key_class];

            Serial.printf("IR sending code %X\n", ir_code);
            irsend.sendSAMSUNG(ir_code, 32, 1);
        }
    }
    else if (brand == IR_ANDROID)
    {
        if (key_class >= 0 && key_class < 10)
        { // Class NAV_KEY
            ir_code = android_nav_key[key_class];

            Serial.printf("IR sending code %X\n", ir_code);
            irsend.sendNEC(ir_code, 32, 1);
        }
    }
}