/*
   
*/
#include "common.h"

// terminal_ops_t* operation_actu;
uint8_t selected_brand = IR_SAMSUNG;
int16_t gamepad_key_class;
u_int8_t ir_send_pin = 6;

IRsend irsend(ir_send_pin, false);

extern "C" void app_main()
{
    initArduino();
    // put your setup code here, to run once:
    Serial.begin(115200);    // make sure your Serial Monitor is also set at this baud rate.
    Dabble.begin("MyEsp32"); // set bluetooth name of your device

    irsend.begin();     // Start ir sender
    Serial.printf("IR send on Pin %d !\n", ir_send_pin);

    while (1)
    {
        Dabble.processInput(); // this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
        // Serial.print("KeyPressed: ");

        gamepad_key_class = -1;     // Réinitialise
        scan_terminal(&selected_brand, &gamepad_key_class);
        notify(&selected_brand);
        scan_gamepad(&selected_brand, &gamepad_key_class);

        //Serial.printf("band %d, key_class %d\n", selected_brand, gamepad_key_class);

        ir_send(irsend, selected_brand, gamepad_key_class);
        delay(50);
    }
}