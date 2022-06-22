/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "Arduino.h"
#include "IRremoteESP8266.h"
#include "IRsend.h"
#include "IRrecv.h"
#include "IRutils.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"

static const char *TAG = "IR_Test";

//IR_Test: Decode address: 7, command: 2, value: 8, protocol: -522174273
//Decoded raw [67] : {4546,4512,556,1694,552,1690,560,1688,558,564,556,566,556,564,562,562,558,566,558,1688,560,1682,562,1688,558,538,586,564,558,560,562,566,558,566,556,566,558,1688,558,564,556,556,566,544,582,564,556,568,556,564,558,1688,558,566,558,1686,558,1662,586,1686,560,1688,558,1688,558,1686,560}
//IR_Test: Decode address: 7, command: 2, value: 8, protocol: -522174273
//Decoded raw [67] : 

uint16_t poweroff_raw[] = {4546,4490,582,1660,588,1662,584,1660,586,538,584,542,582,544,580,556,566,568,556,1660,586,1688,558,1688,558,542,580,540,582,540,582,566,560,538,584,542,582,1660,588,566,558,564,558,564,558,566,556,540,582,540,584,1688,558,566,558,1690,554,1654,592,1640,608,1662,586,1660,584,1690,558};
// uint32_t ir_cmd[] = {0xF720DF, 0xF7A05F, 0xF7609F};
uint32_t ir_cmd[] = {0xE0E040BF, 0xE0E020DF, 0xE0E0A05F};

// An IR detector/demodulator is connected to GPIO pin 7
#define RECV_PIN 7
#define SEND_PIN 6

IRrecv irrecv(RECV_PIN);
IRsend irsend(SEND_PIN, false);

decode_results results;

extern "C" void app_main()
{
    initArduino();

    //irrecv.enableIRIn();  // Start the receiver
    //ESP_LOGI(TAG, "IR receive demo on Pin %d !", RECV_PIN);

    irsend.begin();
    ESP_LOGI(TAG, "IR send demo on Pin %d !", SEND_PIN);

    while (1)
    {
        //ESP_LOGI(TAG, "---IR sending poweroff");
        // irsend.sendSAMSUNG(0xE0E040BF, 32, 1);
        
        
        for (uint8_t i = 0; i < 3; i++) {
            //irsend.sendSony(0xa90, 12);
            //irsend.sendSAMSUNG()
            ESP_LOGI(TAG, "IR sending code %X", ir_cmd[i]);
            irsend.sendSAMSUNG(ir_cmd[i], 32, 1);
            // irsend.sendNEC(ir_cmd[i], 32, 1);
            irsend.send(poweroff_raw, 67, 38000);
            delay(5000);
        }
        delay(5000); //5 second delay between each signal burst 
    }
}