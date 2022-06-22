/*
   Gamepad module provides three different mode namely Digital, JoyStick and Accerleometer.

   You can reduce the size of library compiled by enabling only those modules that you want to
   use. For this first define CUSTOM_SETTINGS followed by defining INCLUDE_modulename.

   Explore more on: https://thestempedia.com/docs/dabble/game-pad-module/
*/
#define CUSTOM_SETTINGS
#define INCLUDE_GAMEPAD_MODULE
#define INCLUDE_TERMINAL_MODULE
#define ESP32
#include <Arduino.h>
#include <DabbleESP32.h>
#include "sdkconfig.h"


String Serialdata = "";
bool dataflag = 0;

extern "C" void app_main()
{
    initArduino();
    // put your setup code here, to run once:
    Serial.begin(115200);    // make sure your Serial Monitor is also set at this baud rate.
    Dabble.begin("MyEsp32"); // set bluetooth name of your device

    while (1)
    {
        Dabble.processInput(); // this function is used to refresh data obtained from smartphone.Hence calling this function is mandatory in order to get data properly from your mobile.
        // Serial.print("KeyPressed: ");
        if (GamePad.isUpPressed())
        {
            Serial.print("Up");
        }

        if (GamePad.isDownPressed())
        {
            Serial.print("Down");
        }

        if (GamePad.isLeftPressed())
        {
            Serial.print("Left");
        }

        if (GamePad.isRightPressed())
        {
            Serial.print("Right");
        }

        if (GamePad.isSquarePressed())
        {
            Serial.print("Square");
        }

        if (GamePad.isCirclePressed())
        {
            Serial.print("Circle");
        }

        if (GamePad.isCrossPressed())
        {
            Serial.print("Cross");
        }

        if (GamePad.isTrianglePressed())
        {
            Serial.print("Triangle");
        }

        if (GamePad.isStartPressed())
        {
            Serial.print("Start");
        }

        if (GamePad.isSelectPressed())
        {
            Serial.print("Select");
        }

        /* int a = GamePad.getAngle();
        int b = GamePad.getRadius();
        float c = GamePad.getXaxisData();
        float d = GamePad.getYaxisData(); */

        while (Serial.available() != 0)
        {
            Serialdata = String(Serialdata + char(Serial.read()));
            dataflag = 1;
        }
        if (dataflag == 1)
        {
            Terminal.print(Serialdata);
            Serialdata = "";
            dataflag = 0;
        }
        if (Terminal.available() != 0)
        {
            while (Terminal.available() != 0)
            {
                Serial.write(Terminal.read());
            }
            Serial.println();
        }

        delay(1);
    }
}