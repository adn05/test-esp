#define INCLUDE_GAMEPAD_MODULE
#define INCLUDE_TERMINAL_MODULE
#define INCLUDE_NOTIFICATION_MODULE
#include "common.h"

bool dataflag = 0;
u_int8_t prev_brand = IR_SAMSUNG;

void scan_terminal(u_int8_t *brand, int16_t *key_class)
{
    String terminal_data = "";
    String operand = "";
    while (Terminal.available() != 0)
    {
        terminal_data = String(terminal_data + char(Terminal.read()));
        dataflag = 1;
    }

    if (dataflag == 1)
    {
        terminal_data.trim();
        terminal_data.toUpperCase();

        Serial.println(terminal_data);
        if (terminal_data.startsWith("SELECT"))
        {
            // terminal_ops_t = &terminal_ops_t::SELECT;
            operand = terminal_data.substring(7);
            operand.trim();
            if (operand == "SAMSUNG")
            {
                *brand = IR_SAMSUNG;
            }
            else if (operand == "ANDROID")
            {
                *brand = IR_ANDROID;
            }
            else if (operand == "NEC")
            {
                *brand = IR_NEC;
            }
            else if (operand == "LG")
            {
                *brand = IR_LG;
            }
            else
            {
                Serial.println("Unkown brand");
            }
        }
        else if (terminal_data.startsWith("CMD"))
        {
            // *terminal_ops_t = terminal_ops_t::CMD;
            operand = terminal_data.substring(4);
            operand.trim();
            if (operand.startsWith("L"))
            {
                *key_class = NAV_KEY_LEFT;
                Serial.println("-> Left");
            }
            else if (operand.startsWith("R"))
            {
                *key_class = NAV_KEY_RIGHT;
                Serial.println("-> Right");
            }
            else if (operand.startsWith("U"))
            {
                *key_class = NAV_KEY_UP;
                Serial.println("-> Up");
            }
            else if (operand.startsWith("D"))
            {
                *key_class = NAV_KEY_DOWN;
                Serial.println("-> Down");
            }
            else if (operand.startsWith("OK"))
            {
                *key_class = NAV_KEY_OK;
                Serial.println("-> Ok");
            } else if (operand.startsWith("BACK"))
            {
                *key_class = NAV_KEY_BACK;
                Serial.println("-> Back");
            }
            else
            {
                Serial.println("Key not supported yet !");
                *key_class = -1;
            }
        }
        else if (terminal_data.startsWith("WHICH"))
        {
            // *terminal_ops_t = terminal_ops_t::WHITCH;
            Serial.printf("Selected remote %d\n", *brand);
            Terminal.print("Selected remote ");
            Terminal.print(*brand);
        }
        else
        {
            // *terminal_ops_t = terminal_ops_t::NONE;
            Serial.println("Command not defined!");
        }

        terminal_data = "";
        dataflag = 0;
    }
}

void scan_gamepad(u_int8_t *brand, int16_t *key_class)
{
    if (GamePad.isLeftPressed())
    {
        Serial.println("Left");
        *key_class = NAV_KEY_LEFT;
    }

    if (GamePad.isRightPressed())
    {
        Serial.println("Right");
        *key_class = NAV_KEY_RIGHT;
    }

    if (GamePad.isUpPressed())
    {
        Serial.println("Up");
        *key_class = NAV_KEY_UP;
    }

    if (GamePad.isDownPressed())
    {
        Serial.println("Down");
        *key_class = NAV_KEY_DOWN;
    }

    if (GamePad.isSelectPressed())
    {
        Serial.println("Select");
        *brand = (*brand + 1) % TOTAL_IR + 1;
    }

    if (GamePad.isStartPressed())
    {
        Serial.println("Start");
    }

    if (GamePad.isSquarePressed())
    {
        Serial.println("Square");
    }

    if (GamePad.isCirclePressed())
    {
        Serial.println("Circle");
        *key_class = NAV_KEY_OK;
    }

    if (GamePad.isCrossPressed())
    {
        Serial.println("Cross");
        *key_class = NAV_KEY_BACK;
    }

    if (GamePad.isTrianglePressed())
    {
        Serial.println("Triangle");
    }

    /* int a = GamePad.getAngle();
    int b = GamePad.getRadius();
    float c = GamePad.getXaxisData();
    float d = GamePad.getYaxisData(); */
}

void notify(u_int8_t* brand){
    if (Dabble.isAppConnected()){
        if(prev_brand != *brand){
            String nofif_msg = "Device changed to ";
            switch (*brand)
            {
            case IR_ANDROID:
                nofif_msg.concat("ANDROID");
                break;
            case IR_SAMSUNG:
                nofif_msg.concat("SAMSUNG");
                break;
            case IR_LG:
                nofif_msg.concat("LG");
                break;
            case IR_NEC:
                nofif_msg.concat("NEC");
                break;      
            default:
                nofif_msg.concat("UNKNOWN");
                break;
            }
            Serial.println(nofif_msg);
            Notification.notifyPhone(nofif_msg);
            prev_brand = *brand;
        }
    }
}